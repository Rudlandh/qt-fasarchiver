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
#include "dir.h"
extern "C" {
#include "connect_c_cpp.h"
}

extern int dialog_auswertung;
extern int anzahl_disk;
extern QString parameter[10];
QString folder_dir;
QString folder_path;
QString dummy;
Qt::CheckState state;
QStringList items;
int endeThread_;
int dir_sekunde_elapsed;
int dir_minute_elapsed;
int dir_sekunde_summe;
QStringList items_dir_kerne;


DialogDIR::DialogDIR(QWidget *parent)
{
        setupUi(this); // this sets up GUI
	connect( bt_save, SIGNAL( clicked() ), this, SLOT( folder_dir_path_einlesen() ) ); 
        connect( chk_path, SIGNAL( clicked() ), this, SLOT( treeview_show() ) );
        //connect( btEnd, SIGNAL( clicked() ), qApp, SLOT(close()));
        connect( bt_end, SIGNAL( clicked() ), this, SLOT(close()));
        connect( chk_key, SIGNAL( clicked() ), this, SLOT(chkkey()));
        dirModel = new QDirModel;
   	selModel = new QItemSelectionModel(dirModel);
        dirModel1 = new QDirModel;
   	selModel1 = new QItemSelectionModel(dirModel1);
   	QModelIndex cwdIndex = dirModel->index(QDir::rootPath());
        treeView_dir->setModel(dirModel);
   	treeView_dir->setSelectionModel(selModel);
   	treeView_dir->setRootIndex(cwdIndex);
	QModelIndex cwdIndex1 = dirModel1->index(QDir::rootPath());
        treeView_path->setModel(dirModel1);
   	treeView_path->setSelectionModel(selModel1);
   	treeView_path->setRootIndex(cwdIndex1);
        items_dir_kerne << "1" << "2" << "3" << "4" <<  "5" << "6" << "7" << "8" ;
   	cmb_kerne->addItems (items_dir_kerne);
   	items_dir_kerne.clear();
        items << "lzo" << tr("gzip fast") << tr("gzip standard") << tr("qzip best") <<  tr("bzip2 fast");
        cmb_zip->addItems (items);
  	items.clear();
        items << tr("bzip2 good") << tr("lzma fast") << tr("lzma medium") << tr("lzma best");
        cmb_zip->addItems (items);
        items.clear();
        // Ini-Datei auslesen
        QString homepath = QDir::homePath();
        QFile file(homepath + "/.config/qt4-fsarchiver/qt4-fsarchiver.conf");
        if (file.exists()) {
   	   QSettings setting("qt4-fsarchiver", "qt4-fsarchiver");
           setting.beginGroup("Basiseinstellungen");
           int auswertung = setting.value("Kompression").toInt();
           cmb_zip -> setCurrentIndex(auswertung); 
           auswertung = setting.value("Kerne").toInt();
           cmb_kerne -> setCurrentIndex(auswertung-1); 
           auswertung = setting.value("overwrite").toInt();
           if (auswertung ==1)
           	chk_overwrite->setChecked(Qt::Checked); 
           auswertung = setting.value("place").toInt();
           if (auswertung ==1)
           	chk_path->setChecked(Qt::Checked);  
           auswertung = setting.value("key").toInt();
           if (auswertung ==1)
           	chk_key->setChecked(Qt::Checked); 
           setting.endGroup();
        } 
        else { 
                cmb_kerne -> setCurrentIndex(0);
        	chk_path->setChecked(Qt::Checked);
        	chk_overwrite->setChecked(Qt::Checked); 
        	cmb_zip -> setCurrentIndex(2); 
        	} 
        chkkey();
	state = chk_key->checkState();
        if (state == Qt::Checked)
           lineKey->setEnabled(true);
        state = chk_path->checkState();
        if (dialog_auswertung == 4){
            chk_key->setText (tr("Encrypt\nbackup", "Sicherung\nverschlüsseln"));
            bt_save->setText (tr("Directory save", "Verzeichnis sichern"));
            label->setText (tr("to saved Directory", "zu sicherndes Verzeichnis"));
            label_2->setText (tr("Location (path) of the backup", "Ort (Pfad) der Sicherung")); 
            chk_path->setEnabled(false);
            chk_overwrite->setEnabled(true);
            cmb_zip->setEnabled(true);
            
            cmb_zip -> setCurrentIndex(2);
            label_4->setEnabled(true);
            label_5->setEnabled(true);
            AnzahlsaveFile->setEnabled(true);
            AnzahlgesicherteFile->setEnabled(true);
            } 
        if (dialog_auswertung == 5){
            chk_key->setText (tr("Decrypt\nBackup", "Sicherung\nentschlüsseln"));
            bt_save->setText (tr("Directory restore", "Verzeichnis zurückschreiben"));  
	    label->setText (tr("back to write backup file", "zurück zu schreibende Sicherungsdatei"));
            label_2->setText (tr("Location (path) of the restore", "Ort (Pfad) der Wiederherstellung")); 
            state = chk_path->checkState();
            if (state == Qt::Checked)
                 treeView_path->setEnabled(false);
            chk_overwrite->setEnabled(false);
            cmb_zip->setEnabled(false);
            label_4->setEnabled(false);
            label_5->setEnabled(false);
            AnzahlsaveFile->setEnabled(false);
            AnzahlgesicherteFile->setEnabled(false);
            } 
 }

void DialogDIR::chkkey(){
     Qt::CheckState state;
     state = chk_key->checkState();
     if (state == Qt::Checked){
        lineKey->setEnabled(true);}
     else {
        lineKey->setEnabled(false);}
}

int DialogDIR::folder_dir_path_einlesen() {
int pos = 0;
QString Befehl = "";
int zip ;
QString compress = "";
QString keyText = "";
Qt::CheckState state1;
   
   	QModelIndex index = treeView_dir->currentIndex();
   	QModelIndexList indexes = selModel->selectedIndexes();
   	folder_dir.append  (dirModel->filePath(index));
   	folder_dir =  (dirModel->filePath(index));
        
   	QModelIndex index1 = treeView_path->currentIndex();
   	QModelIndexList indexes1 = selModel1->selectedIndexes(); 
   	folder_path.append  (dirModel1->filePath(index1));
   	folder_path =  (dirModel1->filePath(index1));
        QFileInfo info(folder_path); 
        keyText = lineKey->text();
        state = chk_overwrite->checkState();
        state1 = chk_key->checkState();
        int indizierung;         
        //Verzeichnis sichern
        if (dialog_auswertung == 4){
           if (state1 == Qt::Checked && keyText.isEmpty())
              {
                QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("There was no key for encryption specified.\n", "Es wurde kein Schlüssel für die Verschlüsselung angegeben.\n"));
		return 1 ;
               }
           if (folder_path == "")
             {
          	QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("Please select the directory in which the to the directory or file to be saved is to be written.\n", "Bitte wählen Sie das Verzeichnis aus, in das das zu sichernde Verzeichnis bzw die zu sichernde Datei geschrieben werden soll.\n"));
		return 1 ;
             }
   	   if (folder_dir == "")
             {
          	QMessageBox::about(this,tr("Note","Hinweis"),
         	tr("Please select the to be directory or file to be saved.\n", "Bitte wählen Sie das zu sichernde Verzeichnis bzw. die zu sichernde Datei aus.\n"));
		return 1 ;
             }
   	  if (info.isFile() && (dialog_auswertung == 4))
            {
      		QMessageBox::about(this, tr("Hinweis"),
      		tr("You have selected a file. You must select a directory\n", "Sie haben eine Datei ausgewählt. Sie müssen ein Verzeichnis auswählen\n"));
      		return 1 ;
            }
        parameter[0] = "fsarchiver";
       	parameter[1] = "savedir"; 
        zip = cmb_zip->currentIndex()+1;
        compress = QString::number(zip);
        compress = "-z" + compress; 
        if (zip == -1)
            parameter[2] = "-z3";
        else
            parameter[2] = compress; 
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
        dummy = folder_dir;
      	while (pos != -1)
      	{
      		pos = dummy.indexOf("/");
      		dummy.replace(pos,1,"-"); 
      		}
      	dummy.replace(0,1,"/"); 
        parameter[indizierung] = folder_path + dummy + ".fsa";
        // ! um Verschlüsselung zu erkennen
	if (state1 == Qt::Checked)
            parameter[indizierung] = folder_path + dummy + "!.fsa";
        parameter[indizierung+1] = folder_dir;
        QFile f(parameter[indizierung]);
        if  (parameter[4] != "-o" && (f.exists())){
            QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("The partition file ", "Die Partitionsdatei ")   + parameter[6] 
	+ tr("already exists. The backup is not performed\n", " ist bereits vorhanden. Die Sicherung wird nicht durchgeführt\n"));
                return 0 ; 
            }
        thread1.setValues(indizierung + 2,0);
            indicator_reset();
            werte_reset();
            bt_save->setEnabled(false);
            startThread1(); // fsarchiver wird im Thread ausgeführt
            while (dialog_auswertung != 0 || endeThread_ != 1)
       		{
                elapsedTime();
                int anzahl  = werte_holen(2);
                QString text_integer;
                text_integer = text_integer.setNum(anzahl);
                AnzahlsaveFile ->setText(text_integer);
                int anzahl1  = werte_holen(3);
                text_integer = text_integer.setNum(anzahl1);
                AnzahlgesicherteFile ->setText(text_integer);
                int meldung = werte_holen(4);
                if (meldung >= 100) // Thread Abschluss mit Fehler
                    break;
                int prozent = werte_holen(1);
                remainingTime(prozent);
                progressBar->setValue(prozent);
                if (prozent >= 99 || dialog_auswertung == 0) {
          	   // bei mehrmaligem Aufruf von fsarchiver wird am Anfang der Sicherung kurzfristig 100 % angezeigt, was falsch ist
                   if (prozent != 100)  
                     progressBar->setValue(100);
                     text_integer = text_integer.setNum(anzahl);
                     AnzahlgesicherteFile ->setText(text_integer);
                     SekundeRemaining ->setText("0");
          	   break;
                   }
                 sleep(1);
       	      } 
            }

      //Verzeichnis zurückschreiben
      if (dialog_auswertung == 5){
      state = chk_path->checkState();
           if (state1 == Qt::Checked && keyText.isEmpty())  
              {
                QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("There was no key to decrypt specified.\n", "Es wurde kein Schlüssel für die Entschlüsselung angegeben.\n"));
		return 1 ;
               }
           if (folder_path == ""  && state != Qt::Checked)
             {
          	QMessageBox::about(this, tr("Note", "Hinweis"),
         	tr("Select the directory in which the saved image will be recovered.\n", "Bitte wählen Sie das Verzeichnis aus, in das das das gesicherte Verzeichnis zurück geschrieben werden soll.\n"));
		return 1 ;
             }
   	   if (folder_dir == "")
             {
          	QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("Please select the existing backup file.\n", "Bitte wählen Sie die vorhandene Sicherungsdatei aus.\n"));
		return 1 ;
             }
   	  if (info.isDir() && (dialog_auswertung == 4))
            {
      		QMessageBox::about(this, tr("Note", "Hinweis"),
      		tr("You have selected a directory. You need a backup file with the extension .fsa select\n", "Sie haben ein Verzeichnis ausgewählt. Sie müssen eine Sicherungsdatei mit der Dateiendung .fsa auswählen\n"));
      		return 1 ;
            }
          pos = folder_dir.indexOf("fsa");
          if (pos == -1)
         	{
                QMessageBox::about(this,tr("Hinweis"),
         	tr("You have chosen the wrong recovery file selected.\n The files should end with. fsa be", "Sie haben eine falsche Wiederherstellungsdatei ausgesucht ausgesucht \nDie Dateiendung muss .fsa sein"));
                return 1;
         	}
        // prüfen ob Verzeichnis verschlüsselt
        int pos = folder_dir.indexOf("!.fsa");
        if (pos > 0 && state1 != Qt::Checked)  //Verzeichnis ist verschlüsselt
           { chk_key->setChecked(Qt::Checked);
             QMessageBox::about(this, tr("Note", "Hinweis"),
              tr("The partition is encrypted. Please enter the Schlüsel\n", "Die Partition ist verschlüsselt. Bitte geben Sie den Schlüsel ein\n"));
              return 0; 
           }
        parameter[0] = "fsarchiver";
        parameter[1] = "restdir"; 
        int kerne = cmb_kerne->currentIndex()+1;
        QString index = QString::number(kerne);
        parameter[2] = "-j" + index;
        indizierung = 3;
        if (state == Qt::Checked){
                parameter[indizierung] = (folder_dir);  //an Originalort zurückschreiben
                indizierung = indizierung + 1;
                parameter[indizierung]= ("/");
                }
        else    {
                parameter[indizierung] = folder_dir ; //an gewähltes Verzeichnis zurückschreiben
                indizierung = indizierung  + 1;
                parameter[indizierung] = folder_path ;
                }
        if (state1 == Qt::Checked) { // Verzeichnis mit Schlüssel gesichert
                parameter[5] = "-c";
                parameter[6] = keyText;
		indizierung = 6;
                }
        thread2.setValues(indizierung + 1, 0); 
        indicator_reset();
        werte_reset();
        bt_save->setEnabled(false);
        startThread2(); // fsarchiver wird im Thread ausgeführt 
        while (dialog_auswertung != 0 || endeThread_ != 1)
       	  {
          elapsedTime();
          int meldung = werte_holen(4);
              if (meldung >= 100) // Thread Abschluss mit Fehler 
                 break;
          int prozent = werte_holen(1);
          remainingTime(prozent);
          // bei mehrmaligem Aufruf von fsarchiver wird am Anfang der Sicherung kurzfristig 100 % angezeigt, was falsch ist
             if (prozent != 100)  
                  progressBar->setValue(prozent);
             if (prozent >= 98) {
          	progressBar->setValue(100);
                SekundeRemaining ->setText("0");
          	break;
                }
          
          sleep(1);
       	  } 
        }
  return 0;
}

void DialogDIR::treeview_show()
{
	state = chk_path->checkState();
        if (state == Qt::Checked)
           treeView_path->setEnabled(false);
        else 
           treeView_path->setEnabled(true);
}

void DialogDIR::startThread1() {
   if( thread1.isRunning() ) return;
   connect( &thread1, SIGNAL(finished()),
            this, SLOT( thread1Ready()));
   thread1.start();
}
void DialogDIR::startThread2() {
   if( thread2.isRunning() ) return;
   connect( &thread2, SIGNAL(finished()),
            this, SLOT( thread2Ready()));
   thread2.start();
}
void DialogDIR::closeEvent(QCloseEvent *event) {
   thread1.wait();
   thread2.wait();
   event->accept();
}

void DialogDIR::thread1Ready()  {
   endeThread_ = endeThread_ + 1;
   extern int dialog_auswertung;
   QString err_regfile_;
   int err_regfile;
   if (endeThread_ == 1) {
     if (dialog_auswertung ==0){
       int cnt_regfile = werte_holen(6);
       QString cnt_regfile_ = QString::number(cnt_regfile);
       int cnt_dir = werte_holen(7);
       QString cnt_dir_ = QString::number(cnt_dir); 
       int cnt_hardlinks = werte_holen(8);
       cnt_hardlinks = cnt_hardlinks + werte_holen(9);
       QString cnt_hardlinks_ = QString::number(cnt_hardlinks); 
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("The backup of the directory was successful.\n", "Die Sicherung des Verzeichnisses war erfolgreich.\n") + cnt_regfile_ +
        tr(" Files, ", " Dateien, ") + cnt_dir_ + tr(" directories, and ", " Verzeichnisse und ") + cnt_hardlinks_ + tr(" links have been saved", " Links wurden gesichert"));
       }
     else {
       // Anzahl nicht korrekt gesicherte Dateien ausgeben
       err_regfile = werte_holen(1);
       err_regfile_ = QString::number(err_regfile);
       int err_dir = werte_holen(2);
       QString err_dir_ = QString::number(err_dir); 
       int err_hardlinks = werte_holen(3);
       err_hardlinks = err_hardlinks + werte_holen(5);
       QString err_hardlinks_ = QString::number(err_hardlinks); 
       QMessageBox::about(this,tr("Note", "Hinweis"), 
         err_regfile_ + tr(" Files, ", " Dateien, ")    + err_dir_ 
	+ tr(" directories, and ", " Verzeichnisse und ") + 
         err_hardlinks_ + tr(" Links were not backed properly. The backup of the directories was only partially successful\n", " Links wurden nicht korrekt gesichert. Die Sicherung der Verzeichnisse war nur teilweise erfolgreich\n") );
     }
   } 
   thread1.exit();
   dialog_auswertung = 4;
   bt_save->setEnabled(true);
}
void DialogDIR::thread2Ready()  {
   endeThread_ = endeThread_ + 1;
   extern int dialog_auswertung;
   QString err_regfile_;
   int err_regfile;
   int meldung = werte_holen(4);
   if (endeThread_ == 1) { 
     if (dialog_auswertung ==0){
       int cnt_regfile = werte_holen(6);
       QString cnt_regfile_ = QString::number(cnt_regfile);
       int cnt_dir = werte_holen(7);
       QString cnt_dir_ = QString::number(cnt_dir); 
       int cnt_hardlinks = werte_holen(8);
       cnt_hardlinks = cnt_hardlinks + werte_holen(9);
       QString cnt_hardlinks_ = QString::number(cnt_hardlinks); 
       QMessageBox::about(this,tr("Note", "Hinweis"), tr("The restoring of the directory was successful.\n", "Die Wiederherstellung des Verzeichnisses war erfolgreich.\n") + cnt_regfile_ + tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, and ", " Verzeichnisse und ") + cnt_hardlinks_ + tr(" links have been restored", " Links wurden wieder hergestellt"));
        }
   if (meldung == 100) {
       // Anzahl nicht korrekt gesicherte Dateien ausgeben
       err_regfile = werte_holen(1);
       err_regfile_ = QString::number(err_regfile);
       int err_dir = werte_holen(2);
       QString err_dir_ = QString::number(err_dir); 
       int err_hardlinks = werte_holen(3);
       err_hardlinks = err_hardlinks + werte_holen(5);
       QString err_hardlinks_ = QString::number(err_hardlinks); 
       QMessageBox::about(this,tr("Note", "Hinweis"),
         err_regfile_ + tr(" Files,", " Dateien, ")    + err_dir_ + 
         tr(" directories, and ", " Verzeichnisse und ") + err_hardlinks_ + 
         tr(" links were not correctly restored. The restoring of the lists was only partially successful", " Links wurden nicht korrekt wiederhergestellt. Die Wiederherstellung der Verzeichnisse war nur teilweise erfolgreich\n"));
       }
   if (meldung == 103) { 
        QMessageBox::about(this, tr("Note", "Hinweis"), tr("You have entered an incorrect password.\n", "Sie haben ein falsches Passwort eingegeben.\n"));
        werte_uebergeben(100,4);
        endeThread_ = 0;
        lineKey->setText ("");
      }
   if (meldung == 104) {
       QMessageBox::about(this, tr("Note", "Hinweis"), 
       tr("You have tried to restore a directory. The selected file can only restore partitions.\n", "Sie haben versucht ein Verzeichnis wiederherzustellen. Die gewählte Datei kann nur Partitionen wiederherstellen.\n"));
   }
   if (meldung == 106) {
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("Error in fsarchiver. The directory can not be restored!", "Fehler in fsarchiver. Das Verzeichnis kann nicht wiederhergestellt werden!!\n"));
       endeThread_ = 0;
   }
}
   thread2.exit();
   dialog_auswertung = 5;
   bt_save->setEnabled(true);
}

void DialogDIR::elapsedTime()
 {
    dir_sekunde_elapsed = dir_sekunde_elapsed + 1; 
    dir_sekunde_summe = dir_sekunde_summe + 1; 
    if (dir_sekunde_elapsed == 60) {
        dir_sekunde_elapsed = 0 ;
        dir_minute_elapsed = dir_minute_elapsed + 1;
       }
    QString sekunde_ = QString::number(dir_sekunde_elapsed);
    SekundeElapsed ->setText(sekunde_);
    QString minute_ = QString::number(dir_minute_elapsed);
    MinuteElapsed ->setText(minute_);
}

void DialogDIR::remainingTime(int prozent)
 {
    int sekunde_tara; 
    if (prozent <= 1) 
       sekunde_tara = dir_sekunde_summe ;
    int sekunde_netto = dir_sekunde_summe - sekunde_tara;
    if (prozent <= 5) {
	SekundeRemaining ->setText(" ");
        MinuteRemaining ->setText(" ");
    }
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

void DialogDIR::indicator_reset() {
// Anzeige zurücksetzen für neuen Aufruf fsarchiver
     progressBar->setValue(0);
     AnzahlgesicherteFile ->setText(0);
     AnzahlsaveFile ->setText(0); 
     SekundeRemaining ->setText(" ");
     MinuteRemaining ->setText(" ");
     SekundeElapsed ->setText("0");
     MinuteElapsed ->setText("0");
     dir_sekunde_summe = 0;
     dir_minute_elapsed = 0;
     endeThread_ = 0;
}
