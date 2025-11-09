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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_File
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QPushButton *delDir_PB;
    QPushButton *flushFile_PB;
    QPushButton *mvFile_PB;
    QPushButton *shareFile_PB;
    QPushButton *mkDir_PB;
    QPushButton *rename_PB;
    QPushButton *delFile_PB;
    QPushButton *uploadFile_PB;
    QPushButton *downloadFile_PB;
    QPushButton *return_PB;
    QListWidget *listWidget;

    void setupUi(QWidget *File)
    {
        if (File->objectName().isEmpty())
            File->setObjectName(QString::fromUtf8("File"));
        File->resize(737, 482);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(File->sizePolicy().hasHeightForWidth());
        File->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(File);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(0);
        gridLayout_2->setVerticalSpacing(2);
        gridLayout_2->setContentsMargins(2, 0, 2, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        delDir_PB = new QPushButton(File);
        delDir_PB->setObjectName(QString::fromUtf8("delDir_PB"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(delDir_PB->sizePolicy().hasHeightForWidth());
        delDir_PB->setSizePolicy(sizePolicy1);
        QFont font;
        font.setFamily(QString::fromUtf8("AcadEref"));
        font.setPointSize(12);
        delDir_PB->setFont(font);

        gridLayout->addWidget(delDir_PB, 2, 0, 1, 1);

        flushFile_PB = new QPushButton(File);
        flushFile_PB->setObjectName(QString::fromUtf8("flushFile_PB"));
        sizePolicy1.setHeightForWidth(flushFile_PB->sizePolicy().hasHeightForWidth());
        flushFile_PB->setSizePolicy(sizePolicy1);
        flushFile_PB->setFont(font);

        gridLayout->addWidget(flushFile_PB, 0, 0, 1, 1);

        mvFile_PB = new QPushButton(File);
        mvFile_PB->setObjectName(QString::fromUtf8("mvFile_PB"));
        sizePolicy1.setHeightForWidth(mvFile_PB->sizePolicy().hasHeightForWidth());
        mvFile_PB->setSizePolicy(sizePolicy1);
        mvFile_PB->setFont(font);

        gridLayout->addWidget(mvFile_PB, 4, 0, 1, 1);

        shareFile_PB = new QPushButton(File);
        shareFile_PB->setObjectName(QString::fromUtf8("shareFile_PB"));
        sizePolicy1.setHeightForWidth(shareFile_PB->sizePolicy().hasHeightForWidth());
        shareFile_PB->setSizePolicy(sizePolicy1);
        shareFile_PB->setFont(font);

        gridLayout->addWidget(shareFile_PB, 2, 1, 1, 1);

        mkDir_PB = new QPushButton(File);
        mkDir_PB->setObjectName(QString::fromUtf8("mkDir_PB"));
        sizePolicy1.setHeightForWidth(mkDir_PB->sizePolicy().hasHeightForWidth());
        mkDir_PB->setSizePolicy(sizePolicy1);
        mkDir_PB->setFont(font);

        gridLayout->addWidget(mkDir_PB, 1, 0, 1, 1);

        rename_PB = new QPushButton(File);
        rename_PB->setObjectName(QString::fromUtf8("rename_PB"));
        sizePolicy1.setHeightForWidth(rename_PB->sizePolicy().hasHeightForWidth());
        rename_PB->setSizePolicy(sizePolicy1);
        rename_PB->setFont(font);

        gridLayout->addWidget(rename_PB, 1, 1, 1, 1);

        delFile_PB = new QPushButton(File);
        delFile_PB->setObjectName(QString::fromUtf8("delFile_PB"));
        sizePolicy1.setHeightForWidth(delFile_PB->sizePolicy().hasHeightForWidth());
        delFile_PB->setSizePolicy(sizePolicy1);
        delFile_PB->setFont(font);

        gridLayout->addWidget(delFile_PB, 3, 0, 1, 1);

        uploadFile_PB = new QPushButton(File);
        uploadFile_PB->setObjectName(QString::fromUtf8("uploadFile_PB"));
        sizePolicy1.setHeightForWidth(uploadFile_PB->sizePolicy().hasHeightForWidth());
        uploadFile_PB->setSizePolicy(sizePolicy1);
        uploadFile_PB->setFont(font);

        gridLayout->addWidget(uploadFile_PB, 4, 1, 1, 1);

        downloadFile_PB = new QPushButton(File);
        downloadFile_PB->setObjectName(QString::fromUtf8("downloadFile_PB"));
        sizePolicy1.setHeightForWidth(downloadFile_PB->sizePolicy().hasHeightForWidth());
        downloadFile_PB->setSizePolicy(sizePolicy1);
        downloadFile_PB->setFont(font);

        gridLayout->addWidget(downloadFile_PB, 3, 1, 1, 1);

        return_PB = new QPushButton(File);
        return_PB->setObjectName(QString::fromUtf8("return_PB"));
        sizePolicy1.setHeightForWidth(return_PB->sizePolicy().hasHeightForWidth());
        return_PB->setSizePolicy(sizePolicy1);
        return_PB->setFont(font);

        gridLayout->addWidget(return_PB, 0, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 1, 1, 1);

        listWidget = new QListWidget(File);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout_2->addWidget(listWidget, 0, 0, 1, 1);


        retranslateUi(File);

        QMetaObject::connectSlotsByName(File);
    } // setupUi

    void retranslateUi(QWidget *File)
    {
        File->setWindowTitle(QCoreApplication::translate("File", "Form", nullptr));
        delDir_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266\345\244\271", nullptr));
        flushFile_PB->setText(QCoreApplication::translate("File", "\345\210\267\346\226\260\346\226\207\344\273\266", nullptr));
        mvFile_PB->setText(QCoreApplication::translate("File", "\347\247\273\345\212\250\346\226\207\344\273\266", nullptr));
        shareFile_PB->setText(QCoreApplication::translate("File", "\345\210\206\344\272\253\346\226\207\344\273\266", nullptr));
        mkDir_PB->setText(QCoreApplication::translate("File", "\345\210\233\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        rename_PB->setText(QCoreApplication::translate("File", "\351\207\215\345\221\275\345\220\215", nullptr));
        delFile_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
        uploadFile_PB->setText(QCoreApplication::translate("File", "\344\270\212\344\274\240\346\226\207\344\273\266", nullptr));
        downloadFile_PB->setText(QCoreApplication::translate("File", "\344\270\213\350\275\275\346\226\207\344\273\266", nullptr));
        return_PB->setText(QCoreApplication::translate("File", "\350\277\224\345\233\236\344\270\212\344\270\200\347\272\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class File: public Ui_File {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_H
