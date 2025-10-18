/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QLabel *label;
    QLineEdit *name_LE;
    QLineEdit *pwd_LE;
    QLabel *label_2;
    QPushButton *regist_PB;
    QPushButton *login_PB;

    void setupUi(QWidget *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName(QString::fromUtf8("Client"));
        Client->resize(497, 292);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(16);
        Client->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../\347\231\273\345\275\225.png"), QSize(), QIcon::Normal, QIcon::Off);
        Client->setWindowIcon(icon);
        label = new QLabel(Client);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 30, 91, 61));
        name_LE = new QLineEdit(Client);
        name_LE->setObjectName(QString::fromUtf8("name_LE"));
        name_LE->setGeometry(QRect(180, 40, 231, 41));
        pwd_LE = new QLineEdit(Client);
        pwd_LE->setObjectName(QString::fromUtf8("pwd_LE"));
        pwd_LE->setGeometry(QRect(180, 110, 231, 41));
        label_2 = new QLabel(Client);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(70, 100, 91, 61));
        regist_PB = new QPushButton(Client);
        regist_PB->setObjectName(QString::fromUtf8("regist_PB"));
        regist_PB->setGeometry(QRect(70, 200, 111, 51));
        login_PB = new QPushButton(Client);
        login_PB->setObjectName(QString::fromUtf8("login_PB"));
        login_PB->setGeometry(QRect(290, 200, 111, 51));

        retranslateUi(Client);

        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QWidget *Client)
    {
        Client->setWindowTitle(QCoreApplication::translate("Client", "\347\231\273\345\275\225", nullptr));
        label->setText(QCoreApplication::translate("Client", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        name_LE->setText(QString());
        pwd_LE->setText(QString());
        label_2->setText(QCoreApplication::translate("Client", "\345\257\206\347\240\201   \357\274\232", nullptr));
        regist_PB->setText(QCoreApplication::translate("Client", "\346\263\250\345\206\214", nullptr));
        login_PB->setText(QCoreApplication::translate("Client", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
