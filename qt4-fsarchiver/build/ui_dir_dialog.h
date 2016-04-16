/********************************************************************************
** Form generated from reading UI file 'dir_dialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIR_DIALOG_H
#define UI_DIR_DIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dir_dialog
{
public:
    QPushButton *bt_end;
    QPushButton *bt_save;
    QTreeView *treeView_dir;
    QLabel *label;
    QTreeView *treeView_path;
    QLabel *label_2;
    QFrame *frame;
    QLabel *label_4;
    QLineEdit *AnzahlsaveFile;
    QLabel *label_5;
    QLineEdit *AnzahlgesicherteFile;
    QProgressBar *progressBar;
    QLabel *label_6;
    QLineEdit *MinuteElapsed;
    QLabel *label_7;
    QLineEdit *SekundeElapsed;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *MinuteRemaining;
    QLineEdit *SekundeRemaining;
    QLabel *label_10;
    QLabel *label_11;
    QPushButton *pushButton_break;
    QComboBox *cmb_kerne;
    QLabel *label_12;
    QFrame *frame_2;
    QLabel *label_3;
    QComboBox *cmb_zip;
    QCheckBox *chk_key;
    QLineEdit *lineKey;
    QCheckBox *chk_overwrite;
    QCheckBox *chk_path;
    QCheckBox *chk_hidden;

    void setupUi(QWidget *dir_dialog)
    {
        if (dir_dialog->objectName().isEmpty())
            dir_dialog->setObjectName(QString::fromUtf8("dir_dialog"));
        dir_dialog->resize(714, 654);
        bt_end = new QPushButton(dir_dialog);
        bt_end->setObjectName(QString::fromUtf8("bt_end"));
        bt_end->setGeometry(QRect(375, 460, 321, 41));
        bt_save = new QPushButton(dir_dialog);
        bt_save->setObjectName(QString::fromUtf8("bt_save"));
        bt_save->setGeometry(QRect(375, 415, 321, 41));
        treeView_dir = new QTreeView(dir_dialog);
        treeView_dir->setObjectName(QString::fromUtf8("treeView_dir"));
        treeView_dir->setGeometry(QRect(20, 52, 331, 301));
        treeView_dir->header()->setDefaultSectionSize(400);
        label = new QLabel(dir_dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 25, 311, 18));
        treeView_path = new QTreeView(dir_dialog);
        treeView_path->setObjectName(QString::fromUtf8("treeView_path"));
        treeView_path->setGeometry(QRect(370, 52, 325, 301));
        treeView_path->header()->setDefaultSectionSize(400);
        label_2 = new QLabel(dir_dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(370, 25, 291, 18));
        frame = new QFrame(dir_dialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 510, 711, 131));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(15, 6, 181, 41));
        AnzahlsaveFile = new QLineEdit(frame);
        AnzahlsaveFile->setObjectName(QString::fromUtf8("AnzahlsaveFile"));
        AnzahlsaveFile->setGeometry(QRect(200, 20, 71, 24));
        AnzahlsaveFile->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 50, 171, 41));
        AnzahlgesicherteFile = new QLineEdit(frame);
        AnzahlgesicherteFile->setObjectName(QString::fromUtf8("AnzahlgesicherteFile"));
        AnzahlgesicherteFile->setGeometry(QRect(200, 60, 71, 25));
        AnzahlgesicherteFile->setAlignment(Qt::AlignCenter);
        progressBar = new QProgressBar(frame);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(20, 100, 631, 23));
        progressBar->setMaximum(100);
        progressBar->setValue(0);
        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(300, 25, 131, 18));
        MinuteElapsed = new QLineEdit(frame);
        MinuteElapsed->setObjectName(QString::fromUtf8("MinuteElapsed"));
        MinuteElapsed->setGeometry(QRect(440, 20, 41, 26));
        MinuteElapsed->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(490, 25, 71, 18));
        SekundeElapsed = new QLineEdit(frame);
        SekundeElapsed->setObjectName(QString::fromUtf8("SekundeElapsed"));
        SekundeElapsed->setGeometry(QRect(560, 20, 41, 26));
        SekundeElapsed->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(frame);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(610, 25, 71, 18));
        label_9 = new QLabel(frame);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(300, 52, 141, 41));
        MinuteRemaining = new QLineEdit(frame);
        MinuteRemaining->setObjectName(QString::fromUtf8("MinuteRemaining"));
        MinuteRemaining->setGeometry(QRect(440, 60, 41, 26));
        MinuteRemaining->setAlignment(Qt::AlignCenter);
        SekundeRemaining = new QLineEdit(frame);
        SekundeRemaining->setObjectName(QString::fromUtf8("SekundeRemaining"));
        SekundeRemaining->setGeometry(QRect(560, 60, 41, 26));
        SekundeRemaining->setAlignment(Qt::AlignCenter);
        label_10 = new QLabel(frame);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(490, 65, 71, 18));
        label_11 = new QLabel(frame);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(610, 65, 71, 18));
        pushButton_break = new QPushButton(frame);
        pushButton_break->setObjectName(QString::fromUtf8("pushButton_break"));
        pushButton_break->setGeometry(QRect(670, 100, 24, 21));
        pushButton_break->setMinimumSize(QSize(24, 21));
        pushButton_break->setMaximumSize(QSize(24, 21));
        pushButton_break->setSizeIncrement(QSize(24, 21));
        pushButton_break->setBaseSize(QSize(21, 21));
        pushButton_break->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/src/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_break->setIcon(icon);
        pushButton_break->setAutoDefault(false);
        pushButton_break->setDefault(false);
        pushButton_break->setFlat(false);
        cmb_kerne = new QComboBox(dir_dialog);
        cmb_kerne->setObjectName(QString::fromUtf8("cmb_kerne"));
        cmb_kerne->setGeometry(QRect(634, 355, 61, 27));
        label_12 = new QLabel(dir_dialog);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(370, 350, 251, 41));
        frame_2 = new QFrame(dir_dialog);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(20, 360, 331, 141));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 0, 191, 31));
        cmb_zip = new QComboBox(frame_2);
        cmb_zip->setObjectName(QString::fromUtf8("cmb_zip"));
        cmb_zip->setGeometry(QRect(200, 0, 131, 27));
        chk_key = new QCheckBox(frame_2);
        chk_key->setObjectName(QString::fromUtf8("chk_key"));
        chk_key->setGeometry(QRect(0, 30, 200, 41));
        lineKey = new QLineEdit(frame_2);
        lineKey->setObjectName(QString::fromUtf8("lineKey"));
        lineKey->setGeometry(QRect(200, 40, 131, 26));
        chk_overwrite = new QCheckBox(frame_2);
        chk_overwrite->setObjectName(QString::fromUtf8("chk_overwrite"));
        chk_overwrite->setGeometry(QRect(0, 75, 330, 23));
        chk_path = new QCheckBox(frame_2);
        chk_path->setObjectName(QString::fromUtf8("chk_path"));
        chk_path->setGeometry(QRect(0, 105, 321, 31));
        chk_hidden = new QCheckBox(dir_dialog);
        chk_hidden->setObjectName(QString::fromUtf8("chk_hidden"));
        chk_hidden->setGeometry(QRect(370, 390, 321, 22));

        retranslateUi(dir_dialog);

        QMetaObject::connectSlotsByName(dir_dialog);
    } // setupUi

   void retranslateUi(QWidget *dir_dialog)
    {
	int w,h, mw, mh, cw, ch;
        QWidget *d = QApplication::desktop();
        w=d->width();                   // returns screen width
        h=d->height();                  // returns screen height
        //qDebug() << "Bildschrimbreite";
        //qDebug() << h;
        //Center position calculation
        mw = dir_dialog->width();
        mh = dir_dialog->height();
        cw = (w/2) - (mw/2);
        ch = (h/2) - (mh/2); 
        dir_dialog->setGeometry(QRect(cw, ch, 714, 654));  

        dir_dialog->setWindowTitle(QApplication::translate("dir_dialog", "Save/restore directory", 0, QApplication::UnicodeUTF8));
        bt_end->setText(QApplication::translate("dir_dialog", "Exit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bt_save->setToolTip(QApplication::translate("dir_dialog", "Start the backup or the restore of a folder.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bt_save->setText(QApplication::translate("dir_dialog", "Save/restore directory", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label->setToolTip(QApplication::translate("dir_dialog", "Select the folder to be backed up or restored.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("dir_dialog", "Saved directory", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_2->setToolTip(QApplication::translate("dir_dialog", "Directory in which the folder is backed up or from the recovery file is removed.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("dir_dialog", "Location (path) of backup:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("dir_dialog", "On the number of\n"
"captive Files/directories:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("dir_dialog", "Already backed\n"
"Files/directories:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("dir_dialog", "elapsed time:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("dir_dialog", "minutes", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("dir_dialog", "seconds", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("dir_dialog", "remaining  time:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("dir_dialog", "minutes", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("dir_dialog", "seconds", 0, QApplication::UnicodeUTF8));
        pushButton_break->setText(QString());
        pushButton_break->setShortcut(QString());
        label_12->setText(QApplication::translate("dir_dialog", "Number of processor cores\n"
"for to be used fsarchiver:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("dir_dialog", "Compression setting:", 0, QApplication::UnicodeUTF8));
        chk_key->setText(QApplication::translate("dir_dialog", "Encrypt\n"
"backup key", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chk_overwrite->setToolTip(QApplication::translate("dir_dialog", "Same backup files are overwritten without prior consultation.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chk_overwrite->setText(QApplication::translate("dir_dialog", "Overwrite existing backup", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        chk_path->setToolTip(QApplication::translate("dir_dialog", "The directory will be written back to the original path", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chk_path->setText(QApplication::translate("dir_dialog", "Backup restore to the Origin", 0, QApplication::UnicodeUTF8));
        chk_hidden->setText(QApplication::translate("dir_dialog", "show hidden directories/files", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dir_dialog: public Ui_dir_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIR_DIALOG_H
