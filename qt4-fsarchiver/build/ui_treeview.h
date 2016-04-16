/********************************************************************************
** Form generated from reading UI file 'treeview.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TREEVIEW_H
#define UI_TREEVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTreeView>

QT_BEGIN_NAMESPACE

class Ui_treeview_dialog
{
public:
    QFrame *frame;
    QPushButton *cmd_cancel;
    QPushButton *cmd_save;
    QTreeView *treeView;
    QLabel *label;
    QPushButton *pushButton_treeview;

    void setupUi(QDialog *treeview_dialog)
    {
        if (treeview_dialog->objectName().isEmpty())
            treeview_dialog->setObjectName(QString::fromUtf8("treeview_dialog"));
        treeview_dialog->resize(473, 415);
        frame = new QFrame(treeview_dialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 10, 451, 391));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        cmd_cancel = new QPushButton(frame);
        cmd_cancel->setObjectName(QString::fromUtf8("cmd_cancel"));
        cmd_cancel->setGeometry(QRect(10, 350, 423, 27));
        cmd_cancel->setAutoDefault(false);
        cmd_cancel->setFlat(false);
        cmd_save = new QPushButton(frame);
        cmd_save->setObjectName(QString::fromUtf8("cmd_save"));
        cmd_save->setGeometry(QRect(10, 320, 423, 27));
        cmd_save->setAutoDefault(false);
        cmd_save->setDefault(true);
        treeView = new QTreeView(frame);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setGeometry(QRect(10, 40, 421, 271));
        treeView->header()->setDefaultSectionSize(400);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 411, 17));
        pushButton_treeview = new QPushButton(frame);
        pushButton_treeview->setObjectName(QString::fromUtf8("pushButton_treeview"));
        pushButton_treeview->setGeometry(QRect(50, 320, 31, 16));
        cmd_cancel->raise();
        treeView->raise();
        label->raise();
        pushButton_treeview->raise();
        cmd_save->raise();
        QWidget::setTabOrder(cmd_cancel, cmd_save);

        retranslateUi(treeview_dialog);
        QObject::connect(treeView, SIGNAL(clicked(QModelIndex)), pushButton_treeview, SLOT(click()));

        QMetaObject::connectSlotsByName(treeview_dialog);
    } // setupUi

   void retranslateUi(QDialog *treeview_dialog)
    {
	int w,h, mw, mh, cw, ch;
        QWidget *d = QApplication::desktop();
        w=d->width();                   // returns screen width
        h=d->height();                  // returns screen height
        //qDebug() << "Bildschrimbreite";
        //qDebug() << h;
        //Center position calculation
        mw = treeview_dialog->width();
        mh = treeview_dialog->height();
        cw = (w/2) - (mw/2);
        ch = (h/2) - (mh/2); 
        treeview_dialog->setGeometry(QRect(cw, ch, 473, 415));

        treeview_dialog->setWindowTitle(QApplication::translate("treeview_dialog", "Select restore file", 0, QApplication::UnicodeUTF8));
        cmd_cancel->setText(QApplication::translate("treeview_dialog", "Cancel", 0, QApplication::UnicodeUTF8));
        cmd_save->setText(QApplication::translate("treeview_dialog", "Next", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("treeview_dialog", "Existing restore files:", 0, QApplication::UnicodeUTF8));
        pushButton_treeview->setText(QApplication::translate("treeview_dialog", "a", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class treeview_dialog: public Ui_treeview_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TREEVIEW_H
