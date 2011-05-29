/*
 * qt4-fsarchiver: Filesystem Archiver
 *
 * Copyright (C) 2010, 2011 Dieter Baum.  All rights reserved.
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
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

// nur für msgprintf(MSG_FORCE
#include "dico.h"
#include "dichl.h"
#include "common.h"
#include "oper_save.h"
#include "strlist.h"
#include "filesys.h"
#include "fs_ext2.h"
#include "fs_xfs.h"
#include "fs_reiserfs.h"
#include "fs_reiser4.h"
#include "fs_jfs.h"
#include "fs_btrfs.h"
#include "fs_ntfs.h"
#include "thread_comp.h"
#include "thread_archio.h"
#include "crypto.h"
#include "error.h"
#include <stdio.h>
#include <mntent.h>
#include <sys/stat.h>
#include <sys/vfs.h>
// nur für msgprintf(MSG_FORCE


#include "fsarchiver.h"
#include "connect_c_cpp.h"


void  start_fsa(struct st_argv *args);
float prozent;
float Anzahl_File_zu_sichern;
float Anzahl_File_gesichert;
float h_links;
float EndeThreadMeldung;
float numberfile;
float numberfolder;
float s_links;
float s_links_;
char* key ;
char fsorigdev[30];

int gl_rezult=0, p_work=0;  // global variable

pthread_attr_t at;
pthread_t thread_main;

int createar(){
   bool probedetailed = 0;  //=True Anzeige Label zB C-System
   //bool probedetailed = 1;  //=False Anzeige UUID
   int test = 0;
   //test = oper_probe(probedetailed);
   return test;
}

void  
fsarchiver_aufruf_c(struct st_argv *args)
{
int ret_ph, argc;
 
    argc=args->argc;
    if ((argc < 0) && (argc > 15))
		return; // error !!!
 
//msgprintf(MSG_FORCE, _("fsarchiver_aufruf in connect_cpp %d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n"),argc, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6],argv[7], argv[8], argv[9], argv[10], argv[11], argv[12], argv[13], argv[14]);
//    ret=fsarchiver_main(argc, argv);

//  !! This is main diff
pthread_attr_init(&at);
pthread_attr_setdetachstate(&at,PTHREAD_CREATE_DETACHED );
ret_ph=pthread_create(&thread_main, NULL, (void*) &start_fsa, &args);


return;
}

void
start_fsa(struct st_argv *args)  // Basa function thread
{
int argc, ret;
char *new_argv;

argc= args->argc;
new_argv= args->argv;
gl_rezult=0;
p_work=1; // function is work

ret=fsarchiver_main(argc, &new_argv);
dialog_auswertung=gl_rezult=ret;   // rezult
p_work=0;   // function is not work
}

void werte_uebergeben(float prozess, int auswahl){
     if (auswahl ==1){
         // bei kleinen Verzeichnissen ist das Ende der Sicherung bzw Wiederherstellung bereits erreicht, bevor werte_uebergeben
         // aufgerufen wird. Das bedeutet, dass der prozess auf 0 zurückgesetzt wird.
         // Um dennoch die 99% oder 100% abrufen zu können, musste nachfolgende Abhängigkeit eingeschoben werden.
         if (prozess != 0)
     	     prozent = prozess;
         }
     if (auswahl ==2){
         if (prozess != 0)
     	    Anzahl_File_zu_sichern = prozess;
         }
     if (auswahl ==3)
         if (prozess != 0){ 
     	    Anzahl_File_gesichert = prozess;
         }
     if (auswahl ==4)
         if (prozess != 0){ 
     	    EndeThreadMeldung = prozess;
         }
     if (auswahl ==5)
         if (prozess != 0){ 
     	    h_links = prozess;
         }
     if (auswahl ==6){
         if (prozess != 0)
     	    numberfile = prozess;
         }
     if (auswahl ==7)
         if (prozess != 0){ 
     	    numberfolder = prozess;
         }
     if (auswahl ==8)
         if (prozess != 0){ 
     	    s_links = prozess;
         }
     if (auswahl ==9)
         if (prozess != 0){ 
     	    s_links_ = prozess;
         }
   }

float werte_holen(int auswahl){
      if (auswahl ==1)        
     	return prozent;
      if (auswahl ==2)
     	return Anzahl_File_zu_sichern;
      if (auswahl ==3)
     	return Anzahl_File_gesichert;
      if (auswahl ==4)
     	return EndeThreadMeldung;
      if (auswahl ==5)
     	return h_links;
      if (auswahl ==6)
     	return numberfile;
      if (auswahl ==7)
     	return numberfolder;
      if (auswahl ==8)
     	return s_links;
      if (auswahl ==9)
     	return s_links_;
    }
 
void werte_reset(){
      prozent = 0;        
      Anzahl_File_zu_sichern = 0;
    }

void meldungen_uebergeben(char* meldung, int auswahl){
     if (auswahl ==1) {
         key = meldung;
         }
     if (auswahl ==2) {
     	 strncpy(fsorigdev,meldung,30);
         fsorigdev[29] = 0;
         }  
  }

char *meldungen_holen(int auswahl){
      if (auswahl ==1) {
         return key;
      }
      if (auswahl ==2)  {
        return fsorigdev;
      }
      
}

float df(char *device, const char *mountPoint, int flag )
{
	struct statfs s;
	long blocks_used;
	long blocks_percent_used;

	if (statfs(mountPoint, &s) != 0) {
		perror(mountPoint);
		return 1;
	}

	if (s.f_blocks > 0) {
		blocks_used = s.f_blocks - s.f_bfree;
		blocks_percent_used = (long)
			(blocks_used * 100.0 / (blocks_used + s.f_bavail) + 0.5);
		/*
		printf("%-20s %9ld %9ld %9ld %3ld%% %s\n",
			   device,
			   (long) (s.f_blocks * (s.f_bsize / 1024.0)),
			   (long) ((s.f_blocks - s.f_bfree) * (s.f_bsize / 1024.0)),
			   (long) (s.f_bavail * (s.f_bsize / 1024.0)),
			   blocks_percent_used, mountPoint);
*/
	}
   if (flag == 1)
		return (blocks_percent_used);
   if (flag == 2)
		return (long) (s.f_blocks * (s.f_bsize / 1024000.0));
	if (flag == 3)
		return (long) ((s.f_blocks - s.f_bfree) * (s.f_bsize / 1024000.0));	
	if (flag == 4)
		return (long) (s.f_bavail * (s.f_bsize / 1024000.0));	
			  
}




float freesize(char *Partition,char *mount, int flag)
{
float prozent=df(Partition, mount, flag);
 	return prozent;
}
