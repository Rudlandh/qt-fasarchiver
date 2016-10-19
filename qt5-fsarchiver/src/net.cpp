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
#include <string.h>
#include <stdio.h>
#include <QtGui> 
#include "net.h"
#include "net_ein.h"
#include "filedialog.h"
#include "mainWindow.h"
#include <iostream>
#include "treeview.h"
#include <unistd.h>
#include "setting.h"

extern "C" {
#include "connect_c_cpp.h"
#include "fsarchiver.h"
}
using namespace std;

extern int dialog_auswertung;
extern int btrfs_flag;
QString zip_net[10];
QString folder_net;
QString file_net;
QString folder_free;
QString folder_free_[100];
QString partition_net; // z.B 192.168.2.5
QString partition_net_; // z.B sda1
QString UUID_net;
QString partition_typ_net;
QString DateiName_net("") ;
QString _Datum_net;
QString part_art_net;
QString user_net;
QString key_net;
QString rechner_IP;
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
QString befehl_pbr_net;
int stopFlag_;
extern int fsarchiver_aufruf(int argc, char *anlage0=NULL, char 
*anlage1=NULL, char *anlage2=NULL, char *anlage3=NULL, char 
*anlage4=NULL, char *anlage5=NULL, char *anlage6=NULL, char 
*anlage7=NULL, char *anlage8=NULL, char *anlage9=NULL, char 
*anlage10=NULL, char *anlage11=NULL, char *anlage12=NULL, char 
*anlage13=NULL, char *anlage14=NULL);
QStringList items_Net;
QString pfad_back;
QString pfad_forward;
int backFlag;
int cmbFlag = 0;
int listwidgetrow = 0;
QString folder_dir_net;

DialogNet::DialogNet(QWidget *parent)
{
QString homepath = QDir::homePath(); 	
QString befehl; 
QStringList items;
QStringList items_kerne_; 
setupUi(this); // this sets up GUI
        folder_net = homepath + "/.qt5-fs-client";
        if ( dialog_auswertung == 6)
        	rdBt_saveFsArchiv->setChecked(Qt::Checked);
        else
		rdBt_restoreFsArchiv->setChecked(Qt::Checked);
        dirModel = new QFileSystemModel;
   	selModel = new QItemSelectionModel(dirModel);
        QModelIndex cwdIndex = dirModel->index(QDir::rootPath());
        dirModel->setRootPath(QDir::rootPath());
        treeView_dir->setModel(dirModel);
        treeView_dir->setSelectionModel(selModel);
   	treeView_dir->setRootIndex(cwdIndex);
        connect( chk_path, SIGNAL( clicked()), this, SLOT(rdButton_auslesen()));
      	connect( pushButton_end, SIGNAL( clicked() ), this, SLOT(end()));
        connect( pushButton_save, SIGNAL( clicked() ), this, SLOT(savePartition()));
   	connect( pushButton_restore, SIGNAL( clicked() ), this, SLOT(restorePartition()));
       	connect( rdBt_saveFsArchiv, SIGNAL( clicked() ), this, SLOT(rdButton_auslesen()));
   	connect( rdBt_restoreFsArchiv, SIGNAL( clicked() ), this, SLOT(rdButton_auslesen())); 
        connect( pushButton_partition, SIGNAL( clicked() ), this, SLOT(listWidget_auslesen()));
        connect( pushButton_folder_free, SIGNAL( clicked() ), this, SLOT(listWidget_folder_free_auslesen()));
        connect( pushButton_break, SIGNAL( clicked() ), this, SLOT(esc_end()));
        connect( chk_key, SIGNAL( clicked() ), this, SLOT(chkkey()));
        connect( bt_net_art, SIGNAL( clicked() ), this, SLOT(cmb_net()));
        connect( bt_toParent, SIGNAL( clicked() ), this, SLOT(button_toParent()));
        connect(treeWidget, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this, SLOT(listWidget_tree_auslesen(QTreeWidgetItem *, int)));
        connect( bt_treeWiget, SIGNAL( clicked() ), this, SLOT(treeWidget_auslesen()));
        connect( rdBt_showPartition, SIGNAL( clicked() ), this, SLOT(save_partitions()));
        connect( rdBt_showDirectories, SIGNAL( clicked() ), this, SLOT(save_directories()));
        connect( chk_hidden, SIGNAL( clicked() ), this, SLOT(chkhidden()));
        if (rdBt_saveFsArchiv->isChecked()){
  	    pushButton_restore->setEnabled(false);
            pushButton_save->setEnabled(true);}
        else
            {
  	    pushButton_restore->setEnabled(true);
            pushButton_save->setEnabled(false);}
        timer = new QTimer(this);
        items_Net << "Samba" << "SSH" << "NFS";
        //items_Net << "Samba" << "NFS";
	cmb_Net->addItems (items_Net);
	items_Net.clear();
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
        pid_ermitteln();
        // Ini-Datei auslesen
   	QFile file1(homepath + "/.config/qt5-fsarchiver/qt5-fsarchiver.conf");
        cmb_Net -> setCurrentIndex(0);
        treeWidget->setHidden(true);
        rdBt_showPartition->setChecked(Qt::Checked);
        if (rdBt_showPartition->isChecked())
           {
           listWidget ->setHidden(false);
           treeView_dir ->setHidden(true);
           }
        if (rdBt_showDirectories->isChecked())
           {
           listWidget ->setHidden(true);
           treeView_dir ->setHidden(false);
           }
   if (file1.exists()) {
        QSettings setting("qt5-fsarchiver", "qt5-fsarchiver");
        setting.beginGroup("Basiseinstellungen");
        int auswertung = setting.value("Kompression").toInt();
        cmb_zip -> setCurrentIndex(auswertung); 
        auswertung = setting.value("Kerne").toInt();
        cmb_kerne -> setCurrentIndex(auswertung-1); 
        auswertung = setting.value("Network").toInt();
        cmb_Net -> setCurrentIndex(auswertung-1); //Auswahl setzen
        if (auswertung == 0)
            cmb_Net -> setCurrentIndex(1); //Auswahl setzen
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
        auswertung = setting.value("place").toInt();
        if (auswertung ==1)
           chk_path->setChecked(Qt::Checked); 
        auswertung = setting.value("Passwort").toInt();
        if (auswertung ==1){
           	lineKey ->setEchoMode(QLineEdit::Normal);
        } 
   	else
		lineKey ->setEchoMode(QLineEdit::Password);
        setting.endGroup();
        } 
   else {
        lineKey ->setEchoMode(QLineEdit::Password);
        cmb_Net -> setCurrentIndex(1); //Auswahl setzen
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
   	addWidget();
          
    QFile file(homepath + "/.config/qt5-fsarchiver/qt5-fsarchiver.conf");
    QTextStream ds(&file);
    int a = 0;
    int b = 0;
    int c = 0;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QString text = ds.readLine();
   	  while (!ds.atEnd())
      	  {
          if (text.indexOf("Network=") > -1)
              c = c + 1; 
          if (text.indexOf("ssh=") > -1)
              a = a + 1;
          if (text.indexOf("sshfs=") > -1)
              b = b + 1;
           text = ds.readLine();
          } }
   	file.close();
        QSettings setting("qt5-fsarchiver", "qt5-fsarchiver");
        setting.beginGroup("Basiseinstellungen"); 
        if (a ==0)
           setting.setValue("ssh",1);
        if (b ==0)
            setting.setValue("sshfs",1);
        if (c ==0)
            setting.setValue("Network",2);
        setting.endGroup();
        cmbFlag = 1;
        cmb_net();
       // dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
        dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Hidden);
} 

void DialogNet::chkkey(){
     Qt::CheckState state;
     state = chk_key->checkState();
     if (state == Qt::Checked)
        lineKey->setEnabled(true);
     else 
	lineKey->setEnabled(false);
}  
void DialogNet::cmb_net(){
if (cmbFlag == 0)
   return;
int net_art = cmb_Net->currentIndex(); //auslesen
if (net_art == -1)
    net_art = 1;
if (net_art == 0){
        listWidget_free_folder->setHidden(false);
        treeWidget->setHidden(true);
	bt_toParent ->setHidden(true);
        Daten_Samba_eintragen();}
if (net_art == 1){
        listWidget_free_folder->setHidden(true);
        treeWidget->setHidden(false);
	bt_toParent ->setHidden(false);
	QString index = QString::number(net_art);
        Daten_SSH_eintragen();}
if (net_art == 2){
        listWidget_free_folder->setHidden(false);
        treeWidget->setHidden(true);
	bt_toParent ->setHidden(true);
	QString index = QString::number(net_art);
        Daten_NFS_eintragen();
       	}
}

void DialogNet:: end()
{ 
QString befehl;
QString filename;
QString homepath = QDir::homePath();
        befehl = "umount -f" + homepath + "/.qt5-fs-client 2>/dev/null";
	system (befehl.toLatin1().data());   
        befehl = "fusermount -u " + homepath + "/.qt5-fs-client 2>/dev/null";
        system (befehl.toLatin1().data());
        befehl = "umount -a -t nfs 2>/dev/null";
        system (befehl.toLatin1().data());

 	QFile file1(homepath + "/.config/qt5-fsarchiver/ip.txt");
	filename = homepath + "/.config/qt5-fsarchiver/ip.txt";
	if (file1.exists()) {
     		befehl = "rm " + filename;
		system (befehl.toLatin1().data());
        } 
	QFile file2(homepath + "/.config/qt5-fsarchiver/smbtree.txt");    
        filename = homepath + "/.config/qt5-fsarchiver/smbtree.txt";
        if (file2.exists()){
     		befehl = "rm " + filename;
		system (befehl.toLatin1().data());
        }
        QFile file3(homepath + "/.config/qt5-fsarchiver/smbclient.txt");    
        filename = homepath + "/.config/qt5-fsarchiver/smbclient.txt";
        if (file3.exists()){
     		befehl = "rm " + filename;
		system (befehl.toLatin1().data()); 
       } 
       cmbFlag = 0;
       close(); 
}

void DialogNet:: Daten_NFS_eintragen()
{
	this->setCursor(Qt::WaitCursor);
	NetEin netein;	
	QString rechner;
	QStringList items;
	rechner = netein.Namen_holen();
   	txt_rechner_net ->setText(rechner);
   	//IP-Adresse Netzrechner
   	items = rechner.split(" ");  
   	rechner_IP = items[0];
   	user_net = netein.user_holen();
   	key_net = netein.key_holen();
        //freigegebene Ordner ermitteln
        int i = nfs_search_folder_free(rechner_IP); 
   	if ( i==1){
        	QMessageBox::about(this,tr("Note", "Hinweis"),
        tr("Can not find a shared directory with the NFS Protokoll.\n", "Mit dem NFS Protokoll wurde kein freigegebenes Verzeichnis gefunden.\n"));
	}
    	this->setCursor(Qt::ArrowCursor); 
}

void DialogNet:: Daten_SSH_eintragen()
{
	this->setCursor(Qt::WaitCursor);
        bt_toParent->setEnabled(true);
	NetEin netein;	
	QString rechner;
	QStringList items;
	rechner = netein.Namen_holen();
   	txt_rechner_net ->setText(rechner);
   	//IP-Adresse Netzrechner
   	items = rechner.split(" ");  
   	rechner_IP = items[0];
   	user_net = netein.user_holen(); 
   	key_net = netein.key_holen();
        // Ordner ermitteln
        listWidget_tree_eintragen(rechner_IP, key_net, user_net, "/", 0);
        this->setCursor(Qt::ArrowCursor);  
}
   
void DialogNet:: Daten_Samba_eintragen()
{
this->setCursor(Qt::WaitCursor);
NetEin netein;	
QString rechner;
QStringList items;
int i = 0;
   rechner = netein.Namen_holen();
   txt_rechner_net ->setText(rechner);
   //IP-Adresse Netzrechner
   items = rechner.split(" ");  
   rechner_IP = items[0];
   user_net = netein.user_holen(); 
   key_net = netein.key_holen();
   //freigegebene Ordner ermitteln
   i = search_folder_free(rechner_IP); 
   if ( i==1){
        QMessageBox::about(this,tr("Note", "Hinweis"),
        tr("Can not find a shared directory with the Samba Protokoll.\n", "Mit Samba wurde kein freigegebenes Verzeichnis gefunden.\n"));
	}
    this->setCursor(Qt::ArrowCursor); 
   
}

int DialogNet::nfs_search_folder_free(QString rechner_IP){
QString befehl;
int i = 0;
QString text;
QStringList items;
QString homepath = QDir::homePath(); 
listWidget_free_folder -> clear();
	// freigegebene Verzeichnisse ermitteln
    	befehl = "showmount -e " + rechner_IP + " 1> " +  homepath + "/.config/qt5-fsarchiver/smbclient.txt";
	system (befehl.toLatin1().data()); 
        QFile file(homepath + "/.config/qt5-fsarchiver/smbclient.txt");
	QTextStream ds(&file);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
           text = ds.readLine();
           
   	  while (!ds.atEnd())
      	{
          text = ds.readLine();
 	  items = text.split(" ");
          folder_free = items[0];
          folder_free = folder_free.trimmed();
          listWidget_free_folder ->addItem (folder_free);
          folder_free_[i] = folder_free;
          i = i + 1;
          } 
   	file.close();
        if ( i==0){
           return 1 ;
        }
     }
    folder_free = "";
    return 0;	
}

int DialogNet::search_folder_free(QString rechner_IP){
QString befehl;
int line = 0;
int i = 0;
int pos = 0;
QString text;
QStringList items;
QString homepath = QDir::homePath(); 
listWidget_free_folder -> clear();
	// freigegebene Verzeichnisse ermitteln
    	befehl = "smbclient -N -L" + rechner_IP + " 1> " +  homepath + "/.config/qt5-fsarchiver/smbclient.txt";
	system (befehl.toLatin1().data()); 
        QFile file(homepath + "/.config/qt5-fsarchiver/smbclient.txt");
	QTextStream ds(&file);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
           text = ds.readLine();
   	  while (!ds.atEnd())
      	{
 	       if (text.indexOf("Disk") > -1)  {
                   if (text.indexOf("print$") == -1){
                     pos = text.indexOf("Disk");
                     folder_free = text.left(pos);
                     folder_free = folder_free.trimmed();
                     listWidget_free_folder ->addItem (folder_free);
                     folder_free_[i] = folder_free;
                     i = i + 1;
		               if (i > 99) {
                        file.close();
                        return 1;
                       }
   	       } }
   	       	line++;
            text = ds.readLine();
      	} 
   	file.close();
        if ( i==0){
           return 1 ;
        }
     }
    folder_free = "";
    return 0;	
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
QModelIndex index = treeView_dir->currentIndex();
folder_dir_net.append  (dirModel->filePath(index));
folder_dir_net =  (dirModel->filePath(index));
QFileInfo info(folder_dir_net); 
FileDialog filedialog;
QFile file(folder_net);
QString homepath = QDir::homePath();
     QString Befehl;
     QString text;
     Qt::CheckState state;
     Qt::CheckState state1;
     Qt::CheckState state3;
     char * part_;
     int err = 0;
     int k = 0;
     QString keyText = "";
     int zip;
     indicator_reset();
     int net_art = cmb_Net->currentIndex();
     int found = 0;
     int stop = 0;
     if (rdBt_saveFsArchiv->isChecked())
     {
     	if (folder_free == "")
           {
          	QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("Please, select a folder.\n", "Bitte wählen Sie einen Ordner aus.\n"));
		return 0 ;
           }
        if (rdBt_showPartition->isChecked() && (partition_net_ == ""))
           {
          	QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("Please, select the partition to be saved.\n", "Bitte wählen Sie die zu sichernde Partition aus.\n"));
		return 0 ;
           }
        if (rdBt_showDirectories->isChecked() && (folder_dir_net == ""))
           {
          	QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("Please, select the directorie to be saved.\n", "Bitte wählen Sie das zu sichernde Verzeichnis aus.\n"));
		return 0 ;
           }
        DateiName_net = lineEdit_DateiName->text();
        if (DateiName_net == "")
           {
          	QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("Please, select the filename of the backup.\n", "Bitte wählen Sie den Dateinamen der Sicherung aus.\n"));
		return 0 ;
           }
        //Überprüfen ob im Dateinamen "/" enthalten ist. Wird durch "-" ersetzt
        while (found > -1){
          found = DateiName_net.indexOf("/");
          if (found > -1)
             	DateiName_net.replace(found, 1, "-"); 
        }
        if (file.open(QIODevice::ReadOnly) && rdBt_showPartition->isChecked())
        	  {
                QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("You have selected a file. You must select a directory\n", "Sie haben eine Datei ausgewählt. Sie müssen ein Verzeichnis auswählen\n"));
		file.close();
		return 0 ;
 	        }
this->setCursor(Qt::WaitCursor);
QString zeichen = "'";
       if (net_art == 1) //SSH
         stop = folder_free_mounten();
         if (stop == 256){
             this->setCursor(Qt::ArrowCursor); 
            return 1;}
       if (net_art == 0){  //Samba
         QString befehl = "mount -t cifs -o username=" + user_net + ",password=" + key_net + ",uid=0,gid=0 //" + rechner_IP + "/'" + folder_free + "' " + homepath + "/.qt5-fs-client";
         k = system (befehl.toLatin1().data());
	}
       if (net_art == 2){ //NFS
           QString befehl = "mount " + rechner_IP + ":" + folder_free +  " " + homepath + "/.qt5-fs-client" ;
           k = system (befehl.toLatin1().data()); 
	}  
this->setCursor(Qt::ArrowCursor); 
	 if (k != 0){
   	QMessageBox::about(this, tr("Note", "Hinweis"),
      	tr("The network computer ",  "Der Netzwerkrechner ") + rechner_IP + tr(" could not be integrated. The program is aborted\n", " konnte nicht eingebunden werden. Das Programm wird abgebrochen\n"));
        return 1;}
         _Datum_net = window.Zeit_auslesen();
         state = chk_Beschreibung->checkState();
         zip = cmb_zip->currentIndex()+1;
         if (zip == 0)
             zip = 1;
         int liveFlag = 0;
         int row;
         char  dev_[50] = "/dev/";
         QString dummy;
         if (rdBt_showPartition->isChecked())
         {
         	//Überprüfung ob gemounted
         	part_ = partition_net_.toLatin1().data();
         	strcat (dev_ , part_);  // String zusammenfügen: dev_  = dev_ + part_
         	dummy = partition_net_;
	 	// Werte sammeln und nach file_dialog übergeben, Partition ist eingehängt
         	part_art_net = window.mtab_einlesen("/dev/" + partition_net);
                row = listWidget->currentRow();
                text = window.beschreibungstext("/dev/" + partition_net_, DateiName_net + "-" + _Datum_net + ".fsa", zip, row);
                filedialog.werte_uebergeben(text);
                partition_net = dummy;
         } 
         if (window.is_mounted(dev_) && rdBt_showPartition->isChecked()) 
            {
		   UUID_net = window.UUID_auslesen(listwidgetrow);
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
                         if (rdBt_showPartition->isChecked())
       			 	           parameter[1] = "savefs"; 
                         if (rdBt_showDirectories->isChecked())
                                parameter[1] = "savedir"; 
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
				   if (rdBt_showPartition->isChecked())
         		          {
                                  QFile file_suse(".snapshots");
                                  if (file.exists())
                                    {
                                    parameter[indizierung] = "--exclude=./snapshots";
                                    indizierung = indizierung + 1;
                                    }
         		           // Windows Auslagerungsdatei pagefile.sys von der Sicherung immer ausschließen
                                  parameter[indizierung] = "--exclude=pagefile.sys";
                                  indizierung = indizierung + 1;
                                  // Vorbereitung für psb
				QString befehl = ("dd if=/dev/" + partition_net_ + " of=" + folder_net + "/" + DateiName_net + "-" + _Datum_net + ".pbr" + " bs=512 count=1");
				state = chk_pbr->checkState();
				if (state == Qt::Checked)
					  system (befehl.toLatin1().data());
 }                                 
                                  parameter[indizierung] = (folder_net + "/" + DateiName_net + "-" + _Datum_net + ".fsa");
                                  SicherungsFolderFileName_net = parameter[indizierung];
                         if (rdBt_showPartition->isChecked())
       			 	parameter[indizierung + 1] = ("/dev/" + partition_net_);
                         if (rdBt_showDirectories->isChecked())
                                parameter[indizierung + 1] = folder_dir_net; 
                                  QFile f(parameter[indizierung]);
                                  if  (rdBt_showPartition->isChecked() && parameter[4] != "-o" && (f.exists())){
				       QMessageBox::about(this, tr("Note", "Hinweis"),
         	  			  tr("The partition file ", "Die Partitionsdatei ")   + parameter[indizierung] + tr("already exists. The backup is not performed\n", " ist bereits vorhanden. Die Sicherung wird nicht durchgeführt\n"));
                  			  return 0 ; 
               				}
                                   if  (rdBt_showDirectories->isChecked() && parameter[4] != "-o" && (f.exists())){
				       QMessageBox::about(this, tr("Note", "Hinweis"),
         	  			  tr("The directorie file ", "Die Verzeichnisdatei ")   + parameter[indizierung] + tr("already exists. The backup is not performed\n", " ist bereits vorhanden. Die Sicherung wird nicht durchgeführt\n"));
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
     	     			 if (rdBt_showPartition->isChecked())
     	     			 {
             			  int wert = dlg->exec();
             			 if (wert == 0 && dialog_auswertung == 2)
                		      {
                		      QMessageBox::about(this, tr("Note", "Hinweis"),
         			      tr("The backup was aborted by the user\n", "Die Sicherung wurde vom Benutzer abgebrochen\n"));
				      pushButton_save->setEnabled(true);
                                      return 0;
                		      }
                	 }
				   }
                       
				thread1.setValues(indizierung + 2,"0");
                                pushButton_save->setEnabled(false);
                                pushButton_end->setEnabled(false); 
                                werte_reset();
                                flag_View_net = 1;
  				timer->singleShot( 1000, this , SLOT(ViewProzent( ))) ; 
                                stopFlag_ = 0;
                                this->setCursor(Qt::WaitCursor);
				startThread1(); // fsarchiver wird im Thread ausgeführt
        			
			}
       }
       return 0;
}

int DialogNet::restorePartition() {
MWindow window;
Qt::CheckState state;
Qt::CheckState state1;
QModelIndex index = treeView_dir->currentIndex();
folder_dir_net.append  (dirModel->filePath(index));
folder_dir_net =  (dirModel->filePath(index));
QFileInfo info(folder_dir_net); 
QFile file(file_net);
QString DateiName("") ;
char * part_;
int err = 0, y = 0;
QString keyText = "";
char * dev_part;
int pos= 0;
int pos1 = 0;
int cmp = 0;
char  dev_[50] = "/dev/";
int stop = 0;
QString str = ""; 
QString str1 = "";
        state = chk_path->checkState();
        state1 = chk_key->checkState();
	indicator_reset();
        int net_art = cmb_Net->currentIndex();
       	if (rdBt_restoreFsArchiv->isChecked())
        {
          keyText = lineKey->text();
          if (state1 == Qt::Checked && keyText.isEmpty())  
              {
                QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("No key was given for the decryption\n", "Es wurde kein Schlüssel für die Entschlüsselung angegeben.\n"));
		return 1 ;
               }
           if (file_net == "")
           {
          	QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("Please, select the filename of the backup.\n", "Bitte wählen Sie den Dateinamen der Sicherung aus.\n"));
                return 0;
           }
          if (rdBt_showPartition->isChecked() && (partition_net_ == ""))
           {
          	QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("Please, select the partition to be written back.\n", "Bitte wählen Sie die zurück zu schreibende Partition aus.\n"));
		return 0 ;
           }
          if (rdBt_showDirectories->isChecked() && (folder_dir_net == "" && state != Qt::Checked))
           {
          	QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("Please, select the directorie to be written back.\n", "Bitte wählen Sie das zurück zu schreibende Verzeichnis aus.\n"));
		return 0 ;
           }
           if (rdBt_restoreFsArchiv->isChecked())
           	{
                 pos = testDateiName(".fsa"); 
                 pos1= testDateiName("part.fsa");
         		if (pos > 0 && pos1 >0)
         	   {
           	   QMessageBox::about(this, tr("Note", "Hinweis"),
         	   tr("You have chosen the wrong recovery file selected.\nThe files should end with. fsa be", "Sie haben eine falsche Wiederherstellungsdatei ausgesucht ausgesucht \nDie Dateiendung muss .fsa sein"));
                    return 0;
         	   }
             	}
          // fsarchinfo Archive type auslesen
             int dir_ = archinfo(file_net);
             if (dir_ == 2  && rdBt_showDirectories->isChecked()){
		QMessageBox::about(this, tr("Note","Hinweis"), tr("The file contains a partition backup. The file can not be restored as a directory.\n", "Die Datei enthält eine Partitionssicherung. Die Datei kann nicht als Verzeichnis zurückgeschrieben werden. \n"));
           		   lineKey->setText ("");
		return 0;
            }
            if (dir_ == 1 && rdBt_showPartition->isChecked()){
		QMessageBox::about(this, tr("Note","Hinweis"), tr("The file contains a directory backup. The file can not be restored as a partition.\n", "Die Datei enthält eine Verzeichnissicherung. Die Datei kann nicht als Partition zurückgeschrieben werden. \n"));
           		   lineKey->setText ("");
		return 0;
            }  
          if (net_art == 1)
              stop = folder_free_mounten();
          this->setCursor(Qt::WaitCursor);  
               state = chk_Beschreibung->checkState();
              this->setCursor(Qt::ArrowCursor);
		// Archinfo einholen um Originalpartition einzulesen und um Verschlüsselung zu überprüfen
               // Annahme zunächst kein Schlüssel
               char * optionkey;
               parameter[0] = "fsarchiver";
       	       parameter[1] = "archinfo";
		if (state1 != Qt::Checked) {
               		parameter[2] = file_net;
               		fsarchiver_aufruf(3,parameter[0].toLatin1().data(),parameter[1].toLatin1().data(),parameter[2].toLatin1().data(),parameter[3].toLatin1().data());
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
			parameter[4] = file_net;
                        int retour =  fsarchiver_aufruf(5,parameter[0].toLatin1().data(),parameter[1].toLatin1().data(),parameter[2].toLatin1().data(),parameter[3].toLatin1().data(),parameter[4].toLatin1().data (),parameter[5].toLatin1().data());
                        if ( werte_holen(4) == 103 && retour != 0){
                          QMessageBox::about(this, tr("Note","Hinweis"), tr("They have entered a wrong password.\n", "Sie haben ein falsches Passwort eingegeben. \n"));
           		   lineKey->setText ("");
                           return 0;
                        }
			optionkey = meldungen_holen(1);
               		dev_part = meldungen_holen(2);
                }
		//Überprüfen, ob in die Originalpartition zurückgeschrieben werden soll
            part_ = partition_net_.toLatin1().data();
	    strcat (dev_ , part_);  // String zusammenfügen: dev_  = dev_ + part_
            // Hier wird verglichen dev_ = die Partition, die zu sichern ist. dev_part = Originalpartition
            cmp = strcmp ( dev_ , dev_part );
            QString str, str1;
	    str = dev_part;
            str1 = dev_;
            if (cmp != 0 && rdBt_showPartition->isChecked()){
               // char in QString wandeln
               QString str, str1;
	       str = dev_part;
               str1 = dev_;
               cmp = window.questionMessage(tr("Partition to restore the ", "Die wiederherzustellende Partition ") + str1 + 
               tr(" does not coincide with the saved  ", " stimmt nicht mit der gesicherten ") + str + tr("Do you want to continue restore?", " überein. Wollen Sie trotzdem die Wiederherstellung durchführen?"));
               if (cmp == 2)  //nicht wiederherstellen
                  return 0;
            }
       if (rdBt_showPartition->isChecked())
          {         
	       if (rdBt_restoreFsArchiv->isChecked())
            	{
                extern QString folder_file_;
                folder_file_ = file_net;
               // folder_file();	
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
			// auf pbr-Datei prüfen
       			pos = filename.indexOf("txt");
       			filename = filename.left(pos);
       			filename.insert(pos, QString("pbr"));
       			QFile f1(filename);
		       if (f1.exists())
       			{
 			cmp = questionMessage(tr("There is a file with the contents of the partition boot sector present. Do you want to perform the restoration of the PBR?", "Es ist eine Datei mit dem Inhalt des Partition Boot Sektors vorhanden. Wollen Sie die Wiederherstellung des PBR durchführen?"));
    			if (cmp == 1)  // wiederherstellen
        			befehl_pbr_net = "dd if="+ filename + " of=/dev/" + partition_net_ + " bs=512 count=1";
		         }
        }
	if (window.is_mounted(dev_)) 
               {
              UUID_net = window.UUID_auslesen(listwidgetrow);
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
   	 } 
   	 if (rdBt_restoreFsArchiv->isChecked())
              {
            Qt::CheckState state;  	
            state = chk_path->checkState();  	
            if (rdBt_showPartition->isChecked() && partition_typ_net == "btrfs"){
                  cmp = questionMessage(tr("If the existing btrfs partition to be formatted? The new UUID is defined here. If you choose <no>, the UUID remains preserved.", "Soll die vorhandene btrfs-Partition formatiert werden? Die UUID wird dabei neu festgelegt. Wenn Sie  nein wählen, bleibt die UUID erhalten."));
    			   if (cmp == 2) { // nicht formatieren
				// flag setzen in fs_btrfs.c
                                btrfs_flag = 1;
        			// Partition muss gelöscht werden
                                // Partition mounten, damit sie gelöscht werden kann
				QString befehl = "mkdir /tmp/btrfs";
				y =  system (befehl.toLatin1().data());
                                befehl = "mount /dev/" + partition_net_ + " /tmp/btrfs";
                                y =  system (befehl.toLatin1().data());
				//der Inhalt von tmp/btrfs und somit der Inhalt der btrfs Partition wird gelöscht
				befehl = "rm -r -f /tmp/btrfs 2> /dev/null";
 				y =  system (befehl.toLatin1().data());
				befehl = "umount /dev/" + partition_net_;
 				y =  system (befehl.toLatin1().data());
				//tmp/btrfs wird gelöscht
				befehl = "rm -r -f /tmp/btrfs";
 				y =  system (befehl.toLatin1().data());
		       } }   
               QString keyText = lineKey->text();
               state1 = chk_key->checkState();
	       parameter[0] = "fsarchiver";
	            if (rdBt_showPartition->isChecked())
       	         parameter[1] = "restfs"; 
       	      if (rdBt_showDirectories->isChecked())
       	         parameter[1] = "restdir";    
               int kerne = cmb_kerne->currentIndex()+1;
               QString index = QString::number(kerne);
               if (index == "0")
                  index = "1"; 
               parameter[2] = "-j" + index;
               parameter[3] = file_net;
               if (rdBt_showPartition->isChecked())
                  parameter[4] = "id=0,dest=/dev/" + partition_net_;
               if (state == Qt::Checked && rdBt_showDirectories->isChecked()) 
                  parameter[4] = "/";  // Originalort zurückschreiben
               if (state != Qt::Checked && rdBt_showDirectories->isChecked()) 
                  parameter[4] = folder_dir_net;  // an gewählten Ort zurückschreiben   
               thread2.setValues(5,"0");
               if (state1 == Qt::Checked) { // Verzeichnis mit Schlüssel gesichert
                 thread2.setValues(7,"0");
                 parameter[3] = "-c";
                 parameter[4] = keyText;
                 int len = parameter[4].size();
               	    if (len < 6 || len > 64) {
                  	QMessageBox::about(this, tr("Note", "Hinweis"),
         	  	tr("The key length must be between 6 and 64 characters\n", "Die Schlüssellänge muss zwischen 6 und 64 Zeichen sein\n"));
                  	return 0 ; 
               	   }
                 parameter[5] = file_net;
                 parameter[6] = "id=0,dest=/dev/" + partition_net_;
                 }
               werte_reset();
               pushButton_restore->setEnabled(false);
               pushButton_end->setEnabled(false);
               flag_View_net = 2;
  	       timer->singleShot( 1000, this , SLOT(ViewProzent( ))) ;
               stopFlag_ = 0;
               this->setCursor(Qt::WaitCursor);
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
            Qt::CheckState state;
            QStringList filters;
            state= chk_hidden->checkState();
            pushButton_save->setText (tr("Save partition/directorie", "Partition/Verzeichnis sichern"));
            lineEdit_DateiName->setEnabled(true);
            pushButton_restore->setEnabled(false);
            pushButton_save->setEnabled(true);
            label_4->setEnabled(true);
            chk_overwrite->setEnabled(true);
            cmb_zip->setEnabled(true);
            chk_Beschreibung->setEnabled(true);
            AnzahlsaveFile->setEnabled(true);
            AnzahlgesicherteFile->setEnabled(true);
            chk_key->setText (tr("Encrypt\nbackup. key:", "Sicherung\nverschlüsseln. Schlüssel:"));
            chk_split->setEnabled(true);
            chk_pbr->setEnabled(true); 
            restore_file_name_txt ->setEnabled(false); 
            label_folder->setEnabled(false);
            filters << "*.*";
            if (state == Qt::Checked)
               dirModel->setFilter(QDir::AllDirs  | QDir::NoDotAndDotDot | QDir::Hidden);
    	    else 
   	        dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
   	    dirModel->setNameFilters(filters); 
            treeView_dir ->setEnabled(true); 
            }

void DialogNet::rdButton_auslesen()
     {
     Qt::CheckState state;
     state= chk_hidden->checkState();
     Qt::CheckState state1;
     state1= chk_path->checkState();
     QStringList filters; 
     if (rdBt_saveFsArchiv->isChecked())
        {
	starteinstellung();
        }
     if (rdBt_restoreFsArchiv->isChecked())
        {
		pushButton_save->setText (tr("Partition/directorie restore", "Partition/Verzeichnis zurückschreiben"));
                pushButton_restore->setEnabled(true);
                pushButton_save->setEnabled(false);
                lineEdit_DateiName->setEnabled(false);
                label_4->setEnabled(false);
                chk_overwrite->setEnabled(false);
                cmb_zip->setEnabled(false);
                chk_Beschreibung->setEnabled(false);
                AnzahlsaveFile->setEnabled(false);
                AnzahlgesicherteFile->setEnabled(false);
                chk_key->setText (tr("Decrypt\nbackup. key:", "Sicherung\nentschlüsseln. Schlüssel"));
                chk_split->setEnabled(false);
		chk_pbr->setEnabled(false);
                filters << "*.fsa";
                if (state1 == Qt::Checked && rdBt_showDirectories->isChecked())
		   {
                   //treeView_dir ->setHidden(false);
                   treeView_dir ->setEnabled(false);
                   }
                 if (state1 != Qt::Checked && rdBt_showDirectories->isChecked())
		   {
                  // treeView_dir ->setHidden(false);
                   treeView_dir ->setEnabled(true);
                   }
                if (state == Qt::Checked)
               	        dirModel->setFilter(QDir::AllDirs  | QDir::NoDotAndDotDot | QDir::Hidden);
	    	else	
	       		dirModel->setFilter(QDir::AllDirs  | QDir::NoDotAndDotDot);
   		dirModel->setNameFilters(filters);  
         }
     } 

QString DialogNet::Zeit_auslesen(){
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
    _Datum_net = Datum_akt.setNum(Tag) + "-" ; 
    _Datum_net = _Datum_net + Datum_akt.setNum(Monat) + "-" ;
    _Datum_net = _Datum_net + Datum_akt.setNum(Jahr) ;
    return _Datum_net;
}

void DialogNet::listWidget_auslesen() {
MWindow window; 
    //window.show_flag = 1;
    QStringList partition_kurz;
    extern QString add_part[100]; 
    listwidgetrow = listWidget->currentRow();
    partition_net_ = add_part[listwidgetrow];
    int pos = partition_net_.indexOf("btrfs");
    if (pos > 0)
       partition_typ_net = "btrfs";
    partition_kurz = partition_net_.split(" ");
    partition_net_ = partition_kurz[0];
    UUID_net = window.UUID_auslesen(listwidgetrow);
}

int DialogNet::listWidget_folder_free_auslesen() {
	QString homepath = QDir::homePath();
QString befehl;
QString dummy;
int k = 0;
int pos = 0;
int net_art = cmb_Net->currentIndex();
    TreeviewRead treeviewread;
    int row;
    restore_file_name_txt ->setText("");
    row = listWidget_free_folder->currentRow();
    if (row > -1)
    	folder_free = folder_free_[row];
    if (rdBt_restoreFsArchiv->isChecked() && net_art == 0) //Samba
       {
       //Verzeichnis mounten
       this->setCursor(Qt::WaitCursor);
       befehl = "umount " + homepath + "/.qt5-fs-client 2>/dev/null";
       k = system (befehl.toLatin1().data()); 
       befehl = "mount -t cifs -o username=" + user_net + ",password=" + key_net + ",uid=0,gid=0 //" + rechner_IP + "/'" + folder_free + "' " + homepath + "/.qt5-fs-client" ;
       k = system (befehl.toLatin1().data()); 
       this->setCursor(Qt::ArrowCursor); 
       }
    if (rdBt_restoreFsArchiv->isChecked() && net_art == 2) //NFS
       {
       //Verzeichnis mounten
       this->setCursor(Qt::WaitCursor);
       k = system (befehl.toLatin1().data()); 
       befehl = "mount " + rechner_IP + ":" + folder_free + " " + homepath + "/.qt5-fs-client" ;
       k = system (befehl.toLatin1().data());
       this->setCursor(Qt::ArrowCursor); 
       }
    if (k != 0){
   	QMessageBox::about(this, tr("Note", "Hinweis"),
      	tr("The network computer ",  "Der Netzwerkrechner ") + rechner_IP + tr(" could not be integrated. The program is aborted\n", " konnte nicht eingebunden werden. Das Programm wird abgebrochen\n"));
        return 1;
        }
    if (rdBt_restoreFsArchiv->isChecked()){
  	TreeviewRead *dialog1 = new TreeviewRead;
        dialog1->exec();
        file_net = treeviewread.folder_treeview_holen(); 
        dummy = file_net;
        pos = dummy.indexOf("qt5-fs-client");
        dummy = dummy.right(dummy.size() -pos - 14); 
        restore_file_name_txt ->setText(dummy);
        if (file_net != "")
	    pushButton_restore->setEnabled(true);
        }
        return 0;
}
void DialogNet::folder_file() {
   extern QString folder_file_;
   folder_file_ = folder_net + "/" + DateiName_net + "-" + _Datum_net + ".txt";
}

void DialogNet::startThread1() {
   if( thread1.isRunning() ) return;
   connect( &thread1, SIGNAL(finished()),this, SLOT( thread1Ready()));
   thread_run_net = 1;
   thread1.start();
}

void DialogNet::startThread2() {
   if( thread2.isRunning() ) return;
   connect( &thread2, SIGNAL(finished()),this, SLOT( thread2Ready()));
   thread_run_net = 2;
   thread2.start();
}

void DialogNet::closeEvent(QCloseEvent *event) {
   thread1.wait();
   thread2.wait();
   event->accept();
}

void DialogNet::thread1Ready()  {
	QString homepath = QDir::homePath();
   endeThread_net = endeThread_net + 1;
   extern int dialog_auswertung;
   this->setCursor(Qt::ArrowCursor);
   QString befehl;
   if (endeThread_net == 1) {
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
       int cnt_hardlinks = werte_holen(8);
       cnt_hardlinks = cnt_hardlinks + werte_holen(9);
       QString cnt_hardlinks_ = QString::number(cnt_hardlinks);
       int cnt_special = werte_holen(10);
       QString cnt_special_ = QString::number(cnt_special); 
       int err = werte_holen(11) + werte_holen(12) + werte_holen(13) + werte_holen(14) + werte_holen(16);
       if (dialog_auswertung == 0 and err == 0){ 
       // Ausgabe progressBar durch Timer unterbinden
       stopFlag_ = 1; 
       QMessageBox::about(this, tr("Note", "Hinweis"), 
           tr("The partition/directorie has been backed up successfully.\n", "Die Partition/das Verzeichnis wurde erfolgreich gesichert.\n") + cnt_regfile_ + 
        tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr(" links and ", " Links und ") + cnt_special_ + tr(" specials have been backed.", " spezielle Daten wurden gesichert."));  
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
          befehl = "rm "  + filename;
          system (befehl.toLatin1().data());
        if (flag_end_net == 1) {
        QMessageBox::about(this, tr("Note", "Hinweis"),
         tr("The backup of the partition/directorie was aborted by the user!\n", "Die Sicherung der Partition/des Verzeichnisses wurde vom Benutzer abgebrochen!\n") );
         pushButton_save->setEnabled(true);
	}
        // Prüfen ob Partitionsart unterstützt wird      
       int part_testen = werte_holen(3);
       if (part_testen == 110){ 
       QMessageBox::about(this, tr("Note", "Hinweis"),
          tr("The partition type is not supported.\n", "Der Partitionstyp wird nicht unterstützt\n" ));
       flag_end_net = 1;
          } 
       // Anzahl nicht korrekt gesicherte Dateien ausgeben
       part_testen = werte_holen(4);
       if (part_testen == 108 && flag_end_net == 0){
	    QMessageBox::about(this, tr("Note", "Hinweis"),
          tr("The partition type is not supported. Maybe the partition is encrypted?\n", "Der Partitionstyp wird nicht unterstützt. Vielleicht ist die Partition verschlüsselt?\n" ));
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
       if (part_testen != 108 && flag_end_net == 0){
       QMessageBox::about(this, tr("Note", "Hinweis"), 
       	  tr("The backup of the partition/directorie was only partially successful.\n", "Die Sicherung der Partition/des Verzeichnis war nur teilweise erfolgreich\n") + cnt_regfile_ + tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr(" links and ", " Links und ") + cnt_special_ + tr(" specials have been backed\n.", " spezielle Daten wurden gesichert\n.")
         + err_regfile_ + tr(" files, ", " Dateien, ")   + err_dir_ + tr(" directories, ", " Verzeichnisse, ") 
         + err_hardlinks_ + tr(" links and ", " Links und ") + err_special_ + tr(" specials were not properly backed\n."," spezielle Daten wurden nicht korrekt gesichert.\n"));
	  }
        }
       
     }
  int net_art = cmb_Net->currentIndex();   
  if (net_art == 1) //SSH
     //befehl = "/etc/init.d/ssh restart";
     befehl = "fusermount -u " + homepath + "/.qt5-fs-client 2>/dev/null";
  if (net_art == 0) //Samba
       befehl = "umount -f " + homepath + "/.qt5-fs-client 2>/dev/null";
  if (net_art == 2) //NFS
      // befehl = "/etc/init.d/nfs-kernel-server restart";
         befehl = "umount -a -t nfs 2>/dev/null";
  system (befehl.toLatin1().data());
  thread_run_net = 0;
  thread1.exit();
}

void DialogNet::thread2Ready()  {
	QString homepath = QDir::homePath();
   QString befehl;
   endeThread_net = endeThread_net + 1;
   this->setCursor(Qt::ArrowCursor);
   extern int dialog_auswertung;
   int meldung = werte_holen(4);
   int i = 0;
   if (meldung == 105) {
      QMessageBox::about(this, tr("Note", "Hinweis"), tr("cannot restore an archive to a partition which is mounted, unmount it first \n", "Die Partition die wiederhergestellt werden soll, ist eingehängt. Sie muss zunächst ausgehängt werden!\n"));
      endeThread_net == 0;
       }
   if (endeThread_net == 1) { 
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
       //if (dialog_auswertung ==0){
       //Rückmeldung von fsarchiver: Zurückschreiben erfolgreich
     if (err ==0){
       // Ausgabe progressBar durch Timer unterbinden
       stopFlag_ = 1; 
       pushButton_restore->setEnabled(true);
       progressBar->setValue(100);
       SekundeRemaining ->setText("0");
       //PBR herstellen
       i = 2;
	if (befehl_pbr_net != "") 
    	i = system (befehl_pbr_net.toLatin1().data());
	if (i!=0 ) { 
       		QMessageBox::about(this, tr("Note", "Hinweis"), tr("The partition/directorie is successful back.\n", "Die Partition/das Verzeichnis wurde erfolgreich wieder hergestellt.\n") + 		cnt_regfile_ + tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr(" links and ", " Links und ") + cnt_special_ + tr(" specials have been restored.", " spezielle Daten wurden wieder hergestellt."));
        }
	if (i==0) { 
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("The partition is successful back.\n", "Die Partition wurde erfolgreich wieder hergestellt.\n") + cnt_regfile_ + 
        tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr(" links ", " Links ") + cnt_special_ + tr(" specials and the Partition Boot Record have been restored.", " spezielle Daten und der Partition Boot Sektor wurden wieder hergestellt."));
        } 
        }
       if (flag_end_net == 1) {
        QMessageBox::about(this, tr("Note", "Hinweis"),
         tr("The restore of the partition/directorie was break by user!\n", "Die Wiederherstellung der Partition/des Verzeichnisses wurde vom Benutzer abgebrochen!\n") );
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
       	  tr("The restore of the partition/directorie was only partially successful.\n", "Die Wiederherstellung der Partition/des Verzeichnisses war nur teilweise erfolgreich\n") + cnt_regfile_ + tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr(" links and ", " Links und ") + cnt_special_ + tr(" specials have been restored\n.", " spezielle Daten wurden wiederhergestellt\n.")
         + err_regfile_ + tr(" files, ", " Dateien, ")   + err_dir_ + tr(" directories and ", " Verzeichnisse und ") 
         + err_hardlinks_ + tr(" links and ", " Links und ") + err_special_ 
         + tr(" specials were not properly restored\n."," spezielle Daten wurden nicht korrekt wiederhergestellt.\n"));
               }
      if (i==0) { 
        QMessageBox::about(this, tr("Note", "Hinweis"), 
       	  tr("The restore of the partition/directorie was only partially successful.\n", "Die Wiederherstellung der Partition/des Verzeicnisses war nur teilweise erfolgreich\n") + cnt_regfile_ + tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") 
         + cnt_hardlinks_ + tr(" links and ", " Links und ") + cnt_special_ + tr(" specials and the Partition Boot Record have been restored\n.", " spezielle Daten und der Partition Boot Sektor wurden wieder hergestellt\n.") + err_regfile_ + tr(" files, ", " Dateien, ")   + err_dir_ + tr(" directories and ", " Verzeichnisse und ") + err_hardlinks_ + tr(" links and ", " Links und ") + err_special_ + tr(" specials were not properly restored\n."," spezielle Daten wurden nicht korrekt wiederhergestellt.\n"));
                }
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
    int net_art = cmb_Net->currentIndex();
    if (net_art == 1) //SSH
       //befehl = "/etc/init.d/ssh restart";
       befehl = "fusermount -u " + homepath + "/.qt5-fs-client 2>/dev/null";
    if (net_art == 0) //Samba
       befehl = "umount -f " + homepath + "/.qt5-fs-client 2>/dev/null";
    if (net_art == 2) //NFS
      //befehl = "/etc/init.d/nfs-kernel-server restart";
       befehl = "umount -a -t nfs 2>/dev/null";
    system (befehl.toLatin1().data());
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
  string str (file_net.toLatin1().data());
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
int prozent = 0;
QString sekunde;
int sekunde_ = 0;
QString minute;
int minute_ = 0;
int meldung = 0;
int anzahl = 0;
int anzahl1 = 0;
QString text_integer;

if (endeThread_net !=1)
{
 timer->singleShot( 1000, this , SLOT(ViewProzent( )) ) ;
  elapsedTime();
  this->repaint();
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
 if (prozent != 100 && stopFlag_ == 0)  
  	progressBar->setValue(prozent);
 return;
} 


void DialogNet::keyPressEvent(QKeyEvent *event) {
MWindow window;   
     QWidget::keyPressEvent(event);
     switch( event->key() ) {
         case Qt::Key_Escape:
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
}

void DialogNet::esc_end()
{
MWindow window;
QString befehl;
   if (thread_run_net > 0) {
    int ret = questionMessage(tr("Do you want really break the save or restore from the partition?", "Wollen Sie wirklich die Sicherung oder Wiederherstellung der Partition beenden?"));
      if (thread_run_net == 1 && ret == 1)
        {
	flag_end_net= 1;     	
	befehl = "rm "  + SicherungsFolderFileName_net;
        system (befehl.toLatin1().data()); 
        int pos = SicherungsFolderFileName_net.indexOf("fsa");
       	SicherungsFolderFileName_net = SicherungsFolderFileName_net.left(pos);
       	SicherungsFolderFileName_net.insert(pos, QString("txt"));
        befehl = "rm "  + SicherungsFolderFileName_net;
        system (befehl.toLatin1().data());
        if (window.bit_version() == "64")
        	{
        	thread1.terminate();
        	thread1.wait(); 
        	close();
        	}
	}
        if (window.bit_version() == "32")
        	{
		befehl = "kill -15 " + pid1_net;  //fsarchiver abbrechen
     		system (befehl.toLatin1().data());
    		befehl = "kill -15 " + pid_net;  //fsarchiver abbrechen
    		system (befehl.toLatin1().data());
		close();
                }
     if (thread_run_net == 2 && ret == 1)
        {
        flag_end_net= 1; 
        if (window.bit_version() == "64")
        	{
        	thread2.terminate();
        	thread2.wait(); 
        	close();
        	}
        if (window.bit_version() == "32")
        	{
		befehl = "kill -15 " + pid1_net;  //fsarchiver abbrechen
     		system (befehl.toLatin1().data());
    		befehl = "kill -15 " + pid_net;  //fsarchiver abbrechen
    		system (befehl.toLatin1().data());
                close();
                }
	}
    }
}

void DialogNet::pid_ermitteln()
{
int pida=getpid();
pid_net = QString::number(pida);
int pida1=getppid();
pid1_net = QString::number(pida1);
}

void DialogNet::addItems(const QString &name, const QStringList& iconMap) {
   QTreeWidgetItem *root1 = new QTreeWidgetItem(treeWidget);
   root1->setText(0, name);  //zuständig für die Anzeige des Foldernamens
   if (name.right(4)== ".fsa")
      root1->setIcon(0, fileIcon );// zuständig für das Filesymbol
   else
      root1->setIcon(0, folderIcon );// zuständig für das Foldersymbol
   QList<QTreeWidgetItem *> items;
   QTreeWidgetItem *tItem=new QTreeWidgetItem(root1); // zuständig für den Pfeil vor dem Namen
   treeWidget->addTopLevelItems( items );
}


void DialogNet::listWidget_tree_auslesen(QTreeWidgetItem *item, int)
{
NetEin netein;	
QString rechner;
	QString name = item->text(0);
	rechner = netein.Namen_holen();
   	user_net = netein.user_holen(); 
   	key_net = netein.key_holen();
        listWidget_tree_eintragen(rechner_IP, key_net, user_net, "/" + name, 0);
}

void DialogNet::treeWidget_auslesen()
{
int pos = 0;
	QString homepath = QDir::homePath();
	QTreeWidgetItem *current = treeWidget->currentItem();
	QString currentFile = current->text(0);
        restore_file_name_txt ->setText("");
	//Verzeichnis, in dem gesichert oder zurückgeschrieben wird
        //Name folder_free wurde aus dem Code Samba-Sicherung übernommen
        folder_free = pfad_forward + "/" + currentFile;
        // Prüfen ob 2 Flash vorhanden sind
        if (folder_free.indexOf ("/", 1) == 1)
		folder_free.replace(1,1,"");
        if (rdBt_restoreFsArchiv->isChecked()){
            file_net = homepath + "/.qt5-fs-client/" + currentFile;
            int pos = file_net.indexOf(".fsa");
            if (pos > 1)
            	restore_file_name_txt ->setText(currentFile);}
}

int DialogNet::folder_free_mounten(){  //ssh mounten
QString homepath = QDir::homePath();
QString befehl;
int i = 0;
        QSettings setting("qt5-fsarchiver", "qt5-fsarchiver");
        setting.beginGroup("Basiseinstellungen");
        int auswertung = setting.value("sshfs").toInt();
        setting.endGroup();
        if (auswertung ==1){
             int ret = questionMessage(tr("If you have not set up SSH authentication, you must now enter the password in the terminal. Should this continue to be displayed? You can change this in the basic settings.", "Wenn Sie keine SSH-Authentifizierung eingerichtet haben, müssen Sie nun das Passwort in dem Terminal eingeben. Wollen Sie diesen Hinweis weiterhin sehen? Sie können dies in den Basiseinstellungen ändern"));
             if (ret == 2){
		//Basiseinstellungen ändern
        	QSettings setting("qt5-fsarchiver", "qt5-fsarchiver");
        	setting.beginGroup("Basiseinstellungen");
        	setting.setValue("sshfs",0);
        	setting.endGroup();
                }
        }
        if (folder_free != "" && rdBt_saveFsArchiv->isChecked() )
		befehl = "sshfs " + user_net+ "@" + rechner_IP + ":" + folder_free + " " +  homepath + "/.qt5-fs-client";
        if (folder_free != "" && rdBt_restoreFsArchiv->isChecked() )
		befehl = "sshfs " + user_net+ "@" + rechner_IP + ":" + pfad_forward + " " + homepath + "/.qt5-fs-client";
        i =system (befehl.toLatin1().data()); 
        if ( i==1){
            QMessageBox::about(this, tr("Note", "Hinweis"), tr("The SSH server is not reachable. Try again or with another network protocol.\n", "Der SSH-Server ist nicht erreichbar. Versuchen Sie es nochmals oder mit einem anderen Netzwerkprotokoll.\n"));
        return 1;
        }
        if ( i==256){
		QString str= QString::number(i);
		QMessageBox::about(this, tr("Note", "Hinweis"),
         			tr("The backup or restore with ssh is not possible. Exit the program and restart it again in the terminal with root right.\n", " Die Sicherung oder Wiederherstellung mit ssh ist nicht möglich. Beenden Sie  das Programm und starten es erneut im Terminal mit Root-Rechten\n"));
         return 256;
         }
}


void DialogNet::listWidget_tree_eintragen(QString rechner, QString pass, QString user, QString pfad, int flag)
{ 
QString homepath = QDir::homePath();
QStringList filters;
QString befehl;
//qDebug() << "homepath" << homepath;
int ret_;
        QSettings setting("qt5-fsarchiver", "qt5-fsarchiver");
        setting.beginGroup("Basiseinstellungen");
        int auswertung = setting.value("ssh").toInt();
        setting.endGroup();
        if (auswertung ==1){
/* QMessageBox::about(this, tr("Note", "Hinweis"), 
        tr("When you first contact the computer with ssh, you must do the following:  In a terminal enter these commands: 1.In the open terminal you must confirm the RSA key fingerprint with yes. 2.Enter the password for accessing the server and 3.leave the server with the command exit.\n", "Wenn Sie noch nicht erfolgreich per ssh auf den Rechner(Server) zugegriffen haben, müssen Sie nun folgendes tun: 1.In dem geöffneten Terminal müssen Sie den RSA key fingerprint mit yes bestätigen. 2.Geben Sie das Passwort für den Zugriff auf den Server ein  und 3.verlassen Sie den Server mit dem Befehl exit.\n")); */
        befehl = "ssh " + user_net + "@" +  rechner_IP;
	ret_ = system (befehl.toLatin1().data());
        //Basiseinstellungen ändern
        //QSettings setting("qt5-fsarchiver", "qt5-fsarchiver");
        setting.beginGroup("Basiseinstellungen");
        setting.setValue("dummy",0);
        auswertung = setting.value("ssh").toInt();
        if (ret_ == 0)
        	setting.setValue("ssh",0);
        else 
		setting.setValue("ssh",1);
        setting.endGroup();
        }
        if (pfad != "/")
        	bt_toParent->setEnabled(true); 
        if (pfad == "/")
                bt_toParent->setEnabled(false); 
        if (flag==0){
               // pfad_back = pfad_forward;
                backFlag = 1;
		pfad_forward = pfad_forward + pfad;
                pfad_back = pfad_forward;
		if (pfad_forward.indexOf ("/", 1) == 1)
			pfad_forward.replace(1,1,"");
	}
	QString filename = homepath + "/.config/qt5-fsarchiver/folder.txt";
	listWidget_base(); 
        ret_ = 0;
//nur Verzeichnisse anzeigen
	if (flag==0 && rdBt_saveFsArchiv->isChecked())
		    befehl = "sshpass -p " + pass + " ssh " + user+ "@" + rechner + " ls -l " + pfad_forward + " | grep '^d'  1>" + filename;
	if (flag==1 && rdBt_saveFsArchiv->isChecked())
		    befehl = "sshpass -p " + pass + " ssh " + user+ "@" + rechner + " ls -l " + pfad_back + " | grep '^d'  1>" + filename;
        if (flag==0 && rdBt_restoreFsArchiv->isChecked())
		    befehl = "sshpass -p " + pass + " ssh " + user+ "@" + rechner + " ls -l " + pfad_forward + " 1>" + filename;
	if (flag==1 && rdBt_restoreFsArchiv->isChecked())
		     befehl = "sshpass -p " + pass + " ssh " + user+ "@" + rechner + " ls -l " + pfad_back + " 1>" + filename;
	ret_ = system (befehl.toLatin1().data());
        if ( ret_ != 0) {
	QMessageBox::about(this, tr("Note", "Hinweis"), tr("The SSH server is not reachable. Try again or with another network protocol.\n", "Der SSH-Server ist nicht erreichbar. Versuchen Sie es nochmals oder mit einem anderen Netzwerkprotokoll.\n"));
        return ;
	}
	QStringList folder;
	QString folder_[500];
	int found = 0;
	int i = 0;

QFile file1(filename);
QTextStream ds(&file1);
QString folder_teilen;
int jj= 0;
        if (rdBt_saveFsArchiv->isChecked()){ //Verzeichnisse auswerten
      	    if( file1.open(QIODevice::ReadOnly|QIODevice::Text)) {
        	while (!ds.atEnd())
        	{
        	folder_teilen= ds.readLine();
			do{
    			found=folder_teilen.indexOf("  ");
				if (found > 0){
             	    			folder_teilen.replace("  ", " ");
			}
		}
			while  (found >= 0);
        folder = folder_teilen.split(" ") ;
        folder_[jj] = folder[folder.size()-1];
        jj = jj + 1;
 	} 
 	}      
 	file1.close();
        }

        if (rdBt_restoreFsArchiv->isChecked()){ //Verzeichnisse und Dateien auswerten
      	    if( file1.open(QIODevice::ReadOnly|QIODevice::Text)) {
                folder_teilen= ds.readLine();
        	while (!ds.atEnd())
        	{
        	folder_teilen= ds.readLine();
                   if (folder_teilen.left(1) == "d" || folder_teilen.right(4) == ".fsa")
                   {
			do{
    			found=folder_teilen.indexOf("  ");
				if (found > 0){
             	    			folder_teilen.replace("  ", " ");
			                      }
		          }
			while  (found >= 0);
        folder = folder_teilen.split(" ") ;
        folder_[jj] = folder[folder.size()-1];
        jj = jj + 1;
                  }
 	} 
 	}      
 	file1.close();
        }
  
 	befehl = "rm " + homepath + "/.config/qt5-fsarchiver/folder.txt";
 	//system (befehl.toLatin1().data()); 
 	treeWidget->clear();
        QDir dir2(QString("%1").arg(""));
   	dir2.setNameFilters(filters);
   	QStringList dirList2 = dir2.entryList(QDir::Files);
 	for ( i = 0; i < jj; ++i){
    		dir2.setNameFilters(filters);
    		dirList2 = dir2.entryList(QDir::Files);
    		addItems(folder_[i], dirList2);
   	}
}
void DialogNet::button_toParent()
{
int found = 1;
int found1;
        //Rücksprungpfad ermitteln
        // vorhandenen / am Ende entfernen
        pfad_back = pfad_back.left(pfad_back.lastIndexOf('/'));
        if (backFlag == 0){
	     do{
		found1 = found;	
		found = pfad_back.indexOf ("/", found);
        	found = found +1;
   	        }
	     while  (found != 0);
             pfad_back = pfad_back.left(found1);
	}
        if (pfad_back.isEmpty()) 
                pfad_back ="/";
        if (pfad_back != "/")
        	bt_toParent->setEnabled(true); 
        if (pfad_back == "/")
                bt_toParent->setEnabled(false); 
        pfad_forward = pfad_back;
        backFlag == 0;
        listWidget_tree_eintragen(rechner_IP, key_net, user_net, pfad_back, 1);
}

void DialogNet::listWidget_base()
{
   	treeWidget->setColumnCount(1);
 	QStringList sList;
 	sList << tr("Name");
 	treeWidget->setHeaderLabels( sList );
	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),QIcon::Normal, QIcon::Off);
        fileIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon),QIcon::Normal, QIcon::On);
	QList<QTreeWidgetItem *> items1;
	treeWidget->addTopLevelItems( items1 );
        treeWidget->setRootIsDecorated(false);  // Pfeil wird nicht angezeigt
}

void DialogNet::save_partitions(){
Qt::CheckState state;
state = chk_path->checkState();
       listWidget ->setHidden(false);
       treeView_dir ->setHidden(true);
       treeView_dir ->setEnabled(true);
}

void DialogNet::save_directories(){
Qt::CheckState state;
state = chk_path->checkState();
       listWidget ->setHidden(true);
       treeView_dir ->setHidden(false);
       if (state == Qt::Checked && rdBt_restoreFsArchiv->isChecked())
          treeView_dir ->setEnabled(false);  
       if (state == Qt::Checked && rdBt_saveFsArchiv->isChecked())
          treeView_dir ->setEnabled(true);
       if (state != Qt::Checked) 
          treeView_dir ->setEnabled(true);
}

void DialogNet::chkhidden(){
     Qt::CheckState state;
     state = chk_hidden->checkState();
     if (rdBt_saveFsArchiv->isChecked())
        starteinstellung();   
     else
        rdButton_auslesen();
}

int DialogNet::archinfo(QString archname){
QString homepath = QDir::homePath();
      QString archiv;
      parameter[0] = "fsarchiver";
      parameter[1] = "archinfo";
      parameter[2] = archname;
      fsarchiver_aufruf(3,parameter[0].toLatin1().data(),parameter[1].toLatin1().data(),parameter[2].toLatin1().data(),parameter[3].toLatin1().data());
      int archart = werte_holen(15);
      if (archart == 200)
	 return 2;
      if (archart == 210)
	 return 1;
      return 0;
 }



