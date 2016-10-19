/*
 * qt5-fsarchiver: Filesystem Archiver
 * 
* Copyright (C) 2008-2016 Dieter Baum.  All rights reserved.
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

#include "mainWindow.h"
#include <string.h>
#include <iostream>
#include <time.h>
#include <sys/times.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <mntent.h>
#include <QtGui>
#include "filedialog.h"
#include "setting.h"
#include "net.h"
#include "net_ein.h"
#include "mbr.h"
#include "clone.h"
#include "dir.h"
#include "qt_probe.h"
extern "C" {
#include "connect_c_cpp.h"
#include "fsarchiver.h"
}
#include <sys/types.h>
#include <unistd.h>
#include "treeview.h"
#include <dirent.h>

using namespace std;

QString Datum_akt("");
QString partition_;
QString UUID;
string partition_str;
QString folder;
QString _Datum;
QString DateiName("") ;
//extern int anzahl_disk;
extern int btrfs_flag;
extern int dialog_auswertung;
extern int show_flag;
extern QString parameter[15];
QString part[100][4];
QString widget[100];
int endeThread;
int sekunde_elapsed;
int minute_elapsed;
int sekunde_summe;
QStringList items_kerne_;
QStringList items_GB;
QString partition_typ_;
QString partition_typ;

QString part_art;
QString zip_[10];
QString SicherungsDateiName;
QString SicherungsFolderFileName;
int dummy_prozent;
int flag_View; 
int flag_end;
int sekunde_tara; 
int thread_run;
pid_t getpid(void);
pid_t getppid(void);
QString pid, pid1;
QString befehl_pbr;
int stopFlag;
int row_1;

extern "C"
{
int fsarchiver_main(int argc, char**argv);
}
int fsarchiver_aufruf(int argc, char *anlage0=NULL, char *anlage1=NULL, char *anlage2=NULL, char *anlage3=NULL, char *anlage4=NULL, char *anlage5=NULL, char *anlage6=NULL, char *anlage7=NULL, char *anlage8=NULL, char *anlage9=NULL, char *anlage10=NULL, char *anlage11=NULL, char *anlage12=NULL, char *anlage13=NULL, char *anlage14=NULL)
{
    char *argv[15];
    int ret;
    argv[0] = anlage0;
    argv[1] = anlage1;
    argv[2] = anlage2;
    argv[3] = anlage3;
    argv[4] = anlage4;
    argv[5] = anlage5;
    argv[6] = anlage6;
    argv[7] = anlage7;
    argv[8] = anlage8;
    argv[9] = anlage9;
    argv[10] = anlage10;
    argv[11] = anlage11;
    argv[12] = anlage12;
    argv[13] = anlage13;
    argv[14] = anlage14;

//    printf(("fsarchiver_aufruf in connect_cpp %d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n"),argc, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6],argv[7], argv[8], argv[9], argv[10], argv[11], argv[12], argv[13], argv[14]);
//    printf("argc", argc);
    ret=fsarchiver_main(argc, argv);
 return ret;
}

MWindow::MWindow()
{
   questionLabel = new QLabel;
   QStringList dummy;
   QStringList partition_kurz;
   QString partition1_;
   QStringList items; 
   int pos = 0, pos1 = 0;
   int i = 0,j = 0, k = 0;
   QString befehl;
   QString homepath = QDir::homePath();
   QString rootpath = QDir::rootPath();
      
   setupUi(this);
   rdBt_saveFsArchiv->setChecked(Qt::Checked);
   dirModel = new QFileSystemModel;
   selModel = new QItemSelectionModel(dirModel);
   treeView->setModel(dirModel);
   treeView->setSelectionModel(selModel);
   treeView->setEnabled(true);
   
   QModelIndex cwdIndex = dirModel->index(QDir::rootPath());
   dirModel->setRootPath(QDir::rootPath());
   treeView->setRootIndex(cwdIndex);
   groupBox->setFlat (true);
   pushButton_restore->setEnabled(false);
   // Signal-Slot-Verbindungen  Werkzeugleiste
   //connect( action_Beenden, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
   connect( action_Beenden, SIGNAL(triggered(bool)), this, SLOT(del_mediafolder()));
   connect( action_partition_save, SIGNAL(triggered(bool)),this, SLOT(save_button()));
   connect( action_partition_restore, SIGNAL(triggered(bool)),this, SLOT(restore_button()));
   connect( action_partition_save_net, SIGNAL(triggered(bool)),this, SLOT(eingabe_net_save()));
   connect( action_partition_restore_net, SIGNAL(triggered(bool)),this, SLOT(eingabe_net_restore()));
   connect( action_Information, SIGNAL(triggered(bool)),this, SLOT(info()));
   connect( action_Einstellungen, SIGNAL(triggered(bool)),this, SLOT(einstellungen()));
   connect( action_mbr_save, SIGNAL(triggered(bool)), this, SLOT(mbr_save()));
   connect( action_mbr_restore, SIGNAL(triggered(bool)), this, SLOT(mbr_restore()));
   connect( action_dir_save, SIGNAL(triggered(bool)), this, SLOT(dir_save()));
   connect( action_dir_restore, SIGNAL(triggered(bool)), this, SLOT(dir_restore()));
   connect( action_CloneDrive, SIGNAL(triggered(bool)), this, SLOT(clone_save()));
   connect( actionClone_HD, SIGNAL(triggered(bool)), this, SLOT(clone_save()));
   connect( action_CloneDrive_net, SIGNAL(triggered(bool)), this, SLOT(clone_save_net()));
   //schließt alle Fenster
   //connect( pushButton_end, SIGNAL( clicked() ), qApp, SLOT(quit()));
   connect( pushButton_end, SIGNAL( clicked() ), this, SLOT(del_mediafolder()));
   connect( pushButton_break, SIGNAL( clicked() ), this, SLOT(esc_end()));
   //schließt das aktuelle (this)Fenster:
   //connect( pushButton_end, SIGNAL( clicked() ), this, SLOT(close()));	
   connect( pushButton_save, SIGNAL( clicked() ), this, SLOT(savePartition()));
   connect( pushButton_restore, SIGNAL( clicked() ), this, SLOT(restorePartition()));
   // pushButton_partition und pushButton_folder sind dummy Button um einen Slot ansprechen zu können
   connect( pushButton_partition, SIGNAL( clicked() ), this, SLOT(listWidget_auslesen()));
   connect( pushButton_folder, SIGNAL( clicked() ), this, SLOT(folder_einlesen()));
   connect( rdBt_saveFsArchiv, SIGNAL( clicked() ), this, SLOT(rdButton_auslesen()));
   connect( rdBt_restoreFsArchiv, SIGNAL( clicked() ), this, SLOT(rdButton_auslesen()));
   connect( chk_key, SIGNAL( clicked() ), this, SLOT(chkkey()));
   connect( chk_hidden, SIGNAL( clicked() ), this, SLOT(chkhidden()));
   connect( chk_split, SIGNAL( clicked() ), this, SLOT(chkGB()));
   // Zeitgeber für Berechnung remainingTime
   timer = new QTimer(this);
 //  timer1->singleShot( 1000, this , SLOT(ViewProzent( )) ;
//   connect(timer1, SIGNAL(timeout()), this, SLOT(ViewProzent());
 //  timer1->start(1000);
   pid_ermitteln();
   items_kerne_ << "1" << "2" << "3" << "4" <<  "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
   cmb_kerne->addItems (items_kerne_);
   items_kerne_.clear();
   items_GB << "2" << "4" << "4,7";
   cmb_GB->addItems (items_GB);
   items_GB.clear();
   zip_[0]=(tr("lzo", "lzo"));
   zip_[1]=(tr("gzip fast","gzip fast"));
   zip_[2]=(tr("gzip standard","gzip standard"));
   zip_[3]=(tr("qzip best","qzip best"));
   zip_[4]=(tr("bzip2 fast","bzip2 fast"));
   zip_[5]=(tr("bzip2 good"," bzip2 good"));
   zip_[6]=(tr("lzma fast","lzma fast"));
   zip_[7]=(tr("lzma medium","lzma medium"));
   zip_[8]=(tr("lzma best","lzma best"));
   items << zip_[0] << zip_[1] << zip_[2] << zip_[3] <<  zip_[4];
   cmb_zip->addItems (items);
   items.clear();
   items << zip_[5] << zip_[6] << zip_[7] << zip_[8];
   cmb_zip->addItems (items);
   items.clear();
   QDir dir1(homepath + "/.config/qt5-fsarchiver");
   if (!dir1.exists()){
      befehl = "mkdir " + homepath + "/.config/qt5-fsarchiver 2>/dev/null" ;
      system (befehl.toLatin1().data());
      }
   QDir dir2(rootpath + "/.config/qt5-fsarchiver");
   if (!dir1.exists()){
      befehl = "mkdir " + rootpath + "/.config/qt5-fsarchiver 2>/dev/null" ;
      system (befehl.toLatin1().data());
      }
   QDir dir(homepath + "/.qt5-fs-client");
   if (!dir.exists()){
       befehl = "mkdir " + homepath + "/.qt5-fs-client 2>/dev/null" ;
       system (befehl.toLatin1().data());
   	//Rechte setzen
       befehl = "chmod a+rwx " + homepath + "/.qt5-fs-client 2>/dev/null";
       system (befehl.toLatin1().data());
   // wegen Suse
   QDir dir2("/media");
   QString media = "/media";
   if (!dir2.exists()){
       befehl = "mkdir " + media + " 2>/dev/null" ;
       system (befehl.toLatin1().data());
       }
   }
   // beim Abbruch einer Sicherung bleiben eventuell Daten in /tmp/fsa erhalten.
   // Bei ersten Start werden eventuell vorhandene Dateien gelöscht.
   // Beim Öffnen einer weiteren Instanz von qt5-fsarchiver, darf /tmp/fsa keinenfalls gelöscht werden.
   // Das System sürzt ab!!
   if (is_running() == 0) {
       befehl = "rm -r -f /tmp/fsa 2>/dev/null"; 
       system (befehl.toLatin1().data());
   }
   // Ini-Datei auslesen
   QFile file(homepath + "/.config/qt5-fsarchiver/qt5-fsarchiver.conf");
   QSettings setting("qt5-fsarchiver", "qt5-fsarchiver");
   if (file.exists()) {
        setting.beginGroup("Basiseinstellungen");
        int auswertung = setting.value("Kompression").toInt();
        cmb_zip -> setCurrentIndex(auswertung); 
        auswertung = setting.value("Kerne").toInt();
        cmb_kerne -> setCurrentIndex(auswertung-1); 
        auswertung = setting.value("overwrite").toInt();
        if (auswertung ==1)
           chk_overwrite->setChecked(Qt::Checked); 
        auswertung = setting.value("pbr").toInt();
        if (auswertung ==1)
           chk_pbr->setChecked(Qt::Checked); 
        auswertung = setting.value("tip").toInt();
        if (auswertung ==1)
           chk_Beschreibung->setChecked(Qt::Checked);  
        auswertung = setting.value("key").toInt();
        if (auswertung ==1)
           chk_key->setChecked(Qt::Checked); 
	auswertung = setting.value("hidden").toInt();
        if (auswertung ==1)
           chk_hidden->setChecked(Qt::Checked);
        auswertung = setting.value("Passwort").toInt(); 
        if (auswertung ==1)
           	lineKey ->setEchoMode(QLineEdit::Normal);
        else
		     lineKey ->setEchoMode(QLineEdit::Password); 
        setting.endGroup();
        } 
   else {
        lineKey ->setEchoMode(QLineEdit::Password);
        cmb_kerne -> setCurrentIndex(0);
        chk_Beschreibung->setChecked(Qt::Checked);
        chk_overwrite->setChecked(Qt::Checked); 
        cmb_zip -> setCurrentIndex(2);
        setting.beginGroup("Basiseinstellungen");
        setting.setValue("showPrg",1); 
        setting.setValue("ssh",1); 
        setting.setValue("sshfs",1); 
        setting.setValue("dummy",2);
        setting.endGroup();
        }
   label_4->setEnabled(false);
   chk_overwrite->setEnabled(true);
   cmb_zip->setEnabled(false);
   cmb_GB->setEnabled(false);
   chk_Beschreibung->setEnabled(true);
   chk_Beschreibung->setChecked(Qt::Checked);
   label->setEnabled(false);
        starteinstellung();
        if (geteuid() == 0) // 0 is the UID of the root  1000 von Dieter
	{
 	qt_probe();
            // Partitionsname, Partitiontyp und Größe einlesen und formatieren für listWidget Ausgabe
           for (i=1; i < 100; i++){
              for (j=0; j < 5; j++){
                 if (PartitionString(i,0) != "") {
                    part[i-1][j] = PartitionString(i,j);
                    //qDebug() << "part[i][j]:" <<  part[i-1][j] << i << j;
                  }
               }
           } 

           format();
           j = 2 ;
           i=0;
           while (j > 1)
	   {
		j = widget[i+1].length();
      partition_ = part[i][0]; // z.B. sda1
		UUID = part[i][3];
                partition_typ = part[i][1];
                pos = partition_typ.indexOf("fat");
                if (pos > -1)
		   partition_typ = "vfat";
                pos = partition_typ.indexOf("ext3");
                pos1 = partition_typ.indexOf("ext4");
                if (pos > -1 || pos1 > -1)
		    partition_typ = "ext";
                partition_ = "/dev/"+ partition_; 
                //Prüfen ob System oder Home Partition
                part_art = mtab_einlesen(partition_); // Partition übergeben
                if (!is_mounted(partition_.toLatin1().data()))
                   { 
                   if (part_art !="system" || part_art != "home")
                     {
			if (partition_typ == "ext" || partition_typ == "btrfs"|| partition_typ == "vfat" || partition_typ ==  "ntfs" )
                   		{
                   	 	char mountpoint[100] = "/media/";
                         	strcat (mountpoint , part[i][0].toLatin1().data());
                                mkdir(mountpoint, 0777);
                         	char mountpoint1[100] = "mount ";
                         	strcat (mountpoint1,partition_.toLatin1().data()); 
			 	strcat (mountpoint1, " ");
			 	strcat (mountpoint1, mountpoint);
			 	k = system (mountpoint1);
                         	}
                        }
 		     }
                i++;
        	if (i > 99)
           	{
           	break;
           	}
           }
        partition_ = "" ;
      	}
        setting.beginGroup("Basiseinstellungen");
        int auswertung = setting.value("showPrg").toInt();
        setting.endGroup();
        if (auswertung ==1 and show_flag == 0){
        int ret = questionMessage(tr("In the file /usr/share/doc/qt5-fsarchiver/doc/Readme are instructions included on the use of the program. Should this continue to be displayed? You can change this in the basic settings.", "In der Datei /usr/share/doc/qt5-fsarchiver/doc/Liesmich sind Hinweise zur Nutzung des Programms enthalten. Wollen Sie diesen Hinweis weiterhin sehen? Sie können dies in den Basiseinstellungen ändern."));
    		if (ret == 2){
		//Basiseinstellungen ändern
        	QSettings setting("qt5-fsarchiver", "qt5-fsarchiver");
        	setting.beginGroup("Basiseinstellungen");
        	setting.setValue("showPrg",0);
        	setting.endGroup();
                }
        show_flag = 1;
	} 
}

void MWindow::chkkey(){
     Qt::CheckState state;
     state = chk_key->checkState();
     if (state == Qt::Checked)
        lineKey->setEnabled(true);
     else 
        lineKey->setEnabled(false);

}

void MWindow::chkhidden(){
     Qt::CheckState state;
     state = chk_hidden->checkState();
     if (rdBt_saveFsArchiv->isChecked())
        starteinstellung();   
     else
        rdButton_auslesen();
}


void MWindow::chkGB(){
     Qt::CheckState state;
     state = chk_split->checkState();
     if (state == Qt::Checked)
        cmb_GB->setEnabled(true);
     else 
        cmb_GB->setEnabled(false);
}

void MWindow::save_button(){
     rdBt_saveFsArchiv->setChecked(Qt::Checked);
     rdButton_auslesen();
}

void MWindow::restore_button(){
     rdBt_restoreFsArchiv->setChecked(Qt::Checked);
     rdButton_auslesen();
}

void MWindow::rdButton_auslesen()
     {
     Qt::CheckState state;
     state= chk_hidden->checkState();
     QStringList filters;
     if (rdBt_saveFsArchiv->isChecked())
        {
	starteinstellung();
        }
     if (rdBt_restoreFsArchiv->isChecked())
        {
		label_folder->setText (tr("Backup File", "Sicherungsdatei"));
      		pushButton_restore->setText (tr("Partition restore", "Partition zurückschreiben"));
                pushButton_restore->setEnabled(true);
                pushButton_save->setEnabled(false);
                lineEdit_DateiName->setEnabled(false);
                label_4->setEnabled(false);
                chk_overwrite->setEnabled(false);
                cmb_zip->setEnabled(false);
                chk_Beschreibung->setEnabled(false);
                label->setEnabled(false);
                label_2->setEnabled(false);
                label_3->setEnabled(false);
                AnzahlsaveFile->setEnabled(false);
                AnzahlgesicherteFile->setEnabled(false);
                chk_key->setText (tr("Decrypt\nbackup. key:",
		 "Sicherung\nentschlüsseln. Schlüssel")); 
                chk_split->setEnabled(false); 
		chk_pbr->setEnabled(false); 
                filters << "*.fsa";
                if (state == Qt::Checked)
               	        dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
	    	else	
	       		dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
   		dirModel->setNameFilters(filters);  
         }
     } 

void MWindow::starteinstellung()
     {
     Qt::CheckState state;
     state= chk_hidden->checkState();
     QStringList filters;
            label_folder->setText (tr("Backup directory", "Sicherungsverzeichnis"));
           // pushButton_save->setText (tr("Save partition", "Partition sichern"));
            lineEdit_DateiName->setEnabled(true);
            pushButton_restore->setEnabled(false);
            pushButton_save->setEnabled(true);
            label_4->setEnabled(true);
            chk_overwrite->setEnabled(true);
            cmb_zip->setEnabled(true);
            chk_Beschreibung->setEnabled(true);
            label->setEnabled(true);
            label_2->setEnabled(true);
            label_3->setEnabled(true);
            AnzahlsaveFile->setEnabled(true);
            AnzahlgesicherteFile->setEnabled(true);
            chk_key->setText (tr("Encrypt\nbackup. key:", "Sicherung\nverschlüsseln. Schlüssel"));
            chk_split->setEnabled(true); 
            chk_pbr->setEnabled(true);  
            chkkey(); 
            filters << "*.*";
            if (state == Qt::Checked){
              if (rdBt_saveFsArchiv->isChecked())
		dirModel->setFilter(QDir::AllDirs  | QDir::NoDotAndDotDot | QDir::Hidden);
              else 
   	        dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
	    }		
	    if (state != Qt::Checked){
              if (rdBt_saveFsArchiv->isChecked())
		dirModel->setFilter(QDir::AllDirs  | QDir::NoDotAndDotDot);
              else 
   	        dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
	    }
   	    dirModel->setNameFilters(filters);  
            }
       
int MWindow::savePartition() 
{
     QFile file(folder);
     QString Befehl;
     Qt::CheckState state;
     Qt::CheckState state1;
     Qt::CheckState state3;
     char * part_;
     int err = 0;
     QString keyText = "";
     int found = 0; 
     int zip;

     indicator_reset();
     if (rdBt_saveFsArchiv->isChecked())
     {
        
     	if (folder == "")
           {
          	QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("Please select from the backup directory.\n", "Bitte wählen Sie das Sicherungsverzeichnis aus.\n"));
		return 0 ;
           }
        if (partition_ == "")
           {
          	QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("Please select the partition to be", "Bitte wählen Sie die zu sichernde Partition aus.\n"));
		return 0 ;
           }
        DateiName = lineEdit_DateiName->text();
        if (DateiName == "")
           {
          	QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("Please select the file name of the backup.\n", 
		"Bitte wählen Sie den Dateinamen der Sicherung aus.\n"));
		return 0 ;
           }
        //Überprüfen ob im Dateinamen "/" enthalten ist. Wird durch "-" ersetzt
        while (found > -1){
          found = DateiName.indexOf("/");
          if (found > -1)
             	DateiName.replace(found, 1, "-"); 
        }
        if (file.open(QIODevice::ReadOnly))
        	  {
                QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("You have selected a file. You must select a directory\n",
		 "Sie haben eine Datei ausgewählt. Sie müssen ein Verzeichnis auswählen\n"));
		file.close();
		return 0 ;
 	        }
 	 Zeit_auslesen();
         state= chk_Beschreibung->checkState();
         //Überprüfung ob gemounted
         part_ = partition_.toLatin1().data();
         char  dev_[50] = "/dev/";
	 strcat (dev_ , part_);  // String zusammenfügen: dev_  = dev_ + part_
	 int liveFlag = 0;
         zip = cmb_zip->currentIndex()+1;
         if (zip==0) 
            zip = 1; 
         QString dummy = partition_;
	 // Werte sammeln und nach file_dialog übergeben, Partition ist eingehängt
         part_art = mtab_einlesen("/dev/" + partition_);
         int row = listWidget->currentRow();
       	 beschreibungstext("/dev/" + partition_, DateiName + "-" + _Datum + ".fsa", zip, row);
         partition_ = dummy;
         if (is_mounted(dev_)) 
            {
		   part_art = mtab_einlesen("/dev/" + partition_);
                   //Überprüfung ob System oder Home Partition
                   int ret = 1; 
                   if (part_art == "system")
                	{
                	ret = questionMessage(tr("To back up the system partition is mounted. Do you want to do a live backup?", "Die zu sichernde Systempartition ist eingehängt. Wollen Sie eine Live-Sicherung durchführen?"));
                        if (ret == 2)
                           return 0;
                        if (ret == 1)
                           liveFlag = 1;
                	}
                  if (part_art == "home")
                	{
                	ret = questionMessage(tr("To back up home partition is mounted. Do you want to do a live backup?", "Die zu sichernde Homepartition ist eingehängt. Wollen Sie eine Live-Sicherung durchführen?"));
                        if (ret == 2)
                           return 0;
                        if (ret == 1)
                           liveFlag = 1;
                	}
                   if (part_art != "system" || part_art != "home")
                	{
                           //char * part_= new char [part_art.size()+1];  //Umwandlung von string nach char
                           //strcpy (part_, part_art.c_str()); 
                           char umountpoint[100] = "umount ";
                           strcat (umountpoint,dev_); 
                           //cout << umountpoint << endl;
                          if (liveFlag == 0)
                         	err = system (umountpoint);                              
                           if (err != 0 && liveFlag == 0)
                                {
				QMessageBox::about(this,tr("Note", "Hinweis"),
         			tr("The partition", "Die Partition ")   + partition_ + tr("can not be unmounted. The program is terminated\n", " kann nicht ausgehängt werden. Das Programm wird abgebrochen\n"));
                                return 0 ; 
                                }  
                 	}
                }
         		if (rdBt_saveFsArchiv->isChecked())
           		{
                                int indizierung;
                                state = chk_overwrite->checkState();
                                state1 = chk_key->checkState(); 
                                state3 = chk_split->checkState();
                                keyText = lineKey->text();
                                parameter[0] = "fsarchiver";
       				parameter[1] = "savefs"; 
                                zip = cmb_zip->currentIndex()+1;
                                QString compress = QString::number(zip);
                                compress = "-z" + compress;
		                if (zip == 0)
            				parameter[2] = "-z3"; 
        			else  {
            				parameter[2] = compress;
                                      }
                                int kerne = cmb_kerne->currentIndex()+1;
                                QString index = QString::number(kerne);
                                if (index == "0")
                                    index = "1";
                                parameter[3] = "-j" + index;
                                indizierung = 4;
                                if (state == Qt::Checked) //Überschreiben ja
                                    {
                                    parameter[4] = "-o";
                                    indizierung = 5;
				    }	
                                 if (state1 == Qt::Checked)   //Schlüssel ja
                                    {     
                                    parameter[indizierung] = "-c";
	    			    parameter[indizierung + 1] = keyText;
                                    int len = parameter[indizierung + 1].size();
               				if (len < 6 || len > 64) {
                  				QMessageBox::about(this,tr("Note", "Hinweis"),
         	  				tr("The key length must be between 6 and 64 characters\n", "Die Schlüssellänge muss zwischen 6 und 64 Zeichen sein\n"));
                  				return 0 ; 
               				        }
                                     indizierung = indizierung + 2;  
				     }
				 QString save_part_art = identify_save_part(folder);
                                 int GB = cmb_GB->currentIndex()+1;
                                 QString splitsize = "1900"; 
                                 if (GB == 1 )
                                    splitsize = "1900";
                                 if (GB == 2) 
                                    splitsize = "3800"; 
				 if (GB == 3) 
                                    splitsize = "4400";
				 if (state3 == Qt::Checked)   //Archiv splitten 
                                     {
                                      parameter[indizierung] = "-s " + splitsize;
                                      indizierung = indizierung + 1;
                                      }
	                         // wenn Archiv splitten gewählt wurde und eine FAT-Partition entdeckt wird, hat der nachfolgende Code Vorrang
				 if (save_part_art == "vfat")   //Archivdatei auf 4 GB begrenzen splitten 
                                     {
                                      if (state3 == Qt::Checked)
					 indizierung = indizierung - 1;
                                      if (splitsize == "4400")
                                         splitsize = "3800"; 
              			      parameter[indizierung] = "-s " + splitsize;
                                      indizierung = indizierung + 1;
				      }
                                  if (liveFlag == 1)
                                      {
                                      parameter[indizierung] = "-A";
	    			      parameter[indizierung + 1] = "-a";
                                      indizierung = indizierung + 2; 
                                      }
                                 QString partitionsart = part[row_1][1];
                                 QString partitionsart_ = widget[row_1];
                                 QFile file_suse(".snapshots");
                                 if (file.exists())
                                    {
                                    parameter[indizierung] = "--exclude=.snapshots";
                                    indizierung = indizierung + 1;
//qDebug() << "snapshot wird ausgeschlossen";
                                    }
       				 int pos_a = partitionsart_.indexOf("ntfs");
                		 if (pos_a > -1)
		                    {
				  // Windows Auslagerungsdatei swapfile.sys  und hiberfil.sys von der Sicherung immer ausschließen
                                  parameter[indizierung] = "--exclude=swapfile.sys";
                                  indizierung = indizierung + 1;
                                 // parameter[indizierung] = "--exclude=hyberfil.sys";
                                 // indizierung = indizierung + 1;
				     }

                                 parameter[indizierung] = (folder + "/" + DateiName + "-" + _Datum + ".fsa");
				// Vorbereitung für psb
				QString befehl = ("dd if=/dev/" + partition_ + " of=" + folder + "/" + DateiName + "-" + _Datum + ".pbr" + " bs=512 count=1");
				state = chk_pbr->checkState();
				if (state == Qt::Checked)
					int  i = system (befehl.toLatin1().data());
                                  SicherungsFolderFileName = parameter[indizierung];
                                  parameter[indizierung + 1] = ("/dev/" + partition_);
                                  QFile f(parameter[indizierung]);
                                  if  (parameter[4] != "-o" && (f.exists())){
				       QMessageBox::about(this, tr("Note", "Hinweis"), 
				       tr("The partition file", "Die Partitionsdatei") + parameter[indizierung] + tr(" already exists. The backup is not performed\n", " ist bereits vorhanden. Die Sicherung wird nicht durchgeführt\n") );
                  			  return 0 ; 
               				}
                                  state = chk_Beschreibung->checkState(); 
				  if (rdBt_saveFsArchiv->isChecked() && (state == Qt::Checked))
            			  {
			 	  extern int dialog_auswertung;
             			  dialog_auswertung = 2;
             			  folder_file();
             			  FileDialog Filedialog;
     	     			  FileDialog *dlg = new FileDialog;
     	     			 // dlg->show(); nicht modal
             			  int wert = dlg->exec();
             			 if (wert == 0 && dialog_auswertung == 2)
                		  {
                		  QMessageBox::about(this,tr("Note", "Hinweis"),
         			  tr("The backup was aborted by the user\n", "Die Sicherung wurde vom Benutzer abgebrochen\n"));
				  pushButton_save->setEnabled(true);
                		  return 0;
                		  }
             			}
   
//qDebug() << "Befehl" << parameter[0] << parameter[1] << parameter[2] << parameter[3] << parameter[4] << parameter[5] << parameter[6] << parameter[7] << parameter[8] << indizierung + 2;
				thread1.setValues(indizierung + 2,"0"); 
                                pushButton_end->setEnabled(false);  
                                pushButton_save->setEnabled(false); 
                                flag_View = 1;
                                werte_reset();
  				timer->singleShot( 1000, this , SLOT(ViewProzent( ))) ;
                                stopFlag = 0;
                                this->setCursor(Qt::WaitCursor);
  				startThread1(); // fsarchiver wird im Thread ausgeführt
                                statusBar()->showMessage(tr("The backup is performed", "Die Sicherung wird durchgeführt"), 15000);
        		        }
                    }
           return 0;

}

void MWindow::ViewProzent()
{
int prozent = 0 ;
QString sekunde;
int sekunde_ = 0;
QString minute;
int minute_ = 0;
int meldung = 0;
int anzahl = 0;
int anzahl1 = 0;
QString text_integer;
//if (endeThread !=1)
if (endeThread ==0)
{
 timer->singleShot( 1000, this , SLOT(ViewProzent( )) ) ;
  elapsedTime();
  this->repaint();
  meldung = werte_holen(4);
  	if (meldung >= 100) // Thread Abschluss mit Fehler
   		return;
if (flag_View == 1)
	{
 	anzahl  = werte_holen(2);
 	text_integer = text_integer.setNum(anzahl);
 	AnzahlsaveFile ->setText(text_integer);
 	anzahl1  = werte_holen(3);
 	text_integer = text_integer.setNum(anzahl1);
 	AnzahlgesicherteFile ->setText(text_integer);
	}
 prozent = werte_holen(1);
 if (dummy_prozent != prozent)
     remainingTime(prozent);
 else {
        if (prozent >= 5)
       {
        // Sekunden nach unten zählen
        // SekundenRemaining einlesen
        sekunde = SekundeRemaining->text();
        sekunde_ = sekunde.toInt();
        minute = MinuteRemaining->text();
        minute_ = minute.toInt();
        if (sekunde_ > 0)
        {
            sekunde_ = sekunde_ - 1;
            if (sekunde_ == 0) 
            {
		if (minute_ > 0)
                {		
			minute_ = minute_ - 1;
                	minute = QString::number(minute_); 
        		MinuteRemaining ->setText(minute);
                       	sekunde_ = 59;
                }
	    }
        }		
        sekunde = QString::number(sekunde_); 
        SekundeRemaining ->setText(sekunde);
	    } 
    }   
 dummy_prozent = prozent;
 }
// bei mehrmaligem Aufruf von fsarchiver wird am Anfang der Sicherung kurzfristig 100 % angezeigt, was falsch ist
 if (prozent != 100 && stopFlag == 0) 
     	progressBar->setValue(prozent);
    return;
} 


int MWindow::restorePartition()
{
Qt::CheckState state;
Qt::CheckState state1;
QFile file(folder);
QString DateiName("") ;
char * part_;
int err;
QString keyText = "";
char * dev_part;
int y = 0;

  indicator_reset();
  if (rdBt_restoreFsArchiv->isChecked())
      string dateiname;
      int pos;
      int pos1;
      keyText = lineKey->text();
      state1 = chk_key->checkState();
      {
          if (state1 == Qt::Checked && keyText.isEmpty())  
              {
                QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("There was no key to decrypt specified.", "Es wurde kein Schlüssel für die Entschlüsselung angegeben.\n"));
		return 1 ;
               }
           if (folder == "")
           {
          	QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("Please select the file name of the backup.\n", "Bitte wählen Sie den Dateinamen der Sicherung aus.\n"));
                return 0;
           }
           if (partition_ == "")
           {
          	QMessageBox::about(this,tr("Note","Hinweis"),
         	tr("Please select from the writing back to partition.\n", "Bitte wählen Sie die zurück zu schreibende Partition aus.\n"));
                return 0;
           }
   	   if (file.open(QIODevice::ReadOnly))
        	{
               	if (rdBt_restoreFsArchiv->isChecked())
           		{
                        pos = testDateiName(".fsa"); 
                        pos1= testDateiName("part.fsa");
         		if (pos > 0 && pos1 >0)
         			   {
           			   QMessageBox::about(this, tr("Note", "Hinweis"),
         			   tr("You have chosen the wrong recovery file selected. \nThe files should end with. fsa be", "Sie haben eine falsche Wiederherstellungsdatei ausgesucht ausgesucht \nDie Dateiendung muss .fsa sein"));
                                   return 0;
         			   }
             		}
 		}
           	
	     else
                {
                QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("You have selected a directory. You must select a file\n", "Sie haben ein Verzeichnis ausgewählt. Sie müssen eine Datei auswählen\n"));
                return 0;
                 }
               state = chk_Beschreibung->checkState();
            // Archinfo einholen um Originalpartition einzulesen und um Verschlüsselung zu überprüfen
            // Annahme zunächst kein Schlüssel
               char * optionkey;
               parameter[0] = "fsarchiver";
       	       parameter[1] = "archinfo";
               if (state1 != Qt::Checked) {
               		parameter[2] = folder;
               		fsarchiver_aufruf(3,parameter[0].toLatin1().data(),parameter[1].toLatin1().data(),parameter[2].toLatin1().data(),parameter[3].toLatin1().data());
               		optionkey = meldungen_holen(1);
               		dev_part = meldungen_holen(2);
                        if (werte_holen(4) == 103){
                  		chk_key->setChecked(Qt::Checked);
                                lineKey->setEnabled(true);
                  		QMessageBox::about(this,tr("Note", "Hinweis"),
         	     		tr("The partition is encrypted. Please enter the key", "Die Partition ist verschlüsselt. Bitte geben Sie den Schlüssel ein\n"));
                   		return 0;
               		} 
        	}
                if (state1 == Qt::Checked) {
            		parameter[2] = "-c";
                        parameter[3] = keyText;
                        if (parameter[3].size() < 6 || parameter[3].size() > 64) {
                  		QMessageBox::about(this,tr("Note", "Hinweis"),
         	  		tr("The key length must be between 6 and 64 characters\n", "Die Schlüssellänge muss zwischen 6 und 64 Zeichen sein\n"));
                  		return 0 ; 
               	   		}
			parameter[4] = folder;
                        int retour =  fsarchiver_aufruf(5,parameter[0].toLatin1().data(),parameter[1].toLatin1().data(),parameter[2].toLatin1().data(),parameter[3].toLatin1().data(),parameter[4].toLatin1().data (),parameter[5].toLatin1().data());
                        if ( werte_holen(4) == 103 && retour != 0){
                           QMessageBox::about(this, tr("Note", "Hinweis"), tr("You have entered an incorrect password.", "Sie haben ein falsches Passwort eingegeben. \n"));
           		   lineKey->setText ("");
                           return 0;
                        }
			optionkey = meldungen_holen(1);
               		dev_part = meldungen_holen(2);
                }
            //Überprüfen, ob in die Originalpartition zurückgeschrieben werden soll
            part_ = partition_.toLatin1().data();
            char  dev_[50] = "/dev/";
	    strcat (dev_ , part_);  // String zusammenfügen: dev_  = dev_ + part_
            // Hier wird verglichen dev_ = die Partition, die zu sichern ist. dev_part = Originalpartition
            int cmp = strcmp ( dev_ , dev_part );
            if (cmp != 0){
               // char in QString wandeln
               QString str, str1;
	       str = dev_part;
               str1 = dev_;
               cmp = questionMessage(tr("Partition to restore the ", "Die wiederherzustellende Partition ") + str1 + 
               tr(" does not coincide with the saved  ", " stimmt nicht mit der gesicherten ") + str + tr("Do you want to continue restore?", " überein. Wollen Sie trotzdem die Wiederherstellung durchführen?"));
               if (cmp == 2)  //nicht wiederherstellen
                  return 0;
            }
           if (rdBt_restoreFsArchiv->isChecked())
            	{
                extern QString folder_file_;
                folder_file();	
		QString filename = folder_file_;
                pos = filename.indexOf("fsa");
        	filename = filename.left(pos);
        	filename.insert(pos, QString("txt"));
                QFile f(filename);
                // Prüfen ob text-Datei vorhanden 
		if (f.exists())
                    {
           		extern int dialog_auswertung;
                       	dialog_auswertung = 3;
              		FileDialog Filedialog;
     	       		FileDialog *dlg = new FileDialog;
     	     		// dlg->show(); nicht modal
             		int wert = dlg->exec();
             		if (wert == 0 && dialog_auswertung == 3)
                	   {
                	   QMessageBox::about(this,tr("Note","Hinweis"),
         		   tr("The restore was aborted by the user", "Das Zurückschreiben wurde vom Benutzer abgebrochen\n"));
                            pushButton_restore->setEnabled(true);
                	   return 0;
                	   }
                       }
		// auf pbr-Datei prüfen
       		pos = filename.indexOf("txt");
       		filename = filename.left(pos);
       		filename.insert(pos, QString("pbr"));
       		QFile f1(filename);
		if (f1.exists())
       			{
 			cmp = questionMessage(tr("There is a file with the contents of the partition boot sector present. Do you want to perform the restoration of the PBR?", "Es ist eine Datei mit dem Inhalt des Partition Boot Sektors vorhanden. Wollen Sie die Wiederherstellung des PBR durchführen?"));
    			if (cmp == 1)  // wiederherstellen
        			befehl_pbr = "dd if="+ filename + " of=/dev/" + partition_ + " bs=512 count=1";
		       }
            	  }
	   if (is_mounted(dev_)) 
              {
              part_art = mtab_einlesen("/dev/" + partition_);
              //Überprüfung ob System oder Home Partition 
                   if (part_art == "system")
                	{
                        QMessageBox::about(this,tr("Note", "Hinweis"),
         			tr("Restore the system partition is mounted and can not be written back. Please use a live CD.", "Die wiederherzustellende Systempartition ist eingehängt und kann nicht zurückgeschrieben werden. Benutzen Sie bitte eine Live-CD\n"));
				return 0;
				      } 
                        
                   if (part_art == "home")
                	{
                        QMessageBox::about(this,tr("Note", "Hinweis"),
         			tr("Restore the home partition is mounted and can not be written back. Please use a live CD.","Die wiederherzustellende Homepartition ist eingehängt und kann nicht zurückgeschrieben werden. Benutzen Sie bitte eine Live-CD\n"));
				return 0; 
                        
                	}
                   if (part_art != "system"|| part_art != "home")
                	{
                          char umountpoint[100] = "umount ";
                          strcat (umountpoint,dev_); 
                          err = system (umountpoint);  
                          if (err != 0)
                                {
				QMessageBox::about(this,tr("Note", "Hinweis"),
         			tr("The partition ", "Die Partition ") + partition_ + tr(" can not be unmounted. The program is terminated", " kann nicht ausgehängt werden. Das Programm wird abgebrochen\n"));
                                return 0 ; 
                                }  
                          }
		   
                                
                }
           if (rdBt_restoreFsArchiv->isChecked())
           {
		if (partition_typ_ == "btrfs"){
                      cmp = questionMessage(tr("If the existing btrfs partition to be formatted? The new UUID is defined here. If you choose <no>, the UUID remains preserved.", "Soll die vorhandene btrfs-Partition formatiert werden? Die UUID wird dabei neu festgelegt. Wenn Sie  nein wählen, bleibt die UUID erhalten."));
    			if (cmp == 2) { // nicht formatieren
				// flag setzen in btrfs.c
                                btrfs_flag = 1;
        			// Partition muss gelöscht werden
                                // Partition mounten, damit sie gelöscht werden kann
				QString befehl = "mkdir /tmp/btrfs";
				y =  system (befehl.toLatin1().data());
                                befehl = "mount /dev/" + partition_ + " /tmp/btrfs";
                                y =  system (befehl.toLatin1().data());
				//der Inhalt von tmp/btrfs und somit der Inhalt der btrfs Partition wird gelöscht
				befehl = "rm -r -f /tmp/btrfs 2> /dev/null"; 
 				y =  system (befehl.toLatin1().data());
				befehl = "umount /dev/" + partition_;
 				y =  system (befehl.toLatin1().data());
				//tmp/btrfs wird gelöscht
				befehl = "rm -r -f /tmp/btrfs";
 				y =  system (befehl.toLatin1().data());
		       } }             
	       QString keyText = lineKey->text();
               state1 = chk_key->checkState(); 
	       parameter[0] = "fsarchiver";
       	       parameter[1] = "restfs"; 
               int kerne = cmb_kerne->currentIndex()+1;
               QString index = QString::number(kerne);
               if (index == "0")
                  index = "1";
               parameter[2] = "-j" + index;
               parameter[3] = folder;
               parameter[4] = "id=0,dest=/dev/" + partition_;
               thread2.setValues(5,"0");       
               if (state1 == Qt::Checked) { // Verzeichnis mit Schlüssel gesichert
                 thread2.setValues(7,"0");  
                 parameter[3] = "-c";
                 parameter[4] = keyText;
                 int len = parameter[4].size();
               	    if (len < 6 || len > 64) {
                  	QMessageBox::about(this,tr("Note", "Hinweis"),
         	  	tr("The key length must be between 6 and 64 characters.", "Die Schlüssellänge muss zwischen 6 und 64 Zeichen sein.\n"));
                  	return 0 ; 
               	   }
                 parameter[5] = folder;
                 parameter[6] = "id=0,dest=/dev/" + partition_;
                 }
//qDebug() << "befehl" << parameter[0] << parameter[1] << parameter[2] << parameter[3] << parameter[4] << parameter[5] << parameter[6] <<parameter[7];
               werte_reset();
               pushButton_restore->setEnabled(false);
               pushButton_end->setEnabled(false); 
               flag_View = 2;
  	       timer->singleShot( 1000, this , SLOT(ViewProzent( ))) ;
               stopFlag = 0;
               this->setCursor(Qt::WaitCursor);
               startThread2(); // fsarchiver wird im Thread ausgeführt
               statusBar()->showMessage(tr("The restoring is performed", "Die Wiederherstellung wird durchgeführt"), 15000); 
               }
     }
  return 0;              
}   

QString MWindow::Zeit_auslesen(){
    int Tag;
    int Monat;
    int Jahr;
    QString Datum_akt;
    time_t Zeitstempel;
    tm *nun;
    Zeitstempel = time(0);
    nun = localtime(&Zeitstempel);
    Tag = nun->tm_mday;
    Monat = nun->tm_mon+1;
    Jahr = nun->tm_year+1900;
    _Datum = Datum_akt.setNum(Tag) + "-" ; 
    _Datum = _Datum + Datum_akt.setNum(Monat) + "-" ;
    _Datum = _Datum + Datum_akt.setNum(Jahr) ;
    return _Datum;

}

void MWindow::listWidget_auslesen() {
//QItemSelectionModel * QAbstractItemView::selectionModel () const
//QModelIndexList QItemSelectionModel::selectedIndexes () const
    int row;
    row = listWidget->currentRow();
    row_1 = row;
    partition_ = widget[row];
    int pos = partition_.indexOf("btrfs");
    if (pos > 0)
       partition_typ_ = "btrfs";
    partition_ = part[row][0]; // z.B. sda1
    UUID = part[row][3];
    
}

QString MWindow::UUID_auslesen(int row){
    UUID = part[row][3]; 
    return part[row][3];
}	

void MWindow::folder_einlesen() {
   QModelIndex index = treeView->currentIndex();
   QModelIndexList indexes = selModel->selectedIndexes();
   folder =  (dirModel->filePath(index));
}

void MWindow::folder_file() {
   extern QString folder_file_;
   folder_file_ = folder + "/" + DateiName + "-" + _Datum + ".txt";
}

void MWindow::info() {
   QMessageBox::information(
      0, tr("qt5-fsarchiver"),
      tr("Backup and restore partitions, directory and MBR\nVersion 0.8.0-3, October 31, 2016",
	 "Sichern und Wiederherstellen von Partitionen, Verzeichnissen und MBR Version 0.8.0-3, 31. Oktober 2016"));
      }

int MWindow::Root_Auswertung(){
       if (geteuid() != 0) // 0 is the UID of the root  1000 von Dieter
	{
      QMessageBox::about(this, tr("Note", "Hinweis"),
         tr("The program must be run with root privileges. The program is terminated.\n", "Das Programm muss mit Root-Rechten gestartet werden. Das Programm wird beendet.\n"));
      return 10;
	}
	else	
	{
        return 20;
    } 
}

int MWindow::is_running(){
      QString homepath = QDir::homePath();
      QString running;
      QString befehl;
      QDir dir1(homepath + "/.config/qt5-fsarchiver");
      if (dir1.exists()){  //vermeidet Fehlermeldung beim ersten Start
          befehl = "ps --user root | grep -w qt5-fsarchiver 1> " +  homepath + "/.config/qt5-fsarchiver/running.txt";
          system (befehl.toLatin1().data());
      }
      QString filename = homepath + "/.config/qt5-fsarchiver/running.txt";
      QFile file(filename);
      int zaehler = 0;
      int i = 0;
      if( file.open(QIODevice::ReadOnly|QIODevice::Text)) {
           QTextStream ds(&file);
           for (i=1; i < 5; i++){
           	running = ds.readLine();
		if (running.indexOf("qt5-fsarchiver") > 0) 
                   zaehler = zaehler + 1;
           } }
           file.close();
           befehl = "rm " + filename + " 2>/dev/null";
           system (befehl.toLatin1().data());
           if (zaehler > 1) {
               // qt5-fsarchiver läuft bereits in einer Instanz 
               return 2;
           }
           return 0; 
}

int MWindow::testDateiName(string endung)
{
  string str (folder.toLatin1().data());
  size_t found;
  // different member versions of find in the same order as above:
  found=str.find(endung);
  if (found!=string::npos)
     {
        return found;
     }
  else
     {
        return 0;
     } 
}

int MWindow::is_mounted (char* dev_path) {
 	FILE * mtab = NULL;
 	struct mntent * part = NULL;
 	int is_mounted = 0;
 
 	if ( ( mtab = setmntent ("/etc/mtab", "r") ) != NULL) {
  	while ( ( part = getmntent ( mtab) ) != NULL) {
   	if ( ( part->mnt_fsname != NULL ) 
    	&& ( strcmp ( part->mnt_fsname, dev_path ) ) == 0 ) 
    		return  1;
    	if ( ( part->mnt_fsname != NULL ) 
    	&& ( strcmp ( part->mnt_fsname, UUID.toLatin1().data() ) ) == 0 ) 
    		return 1;	
   
  }
  endmntent (mtab);
 }
 return 0;
}

int MWindow::questionMessage(QString frage)
{
	QMessageBox msg(QMessageBox::Question, tr("Note", "Hinweis"), frage);
	QPushButton* yesButton = msg.addButton(tr("Yes", "Ja"), QMessageBox::YesRole);
	QPushButton* noButton = msg.addButton(tr("No", "Nein"), QMessageBox::NoRole);
	msg.exec();
	if (msg.clickedButton() == yesButton)
    		return 1;
	else if (msg.clickedButton() == noButton)
    		return 2;
}


void MWindow::mbr_save () {
     extern int dialog_auswertung;
     dialog_auswertung = 4;
     DialogMBR *dialog = new DialogMBR;
     dialog->show();
}
void MWindow::mbr_restore () {
      extern int dialog_auswertung;
      dialog_auswertung = 5;
      DialogMBR *dialog = new DialogMBR;
      dialog->show();
}

void MWindow::dir_save () {
     extern int dialog_auswertung;
     dialog_auswertung = 4;
     DialogDIR *dialog1 = new DialogDIR;
     dialog1->show();
}
void MWindow::dir_restore () {
      extern int dialog_auswertung;
      dialog_auswertung = 5;
      DialogDIR *dialog1 = new DialogDIR;
      dialog1->show();
}

void MWindow::clone_save () {
     extern int dialog_auswertung;
     dialog_auswertung = 7;
     DialogClone *dialog1 = new DialogClone;
     dialog1->show();
}

void MWindow::clone_save_net () {
NetEin netein;
     extern int dialog_auswertung;
     int i = netein.list_net("1");
        if ( i != 1)
	{
     dialog_auswertung = 8;
     NetEin *dialog2 = new NetEin; 
     dialog2->show();
	}
}

void MWindow::einstellungen () {
      DialogSetting *dialog1 = new DialogSetting;
      dialog1->show();
}

void MWindow::eingabe_net_save () {
NetEin netein;
      	extern int dialog_auswertung;
        this->setCursor(Qt::WaitCursor);  
        int i = netein.list_net("1");
        if ( i != 1)
	{
 		dialog_auswertung = 6;
      		NetEin *dialog3 = new NetEin;
      		dialog3->show();
                this->setCursor(Qt::ArrowCursor);  
	}
         this->setCursor(Qt::ArrowCursor);
}

void MWindow::eingabe_net_restore () {
NetEin netein;
      	extern int dialog_auswertung;
        this->setCursor(Qt::WaitCursor);  
        int i = netein.list_net("1");
        if ( i != 1)
	{
                this->setCursor(Qt::WaitCursor);  
      		dialog_auswertung = 7;
      		NetEin *dialog3 = new NetEin;
      		dialog3->show();
                this->setCursor(Qt::ArrowCursor);
        }
        this->setCursor(Qt::ArrowCursor);
}

void MWindow::format() {
     extern QString add_part[100];
     QString teilstring, space1, space2;
     int aa, bb, i,j,k;
     for (i=0; i< 100; i++)
       {
         if (part[i][0] != "") {
           teilstring = part[i][0];
           aa = teilstring.size();
           j = 0; 
           for (k=4; k < 11; k++){
               bb = 36 -j -k;
               j = j +1;
               if (aa==k)
                  break;
           }
           space1.fill (' ',bb);
           teilstring = part[i][1];
           aa = teilstring.size(); 
           teilstring = part[i][2];
           aa = aa + teilstring.size();
           bb = 35 - aa; 
           if (part[i][1] == "<unknown>")
              bb = 26 - aa;
           space2.fill (' ',bb);
           teilstring = part[i][0] + space1 + part[i][1] +space2 + part[i][2];
           if (part[i][1] != "" ){
              widget[i] = teilstring;
              listWidget->addItem (teilstring);
              add_part[i] = teilstring;
              }
         }
      }  
}

void MWindow::startThread1() {
   if( thread1.isRunning() ) return;
   connect( &thread1, SIGNAL(finished()),
            this, SLOT( thread1Ready()));
   thread_run = 1;
   thread1.start();
}

void MWindow::startThread2() {
   if( thread2.isRunning() ) return;
   connect( &thread2, SIGNAL(finished()),
            this, SLOT( thread2Ready()));
   thread_run = 2;
   thread2.start();
}

void MWindow::closeEvent(QCloseEvent *event) {
   thread1.wait();
   thread2.wait();
   event->accept();
}

void MWindow::thread1Ready()  {
int part_testen;
   endeThread = endeThread + 1;
   extern int dialog_auswertung;
     if (endeThread == 1) {
       pushButton_end->setEnabled(true);
       pushButton_save->setEnabled(true);
       progressBar->setValue(100); 
       SekundeRemaining ->setText("0");
       int anzahl  = werte_holen(2);
       QString text_integer = QString::number(anzahl);
       AnzahlgesicherteFile ->setText(text_integer);
       int cnt_regfile = werte_holen(6);
       QString cnt_regfile_ = QString::number(cnt_regfile);
       int cnt_dir = werte_holen(7);
       QString cnt_dir_ = QString::number(cnt_dir); 
       int cnt_symlinks = werte_holen(8);
       QString cnt_symlinks_ = QString::number(cnt_symlinks); 
       int cnt_hardlinks = werte_holen(9);
       QString cnt_hardlinks_ = QString::number(cnt_hardlinks);
       cnt_hardlinks = cnt_hardlinks + cnt_symlinks;
       cnt_hardlinks_ = QString::number(cnt_hardlinks);
       int cnt_special = werte_holen(10);
       QString cnt_special_;
       cnt_special_ = QString::number(cnt_special);
       int err = werte_holen(11) + werte_holen(12) + werte_holen(13) + werte_holen(14) + werte_holen(16);
       if (dialog_auswertung == 0 and err == 0){ 
           //Rückmeldung von fsarchiver: Sicherung erfolgreich
           // Ausgabe progressBar durch Timer unterbinden
           stopFlag = 1; 
           QMessageBox::about(this, tr("Note", "Hinweis"), 
           tr("The partition has been backed up successfully.\n", "Die Partition wurde erfolgreich gesichert.\n") + cnt_regfile_ + 
        tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr(" links and ", " Links und ") 
        + cnt_special_ + tr(" specials have been backed.", " spezielle Daten wurden gesichert."));
        progressBar->setValue(100);     
	}
     else {
   
       pushButton_save->setEnabled(false);
       //Beschreibungsdaten löschen
     	QString filename = folder + "/" + SicherungsDateiName;
        int pos = filename.indexOf("fsa");
       	filename = filename.left(pos);
       	filename.insert(pos, QString("txt"));
        QFile f(filename);
        // Prüfen ob text-Datei vorhanden 
       if (f.exists())  {
          QString befehl = "rm "  + filename;
          system (befehl.toLatin1().data());
          }
       if (flag_end == 1) {
        QMessageBox::about(this, tr("Note", "Hinweis"),
         tr("The backup of the partition was aborted by the user!\n", "Die Sicherung der Partition  wurde vom Benutzer abgebrochen!\n") );
        pushButton_save->setEnabled(true);
	}
       // Prüfen ob Partitionsart unterstützt wird      
       part_testen = werte_holen(3);
       if (part_testen == 110){ 
       QMessageBox::about(this, tr("Note", "Hinweis"),
          tr("The partition type is not supported.\n", "Der Partitionstyp wird nicht unterstützt.\n" ));
          flag_end = 1;
          } 
       // Anzahl nicht korrekt gesicherte Dateien ausgeben
       part_testen = werte_holen(4);
       if (part_testen == 108){
	   QMessageBox::about(this, tr("Note", "Hinweis"),
          tr("The partition type is not supported. Maybe the partition is encrypted?\n", "Der Partitionstyp wird nicht unterstützt. Vielleicht ist die Partition verschlüsselt?\n" ));
          }
       if (part_testen == 109){
	   QMessageBox::about(this, tr("Note", "Hinweis"),
          tr("Error saving partition. File too large. Use is FAT-partition?\n", "Die Sicherung der Partition war nicht erfolgreich. Die Datei ist zu groß. Nutzen Sie eine FAT-Partition?\n" ));
          }
       int err_regfile = werte_holen(12);
       QString err_regfile_ = QString::number(err_regfile);
       int err_dir = werte_holen(13);
       QString err_dir_ = QString::number(err_dir); 
       int err_hardlinks = werte_holen(14);
       err_hardlinks = err_hardlinks + werte_holen(16);
       QString err_hardlinks_ = QString::number(err_hardlinks); 
       int err_special = werte_holen(11);
       QString err_special_ = QString::number(err_special);
       if (part_testen <= 108 && flag_end == 0 ){
       	  QMessageBox::about(this, tr("Note", "Hinweis"), 
       	  tr("The backup of the partition was only partially successful.\n", "Die Sicherung der Partition war nur teilweise erfolgreich\n")
         + cnt_regfile_ + tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr(" links and ", " Links und ") 
         + cnt_special_ + tr(" specials have been backed\n.", " spezielle Daten wurden gesichert\n.")
         + err_regfile_ + tr(" files, ", " Dateien, ")   + err_dir_ + tr(" directories, ", " Verzeichnisse, ") 
         + err_hardlinks_ + tr(" links and ", " Links und ") + err_special_ 
         + tr(" specials were not properly backed\n."," spezielle Daten wurden nicht korrekt gesichert.\n"));
	  }
        }
       
     }
  this->setCursor(Qt::ArrowCursor);
  thread_run = 0;
  thread1.exit();
}

void MWindow::thread2Ready()  {
   endeThread = endeThread + 1;
   extern int dialog_auswertung;
   int meldung = werte_holen(4);
   int i =2;
   if (meldung == 105) {
      QMessageBox::about(this, tr("Note", "Hinweis"), tr("cannot restore an archive to a partition which is mounted, unmount it first \n", "Die Partition die wiederhergestellt werden soll, ist eingehängt. Sie muss zunächst ausgehängt werden!\n"));
      endeThread == 0;
       }
   if (endeThread == 1) { 
     pushButton_end->setEnabled(true);
     int cnt_regfile = werte_holen(6);
     QString cnt_regfile_ = QString::number(cnt_regfile);
     int cnt_dir = werte_holen(7);
     QString cnt_dir_ = QString::number(cnt_dir); 
     int cnt_hardlinks = 0;
     cnt_hardlinks = werte_holen(8) + werte_holen(9) + werte_holen(16);
     QString cnt_hardlinks_ = QString::number(cnt_hardlinks); 
     int cnt_special = werte_holen(10);
     QString cnt_special_;
     cnt_special_ = QString::number(cnt_special);
     int err = werte_holen(11) + werte_holen(12) + werte_holen(13) + werte_holen(14) + werte_holen(16) ;
     //if (dialog_auswertung == 0 and err == 0){
     //Rückmeldung von fsarchiver: Zurückschreiben erfolgreich
     if (err == 0){
       // Ausgabe progressBar durch Timer unterbinden
       stopFlag = 1; 
       pushButton_restore->setEnabled(true);
       progressBar->setValue(100);
       SekundeRemaining ->setText("0");
      //PBR herstellen
      i = 2;
      if (befehl_pbr != "") 
    	i = system (befehl_pbr.toLatin1().data());
      if (i!=0 ) { 
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("The partition is successful back.\n", "Die Partition wurde erfolgreich wieder hergestellt.\n") + cnt_regfile_ + 
        tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr(" links and ", " Links und ") + cnt_special_ + tr(" specials have been restored.", " spezielle Daten wurden wieder hergestellt."));
        }
      if (i==0) { 
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("The partition is successful back.\n", "Die Partition wurde erfolgreich wieder hergestellt.\n") + cnt_regfile_ + 
        tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr(" links ", " Links ") + cnt_special_ + tr(" specials and the Partition Boot Record have been restored.", " spezielle Daten und der Partition Boot Sektor wurden wieder hergestellt."));
        } 
      }  
     if (flag_end == 1) {
        QMessageBox::about(this, tr("Note", "Hinweis"),
         tr("The restore of the partition was break by user!\n", "Die Wiederherstellung der Partition  wurde vom Benutzer abgebrochen!\n") );
	meldung == 0;
        }
     if (err > 0) {
          // Anzahl nicht korrekt zurückgeschriebene Dateien ausgeben
       pushButton_restore->setEnabled(false);
       int err_special = werte_holen(11);
       QString err_special_ = QString::number(err_special);
       int err_regfile = werte_holen(12);
       QString err_regfile_ = QString::number(err_regfile);
       int err_dir = werte_holen(13);
       QString err_dir_ = QString::number(err_dir); 
       int err_hardlinks = werte_holen(14);
       err_hardlinks = err_hardlinks + werte_holen(16);
       QString err_hardlinks_ = QString::number(err_hardlinks); 
       
      if (i!=0) {  
       QMessageBox::about(this, tr("Note", "Hinweis"), 
       	  tr("The restore of the partition was only partially successful.\n", "Die Wiederherstellung der Partition war nur teilweise erfolgreich\n")
         + cnt_regfile_ + tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr(" links and ", " Links und ") 
         + cnt_special_ + tr(" specials have been restored\n.", " spezielle Daten wurden wiederhergestellt\n.")
         + err_regfile_ + tr(" files, ", " Dateien, ")   + err_dir_ + tr(" directories and ", " Verzeichnisse und ") 
         + err_hardlinks_ + tr(" links and ", " Links und ") + err_special_ 
         + tr(" specials were not properly restored\n."," spezielle Daten wurden nicht korrekt wiederhergestellt.\n"));
               }
      if (i==0) { 
        QMessageBox::about(this, tr("Note", "Hinweis"), 
       	  tr("The restore of the partition was only partially successful.\n", "Die Wiederherstellung der Partition war nur teilweise erfolgreich\n")
         + cnt_regfile_ + tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") 
         + cnt_hardlinks_ + tr(" links and ", " Links und ") 
         + cnt_special_ + tr(" specials and the Partition Boot Record have been restored\n.", " spezielle Daten und der Partition Boot Sektor wurden wieder hergestellt\n.")
         + err_regfile_ + tr(" files, ", " Dateien, ")   + err_dir_ + tr(" directories and ", " Verzeichnisse und ") 
         + err_hardlinks_ + tr(" links and ", " Links und ") + err_special_ 
         + tr(" specials were not properly restored\n."," spezielle Daten wurden nicht korrekt wiederhergestellt.\n"));
                }
        }
     if (meldung == 102) { 
        QMessageBox::about(this, tr("Note", "Hinweis"), tr("You have tried restore a partition. The selected file can only restore directories. Please restart the program.\n", "Sie haben versucht eine Partition wiederherzustellen. Die gewählte Datei kann nur Verzeichnisse wiederherstellen. Bitte starten Sie das Programm neu.\n"));
      }
     if (meldung == 103) { 
        QMessageBox::about(this, tr("Note", "Hinweis"), tr("You have entered an incorrect password.\n", "Sie haben ein falsches Passwort eingegeben.\n"));
        endeThread = 0;
        lineKey->setText ("");
      }
    }
    this->setCursor(Qt::ArrowCursor);
    thread_run = 0;
    thread2.exit();
}

void MWindow::elapsedTime()
 {
    sekunde_elapsed = sekunde_elapsed + 1; 
    sekunde_summe = sekunde_summe + 1; 
    if (sekunde_elapsed == 60) {
        sekunde_elapsed = 0 ;
        minute_elapsed = minute_elapsed + 1;
       }
    QString sekunde_ = QString::number(sekunde_elapsed);
    SekundeElapsed ->setText(sekunde_);
    QString minute_ = QString::number(minute_elapsed);
    MinuteElapsed ->setText(minute_);
}

void MWindow::remainingTime(int prozent)
 {
    if (prozent <= 1) 
       sekunde_tara = sekunde_summe ;
    int sekunde_netto = sekunde_summe - sekunde_tara;

    if (prozent >= 5) {
       double remaining_time =((100 * sekunde_netto/prozent)- sekunde_netto);
       int dummy = (int)remaining_time;
       int min_remaining = (int)(remaining_time/60);
       QString minute_ = QString::number(min_remaining);
       MinuteRemaining ->setText(minute_);
       int sek_remaining = (dummy % 60);
       QString sekunde_ = QString::number(sek_remaining);
       SekundeRemaining ->setText(sekunde_);
    }
}

void MWindow::indicator_reset() {
     // Anzeige zurücksetzen für neuen Aufruf fsarchiver
     werte_reset();
     progressBar->setValue(0);
     AnzahlgesicherteFile ->setText(0);
     AnzahlsaveFile ->setText(0); 
     SekundeRemaining ->setText(" ");
     MinuteRemaining ->setText(" ");
     SekundeElapsed ->setText("0");
     MinuteElapsed ->setText("0");
     sekunde_summe = 0;
     minute_elapsed = 0;
     endeThread = 0;
     sekunde_elapsed = 0;
     minute_elapsed = 0;
}

void MWindow::keyPressEvent(QKeyEvent *event) {
     QWidget::keyPressEvent(event);
     switch( event->key() ) {
         case Qt::Key_Escape:
              esc_end();  
         break;
     }
}

QString MWindow::beschreibungstext(QString partitiontext, QString text, int zip, int row)  {
FileDialog filedialog; //Create the main widget; 
//int row;
float prozent;
float compress_[10];
QString part_size_;
QString part_size_compress;
float part_size;
QString free_part_size_;
float free_part_size;
QString Linuxversion;
QString ubuntu_root;
QString ubuntu_home;
QString compress = zip_[zip - 1];
QString mount_point;
QString kernel;
compress_[0] = 0.46;  //lzo
compress_[1] = 0.39;  //gzip fast
compress_[2] = 0.37;  //gzip standard
compress_[3] = 0.365;  //gzip best
compress_[4] = 0.36;  //bzip2 fast
compress_[5] = 0.355;  //bzip2 good
compress_[6] = 0.35; //lzma fast
compress_[7] = 0.322; //lzma medium
compress_[8] = 0.32; //lzma best
        SicherungsDateiName = text;
	partition_ = widget[row];
	mount_point = mountpoint(partitiontext);
        //prozentuale Belegung
	prozent = freesize(partitiontext.toLatin1().data(), mount_point.toLatin1().data(), 1); 
        // freier Bereich der Partition
        free_part_size = freesize(partitiontext.toLatin1().data(), mount_point.toLatin1().data(), 3);
       	free_part_size_ =  format(free_part_size);
        // Gesamter Bereich der Partition
       	part_size = freesize(partitiontext.toLatin1().data(), mount_point.toLatin1().data(), 2);
        part_size_ =  format(part_size);
        free_part_size = free_part_size * compress_[zip - 1];
        part_size_compress = format(free_part_size);
        text = tr("Backup file name: ", "Sicherungsdateiname: ") + text + "\n" + 
	tr("Partition  name: ", "Partitionsname: ")  
	+ PartitionString(row+1,0) + "\n" + tr("Partition type: ", "Partitionsart: ") 
       	+ PartitionString(row+1,1) + "\n" + tr("UUID: ") +  PartitionString(row+1,3) + "\n" + tr("Description: ", "Bezeichnung: ") + PartitionString(row+1,4) + "\n" + 
       	tr("Partition size: ", "Partitionsgröße: ") + part_size_ + "\n" + tr("Assignment of the partition: ", "Belegung der Partition: ") + 
       	free_part_size_ + "\n" + tr("Assignment of the partition: ", "Belegung der Partition: ") + QString::number(prozent)+ " %" + "\n" + tr("Compression: ", "Kompression: ") + compress + "\n" + 
       	tr("Approximate image file sizes: ", "ungefähre Sicherungsdateigröße: ") + part_size_compress + "\n" + "\n" + tr("Other notes: ", "weitere Hinweise:");
        ubuntu_root = tr("to be protected/secured partition: / (root system directory) ", "zu sichernde / gesicherte Partition: / (Wurzel-Systemverzeichnis) ");
        part_art = mtab_einlesen(partitiontext);
	if (part_art == "system"){
		Linuxversion = linux_version();
                kernel = kernel_version();
                text = ubuntu_root + "\n" + tr("Operating system: ", "Betriebsystem: ") + Linuxversion + "\n" + tr("Kernel: ") + kernel + " " + "\n" + text + "\n";
	}
	ubuntu_home = tr("to be protected/secured partition: /home", "zu sichernde/gesicherte Partition: /home");
	if (part_art == "home"){
	        Linuxversion = linux_version();
                kernel = kernel_version();
                text = ubuntu_home + "\n" + tr("Operating system: ", "Betriebsystem: ")  + Linuxversion + " " + "\n"  + tr("Kernel: ") + kernel  + "\n" + text + "\n";
	}
	filedialog.werte_uebergeben(text);
        return text;
}

QString MWindow::format(float zahl)  {
QString zahl_;
int found;
         if (zahl <= 999){
            zahl = (int) (zahl * 100/1.024);
            zahl = zahl/100;
            // Punkt in Komma wandeln
            zahl_ = QString::number(zahl);
            found=zahl_.indexOf(".");
            if (found > 0)
             	zahl_.replace(found, 1, ","); 
            return zahl_  + tr(" MB");
	 } 
        if (zahl >= 1000000){
            zahl = zahl / 1000000/1.024/1.024/1.024; 
            zahl = (int) (zahl * 100);
            zahl = zahl/100;
            // Punkt in Komma wandeln
            zahl_ = QString::number(zahl);
            found=zahl_.indexOf(".");
            if (found > 0)
             	zahl_.replace(found, 1, ","); 
            return zahl_  + tr(" TB");
	 }    
	if (zahl >= 1000){
            zahl = zahl / 1000/1.024/1.024;
            zahl = (int) (zahl * 100);
            zahl = zahl/100;
            // Punkt in Komma wandeln
            zahl_ = QString::number(zahl);
            found=zahl_.indexOf(".");
            if (found > 0)
             	zahl_.replace(found, 1, ","); 
            return zahl_  + tr(" GB");
	 }   
}

QString MWindow::linux_version()  {
QString homepath = QDir::homePath();
QString befehl;
QString Linuxversion;
int i;
	befehl = "cat /etc/*release 1> " +  homepath + "/.config/qt5-fsarchiver/version.txt";
        system (befehl.toLatin1().data());
QString filename = homepath + "/.config/qt5-fsarchiver/version.txt";
QFile file(filename);
        if( file.open(QIODevice::ReadOnly|QIODevice::Text)) {
           QTextStream ds(&file);
           Linuxversion = ds.readLine();
           if (Linuxversion.indexOf("PRETTY_NAME") > -1) {  //Debian
               Linuxversion = Linuxversion.right(Linuxversion.size() -13);
               Linuxversion = Linuxversion.left(Linuxversion.size() -1);
               return Linuxversion;
	   }
     //      if (Linuxversion.indexOf("PRETTY_NAME") == -1) && Linuxversion.indexOf("DISTRIB_ID") == -1) {  //Simply Linux
     //         Linuxversion = ds.readLine(); // Simply Linux
     //         return Linuxversion;
     //      }
           for (i=1; i < 4; i++){
           	Linuxversion = ds.readLine(); // Simply Linux
           }}
           file.close();
           befehl = "rm " + filename;
           system (befehl.toLatin1().data());
           if (Linuxversion.indexOf("DISTRIB_DESCRIPTION=") > 0); 
               // Ubuntu, Debian       
               Linuxversion = Linuxversion.right(Linuxversion.size() -20);
           return Linuxversion;
}

QString MWindow::kernel_version()  {
QString homepath = QDir::homePath();
QStringList kernel;
QString kernel_;
QString befehl;
	befehl = "uname -a 1> " +  homepath + "/.config/qt5-fsarchiver/kernel.txt";
        system (befehl.toLatin1().data());
QString filename = homepath + "/.config/qt5-fsarchiver/kernel.txt";
	QFile file(filename);
        if( file.open(QIODevice::ReadOnly|QIODevice::Text)) {
           QTextStream ds(&file);
           kernel_ = ds.readLine();
           file.close();
           kernel = kernel_.split(" ");
         }
         befehl = "rm " + filename;
         system (befehl.toLatin1().data());
         if (kernel_.indexOf("Debian") > -1)         
		return kernel[0] + " " + kernel[2] ;
         else
		return kernel[0] + " " + kernel[2] +  " " + kernel[11];
}

QString MWindow::identify_save_part(QString save_partition)
{
   QString fileName;
   QString text;
   QStringList items;
   QString save_part;
   QFile file("/etc/mtab");
   QTextStream ds(&file);
   int line = 0;
   if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
     text = ds.readLine();
   	while (!ds.atEnd())
      	{
 	    if (text.indexOf(save_partition) > -1)  // Partitionsname gefunden
      	    { 
   	       if (text.indexOf("vfat") > -1) 
      	          return "vfat";
               }
   	       if( text.isEmpty() )
         	 break;
       		line++;
            text = ds.readLine();
      	} 
   	file.close();
     }
  return save_part.toLatin1().data();
}

QString MWindow::mtab_einlesen(QString partition_if_home)
{
   QString fileName;
   QString text;
   QStringList items;
   QString mount_dir;
   QFile file("/etc/mtab");
   QTextStream ds(&file);
   int pos, pos1,i,j;
   int line = 0;
   if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
     text = ds.readLine();
   	while (!ds.atEnd())
      	{
	      if (text.indexOf(UUID) > -1 )  // Partitionsname gefunden, Debian 7
      	    { 
	      items = text.split(" ");  //Einhängepunkt ermitteln
    	      mount_dir = items[1];
              if (text.indexOf(" /home") > -1) 
      	            return "home";
                 if (text.indexOf(" / ") > -1) 
      	           return "system";
      	    }
      	if (text.indexOf(partition_if_home) > -1 )  // Partitionsname gefunden,  Ubuntu und Mint
      	    { 
	      items = text.split(" ");  //Einhängepunkt ermitteln
    	      mount_dir = items[1];
              //Überprüfen ob sda1 nicht mit sda10 verwechselt wird
              if (items[0].size() == partition_if_home.size()){
                 if (text.indexOf(" /home") > -1) 
      	            return "home";
                 if (text.indexOf(" / ") > -1) 
      	           return "system";
                 if (text.indexOf("ext4") > -1) 
      	        	return "ext4";
                 if (text.indexOf("ext3") > -1) 
      	        	return "ext3";
                 if (text.indexOf("btrfs") > -1) 
      	        	return "btrfs";
               }
      	    }   
   	       if( text.isEmpty() )
         	 break;
       		line++;
            text = ds.readLine();
      	} 
   	file.close();
     }
  //return mount_dir.toLatin1().data();
  return "";
}

//Mountpoint ermitteln
QString MWindow::mountpoint(QString partition)
{
   QString fileName;
   QString text;
   QStringList items;
   QString mount_dir;
   QFile file("/etc/mtab");
   QTextStream ds(&file);
   string text_str;
   int i,j,pos;
   int line = 0;
   text_str = partition.toLatin1().data();
   if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        text = ds.readLine();
        // hier wird die ersten Zeile der mtab überprüft
	pos = text.indexOf(partition); 
        if (pos > -1)  // Partitionsname gefunden
      	     { 
                items = text.split(" ");  //Einhängepunkt ermitteln
                i = items[0].size();
                j =  text_str.size();
                if (i == j)
    	      	   mount_dir = items[1];
       	     }
   	while (!ds.atEnd())
      	{
	     // hier wird auch die letzte Zeile der mtab überprüft	   	     
	     line++;
             text = ds.readLine();
             pos = text.indexOf(partition); 
             if (pos > -1)  // Partitionsname gefunden
      	     { 
                items = text.split(" ");  //Einhängepunkt ermitteln
                i = items[0].size();
                j =  text_str.size();
                if (i == j)
    	      	   mount_dir = items[1];
       	    }
      	} 
   	file.close();
     }
  return mount_dir;
}

void MWindow::esc_end()
{
QString befehl = "";
   if (thread_run > 0) {
    int ret = questionMessage(tr("Do you really want to break the save or restore from the partition?", "Wollen Sie wirklich die Sicherung oder Wiederherstellung der Partition beenden?"));
      if (thread_run == 1 && ret == 1)
        {
        flag_end= 1;
     	befehl = "rm "  + SicherungsFolderFileName;
        system (befehl.toLatin1().data()); 
        int pos = SicherungsFolderFileName.indexOf("fsa");
       	SicherungsFolderFileName = SicherungsFolderFileName.left(pos);
       	SicherungsFolderFileName.insert(pos, QString("txt"));
        befehl = "rm "  + SicherungsFolderFileName;

// pbr-Datei löschen
 pos = SicherungsFolderFileName.indexOf("txt");
SicherungsFolderFileName = SicherungsFolderFileName.left(pos);
 SicherungsFolderFileName.insert(pos, QString("pbr"));
 befehl = "rm "  + SicherungsFolderFileName;
 system (befehl.toLatin1().data());
        flag_end= 1;
        if (bit_version() == "32")
                {
        	befehl = "kill -15 " + pid1;  //fsarchiver abbrechen
        	system (befehl.toLatin1().data());
        	befehl = "kill -15 " + pid;  //fsarchiver abbrechen
        	system (befehl.toLatin1().data());
        	} 
        if (bit_version() == "64") 
		{
		thread1.terminate();
        	thread1.wait();
		}     
	}
     if (thread_run == 2 && ret == 1)
        {
        flag_end= 1;
	if (bit_version() == "32") 
                {
        	befehl = "kill -15 " + pid1;  //fsarchiver abbrechen
        	system (befehl.toLatin1().data());
        	befehl = "kill -15 " + pid;  //fsarchiver abbrechen
        	system (befehl.toLatin1().data());
        	} 
        if (bit_version() == "64") 
		{
		thread2.terminate();
        	thread2.wait();
		}         
	}
    close();
   }
}

QString MWindow::bit_version()
{
QString bit;
QString homepath = QDir::homePath();
QString befehl;
	befehl = "uname -m 1> " +  homepath + "/.config/qt5-fsarchiver/bit.txt";
        system (befehl.toLatin1().data());
QString filename = homepath + "/.config/qt5-fsarchiver/bit.txt";
QFile file(filename);
        if( file.open(QIODevice::ReadOnly|QIODevice::Text)) {
           QTextStream ds(&file);
           bit = ds.readLine();
           file.close();
           befehl = "rm " + filename;
           }
           system (befehl.toLatin1().data());
           if (bit.indexOf("_64") > 0)
		return "64";               
           else 
                return "32";
}

void MWindow::pid_ermitteln()
{
int pida=getpid();
pid = QString::number(pida);
int pida1=getppid();
pid1 = QString::number(pida1);
}


void MWindow::del_mediafolder()
// Programm beenden
// eingehängte Partitionen aushängen und leere Verzeichnisse in /media löschen.
{
      QString foldername[100];
      QString homepath = QDir::homePath();
      QString befehl = "cd /media; ls 1> " +  homepath + "/.config/qt5-fsarchiver/media.txt; cd /";
      system (befehl.toLatin1().data());
      QString media;
      QString filename = homepath + "/.config/qt5-fsarchiver/media.txt";
      QFile file(filename);
      int zaehler = 0;
      qApp->quit();
      if( file.open(QIODevice::ReadOnly|QIODevice::Text)) {
           QTextStream ds(&file);
           while (!ds.atEnd()){
           	media = ds.readLine();
                if (media.indexOf("sd") > -1){
                   foldername[zaehler] = "/media/" + media;
                   befehl = "umount " + foldername[zaehler] + " 2>/dev/null";
                   system (befehl.toLatin1().data());
                   befehl = "rmdir " + foldername[zaehler] + " 2>/dev/null";
                   system (befehl.toLatin1().data());
                   zaehler = zaehler + 1;
           } } }
           file.close();
           befehl = "rm " + filename + " 2>/dev/null";
           system (befehl.toLatin1().data());
           filename = homepath + "/.config/qt5-fsarchiver/disk.txt";
           befehl = "rm " + filename + " 2>/dev/null";
           system (befehl.toLatin1().data());
           filename = homepath + "/.config/qt5-fsarchiver/efi.txt";
           befehl = "rm " + filename + " 2>/dev/null";
           system (befehl.toLatin1().data());
           filename = homepath + "/.config/qt5-fsarchiver/folder.txt";
           befehl = "rm " + filename + " 2>/dev/null";
           system (befehl.toLatin1().data());
           filename = homepath + "/.config/qt5-fsarchiver/ip.txt";
           befehl = "rm " + filename + " 2>/dev/null";
           system (befehl.toLatin1().data());
           filename = homepath + "/.config/qt5-fsarchiver/smbtree.txt";
           befehl = "rm " + filename + " 2>/dev/null";
           system (befehl.toLatin1().data());
           filename = homepath + "/.config/qt5-fsarchiver/crypt1.txt";
           befehl = "rm " + filename + " 2>/dev/null";
           system (befehl.toLatin1().data());
           filename = homepath + "/.config/qt5-fsarchiver/sektornummer.txt";
           befehl = "rm " + filename + " 2>/dev/null";
           system (befehl.toLatin1().data());
           
           befehl = "umount -f" + homepath + "/.qt5-fs-client 2>/dev/null";
	   system (befehl.toLatin1().data());   
           befehl = "fusermount -u " + homepath + "/.qt5-fs-client 2>/dev/null";
           system (befehl.toLatin1().data());
           befehl = "umount -a -t nfs 2>/dev/null";
           system (befehl.toLatin1().data());
           qApp->quit();
          //close ();
}




