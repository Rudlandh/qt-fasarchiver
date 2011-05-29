/*
 * qt4-fsarchiver: Filesystem Archiver
 *
 * Copyright (C) 2010 Dieter Baum.  All rights reserved.
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

#include <QtGui> 
#include "mbr.h"
#include <string.h>

extern int dialog_auswertung;
extern int anzahl_disk;

QString folder_;
QString Ubuntuversion;
int sektor;
int sektor_byte;
int sektor_byte_1;
QString Sektor_byte;
QString Sektor_byte_1;
qint64 size_;


DialogMBR::DialogMBR(QWidget *parent)
{
	setupUi(this); // this sets up GUI
	connect( bt_save, SIGNAL( clicked() ), this, SLOT( mbr() ) ); 
        //connect( btEnd, SIGNAL( clicked() ), qApp, SLOT(close()));
        connect( bt_end, SIGNAL( clicked() ), this, SLOT(close()));
        dirModel = new QDirModel;
   	selModel = new QItemSelectionModel(dirModel);
   	treeView->setModel(dirModel);
   	treeView->setSelectionModel(selModel);
   	QModelIndex cwdIndex = dirModel->index(QDir::rootPath());
   	treeView->setRootIndex(cwdIndex);
        rdbt_sda->setChecked(Qt::Checked);
        rdbt_sdd->setEnabled(false);
	rdbt_sdc->setEnabled(false);
	rdbt_sdb->setEnabled(false);
        if (anzahl_disk == 2)
           rdbt_sdb->setEnabled(true);
        if (anzahl_disk == 3){
           rdbt_sdb->setEnabled(true);
           rdbt_sdc->setEnabled(true);
           }
         if (anzahl_disk == 4){
           rdbt_sdb->setEnabled(true);
           rdbt_sdc->setEnabled(true);
           rdbt_sdd->setEnabled(true);
           }

        if (dialog_auswertung == 4){
            bt_save->setText (tr("MBR save", "MBR sichern"));
            cmb_mbr->setEnabled(false);
            } 
        if (dialog_auswertung == 5){
            bt_save->setText (tr("MBR restore", "MBR zurückschreiben"));  
	    cmb_mbr->setEnabled(true);
            }
        //sektor = sektor_auslesen();
       
 }

int DialogMBR::mbr()
{
QString befehl;
int i;
QString partition;
QString homepath = QDir::homePath();

    //Ubuntu Version einlesen, nur wenn Systempartition
    Ubuntuversion_auslesen();
    sektor_auslesen();
    if (rdbt_sda->isChecked())
        partition = "sda";
    if (rdbt_sdb->isChecked())
        partition = "sdb";
    if (rdbt_sdc->isChecked())
        partition = "sdc";
    if (rdbt_sdd->isChecked())
        partition = "sdd";
    if (dialog_auswertung == 4)	
       {
	i = folder_einlesen();
        if (i ==0) {
                QFile file(folder_);
                befehl = ("dd if=/dev/" + partition + " of=" + folder_ + "/" + Ubuntuversion + "_mbr_" + partition + " bs=" + Sektor_byte + " " + "count=1");
                i = system (befehl.toAscii().data());
   		if (i == 0)
      			QMessageBox::about(this,tr("Note", "Hinweis"), tr("The MBR was successfully backed up.\n", "Der MBR wurde erfolgreich gesichert.\n"));
        	else
      			QMessageBox::about(this, tr("Note", "Hinweis"), tr("The MBR has not been backeed.\n", "Der MBR wurde nicht gesichert.\n"));
                }
       }
     if (dialog_auswertung == 5 && cmb_mbr->currentIndex() != 3)	
       {
        i = folder_einlesen();
        if (i ==1)
	   return 0;
        int auswertung = questionMessage(tr("Caution: If you really want to to write back the MBR completely or partially?\n", "Vorsicht: Wollen Sie wirklich den MBR komplett beziehungsweise teilweise zurückschreiben?\n")); 
            if  (auswertung == 2) 
                return 1; 
        i = folder_einlesen();
        if (i ==0) {
	   if (cmb_mbr->currentIndex() == 0) {
              befehl = ("dd if="+ folder_ + " of=/dev/" + partition + " bs=1 count=446");
              i = system (befehl.toAscii().data());
   	      if (i == 0)
      		QMessageBox::about(this, tr("Note", "Hinweis"), tr("The Boot Loader section is successful return.\n", "Der Bootloaderbereich wurde erfolgreich wieder hergestellt.\n"));
              else
      		QMessageBox::about(this, tr("Note", "Hinweis"), tr("The boot loader area was not restored.\n", "Der Bootloaderbereich wurde nicht wieder hergestellt.\n"));
              }
              if (cmb_mbr->currentIndex() == 2) {
		   //verborgenen Bereich extrahieren                   
                   befehl = ("dd if="+ folder_ + " of=" + homepath + "/.mbr.txt  bs=1 skip=446 count=66");
                   i = system (befehl.toAscii().data());
                   //verborgenen Bereich wiederherstellen 	
                   befehl = ("dd if=" + homepath + "/.mbr.txt of=/dev/" + partition + " bs=1 seek=446 count=66");
                   i = i + system (befehl.toAscii().data());  	
                   //Datei löschen
                   befehl = "rm " + homepath + "/.mbr.txt";
                   system (befehl.toAscii().data());
   	           if (i == 0)
      			QMessageBox::about(this, tr("Note", "Hinweis"), tr("The partition table is successful return.", "Die Partitionstabelle wurde erfolgreich wieder hergestellt.\n"));
                   else
      			QMessageBox::about(this, tr("Note", "Hinweis"), tr("The partition table was not restored.\n", "Die Partitionstabelle wurde nicht wieder hergestellt.\n"));
               }
               if (cmb_mbr->currentIndex() == 1) {
                 befehl = ("dd if="+ folder_ + " of=/dev/" + partition + " bs=1 count=512");
                 i = system (befehl.toAscii().data());
   	       if (i == 0) 
      		   QMessageBox::about(this, tr("Note","Hinweis"), tr("The MBR is successful return.\n", "Der MBR wurde erfolgreich wieder hergestellt.\n"));
                else
      		  QMessageBox::about(this, tr("Note", "Hinweis"), tr("The MBR is not restored.\n", "Der MBR wurde nicht wieder hergestellt.\n"));
               }
               
          }
    }
    if (dialog_auswertung == 5 && cmb_mbr->currentIndex() == 3)	
       {
        i = folder_einlesen();
        if (i ==1)
	   return 0;
        int auswertung = questionMessage(tr("Caution: If you really want to to write back the secret field?\n", "Vorsicht: Wollen Sie wirklich den verborgenen Bereich zurückschreiben? \n")); 
            if  (auswertung == 2) 
                return 1; 
            i = folder_einlesen();
            if (i ==0) {
		   //verborgenen Bereich extrahieren                   
                   befehl = ("dd if="+ folder_ + " of=" + homepath + "/.mbr.txt  bs=1 skip=512 count=" + Sektor_byte_1);
                   i = system (befehl.toAscii().data());
                   //verborgenen Bereich wiederherstellen 	
                   befehl = ("dd if=" + homepath + "/.mbr.txt of=/dev/" + partition + " bs=1 seek=512 count=" + Sektor_byte_1);
                   i = i + system (befehl.toAscii().data());  	
                   //Datei löschen
                   befehl = "rm " + homepath + "/.mbr.txt";
                   i = system (befehl.toAscii().data());
   	        if (i == 0) 
      		   QMessageBox::about(this, tr("Note", "Hinweis"), tr("The hidden area is successful return.\n", "Der verborgene Bereich wurde erfolgreich wieder hergestellt.\n"));
                else
      		  QMessageBox::about(this, tr("Note", "Hinweis"), tr("The hidden area was not restored.\n", "Der verborgene Bereich wurde nicht wieder hergestellt.\n"));
        }
    }
    return 0;
}

void DialogMBR::sektor_auslesen() {
QString Dateiname;
QString befehl;
QString text;
QString partition;
QString homepath = QDir::homePath();
int sektor_;
        partition = "sda"; 
    	if (rdbt_sdb->isChecked())
        	partition = "sdb";
    	if (rdbt_sdc->isChecked())
        	partition = "sdc";
    	if (rdbt_sdd->isChecked())
        	partition = "sdd";
        // fdisk -lu , mit diesem Befehl Startsektor von sda1 ermitteln.
        // derzeit für Ubuntu:  Startsektor sda1 = 63*512 = 32256 
	// Sektornummer in Datei abspeichern
        Dateiname = homepath + "/.sektornummer.txt";
        befehl = "fdisk -lu | grep " + partition + "1 > " + homepath + "/.sektornummer.txt";
        system (befehl.toAscii().data());
        QFile file(Dateiname);
        //Datei auslesen
        if( file.open(QIODevice::ReadOnly|QIODevice::Text)) {
            QTextStream ds(&file);
            text = ds.readLine();
            text = text.left(28);
            text = text.right(10);
            file.close();
            sektor_ = text.toInt();
          }
       //Datei löschen
       befehl = "rm "  + homepath + "/.sektornummer.txt";
       system (befehl.toAscii().data()); 
       if (sektor_ < 2) {
	    QMessageBox::about(this, tr("Note", "Hinweis"), tr("The end of hidden area of the 1st Partition could not be read. Only 512 bytes are saved.", "Das Ende des verborgenen Bereiches der 1. Partition konnte nicht ausgelesen werden. Es werden nur 512 Bytes gesichert.\n"));
            sektor = 2;
	}
        sektor_byte = sektor_  * 512;
        Sektor_byte =  QString::number(sektor_byte);
        sektor_byte_1 = (sektor_ -1) * 512 ;
        Sektor_byte_1=  QString::number(sektor_byte_1);  
}

void DialogMBR::Ubuntuversion_auslesen() {
QString Dateiname;
QString partition;
QString befehl;
QStringList Ubuntu_;
string part_art;
        if (rdbt_sda->isChecked())
        	partition = "sda";
    	if (rdbt_sdb->isChecked())
        	partition = "sdb";
    	if (rdbt_sdc->isChecked())
        	partition = "sdc";
    	if (rdbt_sdd->isChecked())
        	partition = "sdd";
        partition = "/dev/"+ partition;
        //Prüfen ob System Partition
        if (mtab_einlesen(partition.toAscii().data()) == "system") {
             	Dateiname = "/etc/issue";     
        	QFile file(Dateiname);
        	//Datei auslesen
        	if( file.open(QIODevice::ReadOnly|QIODevice::Text)) {
            		QTextStream ds(&file);
            		Ubuntuversion = ds.readLine();
            		Ubuntu_ = Ubuntuversion.split(" ");
            		Ubuntuversion = Ubuntu_[1];
            		file.close();
                }
         }
         else
		Ubuntuversion = "";
   
}

int DialogMBR::folder_einlesen() {
   QString partition;
   QString Festplatte;
   int ret = 1; 
   int pos;
   int pos1;
   QModelIndex index = treeView->currentIndex();
   QModelIndexList indexes = selModel->selectedIndexes();
   folder_.append  (dirModel->filePath(index));
   folder_ =  (dirModel->filePath(index));
   if (dialog_auswertung == 5)
	Festplatte = folder_.right(3);
   QFileInfo info(folder_); 
   size_ = info.size(); 
   if (folder_ == "" && (dialog_auswertung == 4))
      {
       QMessageBox::about(this, tr("Note", "Hinweis"),
      tr("You must select a directory", "Sie müssen ein Verzeichnis auswählen\n"));
      return 1 ;
      }
   if (info.isFile() && (dialog_auswertung == 4))
      {
      QMessageBox::about(this, tr("Note", "Hinweis"),
      tr("You have selected a file. You must select a directory.\n", "Sie haben eine Datei ausgewählt. Sie müssen ein Verzeichnis auswählen\n"));
      return 1 ;
      }
    if (folder_ == "" && (dialog_auswertung == 5))
      {
       QMessageBox::about(this, tr("Note", "Hinweis"),
      tr("You must choose the MBR file\n","Sie müssen die MBR Sicherungsdatei auswählen.\n"));
      return 1 ;
      }
    if (info.isDir() && (dialog_auswertung == 5))
      {
      QMessageBox::about(this, tr("Note", "Hinweis"),
      tr("You have selected a directory. You must select the MBR backup file\n", "Sie haben ein Verzeichnis ausgewählt. Sie müssen die MBR Sicherungsdatei auswählen\n"));
      return 1 ;
      }
    pos = folder_.indexOf("mbr_sd");
    if (rdbt_sda->isChecked()){
        partition = "sda";
        if (dialog_auswertung == 5)
            pos1 = folder_.indexOf("mbr_sda");
    }
    if (rdbt_sdb->isChecked()){
        partition = "sdb";
        if (dialog_auswertung == 5)
           pos1 = folder_.indexOf("mbr_sdb");
    }
    if (rdbt_sdc->isChecked()){
       	partition = "sdc";
        if (dialog_auswertung == 5)
           pos1 = folder_.indexOf("mbr_sdc");
    }
    if (rdbt_sdd->isChecked()){
        partition = "sdd";
        if (dialog_auswertung == 5)
           pos1 = folder_.indexOf("mbr_sdd");
    }
    if (pos1 == -1  && dialog_auswertung == 5){
      //Partitionen stimmen nicht überein
      QString text = tr("You may have the wrong disk is selected. The restore disk is ", "Sie haben eventuell eine falsche Festplatte ausgewählt. Die wiederherzustellende Festplatte ist ") + partition + tr(", the saved hard drive is ", ",  die gesicherte Festplatte ist aber ") +  Festplatte + tr(" Are you sure?", " Wollen Sie fortfahren?");
        ret = questionMessage(text);
        if (ret == 2)
            return 1;
        }
    QString sA = QString::number(size_);
    QString text = tr("You may have a wrong file selected. Restore the hidden field has a size of ", "Sie haben eventuell eine falsche Datei ausgewählt. Der wiederherzustellende verborgene Bereich hat eine Größe von") +  sA + 
    tr(" bytes. The hidden area of the disk ", " Byte. Der verborgene Bereich von der Festplatte ") + partition + tr(" has a size of ", " hat eine Größe von ") + Sektor_byte + tr(" bytes. Are you sure?", " Byte. Wollen Sie fortfahren?");
    if (size_ != sektor_byte && dialog_auswertung == 5 && cmb_mbr->currentIndex() == 3) {
        ret = questionMessage(text);
        if (ret == 2)
            return 1;
        if (ret == 1)
           return 0;
       }
    if (dialog_auswertung == 5 && pos == -1 ){
      //Länge der Datei und Name überprüfen
      QMessageBox::about(this, tr("Note", "Hinweis"),
      tr("You have no MBR backup file selected. Name or size is not correct\n", "Sie haben keine MBR Sicherungsdatei ausgewählt. Name oder Größe ist nicht korrekt\n"));
      return 1 ;
      }
    return 0;
}

int DialogMBR::questionMessage(QString frage)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, tr("Note", "Hinweis"),
                                    frage,
                                    //QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
                                     QMessageBox::No | QMessageBox::Yes | QMessageBox::Default);
    if (reply == QMessageBox::Yes)
        return 1;
    else 
        return 2;
    
    
}

string DialogMBR::mtab_einlesen(string partition_if_home)
{
   QString text;
   QFile file("/etc/mtab");
   QTextStream ds(&file);
   string text_str;
   int pos;
   if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
     text = ds.readLine();
     text_str = text.toAscii().data();
     pos = text_str.find(partition_if_home); 
     if (pos > -1)  // Partitionsname gefunden
        return "system";
     file.close();
     }
  return  " ";
}
