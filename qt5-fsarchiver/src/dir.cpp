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
#include "dir.h"
#include "mainWindow.h"
#include <unistd.h>
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
int dummy_prozent_dir;
int prozent_ = 0;
int flag_View_dir;
int sekunde_tara_dir ;
QString SicherungsFolderFileName_dir;
int thread_run_dir;
int flag_end_dir;
QString pid_dir, pid1_dir;
extern int fsarchiver_aufruf(int argc, char *anlage0=NULL, char 
*anlage1=NULL, char *anlage2=NULL, char *anlage3=NULL, char 
*anlage4=NULL, char *anlage5=NULL, char *anlage6=NULL, char 
*anlage7=NULL, char *anlage8=NULL, char *anlage9=NULL, char 
*anlage10=NULL, char *anlage11=NULL, char *anlage12=NULL, char 
*anlage13=NULL, char *anlage14=NULL);
QStringList filters;


DialogDIR::DialogDIR(QWidget *parent)
{
        setupUi(this); // this sets up GUI
	connect( bt_save, SIGNAL( clicked() ), this, SLOT( folder_dir_path_einlesen() ) ); 
        connect( chk_path, SIGNAL( clicked() ), this, SLOT( treeview_show() ) );
        connect( bt_end, SIGNAL( clicked() ), this, SLOT(close()));
        connect( chk_key, SIGNAL( clicked() ), this, SLOT(chkkey()));
        connect( pushButton_break, SIGNAL( clicked() ), this, SLOT( esc_end() ) ); 
        connect( chk_hidden, SIGNAL( clicked() ), this, SLOT(chkhidden()));
        timer = new QTimer(this);
        dirModel = new QFileSystemModel;
   	selModel = new QItemSelectionModel(dirModel);
        dirModel1 = new QFileSystemModel;
   	selModel1 = new QItemSelectionModel(dirModel1);
   	QModelIndex cwdIndex = dirModel->index(QDir::rootPath());
        dirModel->setRootPath(QDir::rootPath());
        treeView_dir->setModel(dirModel);
   	treeView_dir->setSelectionModel(selModel);
   	treeView_dir->setRootIndex(cwdIndex);
	QModelIndex cwdIndex1 = dirModel1->index(QDir::rootPath());
        dirModel1->setRootPath(QDir::rootPath());
        treeView_path->setModel(dirModel1);
   	treeView_path->setSelectionModel(selModel1);
   	treeView_path->setRootIndex(cwdIndex1);
/*
	QFileSystemModel *model = new QFileSystemModel;
        QItemSelectionModel *selModel = new QItemSelectionModel(model); 
        // oder : selModel = new QItemSelectionModel(model);
        model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
     	model->setRootPath(QDir::currentPath());
        treeView_dir->setModel(model);
        treeView_dir->setRootIndex(model->index(QDir::rootPath()));
*/
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
        QFile file(homepath + "/.config/qt5-fsarchiver/qt5-fsarchiver.conf");
        if (file.exists()) {
   	   QSettings setting("qt5-fsarchiver", "qt5-fsarchiver");
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
           auswertung = setting.value("hidden").toInt();
           if (auswertung ==1)
           	chk_hidden->setChecked(Qt::Checked); 
           	setting.endGroup();
        } 
        else { 
                cmb_kerne -> setCurrentIndex(0);
        	chk_path->setChecked(Qt::Checked);
        	chk_overwrite->setChecked(Qt::Checked); 
        	cmb_zip -> setCurrentIndex(2); 
        	} 
        chkkey();
        pid_ermitteln();
	     state = chk_key->checkState();
        if (state == Qt::Checked)
           lineKey->setEnabled(true);
        state = chk_path->checkState();
        if (dialog_auswertung == 4){
            chk_key->setText (tr("Encrypt\nbackup. key:", "Sicherung\nverschlüsseln. Schlüssel:"));
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
            filters << "*.*";
   	    } 
        if (dialog_auswertung == 5){
            chk_key->setText (tr("Decrypt\nbackup. key:", "Sicherung\nentschlüsseln. Schlüssel:"));
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
            filters << "*.fsa";
            }
            chkhidden();
 }

void DialogDIR::chkkey(){
     Qt::CheckState state;
     state = chk_key->checkState();
     if (state == Qt::Checked)
         lineKey->setEnabled(true);
     else 
        lineKey->setEnabled(false);
}

int DialogDIR::folder_dir_path_einlesen() {
int pos = 0;
QString Befehl = "";
int zip ;
QString compress = "";
QString keyText = "";
Qt::CheckState state1;
   
   	QModelIndex index = treeView_dir->currentIndex();
   	//QModelIndexList indexes = selModel->selectedIndexes(); nicht erforderlich
   	folder_dir.append  (dirModel->filePath(index));
   	folder_dir =  (dirModel->filePath(index));
        //folder_dir.append  (model->filePath(index));
        //folder_dir =  (model->filePath(index));
   	QModelIndex index1 = treeView_path->currentIndex();
   	// QModelIndexList indexes1 = selModel1->selectedIndexes(); nicht erforderlich
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
      		QMessageBox::about(this, tr("Note", "Hinweis"),
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
        dummy = folder_dir;
      	while (pos != -1)
      	{
      		pos = dummy.indexOf("/");
      		dummy.replace(pos,1,"-"); 
      		}
      	dummy.replace(0,1,"/"); 
        parameter[indizierung] = folder_path + dummy + ".fsa";
        SicherungsFolderFileName_dir = parameter[indizierung];
        parameter[indizierung+1] = folder_dir;
        QFile f(parameter[indizierung]);
        if  (parameter[4] != "-o" && (f.exists())){
            QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("The partition file ", "Die Partitionsdatei ")   + parameter[6] 
	+ tr("already exists. The backup is not performed\n", " ist bereits vorhanden. Die Sicherung wird nicht durchgeführt\n"));
                return 0 ; 
            }
//qDebug() << "befehle" << parameter[0] << parameter[1] << parameter[2] << parameter[3] << parameter[4] << parameter[5] << parameter[6] << parameter[7];
        thread1.setValues(indizierung + 2,"0");
            indicator_reset();
            werte_reset();
            bt_save->setEnabled(false);
            bt_end->setEnabled(false);
            flag_View_dir = 1;
            timer->singleShot( 1000, this , SLOT(ViewProzent( ))) ; 
            startThread1(); // fsarchiver wird im Thread ausgeführt
            return 0;
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
                QMessageBox::about(this,tr("Note", "Hinweis"),
         	tr("You have chosen the wrong recovery file selected.\n The files should end with. fsa be", "Sie haben eine falsche Wiederherstellungsdatei ausgesucht ausgesucht \nDie Dateiendung muss .fsa sein"));
                return 1;
         	}
        // prüfen ob Verzeichnis verschlüsselt
        // Annahme zunächst kein Schlüssel
        if (state1 != Qt::Checked) {
        	parameter[0] = "fsarchiver";
       		parameter[1] = "archinfo";
		parameter[2] = folder_dir;
        	fsarchiver_aufruf(3,parameter[0].toLatin1().data(),parameter[1].toLatin1().data(),parameter[2].toLatin1().data(),parameter[3].toLatin1().data());
        if (werte_holen(4) == 103){
                 chk_key->setChecked(Qt::Checked);
                 lineKey->setEnabled(true);
                 QMessageBox::about(this,tr("Note", "Hinweis"),
         	 tr("The partition is encrypted. Please enter the key", "Die Partition ist verschlüsselt. Bitte geben Sie den Schlüssel ein\n"));
                 return 1;
               	 } }

	if (state1 == Qt::Checked) { // Verzeichnis mit Schlüssel gesichert
                parameter[2] = "-c";
                parameter[3] = keyText;
		if (parameter[3].size() < 6 || parameter[3].size() > 64) {
                QMessageBox::about(this, tr("Note", "Hinweis"),
         		tr("The key length must be between 6 and 64 characters\n", "Die Schlüssellänge muss zwischen 6 und 64 Zeichen sein\n"));
                	return 0 ; 
               	}
                parameter[4] = folder_dir;
                int retour = fsarchiver_aufruf(5,parameter[0].toLatin1().data(),parameter[1].toLatin1().data(),parameter[2].toLatin1().data(),parameter[3].toLatin1().data(),parameter[4].toLatin1().data (),parameter[5].toLatin1().data());
                if ( werte_holen(4) == 103 && retour != 0){
                           QMessageBox::about(this, tr("Note", "Hinweis"), tr("You have entered an incorrect password.", "Sie haben ein falsches Passwort eingegeben. \n"));
           		   lineKey->setText ("");
                           return 0;
                        }
                }
        parameter[0] = "fsarchiver";
        parameter[1] = "restdir"; 
        int kerne = cmb_kerne->currentIndex()+1;
        QString index = QString::number(kerne);
        if (index == "0")
   		index = "1"; 
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
        thread2.setValues(indizierung + 1, "0"); 
        indicator_reset();
        werte_reset();
        bt_save->setEnabled(false);
        bt_end->setEnabled(false);
        flag_View_dir = 2;
  	timer->singleShot( 1000, this , SLOT(ViewProzent( ))) ;
        startThread2(); // fsarchiver wird im Thread ausgeführt
        return 0;
        }
  return 1;
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
   thread_run_dir = 1;
   thread1.start();
}
void DialogDIR::startThread2() {
   if( thread2.isRunning() ) return;
   connect( &thread2, SIGNAL(finished()),
            this, SLOT( thread2Ready()));
   thread_run_dir = 2;
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
       QString cnt_special_;
       cnt_special_ = QString::number(cnt_special);
QMessageBox::about(this, tr("Note", "Hinweis"), tr("The backup of the directory was successful.\n", "Die Sicherung des Verzeichnisses war erfolgreich.\n") + cnt_regfile_ + 
        tr(" files, ", " Dateien, ") + cnt_dir_ + tr("  directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr("  links and ", " Links und ") + cnt_special_ + tr(" specials have been backed.", " spezielle Daten wurden gesichert."));
      }
      else if (flag_end_dir == 1)
      {
         QMessageBox::about(this, tr("Note", "Hinweis"),
         tr("The backup of the folder was aborted by the user!\n", "Die Sicherung des Verzeichnisses wurde vom Benutzer abgebrochen!\n") );
         bt_save->setEnabled(true);
	}
       else
       {
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
   thread_run_dir = 0; 
   thread1.exit();
   dialog_auswertung = 4;
   bt_save->setEnabled(true);
   bt_end->setEnabled(true);
   progressBar->setValue(0);
}

void DialogDIR::thread2Ready()  {
   endeThread_ = endeThread_ + 1;
   extern int dialog_auswertung;
   QString err_regfile_;
   int err_regfile;
   int meldung = werte_holen(4);
   if (endeThread_ == 1) { 
     if (dialog_auswertung ==0){
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
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("The restoring of the directory was successful.\n", "Die Wiederherstellung des Verzeichnisses war erfolgreich.\n") + cnt_regfile_ + 
        tr(" files, ", " Dateien, ") + cnt_dir_ + tr(" directories, ", " Verzeichnisse, ") + cnt_hardlinks_ + tr(" links and ", " Links und ") + cnt_special_ + tr(" specials have been restored.", " spezielle Daten wurden wieder hergestellt."));
        }
   if (flag_end_dir == 1) {
        QMessageBox::about(this, tr("Note", "Hinweis"),
         tr("The restore of the folder was break by user!\n", "Die Wiederherstellung des Verzeichnisses wurde vom Benutzer abgebrochen!\n") );
	meldung = 0;
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
         err_regfile_ + tr(" files,", " Dateien, ")    + err_dir_ + 
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
   if (meldung == 107) {
       QMessageBox::about(this, tr("Note", "Hinweis"), tr("Error!  The restoring of the folder was only partially successful!", "Fehler! Das Verzeichnis wurde nur teilweise wiederhergestellt. \n"));
       endeThread_ = 0;
   }
}
   thread_run_dir = 0;
   thread2.exit();
   dialog_auswertung = 5;
   bt_save->setEnabled(true);
   bt_end->setEnabled(true);
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
    if (prozent <= 1) 
       sekunde_tara_dir  = dir_sekunde_summe ;
    int sekunde_netto = dir_sekunde_summe - sekunde_tara_dir ;
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
     werte_reset();
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
     dir_sekunde_elapsed = 0;
     dir_minute_elapsed = 0;
}

void DialogDIR::ViewProzent()
{
QString sekunde;
int sekunde_;
QString minute;
int minute_;
int meldung;
int anzahl;
int anzahl1;
QString text_integer;

if (endeThread_ !=1)
{
 timer->singleShot( 1000, this , SLOT(ViewProzent( )) ) ;
  elapsedTime();
  meldung = werte_holen(4);
if (meldung >= 100) // Thread Abschluss mit Fehler
   		return;
if (flag_View_dir == 1)
	{
 	anzahl  = werte_holen(2);
 	text_integer = text_integer.setNum(anzahl);
 	AnzahlsaveFile ->setText(text_integer);
 	anzahl1  = werte_holen(3);
 	text_integer = text_integer.setNum(anzahl1);
 	AnzahlgesicherteFile ->setText(text_integer);
	}
 
 prozent_ = werte_holen(1);
  
 if (dummy_prozent_dir != prozent_)
     remainingTime(prozent_);
 else {
        if (prozent_ > 5)
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
 dummy_prozent_dir = prozent_;
}
// bei mehrmaligem Aufruf von fsarchiver wird am Anfang der Sicherung kurzfristig 100 % angezeigt, was falsch ist
 if (prozent_ <= 99 ) 
   progressBar->setValue(prozent_);
 if (prozent_ == 99 or endeThread_ >= 1) 
   progressBar->setValue(100);
 return;
} 

void DialogDIR::keyPressEvent(QKeyEvent *event) {
MWindow window;
     QWidget::keyPressEvent(event);
     switch( event->key() ) {
         case Qt::Key_Escape:
              //if (window.bit_version() == "64")
               	esc_end(); 
         break;
     }
}

int DialogDIR::questionMessage(QString frage)
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

void DialogDIR::esc_end()
{
MWindow window; 
QString befehl;
   if (thread_run_dir > 0) {
    int ret = questionMessage(tr("Do you want really break the save or restore from the folder?", "Wollen Sie wirklich die Sicherung oder Wiederherstellung der Verzeichnisse beenden?"));
      if (thread_run_dir == 1 && ret == 1)
        {
        flag_end_dir= 1;
     	befehl = "rm "  + SicherungsFolderFileName_dir;
        system (befehl.toLatin1().data());
        if (window.bit_version() == "64")
		{ 
      		thread1.terminate();
        	thread1.wait();
		close();
		}
        if (window.bit_version() == "32")
        	{
		befehl = "kill -15 " + pid1_dir;  //fsarchiver abbrechen
     		system (befehl.toLatin1().data());
    		befehl = "kill -15 " + pid_dir;  //fsarchiver abbrechen
    		system (befehl.toLatin1().data());
                close();
                }
        }
    if (thread_run_dir == 2 && ret == 1 )
        {
        flag_end_dir= 1; 
        if (window.bit_version() == "64")
		{ 
      		thread1.terminate();
        	thread1.wait();
		close();
		}
	if (window.bit_version() == "32")
        	{
		befehl = "kill -15 " + pid1_dir;  //fsarchiver abbrechen
     		system (befehl.toLatin1().data());
    		befehl = "kill -15 " + pid_dir;  //fsarchiver abbrechen
    		system (befehl.toLatin1().data());
                close();
                }
        }
 }
}

void DialogDIR::pid_ermitteln()

{
int pida=getpid();
pid_dir = QString::number(pida);
int pida1=getppid();
pid1_dir = QString::number(pida1);
}

void DialogDIR::chkhidden(){
     Qt::CheckState state;
     state = chk_hidden->checkState();
     filters << "*.fsa";
     if (state != Qt::Checked)
        {
   	dirModel1->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
   	dirModel1->setNameFilters(filters); 
        dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
   	dirModel->setNameFilters(filters); 
        }
     if (state == Qt::Checked)
       	{
     	dirModel1->setFilter(QDir::AllDirs  | QDir::Hidden | QDir::Files | QDir::NoDotAndDotDot);
      	dirModel1->setNameFilters(filters); 
       	dirModel->setFilter(QDir::AllDirs | QDir::Hidden | QDir::Files | QDir::NoDotAndDotDot);
       	dirModel->setNameFilters(filters); 
       	}
}









