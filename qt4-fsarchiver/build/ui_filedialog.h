/********************************************************************************
** Form generated from reading UI file 'filedialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEDIALOG_H
#define UI_FILEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_FileDialog
{
public:
    QPushButton *cmd_cancel;
    QLabel *label;
    QPushButton *cmd_save;
    QTextEdit *textEdit;

    void setupUi(QDialog *FileDialog)
    {
        if (FileDialog->objectName().isEmpty())
            FileDialog->setObjectName(QString::fromUtf8("FileDialog"));
        FileDialog->resize(557, 441);
        cmd_cancel = new QPushButton(FileDialog);
        cmd_cancel->setObjectName(QString::fromUtf8("cmd_cancel"));
        cmd_cancel->setGeometry(QRect(50, 390, 191, 27));
        label = new QLabel(FileDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 251, 18));
        cmd_save = new QPushButton(FileDialog);
        cmd_save->setObjectName(QString::fromUtf8("cmd_save"));
        cmd_save->setGeometry(QRect(300, 390, 231, 28));
        textEdit = new QTextEdit(FileDialog);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(20, 40, 511, 321));
        textEdit->setLineWidth(1);
        textEdit->setAutoFormatting(QTextEdit::AutoNone);
        textEdit->setTabChangesFocus(true);
        textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        textEdit->setLineWrapColumnOrWidth(0);

        retranslateUi(FileDialog);

        QMetaObject::connectSlotsByName(FileDialog);
    } // setupUi

    void retranslateUi(QDialog *FileDialog)
    {
        FileDialog->setWindowTitle(QApplication::translate("FileDialog", "Description of the backup", 0, QApplication::UnicodeUTF8));
        cmd_cancel->setText(QApplication::translate("FileDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FileDialog", "Description of the backup:", 0, QApplication::UnicodeUTF8));
        cmd_save->setText(QApplication::translate("FileDialog", "Back partition", 0, QApplication::UnicodeUTF8));
        textEdit->setHtml(QApplication::translate("FileDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FileDialog: public Ui_FileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEDIALOG_H
