/********************************************************************************
** Form generated from reading UI file 'cryptfiles.ui'
**
** Created: Mon May 7 17:41:41 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CRYPTFILES_H
#define UI_CRYPTFILES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cryptfiles
{
public:
    QLineEdit *lineEditDestinationPath;
    QLabel *labelDestinationPath;
    QPushButton *pbOpenFolder;
    QPushButton *pbCreate;
    QLabel *labelSourcePath;
    QLineEdit *lineEditSourcePath;
    QPushButton *pushButtonFile;
    QGroupBox *groupBox;
    QRadioButton *rbKey;
    QRadioButton *rbKeyFile;
    QLineEdit *lineEditPass_1;
    QPushButton *pushButtonKeyFile;
    QLabel *labelKey;
    QLineEdit *lineEditPass_2;
    QLabel *labelKey2;
    QPushButton *pushButtonCancel;

    void setupUi(QWidget *cryptfiles)
    {
        if (cryptfiles->objectName().isEmpty())
            cryptfiles->setObjectName(QString::fromUtf8("cryptfiles"));
        cryptfiles->setWindowModality(Qt::ApplicationModal);
        cryptfiles->resize(552, 215);
        lineEditDestinationPath = new QLineEdit(cryptfiles);
        lineEditDestinationPath->setObjectName(QString::fromUtf8("lineEditDestinationPath"));
        lineEditDestinationPath->setGeometry(QRect(120, 40, 311, 31));
        labelDestinationPath = new QLabel(cryptfiles);
        labelDestinationPath->setObjectName(QString::fromUtf8("labelDestinationPath"));
        labelDestinationPath->setGeometry(QRect(10, 40, 101, 31));
        labelDestinationPath->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pbOpenFolder = new QPushButton(cryptfiles);
        pbOpenFolder->setObjectName(QString::fromUtf8("pbOpenFolder"));
        pbOpenFolder->setGeometry(QRect(430, 40, 31, 31));
        pbCreate = new QPushButton(cryptfiles);
        pbCreate->setObjectName(QString::fromUtf8("pbCreate"));
        pbCreate->setGeometry(QRect(200, 180, 81, 31));
        pbCreate->setAutoDefault(false);
        pbCreate->setDefault(true);
        labelSourcePath = new QLabel(cryptfiles);
        labelSourcePath->setObjectName(QString::fromUtf8("labelSourcePath"));
        labelSourcePath->setGeometry(QRect(0, 10, 111, 31));
        labelSourcePath->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditSourcePath = new QLineEdit(cryptfiles);
        lineEditSourcePath->setObjectName(QString::fromUtf8("lineEditSourcePath"));
        lineEditSourcePath->setGeometry(QRect(120, 10, 311, 31));
        pushButtonFile = new QPushButton(cryptfiles);
        pushButtonFile->setObjectName(QString::fromUtf8("pushButtonFile"));
        pushButtonFile->setGeometry(QRect(430, 10, 31, 31));
        groupBox = new QGroupBox(cryptfiles);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(120, 70, 311, 41));
        rbKey = new QRadioButton(groupBox);
        rbKey->setObjectName(QString::fromUtf8("rbKey"));
        rbKey->setGeometry(QRect(6, 8, 111, 31));
        rbKeyFile = new QRadioButton(groupBox);
        rbKeyFile->setObjectName(QString::fromUtf8("rbKeyFile"));
        rbKeyFile->setGeometry(QRect(110, 10, 161, 31));
        lineEditPass_1 = new QLineEdit(cryptfiles);
        lineEditPass_1->setObjectName(QString::fromUtf8("lineEditPass_1"));
        lineEditPass_1->setGeometry(QRect(120, 120, 311, 31));
        pushButtonKeyFile = new QPushButton(cryptfiles);
        pushButtonKeyFile->setObjectName(QString::fromUtf8("pushButtonKeyFile"));
        pushButtonKeyFile->setGeometry(QRect(430, 120, 31, 31));
        labelKey = new QLabel(cryptfiles);
        labelKey->setObjectName(QString::fromUtf8("labelKey"));
        labelKey->setGeometry(QRect(0, 120, 111, 31));
        labelKey->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditPass_2 = new QLineEdit(cryptfiles);
        lineEditPass_2->setObjectName(QString::fromUtf8("lineEditPass_2"));
        lineEditPass_2->setGeometry(QRect(120, 149, 311, 31));
        labelKey2 = new QLabel(cryptfiles);
        labelKey2->setObjectName(QString::fromUtf8("labelKey2"));
        labelKey2->setGeometry(QRect(4, 149, 111, 31));
        labelKey2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButtonCancel = new QPushButton(cryptfiles);
        pushButtonCancel->setObjectName(QString::fromUtf8("pushButtonCancel"));
        pushButtonCancel->setGeometry(QRect(280, 180, 75, 31));
        QWidget::setTabOrder(lineEditDestinationPath, pbOpenFolder);
        QWidget::setTabOrder(pbOpenFolder, pbCreate);

        retranslateUi(cryptfiles);

        QMetaObject::connectSlotsByName(cryptfiles);
    } // setupUi

    void retranslateUi(QWidget *cryptfiles)
    {
        cryptfiles->setWindowTitle(QString());
        labelDestinationPath->setText(QApplication::translate("cryptfiles", "destination path", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pbOpenFolder->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        pbOpenFolder->setText(QString());
        pbCreate->setText(QApplication::translate("cryptfiles", "c&reate", 0, QApplication::UnicodeUTF8));
        labelSourcePath->setText(QApplication::translate("cryptfiles", "source path", 0, QApplication::UnicodeUTF8));
        pushButtonFile->setText(QString());
        groupBox->setTitle(QString());
        rbKey->setText(QApplication::translate("cryptfiles", "passphrase", 0, QApplication::UnicodeUTF8));
        rbKeyFile->setText(QApplication::translate("cryptfiles", "passphrase from keyfile", 0, QApplication::UnicodeUTF8));
        pushButtonKeyFile->setText(QString());
        labelKey->setText(QApplication::translate("cryptfiles", "key", 0, QApplication::UnicodeUTF8));
        labelKey2->setText(QApplication::translate("cryptfiles", "repeat key", 0, QApplication::UnicodeUTF8));
        pushButtonCancel->setText(QApplication::translate("cryptfiles", "&cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class cryptfiles: public Ui_cryptfiles {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CRYPTFILES_H