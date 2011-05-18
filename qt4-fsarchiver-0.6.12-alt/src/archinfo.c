/*
 * fsarchiver: Filesystem Archiver
 *
 * Copyright (C) 2008-2010 Francois Dupoux.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * Homepage: http://www.fsarchiver.org
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <string.h>

#include "fsarchiver.h"
#include "dico.h"
#include "common.h"
#include "archinfo.h"
#include "archreader.h"
#include "error.h"
#include "connect_c_cpp.h"
#include "system.h"

char *compalgostr(int algo)
{
    switch (algo)
    {
        case COMPRESS_NONE:    return "none";
        case COMPRESS_LZO:     return "lzo";
        case COMPRESS_GZIP:    return "gzip";
        case COMPRESS_BZIP2:   return "bzip2";
        case COMPRESS_LZMA:    return "lzma";
        default:               return "unknown";
    }
}

char *cryptalgostr(int algo)
{
    switch (algo)
    {
        case ENCRYPT_NONE:     return "none";
        case ENCRYPT_BLOWFISH: return "blowfish";
        default:               return "unknown";
    }
}

int archinfo_show_mainhead(carchreader *ai, cdico *dicomainhead)
{
    char buffer[256];
    
    if (!ai || !dicomainhead)
    {   errprintf(tr("a parameter is null\n"));
        return -1;
    }
    
    msgprintf(MSG_FORCE, tr("====================== archive information ======================\n"));
    msgprintf(MSG_FORCE, tr("Archive type: \t\t\t%s\n"), (ai->archtype==ARCHTYPE_FILESYSTEMS)?tr("filesystems"):tr("flat files"));
    if ((ai->archtype==ARCHTYPE_FILESYSTEMS))
        msgprintf(0, tr("Filesystems count: \t\t%ld\n"), (long)ai->fscount);
    msgprintf(MSG_FORCE, tr("Archive id: \t\t\t%.8x\n"), (unsigned int)ai->archid);
    msgprintf(MSG_FORCE, tr("Archive file format: \t\t%s\n"), ai->filefmt);
    msgprintf(MSG_FORCE, tr("Archive created with: \t\t%s\n"), ai->creatver);
    msgprintf(MSG_FORCE, tr("Archive creation date: \t\t%s\n"), format_time(buffer, sizeof(buffer), ai->creattime));
    msgprintf(MSG_FORCE, tr("Archive label: \t\t\t%s\n"), ai->label);
    if (ai->minfsaver > 0) // fsarchiver < 0.6.7 had no per-archive minfsaver version requirement
        msgprintf(MSG_FORCE, tr("Minimum fsarchiver version:\t%d.%d.%d.%d\n"), (int)FSA_VERSION_GET_A(ai->minfsaver), 
            (int)FSA_VERSION_GET_B(ai->minfsaver), (int)FSA_VERSION_GET_C(ai->minfsaver), (int)FSA_VERSION_GET_D(ai->minfsaver));
    msgprintf(MSG_FORCE, tr("Compression level: \t\t%d (%s level %d)\n"), ai->fsacomp, compalgostr(ai->compalgo), ai->complevel);
    msgprintf(MSG_FORCE, tr("Encryption algorithm: \t\t%s\n"), cryptalgostr(ai->cryptalgo));
    msgprintf(MSG_FORCE, "\n");
    meldungen_uebergeben(cryptalgostr(ai->cryptalgo),1);
    
    return 0;
}

int archinfo_show_fshead(cdico *dicofshead, int fsid)
{
    char magic[FSA_SIZEOF_MAGIC+1];
    char fsbuf[FSA_MAX_FSNAMELEN];
    u64 temp64;
    u64 fsbytestotal;
    u64 fsbytesused;
    char buffer[256];
    char fslabel[256];
    char fsuuid[256];
    char fsorigdev[256];
    
    // init
    memset(magic, 0, sizeof(magic));
    
    if (!dicofshead)
    {   errprintf(tr("dicofshead is null\n"));
        return -1;
    }
    
    if (dico_get_data(dicofshead, 0, FSYSHEADKEY_FILESYSTEM, fsbuf, sizeof(fsbuf), NULL)!=0)
    {   errprintf(tr("cannot find FSYSHEADKEY_FILESYSTEM in filesystem-header\n"));
        return -1;
    }

    if (dico_get_u64(dicofshead, 0, FSYSHEADKEY_BYTESTOTAL, &fsbytestotal)!=0)
    {   errprintf(tr("cannot find FSYSHEADKEY_BYTESTOTAL in filesystem-header\n"));
        return -1;
    }
    
    if (dico_get_u64(dicofshead, 0, FSYSHEADKEY_BYTESUSED, &fsbytesused)!=0)
    {   errprintf(tr("cannot find FSYSHEADKEY_BYTESUSED in filesystem-header\n"));
        return -1;
    }
    
    if (dico_get_string(dicofshead, 0, FSYSHEADKEY_FSLABEL, fslabel, sizeof(fslabel))<0)
        snprintf(fslabel, sizeof(fslabel), tr("<none>"));
    
    if (dico_get_string(dicofshead, 0, FSYSHEADKEY_ORIGDEV, fsorigdev, sizeof(fsorigdev))<0)
        snprintf(fsorigdev, sizeof(fsorigdev), tr("<unknown>"));
    
    // filesystem uuid: maybe an ntfs uuid or an unix uuid
    snprintf(fsuuid, sizeof(fsuuid), tr("<none>"));
    if (dico_get_u64(dicofshead, 0, FSYSHEADKEY_NTFSUUID, &temp64)==0)
        snprintf(fsuuid, sizeof(fsuuid), "%016llX", (long long unsigned int)temp64);
    else if (dico_get_string(dicofshead, 0, FSYSHEADKEY_FSUUID, buffer, sizeof(buffer))==0 && strlen(buffer)==36)
        snprintf(fsuuid, sizeof(fsuuid), "%s", buffer);
    
    msgprintf(MSG_FORCE, tr("===================== filesystem information ====================\n"));
    msgprintf(MSG_FORCE, tr("Filesystem id in archive: \t%ld\n"), (long)fsid);
    msgprintf(MSG_FORCE, tr("Filesystem format: \t\t%s\n"), fsbuf);
    msgprintf(MSG_FORCE, tr("Filesystem label: \t\t%s\n"), fslabel);
    msgprintf(MSG_FORCE, tr("Filesystem uuid: \t\t%s\n"), fsuuid);
    msgprintf(MSG_FORCE, tr("Original device: \t\t%s\n"), fsorigdev);
    msgprintf(MSG_FORCE, tr("Original filesystem size: \t%s (%lld bytes)\n"), format_size(fsbytestotal, buffer, sizeof(buffer), 'h'), (long long)fsbytestotal);
    msgprintf(MSG_FORCE, tr("Space used in filesystem: \t%s (%lld bytes)\n"), format_size(fsbytesused, buffer, sizeof(buffer), 'h'), (long long)fsbytesused);
    msgprintf(MSG_FORCE, "\n");
    meldungen_uebergeben(fsorigdev,2);
   
    return 0;
}
