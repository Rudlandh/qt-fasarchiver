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
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "filedialog.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "mainWindow.h"
using namespace std;
extern int dialog_auswertung;
QString wort;

FileDialog::FileDialog(QWidget *parent)
  : QDialog(parent)
{
  setupUi(this);
  connect( cmd_save, SIGNAL(clicked()), this, SLOT(folder_einlesen()));
  connect( cmd_cancel, SIGNAL( clicked() ), this, SLOT(reject()));
  textEdit->setPlainText(wort);
 
 if (dialog_auswertung ==3)
     {
	cmd_save->setText(tr("Partition zurückschreiben", "Partition zurückschreiben"));
	file_read();
     }
  
 } 

void FileDialog::folder_einlesen() {
        QString folder;
        if (dialog_auswertung ==2){
            cmd_save->setText(tr("Partition sichern")); 
            file_save();
        }
        dialog_auswertung = 1;
  	close();
}

void FileDialog::file_save()
{
        extern QString folder_file_;
        QString text;
      	QString filename = folder_file_;
        if (filename .isEmpty())
   		return;
	QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
           {
             QMessageBox::about(this,tr("Hinweis"),
              tr("Die Hinweise zur Partition wurden nicht in eine Datei geschrieben.\n"));
            }
        text = textEdit->toPlainText().toAscii();
	file.write((textEdit->toPlainText()).toAscii());
  }

void FileDialog::file_read()
{
	extern QString folder_file_;	
	QString filename = folder_file_;
        int pos = filename.indexOf("fsa");
        filename = filename.left(pos);
        filename.insert(pos, QString("txt"));
        //prüfen ob Datei existiert
                if (!filename.isEmpty()) {
      		QFile file(filename);
        	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) 
               {
               textEdit->setPlainText(file.readAll());
//textEdit->setPlainText("Hallo");
               }
 	  }
  }


QString FileDialog::werte_holen_(int auswahl) {
return  "hallo";
}

void FileDialog::werte_uebergeben(QString wert) {
	wort = wert;
}
