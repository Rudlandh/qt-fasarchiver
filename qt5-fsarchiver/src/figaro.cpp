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

#include "figaro.h"

int p_run=0;
int p_end;
QString f_cmd;


Figaro::Figaro() : QThread() {
p_run=0;

}

void Figaro::run()
{
p_end=1;
while (p_end)
    {
       switch (p_run)
       {
          case 1:
          {
            if (!f_cmd.isEmpty())
            {
            QProcess *proc=new QProcess();
            proc->start (f_cmd);
            }
            break;
           }
        default:
               break;


       }
    }
}



