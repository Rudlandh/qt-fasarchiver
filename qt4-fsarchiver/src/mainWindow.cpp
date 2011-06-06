/*
 * qt4-fsarchiver: Filesystem Archiver
 *
 * Copyright (C) 2010, 2011 Hihin Ruslan und Dieter Baum.  All rights reserved.
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
#include "dir.h"
#include "qt_probe.h"
extern "C" {
#include "connect_c_cpp.h"
#include "fsarchiver.h"
}


using namespace std;

int work_global, work_etap, work_resul;


QString Datum_akt("");
QString partition_;
string partition_str;
QString folder;
QString _Datum;
QString DateiName("") ;
extern int anzahl_disk;
extern int dialog_auswertung;
extern QString parameter[15];
QString part[100][4];
QString widget[100];
int endeThread;
int sekunde_elapsed;
int minute_elapsed;
int sekunde_summe;
QStringList items_kerne_;
QString partition_typ_;
string part_art;
QString zip_[10];
QString SicherungsDateiName;
int sekunde_tara;
int thread_run;

extern "C" {
    int fsarchiver_main(int argc, char**argv);
}

int t_dialog_auswertung;
int dummy_prozent;
int flag_View;

int fsarchiver_aufruf(int argc, char *anlage0=NULL, char *anlage1=NULL, char *anlage2=NULL, char *anlage3=NULL, char *anlage4=NULL, char *anlage5=NULL, char *anlage6=NULL, char *anlage7=NULL, char *anlage8=NULL, char *anlage9=NULL, char *anlage10=NULL, char *anlage11=NULL, char *anlage12=NULL, char *anlage13=NULL, char *anlage14=NULL) {
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

    //msgprintf(MSG_FORCE, _("fsarchiver_aufruf in connect_cpp %d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n"),argc, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6],argv[7], argv[8], argv[9], argv[10], argv[11], argv[12], argv[13], argv[14]);
    work_global=1;
    t_dialog_auswertung=work_resul=0;
    ret=fsarchiver_main(argc, argv);
    t_dialog_auswertung=work_resul=ret;
    work_global=0;
    return ret;
}



MWindow::MWindow() {
    questionLabel = new QLabel;
    QString partition_typ;
    QStringList dummy;
    QStringList partition_kurz;
    QString partition1_;
    QStringList items;
    int pos, pos1;
    string part_art;
    int i,j;
    QString befehl;
    QString homepath = QDir::homePath();


    setupUi(this);
    rdBt_saveFsArchiv->setChecked(Qt::Checked);
    dirModel = new QDirModel;
    selModel = new QItemSelectionModel(dirModel);
    treeView->setModel(dirModel);
    treeView->setSelectionModel(selModel);
    treeView->setEnabled(true);
    QModelIndex cwdIndex = dirModel->index(QDir::rootPath());
    //QModelIndex cwdIndex = dirModel->index("/home/dieter/Programme");
    //QModelIndex cwdIndex = dirModel->index("smb//192.168.2.2/");
    // QModelIndex cwdIndex = dirModel->index("network:///");
    treeView->setRootIndex(cwdIndex);
    groupBox->setFlat (true);
    pushButton_restore->setEnabled(false);
    // Signal-Slot-Verbindungen  Werkzeugleiste
    connect( action_Beenden, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
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
    //schließt alle Fenster
    connect( pushButton_end, SIGNAL( clicked() ), qApp, SLOT(quit()));
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

    timer = new QTimer ( this );

    /*
    // Zeitgeber für Berechnung remainingTime
    QTimer *timer1 = new QTimer(this);
    connect(timer1, SIGNAL(timeout()), this, SLOT(remainingTime()));
    timer1->start(1000);
     */
    items_kerne_ << "1" << "2" << "3" << "4" <<  "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12";
    cmb_kerne->addItems (items_kerne_);
    items_kerne_.clear();

    zip_[0]= "lzo";
    zip_[1]= "gzip fast";
    zip_[2]= "gzip standard";
    zip_[3]= "qzip best";
    zip_[4]= "bzip2 fast";
    zip_[5]= "bzip2 good";
    zip_[6]= "lzma fast";
    zip_[7]= "lzma medium";
    zip_[8]= "lzma best";

    items << zip_[0] << zip_[1] << zip_[2] << zip_[3] <<  zip_[4];
    cmb_zip->addItems (items);
    items.clear();
    items << zip_[5] << zip_[6] << zip_[7] << zip_[8];
    cmb_zip->addItems (items);
    items.clear();
    QFile file2(homepath + "/.config/qt4-fsarchiver");
    befehl = "mkdir " + homepath + "/.config/qt4-fsarchiver" ;
    if (!file2.exists()) {
        system (befehl.toAscii().data());
    }
    QFile file3("/mnt/qt4-fs-client");
    befehl = "mkdir /mnt/qt4-fs-client" ;
    if (!file3.exists()) {
        system (befehl.toAscii().data());
    }

    // Ini-Datei auslesen
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
        auswertung = setting.value("tip").toInt();
        if (auswertung ==1)
            chk_Beschreibung->setChecked(Qt::Checked);
        auswertung = setting.value("key").toInt();
        if (auswertung ==1)
            chk_key->setChecked(Qt::Checked);
        setting.endGroup();
    } else {
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
    label->setEnabled(false);
    starteinstellung();
    if (geteuid() == 0) { // 0 is the UID of the root  1000 von Dieter
        qt_probe();
        // Partitionsname, Partitiontyp und Größe einlesen und formatieren für listWidget Ausgabe
        for (i=1; i < 100; i++) {
            for (j=0; j < 3; j++) {
                if (PartitionString(i,0) != "") {
                    part[i-1][j] = PartitionString(i,j);
                    //qDebug() << "part[i][j]:" <<  part[i-1][j] << i << j;
                }
            }
        }
        format();
        j = 2 ;
        i=0;
        while (j > 1) {
            j = widget[i+1].length();
            partition_ = part[i][0]; // z.B. sda1
            if (pos != -1 )
                anzahl_disk = 1;
            pos = partition_.indexOf("sdb");
            if (pos != -1 )
                anzahl_disk = 2;
            pos = partition_.indexOf("sdc");
            if (pos != -1 )
                anzahl_disk = 3;
            pos = partition_.indexOf("sdd");
            if (pos != -1 )
                anzahl_disk = 4;
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
            part_art = mtab_einlesen(partition_.toAscii().data());
            if (!is_mounted(partition_.toAscii().data())) {
                if (part_art !="system" || part_art != "home") {
                    if (partition_typ == "ext" || partition_typ == "vfat" || partition_typ ==  "ntfs" ) {
                        char mountpoint[100] = "/media/";
                        strcat (mountpoint , part[i][0].toAscii().data());
                        mkdir(mountpoint, 0777);
                        char mountpoint1[100] = "mount ";
                        strcat (mountpoint1,partition_.toAscii().data());
                        strcat (mountpoint1, " ");
                        strcat (mountpoint1, mountpoint);
                        int k = system (mountpoint1);
                    }
                }
            }
            i++;
            if (i > 99) {
                break;
            }
        }
        partition_ = "" ;
    }
}

void MWindow::chkkey() {
    Qt::CheckState state;
    state = chk_key->checkState();
    if (state == Qt::Checked)
        lineKey->setEnabled(true);
    else
        lineKey->setEnabled(false);
}

void MWindow::save_button() {
    rdBt_saveFsArchiv->setChecked(Qt::Checked);
    rdButton_auslesen();
}

void MWindow::restore_button() {
    rdBt_restoreFsArchiv->setChecked(Qt::Checked);
    rdButton_auslesen();
}

void MWindow::rdButton_auslesen() {
    if (rdBt_saveFsArchiv->isChecked()) {
        starteinstellung();
    }
    if (rdBt_restoreFsArchiv->isChecked()) {
        label_folder->setText (tr("Backup File", "Sicherungsdatei"));
        pushButton_save->setText (tr("Partition restore", "Partition zurückschreiben"));
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
    }
}

void MWindow::starteinstellung() {
    label_folder->setText (tr("Backup directory", "Sicherungsverzeichnis"));
    pushButton_save->setText (tr("Save partition", "Partition sichern"));
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
    chkkey();
}

int MWindow::savePartition() {
    QFile file(folder);
    QString Befehl;
    Qt::CheckState state;
    Qt::CheckState state1;
    Qt::CheckState state3;
    char * part_;
    int err = 0;
    QString keyText = "";
//     int prozent;
    int zip;

    indicator_reset();  // reset endeThread and parametrs
    if (rdBt_saveFsArchiv->isChecked()) {

        if (folder == "") {
            QMessageBox::about(this,tr("Note", "Hinweis"),
                               tr("Please select from the backup directory.\n", "Bitte wählen Sie das Sicherungsverzeichnis aus.\n"));
            return 0 ;
        }
        if (partition_ == "") {
            QMessageBox::about(this,tr("Note", "Hinweis"),
                               tr("Please select the partition to be", "Bitte wählen Sie die zu sichernde Partition aus.\n"));
            return 0 ;
        }
        DateiName = lineEdit_DateiName->text();
        if (DateiName == "") {
            QMessageBox::about(this, tr("Note", "Hinweis"),
                               tr("Please select the file name of the backup.\n",
                                  "Bitte wählen Sie den Dateinamen der Sicherung aus.\n"));
            return 0 ;
        }
        if (file.open(QIODevice::ReadOnly)) {
            QMessageBox::about(this, tr("Note", "Hinweis"),
                               tr("You have selected a file. You must select a directory\n",
                                  "Sie haben eine Datei ausgewählt. Sie müssen ein Verzeichnis auswählen\n"));
            file.close();
            return 0 ;
        }
        Zeit_auslesen();
        state= chk_Beschreibung->checkState();
        //Überprüfung ob gemounted
        part_ = partition_.toAscii().data();
        char  dev_[50] = "/dev/";
        strcat (dev_ , part_);  // String zusammenfügen: dev_  = dev_ + part_
        int liveFlag = 0;
        zip = cmb_zip->currentIndex()+1;
        QString dummy = partition_;
        // Werte sammeln und nach file_dialog übergeben, Partition ist eingehängt
        part_art = mtab_einlesen(("/dev/" + partition_).toAscii().data());
        int row = listWidget->currentRow();
        beschreibungstext("/dev/" + partition_, DateiName + "-" + _Datum + ".fsa", zip, row);
        partition_ = dummy;
        if (is_mounted(dev_)) {
            part_art = mtab_einlesen(("/dev/" + partition_).toAscii().data());
            //Überprüfung ob System oder Home Partition
            int ret = 1;
            if (part_art == "system") {
                ret = questionMessage(tr("To back up the system partition is mounted. Do you want to do a live backup?", "Die zu sichernde Systempartition ist eingehängt. Wollen Sie eine Live-Sicherung durchführen?"));
                if (ret == 2)
                    return 0;
                if (ret == 1)
                    liveFlag = 1;
            }
            if (part_art == "home") {
                ret = questionMessage(tr("To back up home partition is mounted. Do you want to do a live backup?", "Die zu sichernde Homepartition ist eingehängt. Wollen Sie eine Live-Sicherung durchführen?"));
                if (ret == 2)
                    return 0;
                if (ret == 1)
                    liveFlag = 1;
            }
            if (part_art != "system" || part_art != "home") {
                //char * part_= new char [part_art.size()+1];  //Umwandlung von string nach char
                //strcpy (part_, part_art.c_str());
                char umountpoint[100] = "umount ";
                strcat (umountpoint,dev_);
                //cout << umountpoint << endl;
                if (liveFlag == 0)
                    err = system (umountpoint);
                if (err != 0 && liveFlag == 0) {
                    QMessageBox::about(this,tr("Note", "Hinweis"),
                                       tr("The partition", "Die Partition ")   + partition_ + tr("can not be unmounted. The program is terminated\n", " kann nicht ausgehängt werden. Das Programm wird abgebrochen\n"));
                    return 0 ;
                }
            }
        }
        if (rdBt_saveFsArchiv->isChecked()) {
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
            if (state == Qt::Checked) { //Überschreiben ja
                parameter[4] = "-o";
                indizierung = 5;
            }
            if (state1 == Qt::Checked) { //Schlüssel ja
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
            if (state3 == Qt::Checked) { //Archiv splitten
                parameter[indizierung] = "-s 4400";
                indizierung = indizierung + 1;
            }
            if (liveFlag == 1) {
                parameter[indizierung] = "-A";
                parameter[indizierung + 1] = "-a";
                indizierung = indizierung + 2;
            }
            parameter[indizierung] = (folder + "/" + DateiName + "-" + _Datum + ".fsa");
            // ! um Verschlüsselung zu erkennen
            //if (state1 == Qt::Checked)
            //   parameter[indizierung] = (folder + "/" + DateiName + "-" + _Datum + "!.fsa");
            parameter[indizierung + 1] = ("/dev/" + partition_);
            QFile f(parameter[indizierung]);
            if  (parameter[4] != "-o" && (f.exists())) {
                QMessageBox::about(this, tr("Note", "Hinweis"),
                                   tr("The partition file", "Die Partitionsdatei") + parameter[6] + tr(" already exists. The backup is not performed\n", " ist bereits vorhanden. Die Sicherung wird nicht durchgeführt\n") );
                return 0 ;
            }
            state = chk_Beschreibung->checkState();
            if (rdBt_saveFsArchiv->isChecked() && (state == Qt::Checked)) {
                extern int dialog_auswertung;
                dialog_auswertung = 2;
                folder_file();
                FileDialog Filedialog;
                FileDialog *dlg = new FileDialog;
                // dlg->show(); nicht modal
                int wert = dlg->exec();
                if (wert == 0 && dialog_auswertung == 2) {
                    QMessageBox::about(this,tr("Note", "Hinweis"),
                                       tr("The backup was aborted by the user\n", "Die Sicherung wurde vom Benutzer abgebrochen\n"));
                    pushButton_save->setEnabled(false);
                    return 0;
                }
            }
            this->repaint();
            thread1.setValues(indizierung + 2,0);
            pushButton_save->setEnabled(false);
            pushButton_end->setEnabled(false);
// 				timer->singleShot ( 1000, this , SLOT ( ViewProzent( ) ) ) ;
            startThread1(); // fsarchiver wird im Thread ausgeführt
            work_etap=c_part; // !!!
//        			statusBar()->showMessage(tr("The backup is performed", "Die Sicherung wird durchgeführt"), 15000);
            werte_reset();

// Fixme
            /*
                    			statusBar()->showMessage(tr("The backup is performed", "Die Sicherung wird durchgeführt"), 15000);
                                            werte_reset();
                                            QString text_integer;

                                            while (endeThread !=1)
                   					{
            //      					sleep(1);
            //                                       elapsedTime();
            //                                        int meldung = werte_holen(4);
            //					if (meldung >= 100) // Thread Abschluss mit Fehler
            //                  				break;
                                                    int anzahl  = werte_holen(2);
                                                    QString text_integer;
                                                    text_integer = text_integer.setNum(anzahl);
                                                    AnzahlsaveFile ->setText(text_integer);
                                                    int anzahl1  = werte_holen(3);
                                                    text_integer = text_integer.setNum(anzahl1);
                                                    AnzahlgesicherteFile ->setText(text_integer);
                                                    prozent = werte_holen(1);
                                                    remainingTime(prozent);
                                                    // bei mehrmaligem Aufruf von fsarchiver wird am Anfang der Sicherung kurzfristig 100 % angezeigt, was falsch ist
                                                    if (prozent != 100)  {
                                                        progressBar->setValue(prozent);
                                                        if (prozent >= 98 ) {
                      						progressBar->setValue(99);
                                                            	text_integer = text_integer.setNum(anzahl);
                                                            	AnzahlgesicherteFile ->setText(text_integer);
                                                            	SekundeRemaining ->setText("0");
                      						break;
                            					}
                                                           }

                   					}

            */
        }

    }
    return 0;

}

int MWindow::restorePartition() {
    Qt::CheckState state;
    Qt::CheckState state1;
    QFile file(folder);
    QString DateiName("") ;
    string part_art;
    char * part_;
    int err;
    int prozent;
    QString keyText = "";
    char * dev_part;
    char * optionkey;


    indicator_reset();
    if (rdBt_restoreFsArchiv->isChecked())
        string dateiname;
    int pos;
    keyText = lineKey->text();
    state1 = chk_key->checkState();
    {
        if (state1 == Qt::Checked && keyText.isEmpty()) {
            QMessageBox::about(this,tr("Note", "Hinweis"),
                               tr("There was no key to decrypt specified.", "Es wurde kein Schlüssel für die Entschlüsselung angegeben.\n"));
            return 1 ;
        }
        if (folder == "") {
            QMessageBox::about(this,tr("Note", "Hinweis"),
                               tr("Please select the file name of the backup.\n", "Bitte wählen Sie den Dateinamen der Sicherung aus.\n"));
            return 0;
        }
        if (partition_ == "") {
            QMessageBox::about(this,tr("Note","Hinweis"),
                               tr("Please select from the writing back to partition.\n", "Bitte wählen Sie die zurück zu schreibende Partition aus.\n"));
            return 0;
        }
        if (file.open(QIODevice::ReadOnly)) {
            if (rdBt_restoreFsArchiv->isChecked()) {
                pos = testDateiName("fsa");
                if (pos == 0) {
                    QMessageBox::about(this, tr("Note", "Hinweis"),
                                       tr("You have chosen the wrong recovery file selected. \nThe files should end with. fsa be", "Sie haben eine falsche Wiederherstellungsdatei ausgesucht ausgesucht \nDie Dateiendung muss .fsa sein"));
                    return 0;
                }
            }
        }

        else {
            QMessageBox::about(this,tr("Note", "Hinweis"),
                               tr("You have selected a directory. You must select a file\n", "Sie haben ein Verzeichnis ausgewählt. Sie müssen eine Datei auswählen\n"));
            return 0;
        }
        state = chk_Beschreibung->checkState();
        // Archinfo einholen um Originalpartition einzulesen und um Verschlüsselung zu überprüfen
        // Annahme zunächst kein Schlüssel

        parameter[0] = "fsarchiver";
        parameter[1] = "archinfo";
        if (state1 != Qt::Checked) {
            parameter[2] = folder;

            thread1.setValues(3,0);
            thread1.start();
            thread1.wait();


//                		fsarchiver_aufruf(3,parameter[0].toAscii().data(),parameter[1].toAscii().data(),parameter[2].toAscii().data(),parameter[3].toAscii().data(),parameter[4].toAscii().data (),parameter[5].toAscii().data(),parameter[6].toAscii().data(),parameter[7].toAscii().data(),parameter[8].toAscii().data(),parameter[9].toAscii().data(),parameter[10].toAscii().data(),parameter[11].toAscii().data(),parameter[12].toAscii().data(),parameter[13].toAscii().data(),parameter[14].toAscii().data());
// // fixme

            optionkey = meldungen_holen(1);
            dev_part = meldungen_holen(2);
            if (werte_holen(4) == 103) {
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

            thread1.setValues(4,0);
            thread1.start();
            thread1.wait();

//                       int retour =
// fixme
// fsarchiver_aufruf(5,parameter[0].toAscii().data(),parameter[1].toAscii().data(),parameter[2].toAscii().data(),parameter[3].toAscii().data(),parameter[4].toAscii().data (),parameter[5].toAscii().data(),parameter[6].toAscii().data(),parameter[7].toAscii().data(),parameter[8].toAscii().data(),parameter[9].toAscii().data(),parameter[10].toAscii().data(),parameter[11].toAscii().data(),parameter[12].toAscii().data(),parameter[13].toAscii().data(),parameter[14].toAscii().data());
// fixme

            int retour = work_resul;
            if ( werte_holen(4) == 103 && retour != 0) {
                QMessageBox::about(this, tr("Note", "Hinweis"), tr("You have entered an incorrect password.", "Sie haben ein falsches Passwort eingegeben. \n"));
                lineKey->setText ("");
                return 0;
            }
            optionkey = meldungen_holen(1);
            dev_part = meldungen_holen(2);

        }
        //Überprüfen, ob in die Originalpartition zurückgeschrieben werden soll
        part_ = partition_.toAscii().data();
        char  dev_[50] = "/dev/";
        strcat (dev_ , part_);  // String zusammenfügen: dev_  = dev_ + part_
        // Hier wird verglichen dev_ = die Partition, die zu sichern ist. dev_part = Originalpartition
        int cmp = strcmp ( dev_ , dev_part );
        if (cmp != 0) {
            // char in QString wandeln
            QString str, str1;
            str = dev_part;
            str1 = dev_;
            cmp = questionMessage(tr("The partition to restory","Die wiederherzustellende Partition ")
                                  + str1 + tr("  does not coincide with the saved " ,
                                              " stimmt nicht mit der gesicherten ") + str +
                                  tr("Do you want to continue restore?", " überein. Wollen Sie trotzdem die Wiederherstellung durchführen?"));
            if (cmp == 2)  //nicht wiederherstellen
                return 0;
        }
        if (rdBt_restoreFsArchiv->isChecked()) {
            extern QString folder_file_;
            folder_file();
            QString filename = folder_file_;
            pos = filename.indexOf("fsa");
            filename = filename.left(pos);
            filename.insert(pos, QString("txt"));
            QFile f(filename);
            // Prüfen ob text-Datei vorhanden
            if (f.exists()) {
                extern int dialog_auswertung;
                dialog_auswertung = 3;
                FileDialog Filedialog;
                FileDialog *dlg = new FileDialog;
                // dlg->show(); nicht modal
                int wert = dlg->exec();
                if (wert == 0 && dialog_auswertung == 3) {
                    QMessageBox::about(this, tr("Note", "Hinweis"),
                                       tr("The restore was canceled by user\n",
                                          "Das Zurückschreiben wurde vom Benutzer abgebrochen\n"));
                    pushButton_restore->setEnabled(false);
                    return 0;
                }
            }
        }
        if (is_mounted(dev_)) {
            part_art = mtab_einlesen(("/dev/" + partition_).toAscii().data());
            //Überprüfung ob System oder Home Partition
            if (part_art == "system") {
                QMessageBox::about(this, tr("Note", "Hinweis"),
                                   tr("To restore system partition is mounted and can not be restored. Please use a live CD\n", "Die wiederherzustellende Systempartition ist eingehängt und kann nicht zurückgeschrieben werden. Benutzen Sie bitte eine Live-CD\n"));
                return 0;
            }

            if (part_art == "home") {
                QMessageBox::about(this, tr("Note", "Hinweis"),
                                   tr("The restored home partition is mounted and can not be restored. Please use a live CD\n",
                                      "Die wiederherzustellende Homepartition ist eingehängt und kann nicht zurückgeschrieben werden. Benutzen Sie bitte eine Live-CD\n"));
                return 0;

            }
            if (part_art != "system"|| part_art != "home") { // fixme
                char umountpoint[100] = "umount ";
                strcat (umountpoint,dev_);
                err = system (umountpoint);
                if (err != 0) {
                    QMessageBox::about(this, tr("Note", "Hinweis"),
                                       tr("The partition", "Die Partition ")   + partition_ +
                                       tr("can not be unmounted. The program is terminated\n", " kann nicht ausgehängt werden. Das Programm wird abgebrochen\n"));
                    return 0 ;

                }
            }

        }
	int ind=0;
        if (rdBt_restoreFsArchiv->isChecked()) {
            QString keyText = lineKey->text();
            state1 = chk_key->checkState();
            parameter[ind++] = "fsarchiver";
            parameter[ind++] = "restfs";
            parameter[ind++] = "-d";
//            parameter[3] = "-j" + index;

            if (state1 == Qt::Checked) { // Verzeichnis mit Schlüssel gesichert
	        parameter[ind++] = "-c";
       		parameter[ind++] = keyText;
                int len = parameter[4].size();
                if (len < 6 || len > 64) {
                    QMessageBox::about(this,tr("Note","Hinweis"),
                                       tr("Die Schlüssellänge muss zwischen 6 und 64 Zeichen sein\n"));
                    return 0 ;
                }

                parameter[ind++] = folder;
                parameter[ind++] = "id=0,dest=/dev/" + partition_;; 
            }

            int kerne = cmb_kerne->currentIndex()+1;
            QString index = QString::number(kerne);
            parameter[ind++] = folder;
            parameter[ind++] = "id=0,dest=/dev/" + partition_;
		
            pushButton_restore->setEnabled(false);
            pushButton_end->setEnabled(false);
		statusBar()->showMessage(tr("The restoring is performed", 
		"Die Wiederherstellung wird durchgeführt"), 15000);
//            timer->singleShot ( 1000, this , SLOT ( ViewProzent( ) ) ) ;
            thread2.setValues(ind,0);
            startThread2(); // fsarchiver wird im Thread ausgeführt
	    work_etap=c_part_z;
            werte_reset(); //            prozent = 0
           
           
            /*               while (endeThread != 1)
                   		  {
                   		  sleep(1);
                              int meldung = werte_holen(4);
                              if (meldung >= 100) // Thread Abschluss mit Fehler
                                break;
                              elapsedTime();
                              prozent = werte_holen(1);
                              remainingTime(prozent);
                              progressBar->setValue(prozent);
                              if (prozent >= 98 ) {
                      		progressBar->setValue(99);
                                    SekundeRemaining ->setText("0");
                      		break;
                            	}
                   		   }
            */
        } // rdBt_restoreFsArchiv
    }
    return 0;
}

QString MWindow::Zeit_auslesen() {
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
    _Datum = Datum_akt.setNum(Tag) + "-" ;
    _Datum = _Datum + Datum_akt.setNum(Monat) + "-" ;
    _Datum = _Datum + Datum_akt.setNum(Jahr) ;
    return _Datum;

}

void MWindow::breakProcess() {

}

void MWindow::ViewProzent_save() {
    static float prozentf=0;
    static int prozent=0;
    QString sekunde;
    int sekunde_=0;
    static int dsecunde=0;
    QString minute;
    int minute_=0;
    int meldung=0;
    static int anzahl=0;
    static int anzahl1=0;
    QString text_integer;
    static int ncicl=0, scicl=0;
    static bool p_rem=false;
    static int dummy_time=0;
    int full_time=0;
    static bool sgl=false;

    ncicl++;
    if (work_global==1) {
        timer->singleShot( 1000, this , SLOT(ViewProzent( )) ) ;
        elapsedTime();
        meldung = werte_holen(4);
        if (meldung >= 100) // Thread Abschluss mit Fehler
            return;

        if (ncicl>6 && !flag_View) {
            flag_View=1;
        }
 
	if ((anzahl1 >= 1) && !p_rem) {
            p_rem=true;
            scicl=ncicl;
        } 	   
	
        if (flag_View == 1) {
            anzahl  = werte_holen(2);
            text_integer = text_integer.setNum(anzahl);
            AnzahlsaveFile ->setText(text_integer);
            anzahl1  = werte_holen(3);
            text_integer = text_integer.setNum(anzahl1);
            AnzahlgesicherteFile ->setText(text_integer);
        }
        prozent = prozentf = werte_holen(1);
        if (prozent>1) {
            full_time=sekunde_= ((ncicl-scicl)*100.)/(prozentf+0.01);
            if (dummy_time!=0) {
                float k_sgl;
                k_sgl= abs(full_time-dummy_time)/full_time;
                if (k_sgl<=0.25)
                    sgl=true;
            }
            dummy_time=full_time;
        }

        if (dummy_prozent != prozent) {
            if (prozent > 5 && !p_rem)
                p_rem=true;

            if (p_rem && sgl) {
                if (  (0.1 <= prozentf) && (prozentf <= 99.99) ) 
                      {
                    dsecunde=sekunde_=(ncicl-scicl)*100.)/(prozentf+0.01)-scicl;
                    minute_=sekunde_/60;
                    minute = QString::number(minute_);
                    MinuteRemaining ->setText(minute);
                    sekunde_=sekunde_%60;
                }
                sekunde = QString::number(sekunde_);
                SekundeRemaining ->setText(sekunde);
            }

            dummy_prozent = prozent;
        } else { // dummy_prozent == prozent
            if ( p_rem && sgl && (dsecunde>1) ) {
                if (  (0.1 <= prozentf) && (prozentf <= 99.99)  ) {
                    if  (  ( dsecunde > 10 && prozentf < 5. )  || dsecunde > 1  ) {
                        sekunde_= --dsecunde;
                        minute_=sekunde_/60;
                        minute = QString::number(minute_);
                        MinuteRemaining ->setText(minute);
                        sekunde_=sekunde_%60;
                    }

                }
                sekunde = QString::number(sekunde_);
                SekundeRemaining ->setText(sekunde);
            }


        }

// bei mehrmaligem Aufruf von fsarchiver wird am Anfang der Sicherung 
// kurzfristig 100 % angezeigt, was falsch ist
        if (prozent != 100)  {
            progressBar->setValue(prozent);
            if (prozentf >= 99.99 ) {
                progressBar->setValue(99);
                text_integer = text_integer.setNum(anzahl);
                AnzahlgesicherteFile ->setText(text_integer);
//                SekundeRemaining ->setText("0");
                return;
            }else
		{  // prozent = 100
                    SekundeRemaining ->setText("0");
		}
        }
        return;
    } //   = 1
    else {
        prozent=0;
        sekunde_=0;
        minute_=0;
        meldung=0;
        anzahl=0;
        anzahl1=0;
        ncicl=0;
	scicl=0;
        p_rem=false;
        dsecunde=0;
        dummy_time=0;
        sgl=false;
        werte_reset();
    }
}

void MWindow::ViewProzent() {
switch (work_etap){
case c_part:	
	ViewProzent_save();
	break;	
case c_part_z:	
	ViewProzent_restore();
	break;
default:
	fprintf(stderr, "Error etap\n");
}

}

void MWindow::ViewProzent_restore() {
    static float prozentf=0;
    static int prozent=0;
    QString sekunde;
    int sekunde_=0;
    static int dsecunde=0;
    QString minute;
    int minute_=0;
 //   int meldung=0;
    static int anzahl=0;
    static int anzahl1=0;
    QString text_integer;
    static int ncicl=0;
    static bool p_rem=false;
    static int dummy_time=0;
    int full_time=0;
    static bool sgl=false;

    ncicl++;
    if (work_global==1) {
        timer->singleShot( 1000, this , SLOT(ViewProzent( )) ) ;
        elapsedTime();
	
        if (ncicl>6 && !flag_View) {
            flag_View=1;
        }

        prozent = prozentf = werte_holen(1);
        if (prozent>1) {
            if (dummy_time!=0) {
                float k_sgl;
                k_sgl= abs(full_time-dummy_time)/full_time;
                if (k_sgl<=0.25)
                    sgl=true;
            }
              dummy_time=full_time;
        }

        if (dummy_prozent != prozent) {
            if (prozent > 5 && !p_rem)
                p_rem=true;

            if (p_rem && sgl) {
                if (  (0.1 <= prozentf) && (prozentf <= 99.99) ) {
	     	    remainingTime(prozent);
	    	    dsecunde=sekunde_=secunde;
                    minute_=sekunde_/60;
                    minute = QString::number(minute_);
                    MinuteRemaining ->setText(minute);
                    sekunde_=sekunde_%60;
                }
                sekunde = QString::number(sekunde_);
                SekundeRemaining ->setText(sekunde);
            }

            dummy_prozent = prozent;
        } else { // dummy_prozent == prozent
            if ( p_rem && sgl && (dsecunde>1) ) {
                if (  (0.1 <= prozentf) && (prozentf <= 99.99)  ) {
                    if  (  ( dsecunde > 10 && prozentf < 5. )  || dsecunde > 1  ) {
                        sekunde_= --dsecunde;
                        minute_=sekunde_/60;
                        minute = QString::number(minute_);
                        MinuteRemaining ->setText(minute);
                        sekunde_=sekunde_%60;
                    }

                }
                sekunde = QString::number(sekunde_);
                SekundeRemaining ->setText(sekunde);
            }


        }

// bei mehrmaligem Aufruf von fsarchiver wird am Anfang der Sicherung 
// kurzfristig 100 % angezeigt, was falsch ist
        if (prozent != 100)  {
            progressBar->setValue(prozent);
//            if (prozentf >= 99.99 ) {
//                progressBar->setValue(99);
//                text_integer = text_integer.setNum(anzahl);
//                AnzahlgesicherteFile ->setText(text_integer);
//                SekundeRemaining ->setText("0");
                return;
            }else
		{ 
                    SekundeRemaining ->setText("0");
		}
        }
        return;
    } //   = 1
    else {
        prozent=0;
        sekunde_=0;
        minute_=0;
//        meldung=0;
        anzahl=0;
        anzahl1=0;
        ncicl=0;
        p_rem=false;
        dsecunde=0;
        dummy_time=0;
        sgl=false;
        werte_reset();
    }
}



void MWindow::listWidget_auslesen() {
    int row;
    QStringList partition_kurz;
    row = listWidget->currentRow();
    partition_ = widget[row];
    partition_kurz = partition_.split(" ");
    partition_ = partition_kurz[0];
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
        0, tr("qt4-fsarchiver"),
        tr("Backup and restore\n"
           "partitions, directory and MBR\n"
           "Copyright (C) 2008-2011 Francois Dupoux, Hihin Ruslan, Dieter Baum.\n"
           "All rights reserved.\n"
           "Version 0.6.12-7, May 31, 2011",

           "Sichern und Wiederherstellen\n"
           "von Partitionen, Verzeichnissen und MBR\n"
           "Copyright (C) 2008-2011 Francois Dupoux, Hihin Ruslan, Dieter Baum.\n"
           "All rights reserved.\n"
           "Version 0.6.12-7, 31. Mai 2011"));
}

int MWindow::Root_Auswertung() {
    if (geteuid() != 0) { // 0 is the UID of the root  1000 von Dieter
        QMessageBox::about(this, tr("Note", "Hinweis"),
                           tr("The program must be run with root privileges. The program is terminated.\n", "Das Programm muss mit Root-Rechten gestartet werden. Das Programm wird beendet.\n"));
        return 10;
    } else {
        return 20;
    }
}

int MWindow::testDateiName(string endung) {
    string str (folder.toAscii().data());
    string str2;
    size_t found;
    // different member versions of find in the same order as above:
    found=str.find(endung);
    if (found!=string::npos) {
        return found;
    } else {
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
                    && ( strcmp ( part->mnt_fsname, dev_path ) ) == 0 ) {
                is_mounted = 1;
            }
        }
        endmntent (mtab);
    }
    return is_mounted;
}

int MWindow::questionMessage(QString frage) {
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

void MWindow::einstellungen () {
    DialogSetting *dialog1 = new DialogSetting;
    dialog1->show();
}

void MWindow::eingabe_net_save () {
    NetEin netein;
    extern int dialog_auswertung;
    int i = netein.list_net("1");
    if ( i != 1) {
        dialog_auswertung = 6;
        NetEin *dialog3 = new NetEin;
        dialog3->show();
    }
}

void MWindow::eingabe_net_restore () {
    NetEin netein;
    extern int dialog_auswertung;
    int i = netein.list_net("1");
    if ( i != 1) {
        dialog_auswertung = 7;
        NetEin *dialog3 = new NetEin;
        dialog3->show();
    }
}

void MWindow::format() {
    extern QString add_part[100];
    QString teilstring, space1, space2;
    int aa, bb, i,j,k;
    for (i=0; i< 100; i++) {
        if (part[i][0] != "") {
            teilstring = part[i][0];
            aa = teilstring.size();
            j = 0;
            for (k=4; k < 11; k++) {
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
            if (part[i][1] != "" ) {
                widget[i] = teilstring;
                listWidget->addItem (teilstring);
                add_part[i] = teilstring;
            }
        }
    }
}

void MWindow::startThread1() {
    if ( thread1.isRunning() ) return;
    connect( &thread1, SIGNAL(finished()),
             this, SLOT( thread1Ready()));
    timer->singleShot ( 1000, this , SLOT ( ViewProzent( ) ) ) ;
    thread_run = 1;
    thread1.start();

}

void MWindow::startThread2() {
    if ( thread2.isRunning() ) return;
    connect( &thread2, SIGNAL(finished()),
             this, SLOT( thread2Ready()));
    thread_run = 1;
    timer->singleShot ( 1000, this , SLOT ( ViewProzent( ) ) ) ;
    thread2.start();
}


void MWindow::closeEvent(QCloseEvent *event) {
    thread1.wait();
    thread2.wait();
    event->accept();
}

void MWindow::thread1Ready()  {
//   endeThread = endeThread + 1;
    extern int t_dialog_auswertung;
    if (work_global == 0) {

        pushButton_end->setEnabled(true);
        if (t_dialog_auswertung ==0) {
            pushButton_save->setEnabled(true);
            progressBar->setValue(100);
            int cnt_regfile = werte_holen(6);
            QString cnt_regfile_ = QString::number(cnt_regfile);
            int cnt_dir = werte_holen(7);
            QString cnt_dir_ = QString::number(cnt_dir);
            int cnt_hardlinks = werte_holen(8);
            cnt_hardlinks = cnt_hardlinks + werte_holen(9);
            QString cnt_hardlinks_ = QString::number(cnt_hardlinks);
            QMessageBox::about(this, tr("Note", "Hinweis"), tr("The partition has been backed up successfully.\n", "Die Partition wurde erfolgreich gesichert.\n") + cnt_regfile_ +
                               tr(" files, ", " Dateien, ") + cnt_dir_ + tr("  directories and ", " Verzeichnisse und ") + cnt_hardlinks_ + tr(" links have been backed.", " Links wurden gesichert."));
        } else {
            pushButton_save->setEnabled(false);
            //Beschreibungsdate löschen
            QString filename = SicherungsDateiName;
            int pos = filename.indexOf("fsa");
            filename = filename.left(pos);
            filename.insert(pos, QString("txt"));
            QFile f(filename);
            // Prüfen ob text-Datei vorhanden
            if (f.exists())
                remove (filename.toAscii().data());
            // Anzahl nicht korrekt gesicherte Dateien ausgeben
            int part_testen = werte_holen(4);
            if (part_testen == 108) {
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
            if (part_testen != 108) {
                QMessageBox::about(this, tr("Note", "Hinweis"),
                                   err_regfile_ + tr(" files, ", " Dateien, ")   + err_dir_ + tr("  directories and ", " Verzeichnisse und ") + err_hardlinks_ + tr(" links were not properly backed. The backup of the partition was only partially successful.\n", " Links wurden nicht korrekt gesichert. Die Sicherung der Partition war nur teilweise erfolgreich.\n") );
            }
        }
        folder_einlesen();
        thread1.exit();

    }

}

void MWindow::thread2Ready()  {
//   endeThread = endeThread + 1;
    extern int t_dialog_auswertung;
    int meldung = werte_holen(4);
    if (endeThread == 1) {
        pushButton_end->setEnabled(true);
        if (t_dialog_auswertung ==0) {
            pushButton_restore->setEnabled(true);
            progressBar->setValue(100);
            int cnt_regfile = werte_holen(6);
            QString cnt_regfile_ = QString::number(cnt_regfile);
            int cnt_dir = werte_holen(7);
            QString cnt_dir_ = QString::number(cnt_dir);
            int cnt_hardlinks = werte_holen(8);
            cnt_hardlinks = cnt_hardlinks + werte_holen(9);
            QString cnt_hardlinks_ = QString::number(cnt_hardlinks);
            QMessageBox::about(this, tr("Note", "Hinweis"), tr("The partition is successful back.\n", "Die Partition wurde erfolgreich wieder hergestellt.\n") + cnt_regfile_ + tr(" files, ", " Dateien, ") + cnt_dir_ + tr("  directories and ", " Verzeichnisse und ") + cnt_hardlinks_ + tr(" links have been restored.", " Links wurden wieder hergestellt."));
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
                               err_regfile_ + tr(" files, ", " Dateien, ")    + err_dir_ + tr(" directories and ",  " Verzeichnisse und ") + err_hardlinks_ + tr("  Links were not correctly restored. The recovery of the volume was only partially successful.", " Links wurden nicht korrekt wiederhergestellt. Die Wiederherstellung der Partition war nur teilweise erfolgreich.\n") );
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
    folder_einlesen();
    thread2.exit();
}

void MWindow::elapsedTime() {
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

void MWindow::remainingTime(int prozent) {
    // int sekunde_tara;
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
}

void MWindow::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    switch ( event->key() ) {
    case Qt::Key_Escape:
        int ret = questionMessage(tr("Do you want to exit the program really?", "Wollen Sie das Programm wirklich beenden?"));
        if (ret == 1)
            close();
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
    QString Ubuntuversion;
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
    prozent = freesize(partitiontext.toAscii().data(), mount_point.toAscii().data(), 1);
    // freier Bereich der Partition
    free_part_size = freesize(partitiontext.toAscii().data(), mount_point.toAscii().data(), 3);
    free_part_size_ =  format(free_part_size);
    // Gesamter Bereich der Partition
    part_size = freesize(partitiontext.toAscii().data(), mount_point.toAscii().data(), 2);
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

    if (part_art == "system") {
        Ubuntuversion = ubuntu_version();
        kernel = kernel_version();
        text = ubuntu_root + "\n" + tr("Operating system: ", "Betriebsystem: ") + Ubuntuversion + "\n" + tr("Kernel: ") + kernel + " " + "\n" + text + "\n";
    }
    ubuntu_home = tr("to be protected/secured partition: /home", "zu sichernde/gesicherte Partition: /home");
    if (part_art == "home") {
        Ubuntuversion = ubuntu_version();
        kernel = kernel_version();
        text = ubuntu_home + "\n" + tr("Operating system: ", "Betriebsystem: ")  + Ubuntuversion + " " + "\n"  + tr("Kernel: ") + kernel  + "\n" + text + "\n";
    }
    filedialog.werte_uebergeben(text);
    return text;
}

QString MWindow::format(float zahl)  {
    QString zahl_;
    int found;
    if (zahl <= 999) {
        zahl = (int) (zahl * 100/1.024);
        zahl = zahl/100;
        // Punkt in Komma wandeln
        zahl_ = QString::number(zahl);
        found=zahl_.indexOf(".");
        if (found > 0)
            zahl_.replace(found, 1, ",");
        return zahl_  + tr(" MB");
    }
    if (zahl >= 1000000) {
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
    if (zahl >= 1000) {
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

QString MWindow::ubuntu_version()  {
    QString homepath = QDir::homePath();
    QString befehl;
    QString Ubuntuversion;
    int i;
    int pos;
    befehl = "cat /etc/*release 1> " +  homepath + "/.config/qt4-fsarchiver/version.txt";
    system (befehl.toAscii().data());
    QString filename = homepath + "/.config/qt4-fsarchiver/version.txt";
    QFile file(filename);
    if ( file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QTextStream ds(&file);
        for (i=1; i < 5; i++) {
            Ubuntuversion = ds.readLine(); // Simply Linux
        }
    }
    file.close();
    befehl = "rm " + filename;
    system (befehl.toAscii().data());
    if (Ubuntuversion.indexOf("DISTRIB_DESCRIPTION=") > 0);
    // Ubuntu, Debian
    Ubuntuversion = Ubuntuversion.right(Ubuntuversion.size() -20);
    return Ubuntuversion;
}


QString MWindow::kernel_version()  {
    QString homepath = QDir::homePath();
    QStringList kernel;
    QString kernel_;
    QString befehl;
    befehl = "uname -a 1> " +  homepath + "/.config/qt4-fsarchiver/kernel.txt";
    system (befehl.toAscii().data());
    QString filename = homepath + "/.config/qt4-fsarchiver/kernel.txt";
    QFile file(filename);
    if ( file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QTextStream ds(&file);
        kernel_ = ds.readLine();
        file.close();
        kernel = kernel_.split(" ");
    }
    befehl = "rm " + filename;
//qDebug() << "befehl" << befehl;
    system (befehl.toAscii().data());
    return kernel[0] + " " + kernel[2] +  " " + kernel[11];
}

string MWindow::mtab_einlesen(string partition_if_home) {
    QString fileName;
    QString text;
    QStringList items;
    QString mount_dir;
    QFile file("/etc/mtab");
    QTextStream ds(&file);
    string text_str;
    int pos, pos1,i,j;
    int line = 0;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        text = ds.readLine();
        while (!ds.atEnd()) {
            text_str = text.toAscii().data();
            pos = text_str.find(partition_if_home);
            if (pos > -1) { // Partitionsname gefunden
                items = text.split(" ");  //Einhängepunkt ermitteln
                mount_dir = items[1];
                i = items[0].size();  //Überprüfen ob sda1 nicht mit sda10 verwechselt wird
                j =  partition_if_home.size();
                if (i == j) {
                    pos1 = text_str.find(" /home");
                    if (pos1 > -1)
                        return "home";
                    pos1 = text_str.find("/ ");
                    if (pos1 > -1)
                        return "system";
                    pos1 = text_str.find("ext4");
                    if (pos1 > -1)
                        return "ext4";
                    pos1 = text_str.find("ext3");
                    if (pos1 > -1)
                        return "ext3";
//  /usr /var ant etc fixme
                }
            }
            if ( text.isEmpty() )
                break;
            line++;
            text = ds.readLine();
        }
        file.close();
    }
    return mount_dir.toAscii().data();
}


//Mountpoint ermitteln
QString MWindow::mountpoint(QString partition) {
    QString fileName;
    QString text;
    QStringList items;
    QString mount_dir;
    QFile file("/etc/mtab");
    QTextStream ds(&file);
    string text_str;
    int i,j,pos;
    int line = 0;
    text_str = partition.toAscii().data();
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        text = ds.readLine();
        // hier wird die ersten Zeile der mtab überprüft
        pos = text.indexOf(partition);
        if (pos > -1) { // Partitionsname gefunden
            items = text.split(" ");  //Einhängepunkt ermitteln
            i = items[0].size();
            j =  text_str.size();
            if (i == j)
                mount_dir = items[1];
        }
        while (!ds.atEnd()) {
            // hier wird auch die letzte Zeile der mtab überprüft
            line++;
            text = ds.readLine();
            pos = text.indexOf(partition);
            if (pos > -1) { // Partitionsname gefunden
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


// void MWindow::esc_end()
// {
// QString befehl;
// QStrinf  SicherungsFolderFileName /*=?*/;   // fixme
//    if (thread_run > 0) {
//     int ret = questionMessage(tr("Do you really want to break the save or restore from the partition?", "Wollen Sie wirklich die Sicherung oder Wiederherstellung der Partition beenden?"));
//       if (thread_run == 1)
//         {
//      	befehl = "rm "  + SicherungsFolderFileName;  // name of the save/restore partitionfile
//         system (befehl.toAscii().data());
//         int pos = SicherungsFolderFileName.indexOf("fsa");
//        	SicherungsFolderFileName = SicherungsFolderFileName.left(pos);
//        	SicherungsFolderFileName.insert(pos, QString("txt"));
//         befehl = "rm "  + SicherungsFolderFileName;
//         system (befehl.toAscii().data());
//         }
// // No ! Fixme
// // 1 we also not want to just go out, we only want to interrupt the process of saving / restoring.
// // 2. We must not use -9, but something softer, like SIGUSR1, SIGTERM
// // 3. We must look for the thread control code strings in the code *. c.
// // Perhaps there is already a thread stops. If not, then we must change the code in C
// // (into thread)
//
//      befehl = "kill -9 " + pid;  //fsarchiver abbrechen
//      system (befehl.toAscii().data());
//      befehl = "kill -9 " + pid1;  //fsarchiver abbrechen
//      system (befehl.toAscii().data());
//      }
//    close();
// }

