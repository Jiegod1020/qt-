/********************************************************************************
** Form generated from reading UI file 'friend.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIEND_H
#define UI_FRIEND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Friend
{
public:
    QListView *listView;
    QPushButton *delFriend_PB;
    QPushButton *chat_PB;
    QPushButton *findUser_PB;
    QPushButton *onlineUser_PB;
    QPushButton *flushFriend_PB;

    void setupUi(QWidget *Friend)
    {
        if (Friend->objectName().isEmpty())
            Friend->setObjectName(QString::fromUtf8("Friend"));
        Friend->resize(400, 300);
        listView = new QListView(Friend);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(10, 10, 281, 281));
        delFriend_PB = new QPushButton(Friend);
        delFriend_PB->setObjectName(QString::fromUtf8("delFriend_PB"));
        delFriend_PB->setGeometry(QRect(300, 40, 91, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(12);
        delFriend_PB->setFont(font);
        chat_PB = new QPushButton(Friend);
        chat_PB->setObjectName(QString::fromUtf8("chat_PB"));
        chat_PB->setGeometry(QRect(300, 80, 91, 31));
        chat_PB->setFont(font);
        findUser_PB = new QPushButton(Friend);
        findUser_PB->setObjectName(QString::fromUtf8("findUser_PB"));
        findUser_PB->setGeometry(QRect(300, 170, 91, 31));
        findUser_PB->setFont(font);
        onlineUser_PB = new QPushButton(Friend);
        onlineUser_PB->setObjectName(QString::fromUtf8("onlineUser_PB"));
        onlineUser_PB->setGeometry(QRect(300, 230, 91, 31));
        onlineUser_PB->setFont(font);
        flushFriend_PB = new QPushButton(Friend);
        flushFriend_PB->setObjectName(QString::fromUtf8("flushFriend_PB"));
        flushFriend_PB->setGeometry(QRect(300, 0, 91, 31));
        flushFriend_PB->setFont(font);

        retranslateUi(Friend);

        QMetaObject::connectSlotsByName(Friend);
    } // setupUi

    void retranslateUi(QWidget *Friend)
    {
        Friend->setWindowTitle(QCoreApplication::translate("Friend", "Form", nullptr));
        delFriend_PB->setText(QCoreApplication::translate("Friend", "\345\210\240\351\231\244\345\245\275\345\217\213", nullptr));
        chat_PB->setText(QCoreApplication::translate("Friend", "\350\201\212\345\244\251", nullptr));
        findUser_PB->setText(QCoreApplication::translate("Friend", "\346\237\245\346\211\276\347\224\250\346\210\267", nullptr));
        onlineUser_PB->setText(QCoreApplication::translate("Friend", "\345\234\250\347\272\277\347\224\250\346\210\267", nullptr));
        flushFriend_PB->setText(QCoreApplication::translate("Friend", "\345\210\267\346\226\260\345\245\275\345\217\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Friend: public Ui_Friend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIEND_H
