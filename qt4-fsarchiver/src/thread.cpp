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
#include "thread.h"
#include "mainWindow.h"
extern "C" {
#include "connect_c_cpp.h"
}

using namespace std;
extern int work_etap;

extern void 
fsarchiver_aufruf(int argc, char *anlage0=NULL, char *anlage1=NULL, char *anlage2=NULL, char *anlage3=NULL, char *anlage4=NULL, char *anlage5=NULL, char *anlage6=NULL, char *anlage7=NULL, char *anlage8=NULL, char *anlage9=NULL, char *anlage10=NULL, char *anlage11=NULL, char *anlage12=NULL,char *anlage13=NULL,char *anlage14=NULL);


Thread::Thread() {
   anzahl = 0;
   qdummy = 0;
}

void Thread::setValues( int anzahl_, int dummy ) {
   anzahl = anzahl_;
   qdummy = dummy;
}

void Thread::run() {
  float prozent;
  extern int dialog_auswertung;
  extern QString parameter[15];
  //Zurücksetzen der bekannten oder unbekannten Fehlermeldung
  werte_uebergeben(1,4);
//  dialog_auswertung =
 fsarchiver_aufruf(anzahl,parameter[0].toAscii().data(),parameter[1].toAscii().data(),parameter[2].toAscii().data(),parameter[3].toAscii().data(),parameter[4].toAscii().data (),parameter[5].toAscii().data(),parameter[6].toAscii().data(),parameter[7].toAscii().data(),parameter[8].toAscii().data(),parameter[9].toAscii().data(),parameter[10].toAscii().data(),parameter[11].toAscii().data(),parameter[12].toAscii().data(),parameter[13].toAscii().data(),parameter[14].toAscii().data());

work_etap=c_th;

//    if (dialog_auswertung != 0){
//      //verhindert das Blockieren des Programmes Abfrage in der while Schleife in dir.cpp und mainwindow.cpp
//      float endeThread = werte_holen(4);
//      // Wenn vom Programm bereits eine Fehlermeldung zurückgeschrieben wurde, wird die Fehlermeldungnummer nicht durch 100 ersetzt.
//      if (endeThread ==0)
//      	werte_uebergeben(100,4);
//     }
 }

 

