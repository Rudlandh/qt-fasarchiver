/*
 * qt4-fsarchiver: Filesystem Archiver
 * 
* Copyright (C) 2008-2015 Dieter Baum.  All rights reserved.
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
#include "clone_net.h"
#include <string.h>
#include <unistd.h>
#include "mainWindow.h"
#include "net_ein.h"
#include "treeview.h"

extern int dialog_auswertung;
extern int anzahl_disk;
QStringList filters_clone_net;
QString part_clone_net[20][4];
int endeThread_clone_net;
int flag_clone_net;
int thread_run_clone_net;
int sekunde_elapsed_clone_net;
int minute_elapsed_clone_net;
int hour_elapsed_clone_net;
double sekunde_summe_clone_net = 0;
QString pid_clone_net, pid1_clone_net;
QString partition_name_net;
int read_write_zaehler_net = 0;
int read_write_zaehler_1_net = 0;
int rrw_net[10];
int rrw_sum_net[11];
double read_write_space_sum_net = 0;
QString homepath_net = QDir::homePath();
double block_size_net;
double partition_exist_size_int_net;
int dummy_prozent_clone_net;
QString folder_free_clone_net;
QString folder_free_clone_net_[100];
QString user_net_clone;
QString key_net_clone;
QString rechner_IP_clone;
QString comNet_clone;
QString comNet_name_clone;
QString file_net_clone;
QString DateiName_clone("") ;



DialogClone_net::DialogClone_net(QWidget *parent)
{
	setupUi(this); // this sets up GUI
        connect( button_save, SIGNAL( clicked() ), this, SLOT( todo() ) );
 	connect( rdbt_image_save, SIGNAL( clicked() ), this, SLOT(rdbutton_image_save() ) ); 
        connect( rdbt_image_restore, SIGNAL( clicked() ), this, SLOT(rdbutton_image_restore() ) ); 
        connect( pushButton_break, SIGNAL( clicked() ), this, SLOT(esc_end()));
        connect( bt_end, SIGNAL( clicked() ), this, SLOT(close()));
        connect( pushButton_folder_free, SIGNAL( clicked() ), this, SLOT(listWidget_folder_free_auslesen()));
        dirModel = new QDirModel;
  	selModel = new QItemSelectionModel(dirModel);
  	QModelIndex cwdIndex = dirModel->index(QDir::rootPath());
        button_save->setText (tr("Save Harddrive Image", "Festplatten Abbild erstellen"));
        timer_clone_net = new QTimer(this);
        timer_read_write_net = new QTimer(this);
        //Erforderlich um Textdatei vor dem ersten Auslesen mit 3 Zeilen zu füllen
        QString befehl = "vmstat 1 2 1> " +  homepath_net + "/.config/qt4-fsarchiver/disk.txt";
        system (befehl.toAscii().data());
        Daten_eintragen();
        format_Disk();
  }

void DialogClone_net::format_Disk() {
QString disk_clone[50];
QString teilstring, space1;
QString befehl;
QStringList disk_kurz;
int  i = 0, j=0 ,k;
int pos;
        befehl = "fdisk -l | grep Disk 1> " +  homepath_net + "/.config/qt4-fsarchiver/disk.txt";
        system (befehl.toAscii().data());
	QString filename = homepath_net + "/.config/qt4-fsarchiver/disk.txt";
	QFile file(filename);
        if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
           QTextStream ds(&file);
          while (!ds.atEnd()){
           	disk_clone[i] = ds.readLine();
                disk_kurz = disk_clone[i].split(" ") ;
                part_clone_net[i][0] = disk_kurz[1];
                part_clone_net[i][1] = disk_kurz[2];
                i = i + 1;
                }
           j = i ;
           }
        file.close();
        befehl = "rm " + filename;
        system (befehl.toAscii().data());
        for (i=0; i< j; i++)
       {
           teilstring = part_clone_net[i][1];
           k = teilstring.size();
           teilstring = part_clone_net[i][0];
           pos = teilstring.indexOf("/sd");
           space1.fill (' ',12 - k);
	   //space1.fill (' ',20 );
	   disk_clone[i] = teilstring.right(pos) + space1 + part_clone_net[i][1] + " GB";
	   qDebug() << "teilstring " << teilstring;
           listWidget_exist->addItem (disk_clone[i]);
       }
}

void DialogClone_net::todo(){
		   
	if (rdbt_image_save->isChecked()) 
            do_image(); 
          
 	if (rdbt_image_restore->isChecked())
           restore_image();
}


int DialogClone_net::do_image()
{
QString befehl;
int row;
int pos;
QString partition_exist;
QString partition_exist_size;
      flag_clone_net =2;
      row = listWidget_exist->currentRow();
      // Blockgröße Festplatte ermitteln
      //block_size_net = block_size(row); 
      if (row > -1){
          partition_exist = part_clone_net[row][0] ;
          partition_exist_size =  part_clone_net[row][1];
          pos = partition_exist_size.indexOf(".");
          if (pos > 0)
             partition_exist_size = partition_exist_size.left(partition_exist_size.size() -2);
             partition_exist_size_int_net = partition_exist_size.toInt();
      }
    //Partitionsgröße in Datei schreiben
    partition_exist = partition_exist.left(partition_exist.size() -1);
    if (partition_exist == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a hard drive.", "Sie müssen eine Festplatte auswählen.\n"));
	return 0;        
	}
    if (folder_free_clone_net == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a folder.", "Sie müssen ein Verzeichnis auswählen.\n"));
	return 0;        
	}
    DateiName_clone = lineEdit_Dateiname->text();
        if (DateiName_clone == "")
           {
          	QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("Please, select the filename of the backup.\n", "Bitte wählen Sie den Dateinamen der Sicherung aus.\n"));
		return 0 ;
           }
    // Überprüfen, ob System oder Home-Partition auf der Festplatte vorhanden ist
    int part_art_clone = mountpoint(partition_exist);
    if (part_art_clone == 1){
       qDebug() << "home?" << part_art_clone;
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("On the hard drive exist a root or a home partition. You must need a live-CD.", "Auf der Festplatte ist eine root- oder home Partition. Sie müssen eine Live-CD benutzen.\n"));
	return 0;
}
	partition_name_net  = partition_exist.right(partition_exist.size() -4);
	befehl = "dd if=" + partition_exist + " | gzip --best > " + folder_free_clone_net +  partition_name_net + ".fsa.gz";
// zum testen nur partition image bilden
        //befehl = "dd if=/dev/sdb20 | gzip > " + folder_free_clone_net +  partition_name_net + ".fsa.gz";
        // befehl= "dd if=/dev/sdb20 | pv --size 10000M | gzip > /sonstiges/sdb.fsa.gz";
qDebug() << "befehl " << befehl;
        // HD-Größe in Datei schreiben
        save_txt(partition_exist_size);
	thread1.setValues( 0,befehl);
	int ret = questionMessage(tr(" Do you want really built a image from the hard drive? ", " Wollen Sie wirklich ein Abbild der Festplatte erstellen? ") );  
              if (ret == 2)
                 return 0;
              if (ret == 1){
     	      	qDebug() << "Das Image wird geschrieben";
              	bt_end->setEnabled(false);
            	button_save->setEnabled(false);
              	QTimer::singleShot(200, this, SLOT(ViewProzent()));
                read_write_hd();
		startThread1();
    		
              }
    return 0;
}


void DialogClone_net::save_txt(QString txt)
{
QString fileName = folder_free_clone_net +  partition_name_net + ".txt";
QFile file(fileName);
if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
  QTextStream out(&file);
  out << txt; }
}

QString DialogClone_net::read_txt()
{
QString text;
QString fileName = folder_free_clone_net + ".txt";
	int pos = fileName.indexOf("fsa");
 	fileName = fileName.left(pos);
       	fileName.insert(pos, QString("txt"));
	QFile file(fileName);
	QTextStream ds(&file);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) 
        	text = ds.readLine();
	return text; 
}

int DialogClone_net::restore_image()
{
QString befehl;
int row;
int pos;
QString partition_exist;
QString partition_exist_size;
      row = listWidget_exist->currentRow();
      // Blockgröße Festplatte ermitteln
     // block_size_net = block_size(row); 
      if (row > -1)
          partition_exist = part_clone_net[row][0] ;
     partition_exist = partition_exist.left(partition_exist.size() -1);
     // Festplattengröße aus gzip Textdatei auslesen
     partition_exist_size = read_txt(); 
     if (partition_exist == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a hard drive.", "Sie müssen eine Festplatte auswählen.\n"));
	return 0;        
	}
     if (folder_free_clone_net == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a fsa.gz file.", "Sie müssen eine fsa.gz Datei auswählen.\n"));
	return 0;        
	}
     if (file_check() == 1)
        return 0;
    // Überprüfen, ob System oder Home-Partition auf der Festplatte vorhanden ist
    int part_art_clone = mountpoint(partition_exist);
    if (part_art_clone == 1){
       qDebug() << "home?" << part_art_clone;
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("On the hard drive exist a root or a home partition. You must need a live-CD.", "Auf der Festplatte ist eine root- oder home Partition. Sie müssen eine Live-CD benutzen.\n"));
	return 0;
	}
	partition_name_net  = partition_exist.right(partition_exist.size() -4);
        befehl = "gunzip -c " + folder_free_clone_net + " | sudo dd of=" + partition_exist; 
        //befehl = "gunzip -c " + folder_free_clone_net + " | sudo dd of=/dev/sdb20";
 qDebug() << "befehl" << befehl;

	int ret = questionMessage(tr(" Do you want really restore the image of the hard drive? ", " Wollen Sie wirklich ein Abbild der Festplatte zurückschreiben? ") );  
              if (ret == 2)
                 return 0;
              if (ret == 1){
     	      	qDebug() << "Das Image wird zurückgespielt";
              	bt_end->setEnabled(false);
            	button_save->setEnabled(false);
                thread2.setValues( 0,befehl);
                read_write_hd();
              	QTimer::singleShot(200, this, SLOT(ViewProzent()));
		startThread2();
    	}
                
    return 0;
}

void DialogClone_net::Daten_eintragen()
{
NetEin netein;	
QString rechner;
QStringList items;
int i = 0;
   rechner = netein.Namen_holen();
   txt_rechner_net ->setText(rechner);
   //IP-Adresse Netzrechner
   items = rechner.split(" ");  
   rechner_IP_clone = items[0];
   user_net_clone = netein.user_holen(); 
   key_net_clone = netein.key_holen();
   //freigegebene Ordner ermitteln
   i = search_folder_free(rechner_IP_clone); 
   if ( i==1){
        QMessageBox::about(this,tr("Note", "Hinweis"),
        tr("Can not find a shared directory. The program is terminated.\n", "Es wurde kein freigegebenes Verzeichnis gefunden. Das Programm kann nicht fortgesetzt werden.\n"));
	}
}

int DialogClone_net::search_folder_free(QString rechner_IP_clone){

QString befehl;
int pos;
int line;
int i = 0;
QString text;
QStringList items;
QString homepath = QDir::homePath(); 
	// freigegebene Verzeichnisse ermitteln
    	befehl = "smbclient -N -L" + rechner_IP_clone + " 1> " +  homepath + "/.config/qt4-fsarchiver/smbclient.txt";
	system (befehl.toAscii().data()); 
        QFile file(homepath + "/.config/qt4-fsarchiver/smbclient.txt");
	QTextStream ds(&file);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
           text = ds.readLine();
   	  while (!ds.atEnd())
      	{
 	       if (text.indexOf("Disk") > -1)  {
                   if (text.indexOf("print$") == -1){
                     items = text.split(" ");
                     folder_free_clone_net = items[0];
                     folder_free_clone_net = folder_free_clone_net.trimmed();
                     listWidget_free_folder ->addItem (folder_free_clone_net);
                     folder_free_clone_net_[i] = folder_free_clone_net;
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
    folder_free_clone_net = "";
    return 0;	
}


int DialogClone_net::questionMessage(QString frage)
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

void DialogClone_net::rdbutton_image_save(){
	lbl_hd_size ->setText("Exist harddrive");
	button_save->setText (tr("Save Harddrive Image", "Festplatten Abbild erstellen"));
        dirModel->setFilter(QDir::AllDirs  | QDir::NoDotAndDotDot);
   	dirModel->setNameFilters(filters_clone_net); 
        block_size_net = 880;
  
}

void DialogClone_net::rdbutton_image_restore(){
	lbl_hd_size ->setText("Restore harddrive");
	button_save->setText (tr("Restore Harddrive Image", "Festplatten Abbild zurückschreiben"));
        filters_clone_net << "*fsa.gz" ;
   	dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
   	dirModel->setNameFilters(filters_clone_net); 
       block_size_net = 880;  
}

//Mountpoint ermitteln
int DialogClone_net::mountpoint(QString partition)
{
   QString text;
   QFile file("/etc/mtab");
   QTextStream ds(&file);
   int pos, pos1;
   int line = 0;
   if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        text = ds.readLine();
        // hier wird die ersten Zeile der mtab überprüft
	pos = text.indexOf(partition);
        if (pos > -1)  // root-Partition gefunden gefunden
      	     { 
                file.close();
		return 1;												
       	     }
	while (!ds.atEnd())
      	{
	     line++;
             text = ds.readLine();
             pos = text.indexOf("/home"); 
             if (pos > -1) { // home Partition gefunden
                 pos1 = text.indexOf(partition); 
                 if (pos1 > -1)
      	     		{ 
               		file.close();
               		return 1;
       	    		}
      		} 
	     } }
	     file.close();
   	    return 0; 
}

int DialogClone_net::file_check() {
   QFileInfo info(folder_free_clone_net); 
   if (!info.isFile() )
      {
      QMessageBox::about(this, tr("Note", "Hinweis"),
      tr("You have selected a folder. You must select a fsa.gz file.\n", "Sie haben ein Verzeichnis ausgewählt. Sie müssen eine fsa.gz Datei auswählen\n"));
      return 1 ;
      }
   return 0;
}


void DialogClone_net::ViewProzent()
{
int prozent ;
QString sekunde;
int sekunde_;
QString minute;
int minute_;
QString hour;
int hour_;
QString text_integer;
QString size;
int size_1;
double size_clone;
        if (endeThread_clone_net ==0)
	{
       	QTimer::singleShot(1000, this, SLOT(ViewProzent()));
	size_clone = read_write_space_sum_net * sekunde_summe_clone_net * block_size_net / 1000000.0;
        lbl_hd_size ->setText("MB");
	prozent = size_clone/(partition_exist_size_int_net * 10);
       // prozent = 100000.0 * size_clone/ 10467971 ;  // 10 GB;
         qDebug() << "MB size_clone read_write_space_sum_net sekunde_summe_clone_net partition_exist_size_int_net" << size_clone << read_write_space_sum_net  << sekunde_summe_clone_net << partition_exist_size_int_net;
        savedBytes->setText("");
        size_1 = (int) size_clone;
        
        if (size_clone > 10000) { 
            size_1 = size_1 / 1000;
            lbl_hd_size ->setText("GB");}
        size = QString::number(size_1);
        if (size_clone > 0) 
        	savedBytes->setText(size);
        this->repaint();
        elapsedTime();
       	progressBar->setValue(prozent);
	      if (dummy_prozent_clone_net != prozent)
     				remainingTime(prozent);
 						else {
        	if (prozent >= 1)
       		{
        	// Sekunden nach unten zählen
        	sekunde = SekundeRemaining->text();
        	sekunde_ = sekunde.toInt();
        	minute = MinuteRemaining->text();
        	minute_ = minute.toInt();
                hour = HourRemaining->text();
        	hour_ = hour.toInt();

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
					if (minute_ == 0 && sekunde == 0)
                			{		
					hour_ = hour_ - 1;
                			hour = QString::number(hour_); 
        				HourRemaining ->setText(hour);
                       			minute_ = 59;
                			}
	    			}
        }		
        sekunde = QString::number(sekunde_); 
        SekundeRemaining ->setText(sekunde);
	    } 
    }   
 dummy_prozent_clone_net = prozent;  
        }
 } 


void DialogClone_net::startThread1() {
   if( thread1.isRunning() ) return;
   connect( &thread1, SIGNAL(finished()),
            this, SLOT( thread1Ready()));
   thread_run_clone_net = 1;
   thread1.start();
}

void DialogClone_net::startThread2() {
   if( thread2.isRunning() ) return;
   connect( &thread2, SIGNAL(finished()),
            this, SLOT( thread2Ready()));
   thread_run_clone_net = 2;
   thread2.start();
}

void DialogClone_net::closeEvent(QCloseEvent *event) {
   thread1.wait();
   thread2.wait();
   event->accept();
}

void DialogClone_net::thread1Ready()  {
QString befehl;
	endeThread_clone_net = endeThread_clone_net + 1;
        if (endeThread_clone_net == 1) {
	    bt_end->setEnabled(true);
            button_save->setEnabled(true);
            progressBar->setValue(100);
       	    SekundeRemaining ->setText("0");
            if (dialog_auswertung == 0)
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("The backup of the image was successfully.", "Die Sichern des Abbildes war erfolgreich.\n"));
	    if (dialog_auswertung != 0)
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("The backup of the image was not successfully.", "Die Sichern des Abbildes war nicht erfolgreich.\n"));
}

	thread_run_clone_net = 0;
	thread1.exit();
        befehl = "rm " +  homepath_net + "/.config/qt4-fsarchiver/disk.txt";
        system (befehl.toAscii().data());
}

void DialogClone_net::thread2Ready()  {
	endeThread_clone_net = endeThread_clone_net + 1;
        if (endeThread_clone_net == 1) {
	    bt_end->setEnabled(true);
            button_save->setEnabled(true);
      	    progressBar->setValue(100);
       	    SekundeRemaining ->setText("0");
            if (dialog_auswertung == 0)
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("The restore from the image was successfully.", "Die Wiederherstellung des Abbildes war erfolgreich.\n"));
	    if (dialog_auswertung != 0)
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("The restore from the image was not successfully.", "Die Wiederherstellung des Abbildes war nicht erfolgreich.\n"));
	}
        thread_run_clone_net = 0;
        thread2.exit();	 
 }

void DialogClone_net::elapsedTime()
 {
    sekunde_elapsed_clone_net = sekunde_elapsed_clone_net + 1; 
    sekunde_summe_clone_net  = sekunde_summe_clone_net + 1; 
    if (sekunde_elapsed_clone_net == 60) {
        sekunde_elapsed_clone_net = 0 ;
        minute_elapsed_clone_net = minute_elapsed_clone_net + 1;
       }
    if (minute_elapsed_clone_net == 60) {
        minute_elapsed_clone_net = 0 ;
        hour_elapsed_clone_net = hour_elapsed_clone_net + 1;
       }
    QString sekunde_ = QString::number(sekunde_elapsed_clone_net);
    SekundeElapsed ->setText(sekunde_);
    QString minute_ = QString::number(minute_elapsed_clone_net);
    MinuteElapsed ->setText(minute_);
    QString hour_ = QString::number(hour_elapsed_clone_net);
    HourElapsed ->setText(hour_);
}

void DialogClone_net::remainingTime(int prozent)
 {
    if (prozent >= 1) {
       double remaining_time =(100 * sekunde_summe_clone_net/prozent)-sekunde_summe_clone_net ;
       int dummy = (int)remaining_time;
       int hour_remaining = (int)(remaining_time/3600);
       QString hour_ = QString::number(hour_remaining);
       HourRemaining ->setText(hour_);
       int min_remaining = (dummy % 3600)/60;
       QString minute_ = QString::number(min_remaining);
       MinuteRemaining ->setText(minute_);
       int sek_remaining = (dummy % 60);
       QString sekunde_ = QString::number(sek_remaining);
       SekundeRemaining ->setText(sekunde_);
    }
}

void DialogClone_net::keyPressEvent(QKeyEvent *event) {
MWindow window;   
     QWidget::keyPressEvent(event);
     switch( event->key() ) {
         case Qt::Key_Escape:
              	esc_end(); 
         break;
     }
}

void DialogClone_net::esc_end()
{
MWindow window;
QString befehl;
   if (thread_run_clone_net > 0) {
    int ret = questionMessage(tr("Do you want really break save or restore an image from the partition?", "Wollen Sie wirklich die Erstellung oder das Zurückschreiben eines Images der Festplatte beenden?"));
      if (thread_run_clone_net  == 1 && ret == 1)
        {
	befehl = "rm "  + folder_free_clone_net +  partition_name_net + ".fsa.gz";	
        system (befehl.toAscii().data()); 
        
        if (window.bit_version() == "64")
        	{
        	thread1.terminate();
        	thread1.wait();
                close();
        	}
	}
        if (window.bit_version() == "32")
        	{
		befehl = "kill -15 " + pid1_clone_net;  //dd abbrechen
     		system (befehl.toAscii().data());
    		befehl = "kill -15 " + pid_clone_net;  //dd abbrechen
    		system (befehl.toAscii().data());
		close();
                }
     if (thread_run_clone_net == 2 && ret == 1)
        {
         if (window.bit_version() == "64")
        	{
        	thread2.terminate();
        	thread2.wait();
                close();
        	}
        if (window.bit_version() == "32")
        	{
		befehl = "kill -15 " + pid1_clone_net;  //dd abbrechen
     		system (befehl.toAscii().data());
    		befehl = "kill -15 " + pid_clone_net;  //dd abbrechen
    		system (befehl.toAscii().data());
                close();
                }
	}
    }
}

void DialogClone_net::pid_ermitteln()
{
int pida=getpid();
	pid_clone_net = QString::number(pida);
int pida1=getppid();
	pid1_clone_net = QString::number(pida1);
}

int DialogClone_net::block_size(int row)
{
QString disk_size[50];
QString teilstring;
QString befehl;
QString harddrive;
int  i = 0, j=0;
int pos;
        harddrive = part_clone_net[row][0];
        harddrive = harddrive.left(harddrive.size() -1);
        befehl = "fdisk -l " + harddrive + " 1> " +  homepath_net + "/.config/qt4-fsarchiver/disk.txt";
        system (befehl.toAscii().data());
	QString filename = homepath_net + "/.config/qt4-fsarchiver/disk.txt";
	QFile file(filename);
        if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
           QTextStream ds(&file);
          while (!ds.atEnd()){
           	disk_size[i] = ds.readLine();
                i = i + 1;
              }
           j = i ;
           }
        file.close();
        befehl = "rm " + filename;
        system (befehl.toAscii().data());
        for (i=0; i< j; i++)
       {
            teilstring = disk_size[i];
            pos = teilstring.indexOf("I/O size (minimum/optimal):");
           if (pos > -1){
		teilstring = teilstring.right(10);
                teilstring = teilstring.left(teilstring.size() -5);
          return teilstring.toInt();
	  } 
 
       }
 return 1;
}

void DialogClone_net::read_write_hd(){
QString teilstring;
QString befehl;
QString harddrive;
int  i = 0, j;
QStringList read_write;
QString rw_;
int read_write_space = 0;
int found;
       if (endeThread_clone_net !=0)
            return;
       read_write_space= 0;
       QTimer::singleShot(5000, this, SLOT(read_write_hd()));
       read_write_zaehler_net = read_write_zaehler_net +1;
        if (read_write_zaehler_net == 10)
	    read_write_zaehler_net = 0;
        befehl = "vmstat 2 2 1> " +  homepath_net + "/.config/qt4-fsarchiver/disk.txt";
        system (befehl.toAscii().data());
        if (sekunde_elapsed_clone_net < 59)
        	sekunde_elapsed_clone_net = sekunde_elapsed_clone_net +1;
        if (sekunde_elapsed_clone_net < 59)
        	sekunde_elapsed_clone_net = sekunde_elapsed_clone_net +1;
        sekunde_summe_clone_net  = sekunde_summe_clone_net + 2;
 	QString filename = homepath_net + "/.config/qt4-fsarchiver/disk.txt";
	QFile file(filename);
        if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
           QTextStream ds(&file);
           teilstring = ds.readLine();
	          teilstring = ds.readLine();
           teilstring = ds.readLine();
           teilstring = ds.readLine();
           }
        file.close();
      	do{
    		found=teilstring.indexOf("  ");
		if (found > 0){
             	    teilstring.replace("  ", " ");

		}
	}
	while  (found >= 0);
        read_write = teilstring.split(" ") ;
        rw_ = read_write[9];  // 10=bo 9=bi
        rrw_net[read_write_zaehler_net]= rw_.toInt();
qDebug() << "rw_[read_write_zaehler1]" <<read_write_zaehler_net << rrw_net[read_write_zaehler_net];
         if (read_write_zaehler_net == 9) {
         //Summe bzw Schnitt bilden der ersten 10 ermittelten Werte
         for (i=0; i< 10; i++){
             read_write_space = read_write_space + rrw_net[i];
            // qDebug() << "rw " <<i << rrw_net[i]  << read_write_space ;
             }
          rrw_sum_net[read_write_zaehler_1_net] = read_write_space / 10;
          read_write_zaehler_1_net = read_write_zaehler_1_net + 1;
          if (read_write_zaehler_1_net == 10)
	    read_write_zaehler_1_net = 0;
          for (j=0; j< 10; j++){
             // Summe bzw Schnitt bilden von 10, 20, 30 40 - 90 eingelesenen Werten
             read_write_space_sum_net = read_write_space_sum_net + rrw_sum_net[j];
            // qDebug() << "rrw_sum_net " <<j << rrw_sum_net[j]  << read_write_space_sum_net ;
            if ( rrw_sum_net[j+1] == 0){
                read_write_space_sum_net = read_write_space_sum_net/(j+1);
                qDebug() << "Abbruch" << j;
                break;
														}  
             }
	  }
         // Durchschnitte errechnen write/read bytes je Sekunde
         qDebug() << "rw_[read_write_sum]"  << read_write_space_sum_net;
   
}

int DialogClone_net::listWidget_folder_free_auslesen() {
    TreeviewRead treeviewread;
    QString homepath = QDir::homePath();
    int row;
    row = listWidget_free_folder->currentRow();
    if (row > -1)
    	folder_free_clone_net = folder_free_clone_net_[row];
    if (rdbt_image_restore->isChecked()) 
       {
       //Verzeichnis mounten
       QString befehl = "umount " + homepath + " /.qt4-fs-client";
       int k = system (befehl.toAscii().data()); 
       befehl = "mount -t cifs -o username=" + user_net_clone + ",password=" + key_net_clone + ",uid=0,gid=0 //" + rechner_IP_clone + "/" + folder_free_clone_net + " " + homepath + "/.qt4-fs-client" ;
       k = system (befehl.toAscii().data()); 
    if (k != 0){
   	QMessageBox::about(this, tr("Note", "Hinweis"),
      	tr("The network computer ",  "Der Netzwerkrechner ") + rechner_IP_clone + tr(" could not be integrated. The program is aborted\n", " konnte nicht eingebunden werden. Das Programm wird abgebrochen\n"));
        return 1;
        	} 
	TreeviewRead *dialog1 = new TreeviewRead;
        dialog1->exec();
        file_net_clone = treeviewread.folder_treeview_holen(); 
        restore_file_name_txt ->setText(file_net_clone);
        if (file_net_clone != "")
	    button_save->setEnabled(true);
        }
        return 0;
}








