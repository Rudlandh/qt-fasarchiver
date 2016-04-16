/********************************************************************************
** Form generated from reading UI file 'mbr_dialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MBR_DIALOG_H
#define UI_MBR_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mbr_dialog
{
public:
    QComboBox *cmb_mbr;
    QPushButton *bt_end;
    QPushButton *bt_save;
    QTreeView *treeView;
    QLabel *label;
    QLabel *label_2;
    QComboBox *cmb_disk;
    QLabel *label_3;
    QPushButton *bt_dummy;

    void setupUi(QWidget *mbr_dialog)
    {
        if (mbr_dialog->objectName().isEmpty())
            mbr_dialog->setObjectName(QString::fromUtf8("mbr_dialog"));
        mbr_dialog->resize(469, 505);
        cmb_mbr = new QComboBox(mbr_dialog);
        cmb_mbr->setObjectName(QString::fromUtf8("cmb_mbr"));
        cmb_mbr->setGeometry(QRect(20, 380, 431, 22));
        bt_end = new QPushButton(mbr_dialog);
        bt_end->setObjectName(QString::fromUtf8("bt_end"));
        bt_end->setGeometry(QRect(20, 460, 201, 28));
        bt_save = new QPushButton(mbr_dialog);
        bt_save->setObjectName(QString::fromUtf8("bt_save"));
        bt_save->setGeometry(QRect(230, 460, 221, 28));
        treeView = new QTreeView(mbr_dialog);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setGeometry(QRect(20, 60, 431, 301));
        treeView->header()->setDefaultSectionSize(400);
        label = new QLabel(mbr_dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 421, 18));
        label_2 = new QLabel(mbr_dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 410, 281, 41));
        cmb_disk = new QComboBox(mbr_dialog);
        cmb_disk->setObjectName(QString::fromUtf8("cmb_disk"));
        cmb_disk->setGeometry(QRect(397, 420, 51, 27));
        label_3 = new QLabel(mbr_dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(305, 410, 81, 41));
        bt_dummy = new QPushButton(mbr_dialog);
        bt_dummy->setObjectName(QString::fromUtf8("bt_dummy"));
        bt_dummy->setGeometry(QRect(190, 460, 21, 27));
        bt_dummy->raise();
        cmb_mbr->raise();
        bt_end->raise();
        bt_save->raise();
        treeView->raise();
        label->raise();
        label_2->raise();
        cmb_disk->raise();
        label_3->raise();
#ifndef QT_NO_SHORTCUT
        label_3->setBuddy(cmb_disk);
#endif // QT_NO_SHORTCUT

        retranslateUi(mbr_dialog);
        QObject::connect(cmb_disk, SIGNAL(currentIndexChanged(QString)), bt_dummy, SLOT(click()));

        QMetaObject::connectSlotsByName(mbr_dialog);
    } // setupUi

    void retranslateUi(QWidget *mbr_dialog)
    {
    int w,h, mw, mh, cw, ch;
        QWidget *d = QApplication::desktop();
        w=d->width();                   // returns screen width
        h=d->height();                  // returns screen height
        //qDebug() << "Bildschrimbreite";
        //qDebug() << h;
        //Center position calculation
        mw = mbr_dialog->width();
        mh = mbr_dialog->height();
        cw = (w/2) - (mw/2);
        ch = (h/2) - (mh/2); 
        mbr_dialog->setGeometry(QRect(cw, ch, 469, 505));
        mbr_dialog->setWindowTitle(QApplication::translate("mbr_dialog", "Save / Restore MBR/GPT", 0, QApplication::UnicodeUTF8));
        cmb_mbr->clear();
        cmb_mbr->insertItems(0, QStringList()
         << QApplication::translate("mbr_dialog", "Only the boot loader area to restore", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mbr_dialog", "Bootloader area and restore the partition table", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mbr_dialog", "Restore partition table", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mbr_dialog", "Secretly area to restore. (Without Boot Loader and without partition table).", 0, QApplication::UnicodeUTF8)
         << QString()
        );
        bt_end->setText(QApplication::translate("mbr_dialog", "Exit", 0, QApplication::UnicodeUTF8));
        bt_save->setText(QApplication::translate("mbr_dialog", "Save / restore MBR", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label->setToolTip(QApplication::translate("mbr_dialog", "Directory in which the MBR/GPT is written.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("mbr_dialog", "MBR/GPT backup directory:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("mbr_dialog", "MBR of that drive this\n"
"Backup / restore:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("mbr_dialog", "Existing\n"
"harddrive:", 0, QApplication::UnicodeUTF8));
        bt_dummy->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class mbr_dialog: public Ui_mbr_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MBR_DIALOG_H
