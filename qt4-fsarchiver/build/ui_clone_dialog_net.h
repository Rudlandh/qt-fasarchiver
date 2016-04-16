/********************************************************************************
** Form generated from reading UI file 'clone_dialog_net.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLONE_DIALOG_NET_H
#define UI_CLONE_DIALOG_NET_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_clone_dialog_net
{
public:
    QPushButton *bt_end;
    QPushButton *button_save;
    QLabel *lbl_hd;
    QRadioButton *rdbt_image_save;
    QRadioButton *rdbt_image_restore;
    QListWidget *listWidget_exist;
    QFrame *frame_2;
    QLabel *label_8;
    QLineEdit *SekundeElapsed;
    QLabel *label_7;
    QLineEdit *MinuteElapsed;
    QLabel *label_5;
    QLabel *label_10;
    QLineEdit *SekundeRemaining;
    QLabel *label_9;
    QLineEdit *MinuteRemaining;
    QLabel *label_6;
    QProgressBar *progressBar;
    QPushButton *pushButton_break;
    QLineEdit *savedBytes;
    QLabel *label_4;
    QLabel *lbl_hd_size;
    QLabel *label_11;
    QLineEdit *HourElapsed;
    QLabel *label_12;
    QLineEdit *HourRemaining;
    QPushButton *pushButton_folder;
    QListWidget *listWidget_free_folder;
    QLabel *lbl_Image;
    QLineEdit *lineEdit_Dateiname;
    QLineEdit *txt_rechner_net;
    QLabel *label_2;
    QLabel *label;
    QPushButton *pushButton_folder_free;
    QLineEdit *restore_file_name_txt;

    void setupUi(QWidget *clone_dialog_net)
    {
        if (clone_dialog_net->objectName().isEmpty())
            clone_dialog_net->setObjectName(QString::fromUtf8("clone_dialog_net"));
        clone_dialog_net->resize(521, 700);
        bt_end = new QPushButton(clone_dialog_net);
        bt_end->setObjectName(QString::fromUtf8("bt_end"));
        bt_end->setGeometry(QRect(40, 640, 201, 28));
        button_save = new QPushButton(clone_dialog_net);
        button_save->setObjectName(QString::fromUtf8("button_save"));
        button_save->setGeometry(QRect(270, 640, 201, 28));
        lbl_hd = new QLabel(clone_dialog_net);
        lbl_hd->setObjectName(QString::fromUtf8("lbl_hd"));
        lbl_hd->setGeometry(QRect(20, 30, 191, 18));
        rdbt_image_save = new QRadioButton(clone_dialog_net);
        rdbt_image_save->setObjectName(QString::fromUtf8("rdbt_image_save"));
        rdbt_image_save->setGeometry(QRect(210, 60, 261, 23));
        rdbt_image_restore = new QRadioButton(clone_dialog_net);
        rdbt_image_restore->setObjectName(QString::fromUtf8("rdbt_image_restore"));
        rdbt_image_restore->setGeometry(QRect(210, 90, 301, 23));
        listWidget_exist = new QListWidget(clone_dialog_net);
        listWidget_exist->setObjectName(QString::fromUtf8("listWidget_exist"));
        listWidget_exist->setGeometry(QRect(20, 50, 181, 111));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        font.setBold(true);
        font.setWeight(75);
        listWidget_exist->setFont(font);
        frame_2 = new QFrame(clone_dialog_net);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(30, 450, 471, 161));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_8 = new QLabel(frame_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(410, 0, 54, 34));
        SekundeElapsed = new QLineEdit(frame_2);
        SekundeElapsed->setObjectName(QString::fromUtf8("SekundeElapsed"));
        SekundeElapsed->setGeometry(QRect(350, 0, 51, 27));
        SekundeElapsed->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(290, 0, 54, 34));
        MinuteElapsed = new QLineEdit(frame_2);
        MinuteElapsed->setObjectName(QString::fromUtf8("MinuteElapsed"));
        MinuteElapsed->setGeometry(QRect(240, 0, 41, 27));
        MinuteElapsed->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(0, 0, 91, 34));
        label_10 = new QLabel(frame_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(420, 40, 54, 34));
        SekundeRemaining = new QLineEdit(frame_2);
        SekundeRemaining->setObjectName(QString::fromUtf8("SekundeRemaining"));
        SekundeRemaining->setGeometry(QRect(350, 40, 31, 27));
        SekundeRemaining->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(frame_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(290, 30, 54, 34));
        MinuteRemaining = new QLineEdit(frame_2);
        MinuteRemaining->setObjectName(QString::fromUtf8("MinuteRemaining"));
        MinuteRemaining->setGeometry(QRect(250, 40, 31, 27));
        MinuteRemaining->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(frame_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(0, 40, 105, 34));
        progressBar = new QProgressBar(frame_2);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(10, 120, 421, 25));
        progressBar->setMaximum(100);
        progressBar->setValue(0);
        pushButton_break = new QPushButton(frame_2);
        pushButton_break->setObjectName(QString::fromUtf8("pushButton_break"));
        pushButton_break->setGeometry(QRect(440, 120, 24, 21));
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
        savedBytes = new QLineEdit(frame_2);
        savedBytes->setObjectName(QString::fromUtf8("savedBytes"));
        savedBytes->setGeometry(QRect(260, 80, 101, 27));
        savedBytes->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(100, 80, 121, 21));
        lbl_hd_size = new QLabel(frame_2);
        lbl_hd_size->setObjectName(QString::fromUtf8("lbl_hd_size"));
        lbl_hd_size->setGeometry(QRect(370, 90, 31, 17));
        label_11 = new QLabel(frame_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(190, 10, 51, 17));
        HourElapsed = new QLineEdit(frame_2);
        HourElapsed->setObjectName(QString::fromUtf8("HourElapsed"));
        HourElapsed->setGeometry(QRect(140, 0, 41, 27));
        label_12 = new QLabel(frame_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(200, 40, 51, 17));
        HourRemaining = new QLineEdit(frame_2);
        HourRemaining->setObjectName(QString::fromUtf8("HourRemaining"));
        HourRemaining->setGeometry(QRect(140, 40, 31, 27));
        pushButton_folder = new QPushButton(clone_dialog_net);
        pushButton_folder->setObjectName(QString::fromUtf8("pushButton_folder"));
        pushButton_folder->setGeometry(QRect(100, 640, 16, 21));
        listWidget_free_folder = new QListWidget(clone_dialog_net);
        listWidget_free_folder->setObjectName(QString::fromUtf8("listWidget_free_folder"));
        listWidget_free_folder->setGeometry(QRect(20, 200, 481, 121));
        lbl_Image = new QLabel(clone_dialog_net);
        lbl_Image->setObjectName(QString::fromUtf8("lbl_Image"));
        lbl_Image->setGeometry(QRect(30, 330, 171, 18));
        lineEdit_Dateiname = new QLineEdit(clone_dialog_net);
        lineEdit_Dateiname->setObjectName(QString::fromUtf8("lineEdit_Dateiname"));
        lineEdit_Dateiname->setGeometry(QRect(30, 360, 201, 27));
        txt_rechner_net = new QLineEdit(clone_dialog_net);
        txt_rechner_net->setObjectName(QString::fromUtf8("txt_rechner_net"));
        txt_rechner_net->setGeometry(QRect(250, 360, 251, 27));
        label_2 = new QLabel(clone_dialog_net);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(250, 330, 351, 21));
        label = new QLabel(clone_dialog_net);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 170, 181, 17));
        pushButton_folder_free = new QPushButton(clone_dialog_net);
        pushButton_folder_free->setObjectName(QString::fromUtf8("pushButton_folder_free"));
        pushButton_folder_free->setGeometry(QRect(70, 400, 16, 27));
        restore_file_name_txt = new QLineEdit(clone_dialog_net);
        restore_file_name_txt->setObjectName(QString::fromUtf8("restore_file_name_txt"));
        restore_file_name_txt->setGeometry(QRect(110, 410, 360, 27));
        pushButton_folder->raise();
        bt_end->raise();
        button_save->raise();
        lbl_hd->raise();
        rdbt_image_save->raise();
        rdbt_image_restore->raise();
        listWidget_exist->raise();
        frame_2->raise();
        listWidget_free_folder->raise();
        lbl_Image->raise();
        lineEdit_Dateiname->raise();
        txt_rechner_net->raise();
        label_2->raise();
        label->raise();
        pushButton_folder_free->raise();
        restore_file_name_txt->raise();

        retranslateUi(clone_dialog_net);
        QObject::connect(listWidget_free_folder, SIGNAL(clicked(QModelIndex)), pushButton_folder_free, SLOT(click()));

        QMetaObject::connectSlotsByName(clone_dialog_net);
    } // setupUi

    void retranslateUi(QWidget *clone_dialog_net)
    {
        clone_dialog_net->setWindowTitle(QApplication::translate("clone_dialog_net", "Clone hard drive", 0, QApplication::UnicodeUTF8));
        bt_end->setText(QApplication::translate("clone_dialog_net", "Cancel", 0, QApplication::UnicodeUTF8));
        button_save->setText(QApplication::translate("clone_dialog_net", "Save / restore MBR", 0, QApplication::UnicodeUTF8));
        lbl_hd->setText(QApplication::translate("clone_dialog_net", "Exist harddrive:", 0, QApplication::UnicodeUTF8));
        rdbt_image_save->setText(QApplication::translate("clone_dialog_net", "Image einer Festplatte erstellen:", 0, QApplication::UnicodeUTF8));
        rdbt_image_restore->setText(QApplication::translate("clone_dialog_net", "Image einer Festplatte zur\303\274ckschreiben:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("clone_dialog_net", "seconds", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("clone_dialog_net", "minutes", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("clone_dialog_net", "elapsed time:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("clone_dialog_net", "seconds", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("clone_dialog_net", "minutes", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("clone_dialog_net", "remaining time:", 0, QApplication::UnicodeUTF8));
        pushButton_break->setText(QString());
        pushButton_break->setShortcut(QString());
        label_4->setText(QApplication::translate("clone_dialog_net", "already saved:", 0, QApplication::UnicodeUTF8));
        lbl_hd_size->setText(QApplication::translate("clone_dialog_net", "MB", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("clone_dialog_net", "hours", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("clone_dialog_net", "hours", 0, QApplication::UnicodeUTF8));
        pushButton_folder->setText(QApplication::translate("clone_dialog_net", "f", 0, QApplication::UnicodeUTF8));
        lbl_Image->setText(QApplication::translate("clone_dialog_net", "Image name:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("clone_dialog_net", "Network computer  name:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("clone_dialog_net", "Exist Image:", 0, QApplication::UnicodeUTF8));
        pushButton_folder_free->setText(QApplication::translate("clone_dialog_net", "l", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class clone_dialog_net: public Ui_clone_dialog_net {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLONE_DIALOG_NET_H
