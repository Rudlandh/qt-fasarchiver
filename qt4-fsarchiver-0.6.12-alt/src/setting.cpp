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
#include "setting.h"


extern int dialog_auswertung;
QStringList items_kerne;
QStringList items_zip; 


DialogSetting::DialogSetting(QWidget *parent)
{
	
        setupUi(this); // this sets up GUI
	connect( cmd_save, SIGNAL( clicked() ), this, SLOT( setting_save())); 
        connect( cmd_cancel, SIGNAL( clicked() ), this, SLOT(close()));
        items_kerne << "1" << "2" << "3" << "4" <<  "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12" ;
        cmb_Kerne->addItems (items_kerne);
   	items_kerne.clear();
        items_zip << tr("lzo") << tr("gzip fast") << tr("gzip standard") << tr("qzip best") <<  tr("bzip2 fast");
   	cmb_zip->addItems (items_zip);
   	items_zip.clear();
   	items_zip << tr("bzip2 good") << tr("lzma fast") << tr("lzma medium") << tr("lzma best");
   	cmb_zip->addItems (items_zip);
   	items_zip.clear();
        QSettings setting("qt4-fsarchiver", "qt4-fsarchiver");
        setting.beginGroup("Basiseinstellungen");
        int auswertung = setting.value("Kompression").toInt();
        cmb_zip -> setCurrentIndex(auswertung); 
        auswertung = setting.value("Kerne").toInt();
        cmb_Kerne -> setCurrentIndex(auswertung-1); 
        auswertung = setting.value("overwrite").toInt();
        if (auswertung ==1)
           chk_file->setChecked(Qt::Checked); 
        auswertung = setting.value("tip").toInt();
        if (auswertung ==1)
           chk_tip->setChecked(Qt::Checked);  
        auswertung = setting.value("key").toInt();
        if (auswertung ==1)
           chk_key->setChecked(Qt::Checked); 
        auswertung = setting.value("place").toInt();
        if (auswertung ==1)
           chk_folder->setChecked(Qt::Checked); 
        auswertung = setting.value("split").toInt();
        if (auswertung ==1)
           chk_split->setChecked(Qt::Checked); 
        auswertung = setting.value("Passwort").toInt();
        if (auswertung ==1)
           chk_password->setChecked(Qt::Checked); 
        auswertung = setting.value("save").toInt();
        if (auswertung ==1)
           chk_datesave->setChecked(Qt::Checked);
        setting.endGroup();
}        

void DialogSetting:: setting_save()
{
     Qt::CheckState state;
     QSettings setting("qt4-fsarchiver", "qt4-fsarchiver");
     setting.beginGroup("Basiseinstellungen");
     setting.setValue("Kerne",cmb_Kerne->currentText());
     int zip = cmb_zip->currentIndex();
     setting.setValue("Kompression",zip);
     state = chk_file->checkState();
     if (state == Qt::Checked) 
            setting.setValue("overwrite",1);
     else
            setting.setValue("overwrite",0);
     state = chk_tip->checkState();
     if (state == Qt::Checked) 
            setting.setValue("tip",1);
     else
            setting.setValue("tip",0);
     state = chk_key->checkState();
     if (state == Qt::Checked) 
            setting.setValue("key",1);
     else
            setting.setValue("key",0);
     state = chk_folder->checkState();
     if (state == Qt::Checked) 
            setting.setValue("place",1);
     else
            setting.setValue("place",0);
     state = chk_split->checkState();
     if (state == Qt::Checked) 
            setting.setValue("split",1);
     else
            setting.setValue("split",0);
     state = chk_password->checkState();
     if (state == Qt::Checked) 
            setting.setValue("Passwort",1);
     else
            setting.setValue("Passwort",0);
     state = chk_datesave->checkState();
     if (state == Qt::Checked) 
            setting.setValue("save",1);
     else
            setting.setValue("save",0);
     
     setting.endGroup();
}

