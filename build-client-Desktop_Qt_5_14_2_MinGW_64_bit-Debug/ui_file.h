/********************************************************************************
** Form generated from reading UI file 'file.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_H
#define UI_FILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_File
{
public:
    QWidget *widget;
    QFormLayout *formLayout_2;
    QListView *listView;
    QFormLayout *formLayout;
    QPushButton *flushFile_PB;
    QPushButton *return_PB;
    QPushButton *mkDir_PB;
    QPushButton *renameFile_PB;
    QPushButton *delDir_PB;
    QPushButton *shareFile_PB;
    QPushButton *delFile_PB;
    QPushButton *downloadFile_PB;
    QPushButton *mvFile_PB;
    QPushButton *uploadFile_PB;

    void setupUi(QWidget *File)
    {
        if (File->objectName().isEmpty())
            File->setObjectName(QString::fromUtf8("File"));
        File->resize(508, 323);
        widget = new QWidget(File);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(9, 9, 491, 311));
        formLayout_2 = new QFormLayout(widget);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        listView = new QListView(widget);
        listView->setObjectName(QString::fromUtf8("listView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
        listView->setSizePolicy(sizePolicy);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, listView);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        flushFile_PB = new QPushButton(widget);
        flushFile_PB->setObjectName(QString::fromUtf8("flushFile_PB"));
        sizePolicy.setHeightForWidth(flushFile_PB->sizePolicy().hasHeightForWidth());
        flushFile_PB->setSizePolicy(sizePolicy);

        formLayout->setWidget(0, QFormLayout::LabelRole, flushFile_PB);

        return_PB = new QPushButton(widget);
        return_PB->setObjectName(QString::fromUtf8("return_PB"));
        sizePolicy.setHeightForWidth(return_PB->sizePolicy().hasHeightForWidth());
        return_PB->setSizePolicy(sizePolicy);

        formLayout->setWidget(0, QFormLayout::FieldRole, return_PB);

        mkDir_PB = new QPushButton(widget);
        mkDir_PB->setObjectName(QString::fromUtf8("mkDir_PB"));
        sizePolicy.setHeightForWidth(mkDir_PB->sizePolicy().hasHeightForWidth());
        mkDir_PB->setSizePolicy(sizePolicy);

        formLayout->setWidget(1, QFormLayout::LabelRole, mkDir_PB);

        renameFile_PB = new QPushButton(widget);
        renameFile_PB->setObjectName(QString::fromUtf8("renameFile_PB"));
        sizePolicy.setHeightForWidth(renameFile_PB->sizePolicy().hasHeightForWidth());
        renameFile_PB->setSizePolicy(sizePolicy);

        formLayout->setWidget(1, QFormLayout::FieldRole, renameFile_PB);

        delDir_PB = new QPushButton(widget);
        delDir_PB->setObjectName(QString::fromUtf8("delDir_PB"));
        sizePolicy.setHeightForWidth(delDir_PB->sizePolicy().hasHeightForWidth());
        delDir_PB->setSizePolicy(sizePolicy);

        formLayout->setWidget(2, QFormLayout::LabelRole, delDir_PB);

        shareFile_PB = new QPushButton(widget);
        shareFile_PB->setObjectName(QString::fromUtf8("shareFile_PB"));
        sizePolicy.setHeightForWidth(shareFile_PB->sizePolicy().hasHeightForWidth());
        shareFile_PB->setSizePolicy(sizePolicy);

        formLayout->setWidget(2, QFormLayout::FieldRole, shareFile_PB);

        delFile_PB = new QPushButton(widget);
        delFile_PB->setObjectName(QString::fromUtf8("delFile_PB"));
        sizePolicy.setHeightForWidth(delFile_PB->sizePolicy().hasHeightForWidth());
        delFile_PB->setSizePolicy(sizePolicy);

        formLayout->setWidget(3, QFormLayout::LabelRole, delFile_PB);

        downloadFile_PB = new QPushButton(widget);
        downloadFile_PB->setObjectName(QString::fromUtf8("downloadFile_PB"));
        sizePolicy.setHeightForWidth(downloadFile_PB->sizePolicy().hasHeightForWidth());
        downloadFile_PB->setSizePolicy(sizePolicy);

        formLayout->setWidget(3, QFormLayout::FieldRole, downloadFile_PB);

        mvFile_PB = new QPushButton(widget);
        mvFile_PB->setObjectName(QString::fromUtf8("mvFile_PB"));
        sizePolicy.setHeightForWidth(mvFile_PB->sizePolicy().hasHeightForWidth());
        mvFile_PB->setSizePolicy(sizePolicy);

        formLayout->setWidget(4, QFormLayout::LabelRole, mvFile_PB);

        uploadFile_PB = new QPushButton(widget);
        uploadFile_PB->setObjectName(QString::fromUtf8("uploadFile_PB"));
        sizePolicy.setHeightForWidth(uploadFile_PB->sizePolicy().hasHeightForWidth());
        uploadFile_PB->setSizePolicy(sizePolicy);

        formLayout->setWidget(4, QFormLayout::FieldRole, uploadFile_PB);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, formLayout);


        retranslateUi(File);

        QMetaObject::connectSlotsByName(File);
    } // setupUi

    void retranslateUi(QWidget *File)
    {
        File->setWindowTitle(QCoreApplication::translate("File", "Form", nullptr));
        flushFile_PB->setText(QCoreApplication::translate("File", "\345\210\267\346\226\260\346\226\207\344\273\266", nullptr));
        return_PB->setText(QCoreApplication::translate("File", "\350\277\224\345\233\236\344\270\212\344\270\200\347\272\247", nullptr));
        mkDir_PB->setText(QCoreApplication::translate("File", "\345\210\233\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        renameFile_PB->setText(QCoreApplication::translate("File", "\351\207\215\345\221\275\345\220\215", nullptr));
        delDir_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266\345\244\271", nullptr));
        shareFile_PB->setText(QCoreApplication::translate("File", "\345\210\206\344\272\253\346\226\207\344\273\266", nullptr));
        delFile_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
        downloadFile_PB->setText(QCoreApplication::translate("File", "\344\270\213\350\275\275\346\226\207\344\273\266", nullptr));
        mvFile_PB->setText(QCoreApplication::translate("File", "\347\247\273\345\212\250\346\226\207\344\273\266", nullptr));
        uploadFile_PB->setText(QCoreApplication::translate("File", "\344\270\212\344\274\240\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class File: public Ui_File {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_H
