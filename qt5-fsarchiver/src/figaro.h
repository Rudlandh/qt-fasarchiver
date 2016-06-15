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


#ifndef FIGARO_H
#define FIGARO_H
#include <QtGui>
#include <QtCore>
#include <QProcess>
#include <QThread>

class Figaro : public QThread {
   Q_OBJECT
public:
    Figaro();
    ~Figaro(){};
protected:
   QProcess Qsystem;
   void run();
};
#endif










