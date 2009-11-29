/*
 * fsarchiver: Filesystem Archiver
 *
 * Copyright (C) 2008-2009 Francois Dupoux.  All rights reserved.
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

#ifndef __FSARCHIVER_H__
#define __FSARCHIVER_H__

#include <assert.h>

#include "config.h"
#include "types.h"
#include "error.h"

// ----------------------------- min and max -----------------------------
#if !defined(min)
#	define min(a, b)       ((a) < (b) ? (a) : (b))
#endif

#if !defined(max)
#	define max(a, b)       ((a) > (b) ? (a) : (b))
#endif

// ----------------------------- fsarchiver const ------------------------
#define FSA_VERSION		PACKAGE_VERSION
#define FSA_RELDATE		PACKAGE_RELDATE
#define FSA_FILEFORMAT		PACKAGE_FILEFMT

#define FSA_MAX_FILEFMTLEN	32
#define FSA_MAX_PROGVERLEN	32
#define FSA_MAX_LABELLEN	512
#define FSA_MAX_FSNAMELEN	128
#define	FSA_MAX_DEVLEN		256
#define	FSA_MAX_UUIDLEN		128
#define FSA_MAX_BLKDEVICES	256

#define FSA_MIN_PASSLEN		6
#define FSA_MAX_PASSLEN		64

#define FSA_MAX_FSPERARCH	128
#define FSA_MAX_COMPJOBS	32
#define FSA_MAX_QUEUESIZE	32
#define FSA_MAX_BLKSIZE		921600
#define FSA_DEF_BLKSIZE		262144
#define FSA_MAX_SMALLFILECOUNT	512 // there can be up to FSA_MAX_SMALLFILECOUNT files copied in a single data block 
#define FSA_MAX_SMALLFILESIZE	131072 // files smaller than that will be grouped with other small files in a single data block

#define FSA_FILESYSID_NULL	0xFFFF
#define FSA_CHECKPASSBUF_SIZE	4096

// ----------------------------- fsarchiver magics --------------------------------------------------
#define FSA_SIZEOF_MAGIC	4
#define FSA_MAGIC_VOLH		"FsA0" // volume header (one per volume at the very beginning)
#define FSA_MAGIC_VOLF		"FsAE" // volume footer (one per volume at the very end)
#define FSA_MAGIC_MAIN		"ArCh" // archive header (one per archive at the beginning of the first volume)
#define FSA_MAGIC_FSIN		"FsIn" // filesys info (one per filesystem at the beginning of the archive)
#define FSA_MAGIC_FSYB		"FsYs" // filesys begin (one per filesystem when the filesys contents start, or when the flatfiles start)
#define FSA_MAGIC_OBJT		"ObJt" // object header (one per object: regfiles, dirs, symlinks, ...)
#define FSA_MAGIC_BLKH		"BlKh" // datablk header (one per data block, each regfile may have [0-n])
#define FSA_MAGIC_FILF		"FiLf" // filedat footer (one per regfile, after the list of data blocks)
#define FSA_MAGIC_DATF		"DaEn" // data footer (one per file system, at the end of its contents, or after the contents of the flatfiles)

// ------------ struct that stores the options passed on the command line ---------------------------
typedef struct s_options
{
	bool	overwrite;
	bool	allowsaverw;
	bool	dontcheckmountopts;
	int	verboselevel;
	int	debuglevel;
	int	compresslevel;
	int	compressjobs;
	u16	compressalgo;
	u32	datablocksize;
	u32	smallfilethresh;
	u64	splitsize;
	u16	encryptalgo;
	u16	fsacomplevel;
	u8	encryptpass[FSA_MAX_PASSLEN+1];
} coptions;

// ------------ struct that stores the options passed on the command line ---------------------------
typedef struct s_stats
{
	u64	cnt_regfile;
	u64	cnt_dir;
	u64	cnt_symlink;
	u64	cnt_hardlink;
	u64	cnt_special;
	u64	err_regfile;
	u64	err_dir;
	u64	err_symlink;
	u64	err_hardlink;
	u64	err_special;
} cstats;

// ------------ global variables ---------------------------
extern coptions g_options;
extern char *valid_magic[];
extern int g_logfile;

// -------------------------------- version_number to u64 -------------------------------------------
#define FSA_VERSION_BUILD(a, b, c, d)	((u64)((((u64)a&0xFFFF)<<48)+(((u64)b&0xFFFF)<<32)+(((u64)c&0xFFFF)<<16)+(((u64)d&0xFFFF)<<0)))
#define FSA_VERSION_GET_A(ver)			((((u64)ver)>>48)&0xFFFF)
#define FSA_VERSION_GET_B(ver)			((((u64)ver)>>32)&0xFFFF)
#define FSA_VERSION_GET_C(ver)			((((u64)ver)>>16)&0xFFFF)
#define FSA_VERSION_GET_D(ver)			((((u64)ver)>>0)&0xFFFF)

// -------------------------------- fsarchiver commands ---------------------------------------------
enum	{OPER_NULL=0, OPER_SAVEFS, OPER_RESTFS, OPER_SAVEDIR, OPER_RESTDIR, OPER_ARCHINFO, OPER_PROBE};

// ----------------------------------- dico sections ------------------------------------------------
enum	{DICO_OBJ_SECTION_STDATTR=0, DICO_OBJ_SECTION_XATTR=1, DICO_OBJ_SECTION_WINATTR=2};

// ----------------------------------- archive types ------------------------------------------------
enum	{ARCHTYPE_NULL=0, ARCHTYPE_FILESYSTEMS, ARCHTYPE_DIRECTORIES};

// ----------------------------------- volume header and footer -------------------------------------
enum	{VOLUMEHEADKEY_VOLNUM, VOLUMEHEADKEY_ARCHID, VOLUMEHEADKEY_FILEFORMATVER, VOLUMEHEADKEY_PROGVERCREAT};
enum	{VOLUMEFOOTKEY_VOLNUM, VOLUMEFOOTKEY_ARCHID, VOLUMEFOOTKEY_LASTVOL};

// ----------------------------------- algorithms used to process data-------------------------------
enum	{COMPRESS_NULL=0, COMPRESS_NONE, COMPRESS_LZO, COMPRESS_GZIP, COMPRESS_BZIP2, COMPRESS_LZMA};
enum	{ENCRYPT_NULL=0, ENCRYPT_NONE, ENCRYPT_BLOWFISH};

// ----------------------------------- dico keys ----------------------------------------------------
enum	{OBJTYPE_NULL=0, OBJTYPE_DIR, OBJTYPE_SYMLINK, OBJTYPE_HARDLINK, OBJTYPE_CHARDEV, 
	OBJTYPE_BLOCKDEV, OBJTYPE_FIFO, OBJTYPE_SOCKET, OBJTYPE_REGFILEUNIQUE, OBJTYPE_REGFILEMULTI};

enum	{DISKITEMKEY_NULL=0, DISKITEMKEY_OBJECTID, DISKITEMKEY_PATH, DISKITEMKEY_OBJTYPE, 
	DISKITEMKEY_SYMLINK, DISKITEMKEY_HARDLINK, DISKITEMKEY_RDEV, DISKITEMKEY_MODE, 
	DISKITEMKEY_SIZE, DISKITEMKEY_UID, DISKITEMKEY_GID, DISKITEMKEY_ATIME, DISKITEMKEY_MTIME,
	DISKITEMKEY_MD5SUM, DISKITEMKEY_MULTIFILESCOUNT, DISKITEMKEY_MULTIFILESOFFSET};

enum	{BLOCKHEADITEMKEY_NULL=0, BLOCKHEADITEMKEY_REALSIZE, BLOCKHEADITEMKEY_BLOCKOFFSET, 
	BLOCKHEADITEMKEY_COMPRESSALGO, BLOCKHEADITEMKEY_ENCRYPTALGO, BLOCKHEADITEMKEY_ARSIZE, 
	BLOCKHEADITEMKEY_COMPSIZE, BLOCKHEADITEMKEY_ARCSUM};

enum	{BLOCKFOOTITEMKEY_NULL=0, BLOCKFOOTITEMKEY_MD5SUM};

enum	{MAINHEADKEY_NULL=0, MAINHEADKEY_FILEFORMATVER, MAINHEADKEY_PROGVERCREAT, MAINHEADKEY_ARCHIVEID, 
	MAINHEADKEY_CREATTIME, MAINHEADKEY_ARCHLABEL, MAINHEADKEY_ARCHTYPE, MAINHEADKEY_FSCOUNT, 
	MAINHEADKEY_COMPRESSALGO, MAINHEADKEY_COMPRESSLEVEL, MAINHEADKEY_ENCRYPTALGO, 
	MAINHEADKEY_BUFCHECKPASSCLEARMD5, MAINHEADKEY_BUFCHECKPASSCRYPTBUF, MAINHEADKEY_FSACOMPLEVEL};

enum	{FSYSHEADKEY_NULL=0, FSYSHEADKEY_FILESYSTEM, FSYSHEADKEY_MNTPATH, FSYSHEADKEY_BYTESTOTAL, 
	FSYSHEADKEY_BYTESUSED, FSYSHEADKEY_FSLABEL, FSYSHEADKEY_FSUUID, FSYSHEADKEY_FSINODESIZE, 
	FSYSHEADKEY_FSVERSION, FSYSHEADKEY_FSEXTDEFMNTOPT, FSYSHEADKEY_FSEXTREVISION, 
	FSYSHEADKEY_FSEXTBLOCKSIZE, FSYSHEADKEY_FSEXTFEATURECOMPAT, FSYSHEADKEY_FSEXTFEATUREINCOMPAT,
	FSYSHEADKEY_FSEXTFEATUREROCOMPAT, FSYSHEADKEY_FSREISERBLOCKSIZE, FSYSHEADKEY_FSREISER4BLOCKSIZE, 
	FSYSHEADKEY_FSXFSBLOCKSIZE, FSYSHEADKEY_FSBTRFSSECTORSIZE, FSYSHEADKEY_NTFSSECTORSIZE,
	FSYSHEADKEY_NTFSCLUSTERSIZE, FSYSHEADKEY_BTRFSFEATURECOMPAT, FSYSHEADKEY_BTRFSFEATUREINCOMPAT,
	FSYSHEADKEY_BTRFSFEATUREROCOMPAT, FSYSHEADKEY_NTFSUUID, FSYSHEADKEY_MINFSAVERSION,
	FSYSHEADKEY_MOUNTINFO, FSYSHEADKEY_ORIGDEV};

enum	{FSINFOKEY_NULL=0, FSINFOKEY_FSID, FSINFOKEY_DEST, FSINFOKEY_MKFS};

#endif // __FSARCHIVER_H__