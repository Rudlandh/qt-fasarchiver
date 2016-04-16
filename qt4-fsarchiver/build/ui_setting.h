/********************************************************************************
** Form generated from reading UI file 'setting.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTING_H
#define UI_SETTING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_setting_dialog
{
public:
    QFrame *frame;
    QLabel *label;
    QComboBox *cmb_Kerne;
    QLabel *label_2;
    QComboBox *cmb_zip;
    QLabel *label_3;
    QComboBox *cmb_language;
    QCheckBox *chk_file;
    QCheckBox *chk_tip;
    QCheckBox *chk_folder;
    QCheckBox *chk_key;
    QCheckBox *chk_split;
    QCheckBox *chk_password;
    QCheckBox *chk_datesave;
    QPushButton *cmd_save;
    QPushButton *cmd_cancel;
    QCheckBox *chk_pbr;
    QComboBox *cmb_network;
    QLabel *label_4;
    QCheckBox *chk_ssh;
    QCheckBox *chk_sshfs;
    QCheckBox *chk_prg;
    QCheckBox *chk_hidden;

    void setupUi(QDialog *setting_dialog)
    {
        if (setting_dialog->objectName().isEmpty())
            setting_dialog->setObjectName(QString::fromUtf8("setting_dialog"));
        setting_dialog->resize(473, 808);
        frame = new QFrame(setting_dialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 10, 451, 701));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 17, 225, 51));
        cmb_Kerne = new QComboBox(frame);
        cmb_Kerne->setObjectName(QString::fromUtf8("cmb_Kerne"));
        cmb_Kerne->setGeometry(QRect(241, 29, 200, 27));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 81, 225, 34));
        cmb_zip = new QComboBox(frame);
        cmb_zip->setObjectName(QString::fromUtf8("cmb_zip"));
        cmb_zip->setGeometry(QRect(241, 84, 200, 27));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 128, 225, 27));
        cmb_language = new QComboBox(frame);
        cmb_language->setObjectName(QString::fromUtf8("cmb_language"));
        cmb_language->setGeometry(QRect(241, 128, 200, 27));
        chk_file = new QCheckBox(frame);
        chk_file->setObjectName(QString::fromUtf8("chk_file"));
        chk_file->setGeometry(QRect(10, 168, 441, 39));
        chk_tip = new QCheckBox(frame);
        chk_tip->setObjectName(QString::fromUtf8("chk_tip"));
        chk_tip->setGeometry(QRect(10, 198, 441, 39));
        chk_folder = new QCheckBox(frame);
        chk_folder->setObjectName(QString::fromUtf8("chk_folder"));
        chk_folder->setGeometry(QRect(10, 293, 441, 39));
        chk_key = new QCheckBox(frame);
        chk_key->setObjectName(QString::fromUtf8("chk_key"));
        chk_key->setGeometry(QRect(10, 228, 441, 39));
        chk_split = new QCheckBox(frame);
        chk_split->setObjectName(QString::fromUtf8("chk_split"));
        chk_split->setGeometry(QRect(10, 328, 441, 39));
        chk_password = new QCheckBox(frame);
        chk_password->setObjectName(QString::fromUtf8("chk_password"));
        chk_password->setGeometry(QRect(10, 388, 441, 39));
        chk_datesave = new QCheckBox(frame);
        chk_datesave->setObjectName(QString::fromUtf8("chk_datesave"));
        chk_datesave->setGeometry(QRect(10, 418, 441, 39));
        cmd_save = new QPushButton(frame);
        cmd_save->setObjectName(QString::fromUtf8("cmd_save"));
        cmd_save->setGeometry(QRect(10, 610, 431, 27));
        cmd_cancel = new QPushButton(frame);
        cmd_cancel->setObjectName(QString::fromUtf8("cmd_cancel"));
        cmd_cancel->setGeometry(QRect(10, 650, 431, 27));
        chk_pbr = new QCheckBox(frame);
        chk_pbr->setObjectName(QString::fromUtf8("chk_pbr"));
        chk_pbr->setGeometry(QRect(10, 258, 441, 39));
        cmb_network = new QComboBox(frame);
        cmb_network->setObjectName(QString::fromUtf8("cmb_network"));
        cmb_network->setGeometry(QRect(240, 460, 200, 27));
        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 460, 225, 27));
        chk_ssh = new QCheckBox(frame);
        chk_ssh->setObjectName(QString::fromUtf8("chk_ssh"));
        chk_ssh->setGeometry(QRect(10, 490, 441, 39));
        chk_sshfs = new QCheckBox(frame);
        chk_sshfs->setObjectName(QString::fromUtf8("chk_sshfs"));
        chk_sshfs->setGeometry(QRect(10, 530, 441, 39));
        chk_prg = new QCheckBox(frame);
        chk_prg->setObjectName(QString::fromUtf8("chk_prg"));
        chk_prg->setGeometry(QRect(10, 570, 441, 39));
        chk_hidden = new QCheckBox(frame);
        chk_hidden->setObjectName(QString::fromUtf8("chk_hidden"));
        chk_hidden->setGeometry(QRect(10, 368, 401, 22));
        QWidget::setTabOrder(cmb_Kerne, cmb_zip);
        QWidget::setTabOrder(cmb_zip, cmb_language);
        QWidget::setTabOrder(cmb_language, chk_file);
        QWidget::setTabOrder(chk_file, chk_tip);
        QWidget::setTabOrder(chk_tip, chk_key);
        QWidget::setTabOrder(chk_key, chk_folder);
        QWidget::setTabOrder(chk_folder, chk_split);
        QWidget::setTabOrder(chk_split, chk_password);
        QWidget::setTabOrder(chk_password, chk_datesave);
        QWidget::setTabOrder(chk_datesave, cmd_save);
        QWidget::setTabOrder(cmd_save, cmd_cancel);

        retranslateUi(setting_dialog);

        QMetaObject::connectSlotsByName(setting_dialog);
    } // setupUi

    void retranslateUi(QDialog *setting_dialog)
    {
        setting_dialog->setWindowTitle(QApplication::translate("setting_dialog", "Basic settings", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("setting_dialog", "Number of processors\n"
"cores, for fsarchiver\n"
"should be used:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("setting_dialog", "Standard\n"
"compression:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("setting_dialog", "Language:", 0, QApplication::UnicodeUTF8));
        chk_file->setText(QApplication::translate("setting_dialog", "Existing files by default override", 0, QApplication::UnicodeUTF8));
        chk_tip->setText(QApplication::translate("setting_dialog", "Backup note enable by default", 0, QApplication::UnicodeUTF8));
        chk_folder->setText(QApplication::translate("setting_dialog", "Directories by default write \n"
"back to the original location", 0, QApplication::UnicodeUTF8));
        chk_key->setText(QApplication::translate("setting_dialog", "Enable encryption by default", 0, QApplication::UnicodeUTF8));
        chk_split->setText(QApplication::translate("setting_dialog", "Archive file size divided on DVD", 0, QApplication::UnicodeUTF8));
        chk_password->setText(QApplication::translate("setting_dialog", "Show passwords", 0, QApplication::UnicodeUTF8));
        chk_datesave->setText(QApplication::translate("setting_dialog", "Network input data save", 0, QApplication::UnicodeUTF8));
        cmd_save->setText(QApplication::translate("setting_dialog", "Save Settings", 0, QApplication::UnicodeUTF8));
        cmd_cancel->setText(QApplication::translate("setting_dialog", "Exit", 0, QApplication::UnicodeUTF8));
        chk_pbr->setText(QApplication::translate("setting_dialog", "Safe PBR by default", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("setting_dialog", "Network protocol:", 0, QApplication::UnicodeUTF8));
        chk_ssh->setText(QApplication::translate("setting_dialog", "Show Note for accessing network computer", 0, QApplication::UnicodeUTF8));
        chk_sshfs->setText(QApplication::translate("setting_dialog", "Show  Note to mount the network computer", 0, QApplication::UnicodeUTF8));
        chk_prg->setText(QApplication::translate("setting_dialog", "Show  Note about the programm qt4-fsarchiver", 0, QApplication::UnicodeUTF8));
        chk_hidden->setText(QApplication::translate("setting_dialog", "Show hidden directories and files", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class setting_dialog: public Ui_setting_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTING_H
