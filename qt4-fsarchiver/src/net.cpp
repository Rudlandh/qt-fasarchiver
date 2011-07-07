/*
 * qt4-fsarchiver: Filesystem Archiver
 *
 * Copyright (C) 2010, 2011 Hihin Ruslan, Dieter Baum.  All rights reserved.
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
#include <string.h>
#include <stdio.h>
#include <QtGui> 
#include "net.h"
#include "net_ein.h"
#include "filedialog.h"
#include "mainWindow.h"
#include <iostream>
extern "C" {
#include "connect_c_cpp.h"
#include "fsarchiver.h"
}
using namespace std;

extern int dialog_auswertung;
QString zip_net[10];
QString folder_net;
QString partition_net; // z.B 192.168.2.5
QString partition_net_; // z.B sda1
QString DateiName_net("") ;
QString _Datum_net;
QString part_art_net;
extern QString parameter[15];
int endeThread_net;
QString SicherungsDateiName_net;
QString SicherungsFolderFileName_net;
int sekunde_elapsed_net;
int minute_elapsed_net;
int sekunde_summe_net;
int dummy_prozent_net;
int flag_View_net; 
int flag_end_net;
int sekunde_tara_net; 
int thread_run_net;
QString pid_net, pid1_net;
extern int fsarchiver_aufruf(int argc, char *anlage0=NULL, char 
*anlage1=NULL, char *anlage2=NULL, char *anlage3=NULL, char 
*anlage4=NULL, char *anlage5=NULL, char *anlage6=NULL, char 
*anlage7=NULL, char *anlage8=NULL, char *anlage9=NULL, char 
*anlage10=NULL, char *anlage11=NULL, char *anlage12=NULL, char 
*anlage13=NULL, char *anlage14=NULL);

DialogNet::DialogNet(QWidget *parent)
{
QString befehl; 
QStringList items;
QStringList items_kerne_;       
	setupUi(this); // this sets up GUI
        folder_net = "/mnt/qt4-fs-client";
        if ( dialog_auswertung == 6)
        	rdBt_saveFsArchiv->setChecked(Qt::Checked);
        else
		rdBt_restoreFsArchiv->setChecked(Qt::Checked);
	connect( pushButton_end, SIGNAL( clicked() ), this, SLOT(end()));
        connect( pushButton_save, SIGNAL( clicked() ), this, SLOT(savePartition()));
   	connect( pushButton_restore, SIGNAL( clicked() ), this, SLOT(restorePartition()));
        connect( pushButton_folder, SIGNAL( clicked() ), this, SLOT(folder_einlesen()));
	connect( rdBt_saveFsArchiv, SIGNAL( clicked() ), this, SLOT(rdButton_auslesen()));
   	connect( rdBt_restoreFsArchiv, SIGNAL( clicked() ), this, SLOT(rdButton_auslesen())); 
        connect( pushButton_partition, SIGNAL( clicked() ), this, SLOT(listWidget_auslesen()));
        connect( pushButton_break, SIGNAL( clicked() ), this, SLOT(esc_end()));
        connect( chk_key, SIGNAL( clicked() ), this, SLOT(chkkey()));
  	dirModel = new QDirModel;
   	selModel = new QItemSelectionModel(dirModel);
   	treeView->setModel(dirModel);
   	treeView->setSelectionModel(selModel);
   	QModelIndex cwdIndex = dirModel->index(QDir::rootPath());
   	treeView->setRootIndex(cwdIndex);
        pushButton_restore->setEnabled(false);
        timer = new QTimer(this);
        items_kerne_ << "1" << "2" << "3" << "4" <<  "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
   	cmb_kerne->addItems (items_kerne_);
   	items_kerne_.clear();
   	zip_net[0]= tr("lzo");
   	zip_net[1]= tr("gzip fast");
   	zip_net[2]= tr("gzip standard");
   	zip_net[3]= tr("qzip best");
   	zip_net[4]= tr("bzip2 fast");
   	zip_net[5]= tr("bzip2 good");
   	zip_net[6]= tr("lzma fast");
   	zip_net[7]= tr("lzma medium");
   	zip_net[8]= tr("lzma best");
   	items << zip_net[0] << zip_net[1] << zip_net[2] << zip_net[3] <<  zip_net[4];
   	cmb_zip->addItems (items);
   	items.clear();
   	items << zip_net[5] << zip_net[6] << zip_net[7] << zip_net[8];
   	cmb_zip->addItems (items);
   	items.clear();
        chkkey();
        // Ini-Datei auslesen
   	QString homepath = QDir::homePath();
   	QFile file1(homepath + "/.config/qt4-fsarchiver/qt4-fsarchiver.conf");
   if (file1.exists()) {
        QSettings setting("qt4-fsarchiver", "qt4-fsarchiver");
        setting.beginGroup("Basiseinstellungen");
        int auswertung = setting.value("Kompression").toInt();
        cmb_zip -> setCurrentIndex(auswertung); 
        auswertung = setting.value("Kerne").toInt();
        cmb_kerne -> setCurrentIndex(auswertung-1); 
        auswertung = setting.value("overwrite").toInt();
        if (auswertung ==1)
           chk_overwrite->setChecked(Qt::Checked); 
        auswertung = setting.value("tip").toInt();
        if (auswertung ==1)
           chk_Beschreibung->setChecked(Qt::Checked);  
        auswertung = setting.value("key").toInt();
        if (auswertung ==1)
           chk_key->setChecked(Qt::Checked); 
        setting.endGroup();
        } 
   else {
        cmb_kerne -> setCurrentIndex(0);
        chk_Beschreibung->setChecked(Qt::Checked);
        chk_overwrite->setChecked(Qt::Checked); 
        cmb_zip -> setCurrentIndex(2); 
        } 
    	label_4->setEnabled(false);
   	chk_overwrite->setEnabled(true);
   	cmb_zip->setEnabled(false);
   	chk_Beschreibung->setEnabled(true);
   	chk_Beschreibung->setChecked(Qt::Checked);
   	rdButton_auslesen();
   	Daten_eintragen();
   	addWidget();
        
} 

void DialogNet::folder_einlesen() {
treeView->setModel(dirModel);
treeView->setSelectionModel(selModel);
   	QModelIndex cwdIndex = dirModel->index("/mnt/qt4-fs-client");
   	treeView->setRootIndex(cwdIndex);
	QModelIndex index = treeView->currentIndex();
        QModelIndexList indexes = selModel->selectedIndexes();
        folder_net =  (dirModel->filePath(index));
} 

void DialogNet::chkkey(){
     Qt::CheckState state;
     state = chk_key->checkState();
     if (state == Qt::Checked)
        lineKey->setEnabled(true);
     else 
	lineKey->setEnabled(false);
}  

void DialogNet:: end()
{ 
QString befehl;
QString filename;
QFile f(filename);
QString homepath = QDir::homePath();
    
        befehl = "umount /mnt/qt4-fs-client";
	system (befehl.toAscii().data()); // Dateien entfernen 
  	filename = homepath + "/.config/qt4-fsarchiver/ip.txt";
	if (f.exists()){
     		befehl = "rm " + filename;
		system (befehl.toAscii().data());
       }     
       filename = homepath + "/.config/qt4-fsarchiver/smbtree.txt";
       if (f.exists()){
     		befehl = "rm " + filename;
		system (befehl.toAscii().data());
       } 
       close(); 
}

void DialogNet:: Daten_eintragen()
{
NetEin netein;	
QString rechner;
   rechner = netein.Namen_holen();
	txt_rechner_net ->setText(rechner);
   
}

QString DialogNet::hostname()
{
QString homepath = QDir::homePath(); 
// eigenen Hostname einlesen
QFile file("/etc/hostname");
    	QTextStream ds(&file);
        QString text; 
        if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
     	     text = ds.readLine();
             file.close();
        }
     text = text.toLower();
     return text;
}

int DialogNet::savePartition() {
MWindow window;
FileDialog filedialog;
QFile file(folder_net);
     QString Befehl;
     QString text;
     Qt::CheckState state;
     Qt::CheckState state1;
     Qt::CheckState state3;
     char * part_;
     int err = 0;
     QString keyText = "";
     int prozent;
     int zip;
     indicator_reset();
     if (rdBt_saveFsArchiv->isChecked())
     {
     	if (folder_net == "")
           {
          	QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("Please, select the saved directory..\n", "Bitte wählen Sie das Sicherungsverzeichnis aus.\n"));
		return 0 ;
           }
        if (partition_net_ == "")
           {
          	QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("Please, select the partition to be saved.\n", "Bitte wählen Sie die zu sichernde Partition aus.\n"));
		return 0 ;
           }
        DateiName_net = lineEdit_DateiName->text();
        if (DateiName_net == "")
           {
          	QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("Please, select the filename of the backup.\n", "Bitte wählen Sie den Dateinamen der Sicherung aus.\n"));
		return 0 ;
           }
        if (file.open(QIODevice::ReadOnly))
        	  {
                QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("You have selected a file. You must select a directory\n", "Sie haben eine Datei ausgewählt. Sie müssen ein Verzeichnis auswählen\n"));
		file.close();
		return 0 ;
 	        }
         _Datum_net = window.Zeit_auslesen();
         state = chk_Beschreibung->checkState();
         //Überprüfung ob gemounted
         part_ = partition_net_.toAscii().data();
         char  dev_[50] = "/dev/";
	 strcat (dev_ , part_);  // String zusammenfügen: dev_  = dev_ + part_
	 int liveFlag = 0;
         zip = cmb_zip->currentIndex()+1;
         QString dummy = partition_net_;
	 // Werte sammeln und nach file_dialog übergeben, Partition ist eingehängt
         part_art_net = window.mtab_einlesen("/dev/" + partition_net);
         int row = listWidget->currentRow();
       	 text = window.beschreibungstext("/dev/" + partition_net_, DateiName_net + "-" + _Datum_net + ".fsa", zip, row);
         filedialog.werte_uebergeben(text);
         partition_net = dummy;
         if (window.is_mounted(dev_)) 
            {
		   part_art_net = window.mtab_einlesen("/dev/" + partition_net_);
                   //Überprüfung ob System oder Home Partition
                   int ret = 1; 
                   if (part_art_net == "system")
                	{
                	ret = window.questionMessage(tr("The system partition to be saved is mounted. Do you want to do a live backup?", "Die zu sichernde Systempartition ist eingehängt. Wollen Sie eine Live-Sicherung durchführen?"));
                        if (ret == 2)
                           return 0;
                        if (ret == 1)
                           liveFlag = 1;
                	}
                  if (part_art_net == "home")
                	{
                	ret = window.questionMessage(tr("The home partition to be saved is mounted. Do you want to do a live backup?", "Die zu sichernde Homepartition ist eingehängt. Wollen Sie eine Live-Sicherung durchführen?"));
                        if (ret == 2)
                           return 0;
                        if (ret == 1)
                           liveFlag = 1;
                	}
                   if (part_art_net != "system" || part_art_net != "home")
                	{
                           char umountpoint[100] = "umount ";
                           strcat (umountpoint,dev_); 
                           if (liveFlag == 0)
                         	err = system (umountpoint);
                           if (err != 0 && liveFlag == 0)
                                {
				QMessageBox::about(this, tr("Note", "Hinweis"),
         			tr("The partition ", "Die Partition ")   + partition_net_ + 
				tr("can not be unmounted. The program is terminated\n", " kann nicht ausgehängt werden. Das Programm wird abgebrochen\n"));
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
		         if (zip == -1)
            		       parameter[2] = "-z3"; 
        		 else  {
            			parameter[2] = compress;
                               }
                         int kerne = cmb_kerne->currentIndex()+1;
                         QString index = QString::number(kerne);
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
                                  if (state3 == Qt::Checked)   //Archiv splitten 
                                     {
                                      parameter[indizierung] = "-s 4400";
                                      indizierung = indizierung + 1;
                                      }
                                  if (liveFlag == 1)
                                      {
                                      parameter[indizierung] = "-A";
	    			      parameter[indizierung + 1] = "-a";
                                      indizierung = indizierung + 2; 
                                      }
                                  parameter[indizierung] = (folder_net + "/" + DateiName_net + "-" + _Datum_net + ".fsa");
                                  SicherungsFolderFileName_net = parameter[indizierung];
                                  parameter[indizierung + 1] = ("/dev/" + partition_net_);
                                  QFile f(parameter[indizierung]);
                                  if  (parameter[4] != "-o" && (f.exists())){
				       QMessageBox::about(this, tr("Note", "Hinweis"),
         	  			  tr("The partition file ", "Die Partitionsdatei ")   + parameter[6] + tr("already exists. The backup is not performed\n", " ist bereits vorhanden. Die Sicherung wird nicht durchgeführt\n"));
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
                		      QMessageBox::about(this, tr("Note", "Hinweis"),
         			      tr("The backup was aborted by the user\n", "Die Sicherung wurde vom Benutzer abgebrochen\n"));
				      pushButton_save->setEnabled(false);
                                      return 0;
                		      }
				   }
                                thread1.setValues(indizierung + 2,0);
                                pushButton_save->setEnabled(false);
                                pushButton_end->setEnabled(false); 
                                werte_reset();
                                flag_View_net = 1;
  				timer->singleShot( 1000, this , SLOT(ViewProzent( ))) ; 
				startThread1(); // fsarchiver wird im Thread ausgeführt
        			
			}
       }
       return 0;
}

int DialogNet::restorePartition() {
MWindow window;
Qt::CheckState state;
Qt::CheckState state1;
QFile file(folder_net);
QString DateiName("") ;
string part_art;
char * part_;
int err;
int prozent;
QString keyText = "";
char * dev_part;
string dateiname;
int pos;
         keyText = lineKey->text();
         state1 = chk_key->checkState();
	indicator_reset();
	if (rdBt_restoreFsArchiv->isChecked())
        {
          if (state1 == Qt::Checked && keyText.isEmpty())  
              {
                QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("No key was given for the decryption\n", "Es wurde kein Schlüssel für die Entschlüsselung angegeben.\n"));
		return 1 ;
               }
           if (folder_net == "")
           {
          	QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("Please, select the filename of the backup.\n", "Bitte wählen Sie den Dateinamen der Sicherung aus.\n"));
                return 0;
           }
           if (partition_net_ == "")
           {
          	QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("Please, select the partition to be written back.\n", "Bitte wählen Sie die zurück zu schreibende Partition aus.\n"));
                return 0;
           }
   	   if (file.open(QIODevice::ReadOnly))
        	{
               	if (rdBt_restoreFsArchiv->isChecked())
           		{
                        pos = testDateiName("fsa"); 
         		if (pos == 0)
         			   {
           			   QMessageBox::about(this, tr("Note", "Hinweis"),
         			   tr("You have chosen the wrong recovery file selected.\nThe files should end with. fsa be", "Sie haben eine falsche Wiederherstellungsdatei ausgesucht ausgesucht \nDie Dateiendung muss .fsa sein"));
                                   return 0;
         			   }
             		}
 		}
           	
	     else
                {
                QMessageBox::about(this, tr("Note", "Hinweis"),
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
               		parameter[2] = folder_net;
               		fsarchiver_aufruf(3,parameter[0].toAscii().data(),parameter[1].toAscii().data(),parameter[2].toAscii().data(),parameter[3].toAscii().data());
               		optionkey = meldungen_holen(1);
               		dev_part = meldungen_holen(2);
                        if (werte_holen(4) == 103){
                  		chk_key->setChecked(Qt::Checked);
                                lineKey->setEnabled(true);
                  		QMessageBox::about(this, tr("Note", "Hinweis"),
         	     		tr("The partition is encrypted. Please enter the key\n", "Die Partition ist verschlüsselt. Bitte geben Sie den Schlüssel ein\n"));
                   		return 0;
               		} 
        	}
		if (state1 == Qt::Checked) {
            		parameter[2] = "-c";
                        parameter[3] = keyText;
                        if (parameter[3].size() < 6 || parameter[3].size() > 64) {
                  		QMessageBox::about(this, tr("Note", "Hinweis"),
         	  		tr("The key length must be between 6 and 64 characters\n", "Die Schlüssellänge muss zwischen 6 und 64 Zeichen sein\n"));
                  		return 0 ; 
               	   		}
			parameter[4] = folder_net;
                        int retour = fsarchiver_aufruf(5,parameter[0].toAscii().data(),parameter[1].toAscii().data(),parameter[2].toAscii().data(),parameter[3].toAscii().data(),parameter[4].toAscii().data (),parameter[5].toAscii().data());
                        if ( werte_holen(4) == 103 && retour != 0){
                          QMessageBox::about(this, tr("Note","Hinweis"), tr("They have entered a wrong password.\n", "Sie haben ein falsches Passwort eingegeben. \n"));
           		   lineKey->setText ("");
                           return 0;
                        }
			optionkey = meldungen_holen(1);
               		dev_part = meldungen_holen(2);
                }
		//Überprüfen, ob in die Originalpartition zurückgeschrieben werden soll
            part_ = partition_net_.toAscii().data();
            char  dev_[50] = "/dev/";
	    strcat (dev_ , part_);  // String zusammenfügen: dev_  = dev_ + part_
            // Hier wird verglichen dev_ = die Partition, die zu sichern ist. dev_part = Originalpartition
            int cmp = strcmp ( dev_ , dev_part );
            if (cmp != 0){
               // char in QString wandeln
               QString str, str1;
	       str = dev_part;
               str1 = dev_;
               cmp = window.questionMessage(tr("Partition to restore the ", "Die wiederherzustellende Partition ") + str1 + 
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
                	   QMessageBox::about(this, tr("Note", "Hinweis"),
         		   tr("The restore was canceled by user\n", "Das Zurückschreiben wurde vom Benutzer abgebrochen\n"));
                            pushButton_restore->setEnabled(false);
                            return 0;
                	   }
                       }
             	  }
	if (window.is_mounted(dev_)) 
               {
              part_art_net = window.mtab_einlesen("/dev/" + partition_net_);
              //Überprüfung ob System oder Home Partition 
                   if (part_art_net == "system")
                	{
                        QMessageBox::about(this, tr("Note", "Hinweis"),
         			tr("To restore system partition is mounted and can not be restored. Please use a live CD\n", "Die wiederherzustellende Systempartition ist eingehängt und kann nicht zurückgeschrieben werden. Benutzen Sie bitte eine Live-CD\n"));
				return 0;
				      } 
                        
                   if (part_art_net == "home")
                	{
                        QMessageBox::about(this, tr("Note", "Hinweis"),
         			tr("The restored home partition is mounted and can not be restored. Please use a live CD\n", "Die wiederherzustellende Homepartition ist eingehängt und kann nicht zurückgeschrieben werden. Benutzen Sie bitte eine Live-CD\n"));
				return 0; 
                        
                	}
                   if (part_art_net != "system"|| part_art_net != "home")
                	{
                          char umountpoint[100] = "umount ";
                          strcat (umountpoint,dev_); 
                          err = system (umountpoint);  
                          if (err != 0)
                                {
				QMessageBox::about(this, tr("Note", "Hinweis"),
         			tr("The partition", "Die Partition ")   + partition_net_ + 
         			tr("can not be unmounted. The program is terminated\n", " kann nicht ausgehängt werden. Das Programm wird abgebrochen\n"));
                                return 0 ; 
                                }  
                          }
                                
                } 
   	  if (rdBt_restoreFsArchiv->isChecked())
              {
               QString keyText = lineKey->text();
               state1 = chk_key->checkState(); 
	       parameter[0] = "fsarchiver";
       	       parameter[1] = "restfs"; 
               int kerne = cmb_kerne->currentIndex()+1;
               QString index = QString::number(kerne);
               parameter[2] = "-j" + index;
               parameter[3] = folder_net;
               parameter[4] = "id=0,dest=/dev/" + partition_net_;
               thread2.setValues(5,0);
               if (state1 == Qt::Checked) { // Verzeichnis mit Schlüssel gesichert
                 thread2.setValues(7,0);
                 parameter[3] = "-c";
                 parameter[4] = keyText;
                 int len = parameter[4].size();
               	    if (len < 6 || len > 64) {
                  	QMessageBox::about(this, tr("Note", "Hinweis"),
         	  	tr("The key length must be between 6 and 64 characters\n", "Die Schlüssellänge muss zwischen 6 und 64 Zeichen sein\n"));
                  	return 0 ; 
               	   }
                 parameter[5] = folder_net;
                 parameter[6] = "id=0,dest=/dev/" + partition_net_;
                 }
               werte_reset();
               pushButton_restore->setEnabled(false);
               pushButton_end->setEnabled(false);
               flag_View_net = 2;
  	       timer->singleShot( 1000, this , SLOT(ViewProzent( ))) ;
               startThread2(); // fsarchiver wird im Thread ausgeführt
               }	
	}
   return 0;
}

void DialogNet::addWidget() {
extern QString add_part[100];
int i = 0;
    while (add_part[i] != "")
    {
	listWidget->addItem (add_part[i]);
        i++ ;
     }
}

void DialogNet::starteinstellung(){
            label_folder->setText (tr("Backup directory", "Sicherungsverzeichnis"));
            pushButton_save->setText (tr("Save partition", "Partition sichern"));
            lineEdit_DateiName->setEnabled(true);
            pushButton_restore->setEnabled(false);
            pushButton_save->setEnabled(true);
            label_4->setEnabled(true);
            chk_overwrite->setEnabled(true);
            cmb_zip->setEnabled(true);
            chk_Beschreibung->setEnabled(true);
            
            label_2->setEnabled(true);
           
            AnzahlsaveFile->setEnabled(true);
            AnzahlgesicherteFile->setEnabled(true);
            chk_key->setText (tr("Encrypt\nbackup. key:", "Sicherung\nverschlüsseln. Schlüssel:"));
            chk_split->setEnabled(true);
            QModelIndex cwdIndex = dirModel->index("/");
      	    treeView->setRootIndex(cwdIndex);  
            treeView->setEnabled(false);
            }

void DialogNet::rdButton_auslesen()
     {
     if (rdBt_saveFsArchiv->isChecked())
        {
	starteinstellung();
        }
     if (rdBt_restoreFsArchiv->isChecked())
        {
		label_folder->setText (tr("Backup file", "Sicherungsdatei"));
      		pushButton_save->setText (tr("Partition restore", "Partition zurückschreiben"));
                pushButton_restore->setEnabled(true);
                pushButton_save->setEnabled(false);
                lineEdit_DateiName->setEnabled(false);
                label_4->setEnabled(false);
                chk_overwrite->setEnabled(false);
                cmb_zip->setEnabled(false);
                chk_Beschreibung->setEnabled(false);
                
                label_2->setEnabled(false);
                
                AnzahlsaveFile->setEnabled(false);
                AnzahlgesicherteFile->setEnabled(false);
                chk_key->setText (tr("Decrypt\nbackup. key:", "Sicherung\nentschlüsseln. Schlüssel"));
                chk_split->setEnabled(false);
                treeView->setEnabled(true);
                QModelIndex cwdIndex = dirModel->index("/mnt/qt4-fs-client");
      		treeView->setRootIndex(cwdIndex); 
         }
     } 

QString DialogNet::Zeit_auslesen(){
    int Tag;
    int Monat;
    int Jahr;
    QString Datum_akt;
    
    string stringvariable;
    time_t Zeitstempel;
    tm *nun;
    Zeitstempel = time(0);
    nun = localtime(&Zeitstempel);
    Tag = nun->tm_mday;
    Monat = nun->tm_mon+1;
    Jahr = nun->tm_year+1900;
    _Datum_net = Datum_akt.setNum(Tag) + "-" ; 
    _Datum_net = _Datum_net + Datum_akt.setNum(Monat) + "-" ;
    _Datum_net = _Datum_net + Datum_akt.setNum(Jahr) ;
    return _Datum_net;
}

void DialogNet::listWidget_auslesen() {
    int row;
    extern QString add_part[100];    QStringList partition_kurz;
    row = listWidget->currentRow();
    partition_net_ = add_part[row];
    partition_kurz = partition_net_.split(" ");
    partition_net_ = partition_kurz[0];
}

void DialogNet::folder_file() {
   extern QString folder_file_;
   folder_file_ = folder_net + "/" + DateiName_net + "-" + _Datum_net + ".txt";
}

void DialogNet::startThread1() {
   if( thread1.isRunning() ) return;
   connect( &thread1, SIGNAL(finished()),
            this, SLOT( thread1Ready()));
   thread_run_net = 1;
   thread1.start();
}

void DialogNet::startThread2() {
   if( thread2.isRunning() ) return;
   connect( &thread2, SIGNAL(finished()),
            this, SLOT( thread2Ready()));
   thread_run_net = 2;
   thread2.start();
}

void DialogNet::closeEvent(QCloseEvent *event) {
   thread1.wait();
   thread2.wait();
   event->accept();
}

void DialogNet::thread1Ready()  {
   endeThread_net = endeThread_net + 1;
   extern int dialog_auswertung;
   if (endeThread_net == 1) {
     pushButton_end->setEnabled(true);
     if (dialog_auswertung ==0){ 
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
       int cnt_hardlinks = werte_holen(8);
       cnt_hardlinks = cnt_hardlinks + werte_holen(9);
       QString cnt_hardlinks_ = QString::number(cnt_hardlinks);
       int cnt_special = werte_holen(10);
       QString cnt_special_ = QString::number(cnt_special);   
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("The partition has been backed up successfully.\n", "Die Partition wurde erfolgreich gesichert.\n") + cnt_regfile_ + 
	tr(" files, ", " Dateien, ") + cnt_dir_ + tr("  directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr("  links and ", " Links und ") + cnt_special_ + tr(" specials have been backed.", " Spezielle Daten wurden gesichert."));
     }
     else {
       pushButton_save->setEnabled(false);
       //Beschreibungsdate löschen
     	QString filename = SicherungsDateiName_net;
        int pos = filename.indexOf("fsa");
       	filename = filename.left(pos);
       	filename.insert(pos, QString("txt"));
        QFile f(filename);
        // Prüfen ob text-Datei vorhanden 
       if (f.exists())
          remove (filename.toAscii().data());
        if (flag_end_net == 1) {
        QMessageBox::about(this, tr("Note", "Hinweis"),
         tr("The backup of the partition was aborted by the user!\n", "Die Sicherung der Partition wurde vom Benutzer abgebrochen!\n") );
	}
       // Anzahl nicht korrekt gesicherte Dateien ausgeben
       int part_testen = werte_holen(4);
       if (part_testen == 108 && flag_end_net == 0){
	    QMessageBox::about(this, tr("Note", "Hinweis"),
          tr("The partition type is not supported. Maybe the partition is encrypted?\n", "Der Partitionstyp wird nicht unterstützt. Vielleicht ist die Partition verschlüsselt?\n" ));
          }
       int err_regfile = werte_holen(1);
       QString err_regfile_ = QString::number(err_regfile);
       int err_dir = werte_holen(2);
       QString err_dir_ = QString::number(err_dir); 
       int err_hardlinks = werte_holen(3);
       err_hardlinks = err_hardlinks + werte_holen(5);
       QString err_hardlinks_ = QString::number(err_hardlinks); 
       if (part_testen != 108 && flag_end_net == 0){
       	   QMessageBox::about(this, tr("Note", "Hinweis"),
          err_regfile_ + tr(" files", " Dateien, ") 
	+ err_dir_ + tr(" directories and ", " Verzeichnisse und ") + err_hardlinks_ 
	+ tr(" links were not saved properly. The backup of the partition was only partially successful\n", 
	" Links wurden nicht korrekt gesichert. Die Sicherung der Partition war nur teilweise erfolgreich\n" ));
	  }
        }
       
     }
  thread_run_net = 0;
  thread1.exit();
}

void DialogNet::thread2Ready()  {
   endeThread_net = endeThread_net + 1;
   extern int dialog_auswertung;
   int meldung = werte_holen(4);
   if (meldung == 105) {
      QMessageBox::about(this, tr("Note", "Hinweis"), tr("cannot restore an archive to a partition which is mounted, unmount it first \n", "Die Partition die wiederhergestellt werden soll, ist eingehängt. Sie muss zunächst ausgehängt werden!\n"));
      endeThread_net == 0;
       }
   if (endeThread_net == 1) { 
     pushButton_end->setEnabled(true);
     if (dialog_auswertung ==0){
       pushButton_restore->setEnabled(true);
       progressBar->setValue(100);
       SekundeRemaining ->setText("0");
       int cnt_regfile = werte_holen(6);
       QString cnt_regfile_ = QString::number(cnt_regfile);
       int cnt_dir = werte_holen(7);
       QString cnt_dir_ = QString::number(cnt_dir); 
       int cnt_hardlinks = werte_holen(8);
       cnt_hardlinks = cnt_hardlinks + werte_holen(9);
       QString cnt_hardlinks_ = QString::number(cnt_hardlinks); 
       int cnt_special = werte_holen(10);
       QString cnt_special_;
       cnt_special_ = QString::number(cnt_special);
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("The partition is successful back..\n", "Die Partition wurde erfolgreich wieder hergestellt..\n") + cnt_regfile_ + 
        tr(" files, ", " Dateien, ") + cnt_dir_ + tr("  directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr("  links and ", " Links und ") + cnt_special_ + tr(" specials have been restored.", " spezielle Daten wurden wieder hergestellt."));
        }
       if (flag_end_net == 1) {
        QMessageBox::about(this, tr("Note", "Hinweis"),
         tr("The restore of the partition was break by user!\n", "Die Wiederherstellung der Partition wurde vom Benutzer abgebrochen!\n") );
	meldung == 0;
        }
     if (meldung == 100) {
          // Anzahl nicht korrekt zurückgeschriebene Dateien ausgeben
       pushButton_restore->setEnabled(false);
       int err_regfile = werte_holen(1);
       QString err_regfile_ = QString::number(err_regfile);
       int err_dir = werte_holen(2);
       QString err_dir_ = QString::number(err_dir); 
       int err_hardlinks = werte_holen(3);
       err_hardlinks = err_hardlinks + werte_holen(5);
       QString err_hardlinks_ = QString::number(err_hardlinks); 
       QMessageBox::about(this, tr("Note", "Hinweis"),
         err_regfile_ + tr(" files", " Dateien, ") + err_dir_ +
         tr(" directories and ", " Verzeichnisse und ") + err_hardlinks_ + 
         tr(" links were not recovered correctly. The recovery of the partition was only partly successful.\n", " Links wurden nicht korrekt wiederhergestellt. Die Wiederherstellung der Partition war nur teilweise erfolgreich.\n" ));
        }
     if (meldung == 102) { 
        QMessageBox::about(this, tr("Note", "Hinweis"), 
        tr("You tried to restore a partition. The selected file can only restore directories. Please restart the program.\n", "Sie haben versucht eine Partition wiederherzustellen. Die gewählte Datei kann nur Verzeichnisse wiederherstellen. Bitte starten Sie das Programm neu.\n"));
      }
     if (meldung == 103) { 
        QMessageBox::about(this, tr("Note", "Hinweis"), tr("You have entered an incorrect password.\n", "Sie haben ein falsches Passwort eingegeben. \n"));
        endeThread_net = 0;
        lineKey->setText ("");
      }
    }
    thread_run_net = 0;
    thread2.exit();
}

void DialogNet::elapsedTime()
 {
    sekunde_elapsed_net = sekunde_elapsed_net + 1; 
    sekunde_summe_net = sekunde_summe_net + 1; 
    if (sekunde_elapsed_net == 60) {
        sekunde_elapsed_net = 0 ;
        minute_elapsed_net = minute_elapsed_net + 1;
       }
    QString sekunde_ = QString::number(sekunde_elapsed_net);
    SekundeElapsed ->setText(sekunde_);
    QString minute_ = QString::number(minute_elapsed_net);
    MinuteElapsed ->setText(minute_);
}

void DialogNet::remainingTime(int prozent)
 {
    if (prozent <= 1) 
       sekunde_tara_net = sekunde_summe_net ;
    int sekunde_netto = sekunde_summe_net - sekunde_tara_net;

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

void DialogNet::indicator_reset() {
     // Anzeige zurücksetzen für neuen Aufruf fsarchiver
     werte_reset();
     progressBar->setValue(0);
     AnzahlgesicherteFile ->setText(0);
     AnzahlsaveFile ->setText(0); 
     SekundeRemaining ->setText(" ");
     MinuteRemaining ->setText(" ");
     SekundeElapsed ->setText("0");
     MinuteElapsed ->setText("0");
     sekunde_summe_net = 0;
     minute_elapsed_net = 0;
     endeThread_net = 0;
     sekunde_elapsed_net = 0;
     minute_elapsed_net = 0;
}

int DialogNet::testDateiName(string endung)
{
  string str (folder_net.toAscii().data());
  string str2;
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

void DialogNet::ViewProzent()
{
int prozent;
QString sekunde;
int sekunde_;
QString minute;
int minute_;
int meldung;
int anzahl;
int anzahl1;
QString text_integer;

if (endeThread_net !=1)
{
 timer->singleShot( 1000, this , SLOT(ViewProzent( )) ) ;
  elapsedTime();
  meldung = werte_holen(4);
  	if (meldung >= 100) // Thread Abschluss mit Fehler
   		return;
if (flag_View_net == 1)
	{
 	anzahl  = werte_holen(2);
 	text_integer = text_integer.setNum(anzahl);
 	AnzahlsaveFile ->setText(text_integer);
 	anzahl1  = werte_holen(3);
 	text_integer = text_integer.setNum(anzahl1);
 	AnzahlgesicherteFile ->setText(text_integer);
	}
 prozent = werte_holen(1);
 if (dummy_prozent_net != prozent)
     remainingTime(prozent);
 else {
        if (prozent > 5)
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
 dummy_prozent_net = prozent;
 }
// bei mehrmaligem Aufruf von fsarchiver wird am Anfang der Sicherung kurzfristig 100 % angezeigt, was falsch ist
 if (prozent != 100)  
  	progressBar->setValue(prozent);
 return;
} 


void DialogNet::keyPressEvent(QKeyEvent *event) {
MWindow window;   
     QWidget::keyPressEvent(event);
     switch( event->key() ) {
         case Qt::Key_Escape:
             // int ret = questionMessage(tr("Do you want really break the save or restore from the partition?", "Wollen Sie wirklich die Sicherung oder Wiederherstellung der Partition beenden?"));
            //  if (ret == 1)
               if (window.bit_version() == "64")
               	esc_end(); 
         break;
     }
}

int DialogNet::questionMessage(QString frage)
{
	QMessageBox msg(QMessageBox::Question, tr("Note", "Hinweis"), frage);
	QPushButton* yesButton = msg.addButton(tr("Yes", "Ja"), QMessageBox::YesRole);
	QPushButton* noButton = msg.addButton(tr("No", "Nein"), QMessageBox::NoRole);
	msg.exec();
	if (msg.clickedButton() == yesButton)
    		return 1;
	else if (msg.clickedButton() == noButton)
    		return 2;
   return -1;
}

void DialogNet::esc_end()
{
QString befehl;
   if (thread_run_net > 0) {
    int ret = questionMessage(tr("Do you want really break the save or restore from the partition?", "Wollen Sie wirklich die Sicherung oder Wiederherstellung der Partition beenden?"));
      if (thread_run_net == 1 && ret == 1)
        {
	flag_end_net= 1;     	
	befehl = "rm "  + SicherungsFolderFileName_net;
        system (befehl.toAscii().data()); 
        int pos = SicherungsFolderFileName_net.indexOf("fsa");
       	SicherungsFolderFileName_net = SicherungsFolderFileName_net.left(pos);
       	SicherungsFolderFileName_net.insert(pos, QString("txt"));
        befehl = "rm "  + SicherungsFolderFileName_net;
        system (befehl.toAscii().data());
        thread1.terminate();
        thread1.wait(); 
        close();
        }
     if (thread_run_net == 2 && ret == 1)
        {
        flag_end_net= 1; 
        thread2.terminate();
        thread2.wait();
        close(); 
	}}
}

void DialogNet:: bit()
{
 MWindow window;   
	if (window.bit_version() == "32")
	        pushButton_break->setEnabled(false);
 }
