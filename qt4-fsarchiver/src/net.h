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

#ifndef NET_H
#define NET_H
#include <QtGui>
#include <QDialog>
#include "ui_net.h"
#include "thread.h"


using namespace std;

class DialogNet : public QDialog, private Ui::Dialog_Net
{
	Q_OBJECT

public:
	DialogNet(QWidget *parent = 0);
        
public slots:
        
        
protected slots:
   	int restorePartition();
   	int savePartition();
        void closeEvent(QCloseEvent* event);
        void elapsedTime();
   	void remainingTime(int prozent);
        void indicator_reset();
        void folder_einlesen();
        int testDateiName(string endung);
        void Daten_eintragen();
        void ViewProzent();
        void keyPressEvent(QKeyEvent *event);
        int questionMessage(QString frage);
        void esc_end();
        void bit();
               
private:
	QDirModel *dirModel;
    	QItemSelectionModel *selModel;
        Thread thread1;
        Thread thread2;
        QTimer *timer; 		
	
private slots:
	void end();
        QString hostname();
        void chkkey();
	void addWidget();
        void starteinstellung();
        void rdButton_auslesen();
        QString Zeit_auslesen();
        void listWidget_auslesen();
        void folder_file();
        void startThread1();
        void thread1Ready();
        void startThread2();
        void thread2Ready();
};

#endif
