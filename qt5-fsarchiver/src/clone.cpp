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

#include <QtGui> 
#include <QMessageBox> 
#include "clone.h"
#include <string.h>
#include <unistd.h>
#include "mainWindow.h"

extern int dialog_auswertung;
extern int anzahl_disk;
QStringList filters_clone;
QString part_clone[100][4];
QString folder_clone;
int endeThread_clone;
int flag_clone;
int thread_run_clone;
int sekunde_elapsed_clone;
int minute_elapsed_clone;
int hour_elapsed_clone;
float sekunde_summe_clone = 0;
float sekunde_summe_clone_1 = 0;
QString pid_dd = " ";
QString pid_2_dd[5];
QString partition_name;
int read_write_counter = 0;
int read_write_counter_1 = 1;
int rrw[10];
float read_write_space_sum = 0;
float read_write_space_summe = 0;
QString homepath = QDir::homePath();
float partition_exist_size_int;
int dummy_prozent_clone;
float size_clone;
float size_clone_dummy;
QString read_write_space_sec;
QString img_partition_clone;
QString img_partition_size;


DialogClone::DialogClone(QWidget *parent)
{

	setupUi(this); // this sets up GUI
	connect( bt_save, SIGNAL( clicked() ), this, SLOT( todo() ) );
        connect( rdbt_clone, SIGNAL( clicked() ), this, SLOT(rdbutton_clone() ) ); 
	connect( rdbt_image_save, SIGNAL( clicked() ), this, SLOT(rdbutton_image_save() ) ); 
        connect( rdbt_image_restore, SIGNAL( clicked() ), this, SLOT(rdbutton_image_restore() ) ); 
        connect( rdbt_partition_save, SIGNAL( clicked() ), this, SLOT(rdbutton_partition_image_save() ) ); 
        connect( rdbt_partition_restore, SIGNAL( clicked() ), this, SLOT(rdbutton_partition_image_restore() ) ); 
        connect( pushButton_break, SIGNAL( clicked() ), this, SLOT(esc_end()));
        connect( bt_end, SIGNAL( clicked() ), this, SLOT(close()));
        connect( pushButton_folder, SIGNAL( clicked() ), this, SLOT(folder_einlesen()));
        connect( pushButton_partition, SIGNAL( clicked() ), this, SLOT(listWidget_auslesen()));
        dirModel = new QFileSystemModel;
   	selModel = new QItemSelectionModel(dirModel);
   	treeView_clone->setModel(dirModel);
   	treeView_clone->setSelectionModel(selModel);
   	QModelIndex cwdIndex = dirModel->index(QDir::rootPath());
        dirModel->setRootPath(QDir::rootPath());
   	treeView_clone->setRootIndex(cwdIndex);
        rdbt_clone->setChecked(Qt::Checked);
        treeView_clone->setEnabled(false);
       	format_Disk();
        bt_save->setText (tr("Clone Harddrive", "Festplatte klonen"));
        timer_clone = new QTimer(this);
        timer_read_write = new QTimer(this);
       // Erforderlich um Textdatei vor dem ersten Auslesen mit 3 Zeilen zu füllen
        QString befehl = "vmstat 1 2 1> " +  homepath + "/.config/qt5-fsarchiver/disk.txt";
        system (befehl.toLatin1());
        chk_zip->setEnabled(false);
        chk_zip->setChecked(Qt::Checked);
        //chk_zip->set| grepHidden(true);
        listWidget->setHidden(true);
        addWidget(); 
 }

void DialogClone::format_Disk() {
QString disk_part_name = "a b c d e f g h i j k l m n o p q r s t u v w x y z";
QStringList disk_part_name_list;
QString disk_part_name_1;
QString teilstring, space1, space2;
QString befehl;
QString disk_clone[50];
QStringList disk_kurz;
int  i = 0, j=0 ,k=0, m=0;
int pos;
//float  dummy1;
QString dummy;
        QString filename = homepath + "/.config/qt5-fsarchiver/disk.txt";
	QFile file(filename);
        disk_part_name_list = disk_part_name.split(" ") ;
        for (m=0; m < 26; m++){
             befehl = "fdisk -l | grep ' /dev/sd" + disk_part_name_list[m] + "': 1> " +  homepath + "/.config/qt5-fsarchiver/disk.txt"; //Festplatten auslesen
             system (befehl.toLatin1().data());
             if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
                QTextStream ds(&file);
                disk_clone[m] = ds.readLine();
                if (disk_clone[m] != ""){
                disk_kurz = disk_clone[m].split(" ") ;
                part_clone[i][0] = disk_kurz[1];
                dummy = disk_kurz[4];
                part_clone[i][1] = disk_kurz[4].left(disk_kurz[4].size() -6);
                part_clone[i][2] = disk_kurz[4].left(disk_kurz[4].size() -9);
                //dummy1 = disk_kurz[4].toDouble();
                //dummy1 = qRound(dummy1);
		i = i + 1;
                }
		}
		file.close();
	}
                befehl = "rm " + filename;
                system (befehl.toLatin1().data());
	        
	j = i;
        for (i=0; i< j; i++)
       {
           teilstring = part_clone[i][1];
           k = teilstring.size();
           teilstring = part_clone[i][0];
           pos = teilstring.indexOf("/sd");
          // space1.fill (' ',12 - k);
          // space2.fill (' ',15 - k);
           space1.fill (' ',10 - k);
           space2.fill (' ',13 - k);
           if (part_clone[i][1].toInt() > 10000)
   	      disk_clone[i] = teilstring.right(pos) + space2 + part_clone[i][2] + " GB";
           if (part_clone[i][1].toInt() < 10000)
              disk_clone[i] = teilstring.right(pos) + space1 + part_clone[i][1] + " MB";
           listWidget_clone->addItem (disk_clone[i]);
           listWidget_exist->addItem (disk_clone[i]);
 
       }
        
}

void DialogClone::addWidget() {
extern QString add_part[100];
int i = 0;
    while (add_part[i] != "")
    {
	listWidget->addItem (add_part[i]);
        i++ ;
     }
}

void DialogClone::todo(){
	if (rdbt_clone->isChecked())
	   do_clone();
	if (rdbt_image_save->isChecked()) 
            do_image();
 	if (rdbt_image_restore->isChecked())
           restore_image();
        if (rdbt_partition_save->isChecked()) 
            do_image_partition();
        if (rdbt_partition_restore->isChecked()) 
            restore_image_partition();
 }

int DialogClone::do_image_partition()
{
MWindow window;
QString befehl;
QString _Datum_clone = window.Zeit_auslesen();
Qt::CheckState state;
      lbl_save->setText (tr("already saved", "bereits gesichert"));
      state = chk_zip->checkState();
      flag_clone =3;
      if (img_partition_clone == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a partition.", "Sie müssen eine Partition auswählen.\n"));
	return 0;        
	}
      if (folder_clone == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a folder.", "Sie müssen ein Verzeichnis auswählen.\n"));
	return 0;        
	}
     partition_exist_size_int = img_partition_size.toInt();
    // Überprüfen, ob System oder Home-Partition auf der Festplatte vorhanden ist
    int part_art_clone = mountpoint(img_partition_clone);
    if (part_art_clone == 1){
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("On the hard drive exist a root or a home partition. You must need a live-CD.", "Auf der Festplatte ist eine root- oder home Partition. Sie müssen eine Live-CD benutzen.\n"));
	return 0;
      }
       if (state == Qt::Checked)
		befehl = "dd if=/dev/" + img_partition_clone + " | gzip --best > " + folder_clone + "/" + img_partition_clone + "-" + _Datum_clone + "-gz.part.fsa";
       else
	        befehl = "dd if=/dev/" + img_partition_clone + " of=" + folder_clone + "/" + img_partition_clone + "-" + _Datum_clone + "-part.fsa bs=1M 2>" + homepath + "/.config/qt5-fsarchiver/disk.txt"; 
 	thread1.setValues( 0,befehl);
	int ret = questionMessage(tr(" Do you want really built a image from a partition? ", " Wollen Sie wirklich ein Abbild einer Partition erstellen? ") );  
              if (ret == 2)
                 return 0;
              if (ret == 1){
               	bt_end->setEnabled(false);
            	bt_save->setEnabled(false);
              	ViewProzent();
                this->setCursor(Qt::WaitCursor);
		if (state == Qt::Checked){
			read_write_hd();
			startThread1(1);
                   }
                else 
                	startThread1(0);
                qDebug() << "The image is created";
               }
    return 0;
}

int DialogClone::restore_image_partition()
{
QString befehl;
int pos;
QString partition_exist;
QString partition_exist_size;
QString part_name;
Qt::CheckState state;
      lbl_save->setText (tr("already restored", "bereits zurückgeschrieben"));
      state = chk_zip->checkState();
      flag_clone = 3;
      partition_exist_size_int = img_partition_size.toInt();
     if (img_partition_clone == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a partition.", "Sie müssen eine Partitiom auswählen.\n"));
	return 0;        
	}
     if (folder_clone == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a part.fsa file or an part.fsa.gz file.", "Sie müssen eine part.fsa- oder eine part.fsa.gz-Datei auswählen.\n"));
	return 0;        
	}
     if (file_check() == 1)
        return 0;
     // Prüfen, ob gesicherte und wiederherzustellende Partition übereinstimmt.
     // Partition aus folder_clone extrahieren
         pos = folder_clone.indexOf("-gz.part");
         if (pos > -1)
             part_name = folder_clone.mid(pos-15,4);
         pos = folder_clone.indexOf("-part");
         if (pos > -1)
             part_name = folder_clone.mid(pos-15,4);
         //Prüfen ob Partition zeistellig ist
         pos = part_name.indexOf("sd");
         if (pos == -1)
            part_name= "s" + part_name;
         if (folder_clone.indexOf(img_partition_clone) == -1)
            pos = questionMessage(tr("Partition to restore ", "Die wiederherzustellende Partition ") + part_name + 
               tr(" does not coincide with the saved  ", " stimmt nicht mit der gesicherten ") + img_partition_clone + tr(" Do you want to continue restore?", " überein. Wollen Sie trotzdem die Wiederherstellung durchführen?"));
               if (pos == 2)  //nicht wiederherstellen
                  return 0;
    // Überprüfen, ob System oder Home-Partition auf der Festplatte vorhanden ist
    int part_art_clone = mountpoint(img_partition_clone);
    if (part_art_clone == 1){
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("On the hard drive exist a root or a home partition. You must need a live-CD.", "Auf der Festplatte ist eine root- oder home Partition. Sie müssen eine Live-CD benutzen.\n"));
	return 0;
	}
	partition_name  = partition_exist.right(partition_exist.size() -4);
//Prüfen ob img.fsa oder gz.fsa Datei
        if (folder_clone.indexOf("-gz.part.fsa") > -1)
        	befehl = "gunzip -c " + folder_clone + " | sudo dd of=/dev/" + img_partition_clone; 
        if (folder_clone.indexOf("-part.fsa") > -1)
                befehl = "dd if=" + folder_clone +  " of=/dev/" +  img_partition_clone + " bs=1M 2>" + homepath + "/.config/qt5-fsarchiver/disk.txt";
            int ret = questionMessage(tr(" Do you want really restore an image of a partition? ", " Wollen Sie wirklich ein Abbild einer Partition zurückschreiben? ") );  
              if (ret == 2)
                 return 0;
              if (ret == 1){
               	bt_end->setEnabled(false);
            	bt_save->setEnabled(false);
                thread2.setValues( 0,befehl);
                ViewProzent();
                this->setCursor(Qt::WaitCursor);
                pos = folder_clone.indexOf("gz.part");
		if (pos > 0){
                      read_write_hd();
		      startThread2(1);
			}
                else 
                	startThread2(0);
                qDebug() << "The image is written back";
    	}
                
    return 0;
}

int DialogClone::do_clone()
{
QString befehl;
int row;
int pos;
QString partition_exist;
QString partition_exist_size;
QString partition_clone;
QString partition_clone_size;
int partition_clone_size_int;
lbl_save->setText (tr("already saved", "bereits gesichert"));
QFile file(homepath + "/.config/qt5-fsarchiver/disk.txt");
      befehl = "rm "  + homepath + "/.config/qt5-fsarchiver/disk.txt";	
      if (file.exists()) 
	  system (befehl.toLatin1().data()); 
      flag_clone =1;
      row = listWidget_exist->currentRow();
      if (row > -1){
          partition_exist = part_clone[row][0] ;
          partition_exist_size =  part_clone[row][1];
          pos = partition_exist_size.indexOf(".");
          if (pos > 0)
             partition_exist_size = partition_exist_size.left(partition_exist_size.size() -2);
             partition_exist_size_int = partition_exist_size.toInt();
      }
      row = listWidget_clone->currentRow();
      if (row > -1){
         partition_clone = part_clone[row][0] ;
         partition_clone_size =  part_clone[row][1];
         pos = partition_clone_size.indexOf(".");
         if (pos > 0)
             partition_clone_size = partition_clone_size.left(partition_clone_size.size() -2);
             partition_clone_size_int = partition_clone_size.toInt();
    }
    partition_exist = partition_exist.left(partition_exist.size() -1);
    partition_clone = partition_clone.left(partition_clone.size() -1);
    if (partition_exist == "" or partition_clone == ""){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a hard drive.", "Sie müssen eine Festplatte auswählen.\n"));
	return 0;        
	}
    // Überprüfen, ob Festplatte groß genug ist
    if (partition_exist_size_int > partition_clone_size_int){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("The selected hard drive is too small.", "Die gewählte Festplatte ist zu klein.\n"));
	return 0;
	}
   // Überprüfen, ob System oder Home-Partition auf der Festplatte vorhanden ist
    int part_art_clone = mountpoint(partition_exist);
    if (part_art_clone == 1){
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("On the hard drive exist a root or a home partition. You must need a live-CD.", "Auf der Festplatte ist eine root- oder home Partition. Sie müssen eine Live-CD benutzen.\n"));
	return 0;
}
       befehl = "dd if=" + partition_exist + " of=" + partition_clone + " bs=1M 2>" + homepath + "/.config/qt5-fsarchiver/disk.txt"; 
       int ret = questionMessage(tr(" Do you want really clone the hard drive? All dates on  ", " Wollen Sie wirklich die Festplatte klonen? Alle Daten auf der Festplatte ")   + partition_clone + tr(" are deleted!", " werden gelöscht!") );
              if (ret == 2)
                 return 0;
              thread1.setValues( 0,befehl);
    	      ViewProzent();
              this->setCursor(Qt::WaitCursor);
              startThread1(0);
    return 0;
}

int DialogClone::do_image()
{
MWindow window;
QString _Datum_clone = window.Zeit_auslesen();
QString befehl;
int row;
int pos;
QString partition_exist;
QString partition_exist_size;
Qt::CheckState state;
      lbl_save->setText (tr("already saved", "bereits gesichert"));
      state = chk_zip->checkState();
      flag_clone =2;
      row = listWidget_exist->currentRow();
      if (row > -1){
          partition_exist = part_clone[row][0] ;
          partition_exist_size =  part_clone[row][1];
          pos = partition_exist_size.indexOf(".");
          if (pos > 0)
             partition_exist_size = partition_exist_size.left(partition_exist_size.size() -2);
             partition_exist_size_int = partition_exist_size.toInt();
      }
    partition_exist = partition_exist.left(partition_exist.size() -1);
//qDebug() << "partition_exist_size_int" << partition_exist_size_int;
//return 0;
    if (partition_exist == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a partition.", "Sie müssen eine Partition auswählen.\n"));
	return 0;        
	}
    if (folder_clone == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a folder.", "Sie müssen ein Verzeichnis auswählen.\n"));
	return 0;        
	}
    // Überprüfen, ob System oder Home-Partition auf der Festplatte vorhanden ist
    int part_art_clone = mountpoint(partition_exist);
    if (part_art_clone == 1){
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("On the hard drive exist a root or a home partition. You must need a live-CD.", "Auf der Festplatte ist eine root- oder home Partition. Sie müssen eine Live-CD benutzen.\n"));
	return 0;
}
	partition_name  = partition_exist.right(partition_exist.size() -4);
     
       if (state == Qt::Checked)
		befehl = "dd if=" + partition_exist + " | gzip --best > " + folder_clone +  partition_name + "-" + _Datum_clone + ".gz.fsa";
       else
	        befehl = "dd if=" + partition_exist + " of=" + folder_clone +  partition_name + "-" + _Datum_clone + ".img.fsa bs=1M 2>" + homepath + "/.config/qt5-fsarchiver/disk.txt"; 
 	thread1.setValues( 0,befehl);
	int ret = questionMessage(tr(" Do you want really built a image from the hard drive? ", " Wollen Sie wirklich ein Abbild der Festplatte erstellen? ") );  
              if (ret == 2)
                 return 0;
              if (ret == 1){
               	bt_end->setEnabled(false);
            	bt_save->setEnabled(false);
              	ViewProzent();
                this->setCursor(Qt::WaitCursor);
		if (state == Qt::Checked){
			read_write_hd();
			startThread1(1);}
                else 
                	startThread1(0);
                qDebug() << "The image is created";
               }
    return 0;
}


int DialogClone::restore_image()
{
QString befehl;
int row;
int pos;
QString partition_exist;
QString partition_exist_size;
Qt::CheckState state;
      lbl_save->setText (tr("already restored", "bereits zurückgeschrieben"));
      state = chk_zip->checkState();
      flag_clone =2;
      row = listWidget_exist->currentRow();
      if (row > -1){
          partition_exist = part_clone[row][0] ;
          partition_exist_size =  part_clone[row][1];
          pos = partition_exist_size.indexOf(".");
          if (pos > 0)
             partition_exist_size = partition_exist_size.left(partition_exist_size.size() -2);
             partition_exist_size_int = partition_exist_size.toInt();
      }
     partition_exist = partition_exist.left(partition_exist.size() -1);
     if (partition_exist == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a hard drive.", "Sie müssen eine Festplatte auswählen.\n"));
	return 0;        
	}
     if (folder_clone == "" ){
    	QMessageBox::about(this, tr("Note", "Hinweis"), tr("You must select a gz.fsa file or an img.fsa file.", "Sie müssen eine gz.fsa- oder eine img.fsa-Datei auswählen.\n"));
	return 0;        
	}
     if (file_check() == 1)
        return 0;
// Prüfen, ob gesicherte und wiederherzustellende Partition übereinstimmt.
// Festplatte aus folder_clone extrahieren
QString disk_name;
         pos = folder_clone.indexOf(".gz.fsa");
         if (pos > -1)
             disk_name = folder_clone.mid(pos-14,3);
         pos = folder_clone.indexOf(".img.fsa");
         if (pos > -1)
             disk_name = folder_clone.mid(pos-14,3);
         disk_name = partition_exist.right(partition_exist.size() -4);
         if (folder_clone.indexOf(disk_name) == -1)
            pos = questionMessage(tr("Partition to restore ", "Die wiederherzustellende Partition ") + disk_name + 
               tr(" does not coincide with the saved  ", " stimmt nicht mit der gesicherten ") + img_partition_clone + tr(" Do you want to continue restore?", " überein. Wollen Sie trotzdem die Wiederherstellung durchführen?"));
               if (pos == 2)  //nicht wiederherstellen
                  return 0;
    // Überprüfen, ob System oder Home-Partition auf der Festplatte vorhanden ist
    int part_art_clone = mountpoint(partition_exist);
    if (part_art_clone == 1){
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("On the hard drive exist a root or a home partition. You must need a live-CD.", "Auf der Festplatte ist eine root- oder home Partition. Sie müssen eine Live-CD benutzen.\n"));
	return 0;
	}
	partition_name  = partition_exist.right(partition_exist.size() -4);
//Prüfen ob img.fsa oder gz.fsa Datei
        if (folder_clone.indexOf("gz.fsa") > 0)
        	befehl = "gunzip -c " + folder_clone + " | sudo dd of=" + partition_exist; 
        if (folder_clone.indexOf("img.fsa") > 0)
                befehl = "dd if=" + folder_clone +  " of=" + partition_exist + " bs=1M 2>" + homepath + "/.config/qt5-fsarchiver/disk.txt"; 
            int ret = questionMessage(tr(" Do you want really restore the image of the hard drive? ", " Wollen Sie wirklich ein Abbild der Festplatte zurückschreiben? ") );  
              if (ret == 2)
                 return 0;
              if (ret == 1){
               	bt_end->setEnabled(false);
            	bt_save->setEnabled(false);
                thread2.setValues( 0,befehl);
                ViewProzent();
                this->setCursor(Qt::WaitCursor);
                pos = folder_clone.indexOf("gz.fsa");
		if (pos > 0){
                      read_write_hd();
		      startThread2(1);
			}
                else 
                	startThread2(0);
                qDebug() << "The image is written back";
    	}
                
    return 0;
}

int DialogClone::questionMessage(QString frage)
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

void DialogClone::rdbutton_clone(){
	treeView_clone->setEnabled(false);
	bt_save->setText (tr("Clone Harddrive", "Festplatte klonen"));
        treeView_clone->setEnabled(false);
        //chk_zip->setHidden(true);
        chk_zip->setEnabled(false);
        listWidget->setHidden(true); 
        label_3->setHidden(false);
        lbl_hd->setText (tr("Exist harddrive", "Existierende Festplatte"));
        listWidget_exist->setHidden(false);
        listWidget_clone->setEnabled(true); 
        commandLinkButton_2->setEnabled(true); 
        commandLinkButton->setEnabled(true); 
        commandLinkButton_3->setEnabled(true); 
        commandLinkButton_4->setEnabled(true); 
        commandLinkButton_5->setEnabled(true); 
}

void DialogClone::rdbutton_image_save(){
	treeView_clone->setEnabled(true);
        bt_save->setText (tr("Save Harddrive Image", "Festplatten Abbild erstellen"));
        dirModel->setFilter(QDir::AllDirs  | QDir::NoDotAndDotDot);
   	dirModel->setNameFilters(filters_clone); 
        listWidget_exist->setHidden(false); 
        chk_zip->setEnabled(true);
        lbl_hd->setText (tr("Exist harddrive", "Existierende Festplatte"));
        label_3->setHidden(false);
        listWidget->setHidden(true);
        listWidget_clone->setEnabled(false); 
        commandLinkButton_2->setEnabled(false);
        commandLinkButton->setEnabled(false);
        commandLinkButton_3->setEnabled(false); 
        commandLinkButton_4->setEnabled(false); 
        commandLinkButton_5->setEnabled(false);   
}

void DialogClone::rdbutton_image_restore(){
	treeView_clone->setEnabled(true);
        bt_save->setText (tr("Restore Harddrive Image", "Festplatten Abbild zurückschreiben"));
        filters_clone << "*gz.fsa" << "*img.fsa" ;
   	dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
   	dirModel->setNameFilters(filters_clone); 
        listWidget_exist->setHidden(false); 
        chk_zip->setEnabled(false);
        lbl_hd->setText (tr("Exist harddrive", "Existierende Festplatte"));
        label_3->setHidden(false); 
        listWidget->setHidden(true);
        listWidget_clone->setEnabled(false); 
        commandLinkButton_2->setEnabled(false); 
        commandLinkButton->setEnabled(false);
        commandLinkButton_3->setEnabled(false); 
        commandLinkButton_4->setEnabled(false); 
        commandLinkButton_5->setEnabled(false);    
}

void DialogClone::rdbutton_partition_image_save(){
        treeView_clone->setEnabled(true);
        listWidget->setEnabled(true);
        listWidget_exist->setHidden(true); 
        label_3->setHidden(true); 
        lbl_hd->setText (tr("Save Partition Image", "Image einer Partition erstellen"));
        chk_zip->setEnabled(true);
        bt_save->setText (tr("Save Partition Image", "Partition Abbild erstellen"));
        listWidget->setHidden(false);
        dirModel->setFilter(QDir::AllDirs  | QDir::NoDotAndDotDot);
   	dirModel->setNameFilters(filters_clone); 
}

void DialogClone::rdbutton_partition_image_restore(){
        treeView_clone->setEnabled(true);
        listWidget->setEnabled(true);
        listWidget_exist->setHidden(true); 
        label_3->setHidden(true);  
        lbl_hd->setText (tr("Restore Partition Image", "Image einer Partition zurückschreiben"));
        chk_zip->setEnabled(false);
        bt_save->setText (tr("Restore Partition Image", "Partition Abbild zurückschreiben"));
        filters_clone << "*-part.fsa" << "*-gz.part.fsa" ;
   	dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
   	dirModel->setNameFilters(filters_clone); 
        listWidget->setHidden(false);
}

void DialogClone::listWidget_auslesen() {
    int row = 0, pos = 0, pos1 = 0, pos2 = 0;;
    extern QString add_part[100];
    QStringList partition_kurz;  
    QString widget[100];
    row = listWidget->currentRow();
    img_partition_clone = add_part[row];
    img_partition_size = add_part[row];
    partition_kurz = img_partition_clone.split(" ");
    img_partition_clone = partition_kurz[0]; // z.B. sda1
    img_partition_clone = img_partition_clone.trimmed();
    img_partition_size = img_partition_size.right(12);
    pos = img_partition_size.indexOf("GB");
    pos1 = img_partition_size.indexOf("MB");
    pos2 = img_partition_size.indexOf("TB");
    img_partition_size = img_partition_size.left(9);
    img_partition_size = img_partition_size.trimmed();
    img_partition_size = img_partition_size.replace(",","");
    if (pos > -1)
       img_partition_size = img_partition_size + "0";
   // if (pos1 > -1)
   //    img_partition_size = img_partition_size + "0000";
    if (pos2 > -1)
       img_partition_size = img_partition_size + "0000";
}

//Mountpoint ermitteln
int DialogClone::mountpoint(QString partition)
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

void DialogClone::folder_einlesen() {
   
   QModelIndex index = treeView_clone->currentIndex();
   QModelIndexList indexes = selModel->selectedIndexes();
   folder_clone =  (dirModel->filePath(index));
   
}

int DialogClone::file_check() {
   QFileInfo info(folder_clone); 
   if (!info.isFile() )
      {
      QMessageBox::about(this, tr("Note", "Hinweis"),
      tr("You have selected a folder. You must select a gz.fsa file.\n", "Sie haben ein Verzeichnis ausgewählt. Sie müssen eine gz.fsa Datei auswählen\n"));
      return 1 ;
      }
   return 0;
}


void DialogClone::ViewProzent()
{
int prozent = 0 ;
QString sekunde;
int sekunde_ = 0;
QString minute;
int minute_ = 0;
QString hour;
int hour_ = 0;
QString text_integer;
QString size;
int size_1 = 0;
QString letzte_zeile;
QString befehl;
int diff = 0;
QString mb_sec;
        if (endeThread_clone ==0)
	{
       	QTimer::singleShot(1000, this, SLOT(ViewProzent()));
                // Prüfen, nach wieviel Sekunden ViewProzent erneut aufgerufen wird
        diff = sekunde_summe_clone - sekunde_summe_clone_1;
	size_clone = read_write_space_sum * sekunde_summe_clone; 
//qDebug() << "size_clone" << size_clone; 
        if (size_clone_dummy > size_clone)
		size_clone=size_clone_dummy; //Verhindert, dass die bereits angezeigten gespeicherten Daten nicht reduziert werden
        size_clone_dummy = size_clone; 
        lbl_hd_size ->setText("MB");
        prozent = size_clone/(partition_exist_size_int)/10;
	prozent = 100 * size_clone/(partition_exist_size_int);
        if (prozent > 100)
            prozent = 100;
        sekunde_summe_clone_1 = sekunde_summe_clone;
        savedBytes->setText("");
        size_1 = (int) size_clone;
        if (size_clone > 10000) { 
            size_1 = size_1 / 1000;
            lbl_hd_size ->setText("GB");}
        size = QString::number(size_1);
        mb_sec = QString::number(read_write_space_sum, 'f',1);
        //qDebug() << "prozent size_clone read_write_space_sum sekunde_summe_clone size mb_sec" << prozent <<read_write_space_sum << sekunde_summe_clone<< size << mb_sec;
        if (read_write_space_sum > 0)
        	bytes_sec ->setText(mb_sec);
        if (read_write_space_sec != "")
        	bytes_sec ->setText(read_write_space_sec);
        if (size_clone > 0) 
        	savedBytes->setText(size);
        // verhindert dass mehr gesicherte Bytes angezeigt werden als Festplattengröße
        if (size_clone > partition_exist_size_int)
            savedBytes->setText(QString::number(partition_exist_size_int));
        this->repaint();
        elapsedTime();
       	progressBar->setValue(prozent);
        if (dummy_prozent_clone != prozent)
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
            		sekunde_ = sekunde_ - diff;
                                if (sekunde_ < 0) 
                                    sekunde_ = 0; 
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
 dummy_prozent_clone = prozent;  
        }
} 


void DialogClone::startThread1(int flag) {
QString befehl;
   if( thread1.isRunning() ) return;
   connect( &thread1, SIGNAL(finished()),
            this, SLOT( thread1Ready()));
   thread_run_clone = 1;
   thread1.start();
   bt_end->setEnabled(false);
   bt_save->setEnabled(false);
   // flag=0: Platte klonen, Image ohne zip schreiben
if (flag == 0)
   read_write_hd_1(); //Fortschrittsbalken
}

void DialogClone::startThread2(int flag) {
   if( thread2.isRunning() ) return;
   connect( &thread2, SIGNAL(finished()),
            this, SLOT( thread2Ready()));
   thread_run_clone = 2;
   thread2.start();
   bt_end->setEnabled(false);
   bt_save->setEnabled(false);
   // flag = 0: ungezipptes Image zurückschreiben
   if (flag == 0)
      read_write_hd_1(); //Fortschrittsbalken anzeigen
}

void DialogClone::closeEvent(QCloseEvent *event) {
   thread1.wait();
   thread2.wait();
   event->accept();
}

void DialogClone::thread1Ready()  {
QString befehl;
        extern int dialog_auswertung;
	endeThread_clone = endeThread_clone + 1;
        if (endeThread_clone == 1) {
 	    bt_end->setEnabled(true);
            bt_save->setEnabled(true);
            progressBar->setValue(100);
       	    SekundeRemaining ->setText("0");
            if (size_clone > 10000) { 
               partition_exist_size_int = qRound(partition_exist_size_int)/ 1000;
               lbl_hd_size ->setText("GB");
            }
            savedBytes->setText(QString::number(partition_exist_size_int));
            if (flag_clone==1 && dialog_auswertung == 0)
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("The clone of the hard drive was successfully.", "Das Klonen der Festplatte war erfolgreich.\n"));
            if (flag_clone==2 && dialog_auswertung == 0)
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("Backing up the image of the hard disk was successful.", "Das Sichern des Abbildes der Festplatte war erfolgreich.\n"));
            if (flag_clone==3 && dialog_auswertung == 0)
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("Backing up the image of a partition was successful.", "Die Sichern des Abbildes der Partition war erfolgreich.\n"));
            if (flag_clone==1 && dialog_auswertung != 0){
            	progressBar->setValue(0);
                savedBytes->setText("0");
                this->repaint();
QMessageBox::about(this, tr("Note", "Hinweis"), tr("The clone of the hard drive was not successfully.", "Das Klonen der Festplatte war nicht erfolgreich.\n"));
            }
            if (flag_clone==2 && dialog_auswertung != 0)
                {
                progressBar->setValue(0);
                savedBytes->setText("0");
                this->repaint();
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("Backing up the image of the hard disk was not successful.", "Die Sichern des Abbildes der Festplatte war nicht erfolgreich.\n"));
		}
            if (flag_clone==3 && dialog_auswertung != 0)
                {
                progressBar->setValue(0);
                savedBytes->setText("0");
                this->repaint();
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("Backing up the image of a partition was not successful.", "Die Sichern des Abbildes der Partition war nicht erfolgreich.\n"));
		}
        }
	thread_run_clone = 0;
	thread1.exit();
        befehl = "rm " +  homepath + "/.config/qt5-fsarchiver/disk.txt";
        system (befehl.toLatin1().data());
        SekundeRemaining ->setText("0");
        MinuteRemaining ->setText("0");
        HourRemaining ->setText("0");
        this->setCursor(Qt::ArrowCursor);
}

void DialogClone::thread2Ready()  {
        extern int dialog_auswertung;
	endeThread_clone = endeThread_clone + 1;
        if (endeThread_clone == 1 ) {
	           bt_end->setEnabled(true);
            bt_save->setEnabled(true);
      	    progressBar->setValue(100);
       	    SekundeRemaining ->setText("0");
            if (size_clone > 10000) { 
               partition_exist_size_int = qRound(partition_exist_size_int) / 1000;
               lbl_hd_size ->setText("GB");
            }
            savedBytes->setText(QString::number(partition_exist_size_int));
            if (flag_clone==2 && dialog_auswertung == 0)
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("The restore of the image of the hard disk was successful.", "Die Wiederherstellung des Abbildes der Festplatte war erfolgreich.\n"));
            if (flag_clone==3 && dialog_auswertung == 0)
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("The restore of the image of the partition was successful.", "Die Wiederherstellung des Abbildes der Partition war erfolgreich.\n"));
            if (flag_clone==2 && dialog_auswertung != 0){
                progressBar->setValue(0);
                savedBytes->setText("0");
                this->repaint();
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("The restore of the image of the hard disk was not successful.", "Die Wiederherstellung des Abbildes der Festplatte war nicht erfolgreich.\n"));
                }
            if (flag_clone==3 && dialog_auswertung != 0){
                progressBar->setValue(0);
                savedBytes->setText("0");
                this->repaint();
            	QMessageBox::about(this, tr("Note", "Hinweis"), tr("The restore of the image of the partition was not successful.", "Die Wiederherstellung des Abbildes der Partition war nicht erfolgreich.\n"));
                }
	}
        thread_run_clone = 0; 
        thread2.exit();	
        QString befehl = "rm " +  homepath + "/.config/qt5-fsarchiver/disk.txt";
        system (befehl.toLatin1().data());
        progressBar->setValue(100);
        if (dialog_auswertung != 0) 
           progressBar->setValue(0);
        SekundeRemaining ->setText("0");
        MinuteRemaining ->setText("0");
        HourRemaining ->setText("0");
        this->setCursor(Qt::ArrowCursor);
 }


void DialogClone::elapsedTime()
 {
    sekunde_elapsed_clone = sekunde_elapsed_clone + 1; 
    sekunde_summe_clone  = sekunde_summe_clone + 1; 
    if (sekunde_elapsed_clone == 60) {
        sekunde_elapsed_clone = 0 ;
        minute_elapsed_clone = minute_elapsed_clone + 1;
       }
    if (minute_elapsed_clone == 60) {
        minute_elapsed_clone = 0 ;
        hour_elapsed_clone = hour_elapsed_clone + 1;
       }
    QString sekunde_ = QString::number(sekunde_elapsed_clone);
    SekundeElapsed ->setText(sekunde_);
    QString minute_ = QString::number(minute_elapsed_clone);
    MinuteElapsed ->setText(minute_);
    QString hour_ = QString::number(hour_elapsed_clone);
    HourElapsed ->setText(hour_);
}

void DialogClone::remainingTime(int prozent)
 {
    if (prozent >= 1) {
       float remaining_time =(100 * sekunde_summe_clone/prozent)-sekunde_summe_clone ;
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

void DialogClone::keyPressEvent(QKeyEvent *event) {
     QWidget::keyPressEvent(event);
     switch( event->key() ) {
         case Qt::Key_Escape:
              	esc_end(); 
         break;
     }
}

void DialogClone::esc_end()
{
QString befehl;
   pid_2_ermitteln("dd");
   if (thread_run_clone > 0) {
    int ret = questionMessage(tr("Do you want really break clone, save or restore an image from the partition?", "Wollen Sie wirklich das Klonen der Festplatte, die Erstellung oder die Wiederherstellung eines Images der Festplatte beenden?"));
      if (thread_run_clone  == 1 && ret == 1 or thread_run_clone == 2 && ret == 1)
        {
        befehl = "rm "  + folder_clone +  partition_name + ".gz.fsa";	
        QFile file(folder_clone +  partition_name + ".gz.fsa");
      if (file.exists()) 
	  system (befehl.toLatin1().data()); 
      befehl = "kill -15 " + pid_2_dd[0];  //dd abbrechen
      system (befehl.toLatin1().data());
      befehl = "kill -15 " + pid_2_dd[1];  //dd abbrechen
      system (befehl.toLatin1().data());
       //  qApp->quit();
        flag_clone =0;
    	close();
        }}
}

void DialogClone::pid_2_ermitteln(QString prozess)
{
QString befehl;
QString pid_nummer;
QStringList pid;
int i = 0;
int k = 0;
      QString filename = homepath + "/.config/qt5-fsarchiver/pid_2.txt";
      QFile file(filename);
      if (file.exists())
      {
         befehl = "rm " + filename;
         system (befehl.toLatin1().data());
      }  
      befehl = "ps -aux  1> " +  homepath + "/.config/qt5-fsarchiver/pid_2.txt";
      system (befehl.toLatin1().data());
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream ds(&file);        
        pid_nummer = ds.readLine();
   	while (!ds.atEnd())
      	{
            pid_nummer = ds.readLine();
 	    if (pid_nummer.indexOf("dd if=") >= 0)  // dd Prozess gefunden
               {
      	       pid_2_dd[i] = pid_nummer;
               i = i +1;
               }
   	       if(pid_nummer.isEmpty() )
         	 break;
      	} 
   	file.close();
     }
        befehl = "rm " + filename;
        system (befehl.toLatin1().data());
        pid_nummer = pid_2_dd[0];
        do{
    	   k=pid_nummer.indexOf("  ");
	   if (k > 0)
              pid_nummer.replace("  ", " ");
	   }
	while  (k >= 0);
	     pid = pid_nummer.split(" ");
             pid_2_dd[0] = pid[1];
        pid_nummer = pid_2_dd[1];
        do{
    	   k=pid_nummer.indexOf("  ");
	   if (k > 0)
              pid_nummer.replace("  ", " ");
	   }
	while  (k >= 0);
	     pid = pid_nummer.split(" ");
             pid_2_dd[1] = pid[1];
}

QString DialogClone::pid_ermitteln(QString prozess)
{
QString befehl = "";
QString pid_nummer = "";
QStringList pid;
      befehl = "ps -C " + prozess + " 1> " +  homepath + "/.config/qt5-fsarchiver/pid.txt";
      system (befehl.toLatin1().data());
    QString filename = homepath + "/.config/qt5-fsarchiver/pid.txt";
	QFile file(filename);
    if(file.size() == 0) 
       return " ";
    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
           QTextStream ds(&file);
        pid_nummer = ds.readLine();
        pid_nummer = ds.readLine();
        file.close();
     }
        befehl = "rm " + filename;
        system (befehl.toLatin1().data());
      if (pid_nummer != "")
        {
          pid = pid_nummer.split(" ") ;
          pid_nummer = pid[0]; // 5-stellig
          if (pid_nummer == "")
             pid_nummer = pid[1];// 4-stellig
         }
        return pid_nummer;
}


void DialogClone::read_write_hd(){
QString teilstring = "";
QString befehl = "";
QString harddrive;
int  i = 0;
QStringList read_write;
QString rw_;
QString rw_1;
int read_write_space = 0;
int found = 0;
       
       if (endeThread_clone !=0)
            return;
       read_write_space= 0;
       if (partition_exist_size_int < 20000 )
       		QTimer::singleShot(3000, this, SLOT(read_write_hd()));
       if (partition_exist_size_int > 20000 )
       		QTimer::singleShot(10000, this, SLOT(read_write_hd()));
       
       read_write_counter = read_write_counter +1;
      
        if (read_write_counter == 10){
	    read_write_counter = 0;
            read_write_counter_1 = read_write_counter_1 +1;}
        befehl = "vmstat 2 2 1> " +  homepath + "/.config/qt5-fsarchiver/disk.txt";
        system (befehl.toLatin1().data());

        if (sekunde_elapsed_clone < 59)
        	sekunde_elapsed_clone = sekunde_elapsed_clone +1;
        if (sekunde_elapsed_clone < 59)
        	sekunde_elapsed_clone = sekunde_elapsed_clone +1;
        sekunde_summe_clone  = sekunde_summe_clone + 2;
 	QString filename = homepath + "/.config/qt5-fsarchiver/disk.txt";
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

        if (rdbt_image_save->isChecked() or rdbt_partition_save->isChecked()){
        	rw_ = read_write[9];  // 10=bo 9=bi Auswertung geschriebene Blöcke
                rrw[read_write_counter]= 1.08 * rw_.toInt();}  //1.1 Erfahrungsfaktor
	if (rdbt_image_restore->isChecked() or rdbt_partition_restore->isChecked()){
                rw_ = read_write[10];  // 10=bo 9=bi  Auswertung gelesene Blöcke
          	rrw[read_write_counter]= 1.08 * rw_.toInt();}  //1.1 Erfahrungsfaktor
//qDebug() << "rw_ rrw[read_write_counter]" << rw_ << rrw[read_write_counter];
         if (read_write_counter == 9) {
         //Summe bzw Schnitt bilden der ersten 10 ermittelten Werte
         for (i=0; i< 10; i++){
             read_write_space = read_write_space + rrw[i];
             }
             read_write_space = read_write_space / 10000.0;
             read_write_space_summe = read_write_space_summe + read_write_space;
             // Durchschnitte errechnen write/read bytes je Sekunde
             read_write_space_sum = read_write_space_summe /read_write_counter_1;
             read_write_space = 0;
   }
}

void DialogClone::read_write_hd_1(){
QString befehl;
QString bytes_;
QStringList bytes;
int pos = 0;
	if (endeThread_clone !=0)
            return;
         while (pid_dd.size() < 4) 
        //if (pid_dd.size() < 4) 
           pid_dd = pid_ermitteln("dd"); 
        QTimer::singleShot(10000, this, SLOT(read_write_hd_1()));  //10 sekunden
        // Wird für den Fortschrittsbalken benötigt
        if (pid_dd.size() > 3)
        {
        befehl = "kill -USR1 " + pid_dd;
       //befehl = "kill -USR1 " + pid_dd + " 2>" + homepath + "/.config/qt5-fsarchiver/disk.txt";
        system (befehl.toLatin1().data());
        } 
	QString filename = homepath + "/.config/qt5-fsarchiver/disk.txt";
	QFile file(filename);
        if(file.size() == 0)
           return;
        if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
           QTextStream ds(&file);
          while (!ds.atEnd())
      	{
	     bytes_ = ds.readLine();
      	} }
   	file.close();
        if (bytes_ != ""){
           bytes = bytes_.split(" ") ;
           bytes_ = bytes[0];
           bytes_ =  bytes_.left( bytes_.size() -6);
           read_write_space_sum = (bytes_.toInt()/ sekunde_summe_clone);}
        if (bytes_ != ""){
           read_write_space_sec = bytes[7];
           pos = read_write_space_sec.indexOf(".");
       	  read_write_space_sec = read_write_space_sec.left(pos+1);
           }
}



        










