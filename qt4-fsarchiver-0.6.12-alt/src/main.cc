/*
 * qt4-fsarchiver: Filesystem Archiver
 *
 * Copyright (C) 2010-2011 Dieter Baum.  All rights reserved.
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

#ifdef HAVE_GETOPT_H
  #include <getopt.h>
#endif

#ifdef HAVE_SYS_PARAM_H
  #include <sys/param.h>
#endif
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "mainWindow.h"
#include <iostream>
#include <fstream>

#define __STDC_FORMAT_MACROS
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include "qt_probe.h"
using namespace std;


FILE * g_fDebug; // debug file
QString folder_file_;
int dialog_auswertung;
int anzahl_disk;
QString parameter[15];
QString add_part[100];

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

//	/*---install translation file for application strings---*/

	QString sLocPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	QTranslator *translator = new QTranslator(0);
	translator->load(QString("qt4-fsarchiver_"+QLocale::system().name()), sLocPath);
	app.installTranslator(translator);


   QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); 
   QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); 
   QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8")); 
   MWindow window;
   
   if (window.Root_Auswertung() != 10)
       
   {
   	window.show();
   	return app.exec();
  	}
  	else
  	{
  	return 0;	
 		}
  // qDebug() << window.Root_Auswertung();
 }
  

