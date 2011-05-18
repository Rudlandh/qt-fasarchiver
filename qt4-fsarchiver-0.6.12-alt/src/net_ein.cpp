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

#include <QtGui> 
#include "net_ein.h"
#include "net.h"
#include "mainWindow.h"
#include <iostream>

QString widget_net[100]; // Netzwerk
QString comNet;
QString comNet_name;
QString user;
QString key;


NetEin::NetEin(QWidget *parent)
{
QString homepath = QDir::homePath(); 
int i = 0;
setupUi(this);
connect( pushButton_net, SIGNAL( clicked() ), this, SLOT(listWidget_show()));
connect( pushButton_go, SIGNAL( clicked() ), this, SLOT(go()));
connect( pushButton_end, SIGNAL( clicked() ), this, SLOT(end()));
connect( chk_password, SIGNAL( clicked() ), this, SLOT(Kennwort()));

// Ini-Datei auslesen
   QFile file(homepath + "/.config/qt4-fsarchiver/qt4-fsarchiver.conf");
   if (file.exists()) {
        QSettings setting("qt4-fsarchiver", "qt4-fsarchiver");
        setting.beginGroup("Basiseinstellungen");
        int auswertung = setting.value("Passwort").toInt();
        if (auswertung ==1){
           	txt_key ->setEchoMode(QLineEdit::Normal);
                chk_password->setChecked(Qt::Checked);
        } 
   	else
		txt_key ->setEchoMode(QLineEdit::Password);
        auswertung = setting.value("save").toInt();
        if (auswertung ==1)
           chk_datesave->setChecked(Qt::Checked);      
        setting.endGroup();
   }
    while (widget_net[i] != ""){
    	listWidget_net->addItem (widget_net[i]);
        i++;
        }
}

void NetEin::Kennwort()
{
Qt::CheckState state;
state = chk_password->checkState(); 
	if (state != Qt::Checked) {
		txt_key ->setEchoMode(QLineEdit::Password);
        } 
   	else
		txt_key ->setEchoMode(QLineEdit::Normal);
}

int NetEin:: list_net(QString flag)
{
int pos = 0;
int pos1 = 0;
int i = 0;
int j;
int m;
QString befehl;
QString homepath = QDir::homePath(); 
QString hostname_;
        hostname_ = hostname();
//Auswertung findsmbs Windows-Rechner werden nichterkannt
// -N verhindert die sudo-Abfrage
        befehl = "smbtree -N 1> " +  homepath + "/.config/qt4-fsarchiver/smbtree.txt";
	system (befehl.toAscii().data()); 
        QFile file(homepath + "/.config/qt4-fsarchiver/smbtree.txt");
    	QTextStream ds(&file);
        QString text = ds.readLine();
        if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
     	     text = ds.readLine();
   	     while (!ds.atEnd()) {
             	text = ds.readLine();
                text = text.toLower(); 
                pos = text.indexOf("qt4-fs");
                pos1 = text.indexOf(hostname_);
                text = text.left(pos-1);
                text = text.trimmed();
                j = text.size();
		text = text.right(j-2);
                if (pos > -1 && pos1 == -1) {
                   text = IP(text);
                   listWidget_net->addItem (text);
                   widget_net[i]= text;
                   i++;
                }
             } 
        } 
	file.close();

//Auswertung findsmbs Windows-Rechner werden nichterkannt
        QStringList adresse_;
        befehl = "findsmb 1> " +  homepath + "/.config/qt4-fsarchiver/findsmb.txt";
	system (befehl.toAscii().data()); 
        QFile file1(homepath + "/.config/qt4-fsarchiver/findsmb.txt");
    	QTextStream ds1(&file1);
        QString adresse = ds1.readLine();
        QString adresse1; 
       	int k;
        if (file1.open(QIODevice::ReadWrite | QIODevice::Text)) {
            for (k = 0; k < 5; k++){
               	adresse = ds1.readLine();
             }
   	     while (!ds1.atEnd()) {
             	adresse = ds1.readLine();
                adresse = adresse.toLower();
                k = adresse.size();
                adresse_ = adresse.split(" ");
                j = adresse_[0].size();
		adresse = adresse.right(k-j);
                adresse = adresse.trimmed();
                adresse_ = adresse.split(" ");
                adresse = adresse_[0];
                adresse = IP(adresse);
		// Prüfung ob adresse im Array widget_net schon vorhanden ist
                k = Array_pruefen(adresse);
                if (k == 2) {
         	     listWidget_net->addItem (adresse);
                     widget_net[i]= adresse;
                     i++;
                }
             } 
        }
	file1.close();
        if (i == 0 && flag == "1"){
   		QMessageBox::about(this,tr("Hinweis"),
      		tr("Es ist derzeit kein Netzwerkrechner erreichbar.\n"));
        	return 1;
   }
   return 0;
}

int NetEin::Array_pruefen(QString ip){
int k= 0;
int pos;
int pos1;
QString hostname_;
        hostname_ = hostname();
        pos = widget_net[k].indexOf(ip);
        pos1 = ip.indexOf(hostname_);
        if (pos >= 0)  // Name im Array vorhanden
               return 0;
        if (pos1 >= 0)  // Hostname, Abbruch
               return 0;
	while (widget_net[k] != "") {
            pos = widget_net[k].indexOf(ip);
            pos1 = ip.indexOf(hostname_);
            if (pos >= 0)  // Name im Array vorhanden
               return 0;
            if (pos1 >= 0)  // Hostname, Abbruch
               return 0;
            k++;
            }
        return 2;
}

int NetEin:: setting_save()
{
   extern int dialog_auswertung;   
   QString befehl;
   QString text;
   QString filename;
   QFile f(filename);
   QString homepath = QDir::homePath(); 
   Qt::CheckState state;
   state = chk_datesave->checkState();
   QSettings setting("qt4-fsarchiver", "qt4-fsarchiver");
   setting.beginGroup(comNet_name);
   user = txt_user->text();
   key = txt_key->text();
   //Neue oder geänderte Daten in setting eingeben
    if (state == Qt::Checked && comNet != "")   
       {
       setting.setValue("Name",user);
       // Key verschlüsseln und in setting eingeben
       setting.setValue("key",crypt(key));
      }
   setting.endGroup();
   
   //Netzwerkrechner mounten auf /mnt/qt4-fs-client
   befehl = "sudo mount -t cifs -o username=" + user + ",password=" + key + ",uid=0,gid=0 //" + comNet + "/qt4-fs /mnt/qt4-fs-client" ;
   //qDebug() << "befehl" << befehl;
   int k = system (befehl.toAscii().data());
   if (k == 0 && dialog_auswertung == 6){
   	QMessageBox::about(this, tr("Hinweis"),
      	tr("Der Netzrechner ") + comNet_name + tr(" wurde erfolgreich eingebunden. Sie können mit der Sicherung fortfahren\n"));
       }
   if (k == 0 && dialog_auswertung == 7){
   	QMessageBox::about(this, tr("Hinweis"),
      	tr("Der Netzwerkrechner ") + comNet_name + tr(" wurde erfolgreich eingebunden. Sie können mit dem Zurückschreiben fortfahren\n"));
      } 
   if (k != 0){
   	QMessageBox::about(this, tr("Hinweis"),
      	tr("Der Netzwerkrechner ") + comNet_name + tr(" konnte nicht eingebunden werden. Das Programm wird abgebrochen\n"));
        return 1;
   }
  // Dateien entfernen 
  filename = homepath + "/.config/qt4-fsarchiver/ip.txt";
	if (f.exists()){
     		befehl = "rm filename";
		system (befehl.toAscii().data());
       }     
       filename = homepath + "/.config/qt4-fsarchiver/smbtree.txt";
       if (f.exists()){
     		befehl = "rm filename";
		system (befehl.toAscii().data());
       } 
   return 0;
} 


QString NetEin::hostname()
{
QString befehl;
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

QString NetEin:: IP(QString adresse)
{
QString befehl;
int pos;
QString homepath = QDir::homePath(); 
	befehl = "nmblookup " + adresse + " 1> " +  homepath + "/.config/qt4-fsarchiver/ip.txt";
	system (befehl.toAscii().data()); 
        // IP-Adresse auslesen
	QFile file(homepath + "/.config/qt4-fsarchiver/ip.txt");
    	QTextStream ds(&file);
        QString text;
        if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
     	     text = ds.readLine();
             text = ds.readLine();
             file.close();
        }
     pos = text.indexOf("<");
     text = text.left(pos);
     text = text.toLower();
     return text;
}

void NetEin:: listWidget_show()
{
extern int dialog_auswertung;
int row = 1;
QString key_;
QStringList comNet_;
    row = listWidget_net->currentRow();
    comNet = widget_net[row];
    comNet_ = comNet.split(" ");
    comNet = comNet_[0];
    comNet_name = comNet_[1];
    // Prüfen ob Daten in setting
   QSettings setting("qt4-fsarchiver", "qt4-fsarchiver");
   setting.beginGroup(comNet_name);
   user = setting.value("Name").toString();
   key_ = setting.value("key").toString();
 //key entschlüsseln 
   key = decrypt(key_); 
   setting.endGroup();
   //Netzwerk-Daten in Textfeld eintragen  
   txt_user ->setText(user);
   txt_key ->setText(key);
}

QString NetEin::Namen_holen()
{
  return comNet + " " + comNet_name;
}

int NetEin:: end()
{ 
   close();
}

int NetEin:: go()
{ 
extern int dialog_auswertung;
Qt::CheckState state;
state = chk_datesave->checkState();
QSettings setting("qt4-fsarchiver", "qt4-fsarchiver");
setting.beginGroup(comNet_name);
     user = txt_user->text();
     key = txt_key->text();
     if (comNet == "" && dialog_auswertung == 6)
       {
       QMessageBox::about(this, tr("Hinweis"),
      tr("Sie müssen den Rechner auswählen, auf den die Sicherungsdaten geschrieben werden sollen\n"));
      return 1 ;
      }
     if (comNet == "" && dialog_auswertung == 7)
      {
       QMessageBox::about(this, tr("Hinweis"),
       tr("Sie müssen den Rechner auswählen, von dem die Sicherungsdaten zurück geschrieben werden sollen\n"));
      return 1 ;
      }
     if (user == "" )
      {
       QMessageBox::about(this, tr("Hinweis"),
      tr("Sie müssen den Benutzernamen eingeben. Sonst kann nicht auf Netzrechner zugegriffen werden\n"));
      return 1 ;
      }
     if (key == "" ) {
          int ret = questionMessage(tr("Ist das richtig, dass Sie keinen Schlüssel eingegeben haben?"));
              if (ret == 2)
              return 1;
      }
     
     if (state == Qt::Checked )   
      {
       setting.setValue("Name",user);
       setting.setValue("key",crypt(key));
      }
     setting.endGroup();
     int i = setting_save();     
     if (dialog_auswertung == 6 && i == 0)
	save_net();
     if (dialog_auswertung == 7 && i == 0)
	restore_net(); 
     close();
}

void NetEin::save_net () {
extern int dialog_auswertung;
    	dialog_auswertung = 6;
     	DialogNet *dialog2 = new DialogNet;
     	dialog2->show();
}
void NetEin::restore_net () {
extern int dialog_auswertung;
      	dialog_auswertung = 7;
      	DialogNet *dialog2 = new DialogNet;
        dialog2->show();
}

QString NetEin::crypt(QString key){
FILE *lesen, *schreiben;
char dateiname_eingabe[100], dateiname_ausgabe[100];
char *homepath_;
char *suffix;
char zeichen;
int schluessel=5;
int j;
int i;
QString text;
QString homepath = QDir::homePath();
QFile file(homepath + "/.config/qt4-fsarchiver/crypt.txt");
QFile file1(homepath + "/.config/qt4-fsarchiver/crypt1.txt");
QTextStream ds(&file1);
j = key.size();
QTextStream stream(&file);
       if (file.open(QIODevice::ReadWrite| QIODevice::Text)) {
	  file.resize(1);
	  stream << key << "\n";
          file.close(); 
       }
homepath_ =  homepath.toAscii().data();
suffix = "/.config/qt4-fsarchiver/crypt.txt";
strcpy (dateiname_eingabe, homepath_);
strcat (dateiname_eingabe, suffix);
lesen=fopen(dateiname_eingabe,"r");
suffix = "/.config/qt4-fsarchiver/crypt1.txt";
strcpy (dateiname_ausgabe, homepath_);
strcat (dateiname_ausgabe, suffix);
schreiben=fopen(dateiname_ausgabe,"w");

for (i=0; i < j + 1; i++)
	{
		zeichen=fgetc(lesen);
		//Zahlen
		if((zeichen>47)&&(zeichen<58)&&((zeichen+schluessel)<58)&&((zeichen+schluessel)>47))
			fputc(zeichen+schluessel, schreiben);

		else if((zeichen>47)&&(zeichen<58)&&((zeichen+schluessel)>57))
			fputc(zeichen+schluessel+47-57, schreiben);

		else if((zeichen>47)&&(zeichen<58)&&((zeichen+schluessel)<48))
			fputc(zeichen+schluessel-47+57, schreiben);
		//Grossbuchstaben
		else if((zeichen>64)&&(zeichen<91)&&((zeichen+schluessel)<91)&&((zeichen+schluessel)>64))
			fputc(zeichen+schluessel, schreiben);

		else if((zeichen>64)&&(zeichen<91)&&((zeichen+schluessel)>90))
			fputc(zeichen+schluessel+64-90, schreiben);

		else if((zeichen>64)&&(zeichen<91)&&((zeichen+schluessel)<65))
         		fputc(zeichen+schluessel-64+90, schreiben);
		//Kleinbuchstaben
		else if((zeichen>96)&&(zeichen<123)&&((zeichen+schluessel)<123)&&((zeichen+schluessel)>96))
			fputc(zeichen+schluessel, schreiben);

		else if((zeichen>96)&&(zeichen<123)&&((zeichen+schluessel)>122))
			fputc(zeichen+schluessel+96-122, schreiben);

		else if((zeichen>96)&&(zeichen<123)&&((zeichen+schluessel)<97))
			fputc(zeichen+schluessel-96+122, schreiben);

		//sonstige Zeichen
		else
			fputc(zeichen, schreiben);
	}

	fclose(lesen);
	fclose(schreiben);
	file.remove();
        if (file1.open(QIODevice::ReadWrite | QIODevice::Text)) {
     	     text = ds.readLine();
             file1.close();
        }
        return text;
}


QString NetEin::decrypt(QString key_){
FILE *lesen, *schreiben;
char dateiname_eingabe[100], dateiname_ausgabe[100];
char *homepath_;
char *suffix;
char zeichen;
int schluessel= -5;
int j;
int i;
QString homepath = QDir::homePath();
QFile file(homepath + "/.config/qt4-fsarchiver/crypt1.txt");
QFile file1(homepath + "/.config/qt4-fsarchiver/crypt2.txt");
QTextStream ds(&file1);
QTextStream ds1(&file);
QString text;
// key_ in Datei crypt1.txt eintragen
if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
     	     ds1 << key_;
             file.close();
        }
j = file.size();
if (j == 0) // Datei noch nicht vorhanden
    return "";
homepath_ =  homepath.toAscii().data();
suffix = "/.config/qt4-fsarchiver/crypt1.txt";
strcpy (dateiname_eingabe, homepath_);
strcat (dateiname_eingabe, suffix);
lesen=fopen(dateiname_eingabe,"r");
suffix = "/.config/qt4-fsarchiver/crypt2.txt";
strcpy (dateiname_ausgabe, homepath_);
strcat (dateiname_ausgabe, suffix);
schreiben=fopen(dateiname_ausgabe,"w");

for (i=0; i < j; i++)
	{
		zeichen=fgetc(lesen);
		//Zahlen
		if((zeichen>47)&&(zeichen<58)&&((zeichen+schluessel)<58)&&((zeichen+schluessel)>47))
			fputc(zeichen+schluessel, schreiben);

		else if((zeichen>47)&&(zeichen<58)&&((zeichen+schluessel)>57))
			fputc(zeichen+schluessel+47-57, schreiben);

		else if((zeichen>47)&&(zeichen<58)&&((zeichen+schluessel)<48))
			fputc(zeichen+schluessel-47+57, schreiben);
		//Grossbuchstaben
		else if((zeichen>64)&&(zeichen<91)&&((zeichen+schluessel)<91)&&((zeichen+schluessel)>64))
			fputc(zeichen+schluessel, schreiben);

		else if((zeichen>64)&&(zeichen<91)&&((zeichen+schluessel)>90))
			fputc(zeichen+schluessel+64-90, schreiben);

		else if((zeichen>64)&&(zeichen<91)&&((zeichen+schluessel)<65))
         		fputc(zeichen+schluessel-64+90, schreiben);
		//Kleinbuchstaben
		else if((zeichen>96)&&(zeichen<123)&&((zeichen+schluessel)<123)&&((zeichen+schluessel)>96))
			fputc(zeichen+schluessel, schreiben);

		else if((zeichen>96)&&(zeichen<123)&&((zeichen+schluessel)>122))
			fputc(zeichen+schluessel+96-122, schreiben);

		else if((zeichen>96)&&(zeichen<123)&&((zeichen+schluessel)<97))
			fputc(zeichen+schluessel-96+122, schreiben);

		//sonstige Zeichen
		else
			fputc(zeichen, schreiben);
	}
	fclose(lesen);
	fclose(schreiben);
        if (file1.open(QIODevice::ReadWrite | QIODevice::Text)) {
     	     text = ds.readLine();
             file1.close();
        }
	file1.remove();
        return text;
}

int NetEin::questionMessage(QString frage)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Hinweis"),
            frage,
            QMessageBox::No | QMessageBox::Yes | QMessageBox::Default);
    if (reply == QMessageBox::Yes)
        return 1;
    else 
        return 2;

}
