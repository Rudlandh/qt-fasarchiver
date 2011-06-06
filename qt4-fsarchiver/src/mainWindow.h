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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui>
#include <string.h>
#include "ui_fsarchiverMain.h"
#include "thread.h"

using namespace std;



class MWindow : public QMainWindow,
                private Ui::MainWindow {
    Q_OBJECT

public:
   MWindow();
   ~MWindow() {}
   int Root_Auswertung();
   QString Zeit_auslesen();
   string mtab_einlesen(string partition_if_home);
   QString beschreibungstext(QString partitiontext, QString text, int zip, int row);
   int is_mounted (char* dev_path);
   int questionMessage(QString frage);

public slots:
   void folder_einlesen();
   void save_button();

protected slots:
   void ViewProzent();
   void ViewProzent_save();
   void ViewProzent_restore();

   int testDateiName(string endung);
   int savePartition();
   int restorePartition();
   void info();
   void einstellungen();
   void listWidget_auslesen();
   void rdButton_auslesen();
   void mbr_save ();
   void mbr_restore ();
   void dir_save ();
   void dir_restore ();
   void folder_file();
   void starteinstellung();
   void format();
   void closeEvent(QCloseEvent* event);
   void elapsedTime();
   void remainingTime(int prozent);
   void indicator_reset();
   void keyPressEvent(QKeyEvent *event);
   void restore_button();
   void eingabe_net_save();
   void eingabe_net_restore();
   void chkkey();
   QString kernel_version();
   QString ubuntu_version();
   QString mountpoint(QString uuid);
   QString format(float zahl);
   void breakProcess();

private:
    QDirModel *dirModel;
    QItemSelectionModel *selModel;
    QLabel *questionLabel;
    Thread thread1;
    Thread thread2;
    QTimer *timer;

private slots:
   void startThread1();
   void thread1Ready();
   void startThread2();
   void thread2Ready();
//   void esc_end();
//   void pid_ermitteln();
};
#endif

