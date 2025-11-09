/********************************************************************************
** Form generated from reading UI file 'index.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INDEX_H
#define UI_INDEX_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <friend.h>
#include "file.h"

QT_BEGIN_NAMESPACE

class Ui_Index
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *friend_PB;
    QPushButton *file_PB;
    QStackedWidget *stackedWidget;
    Friend *friendPage;
    File *filePage;

    void setupUi(QWidget *Index)
    {
        if (Index->objectName().isEmpty())
            Index->setObjectName(QString::fromUtf8("Index"));
        Index->resize(700, 500);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/shouye.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        Index->setWindowIcon(icon);
        gridLayout = new QGridLayout(Index);
        gridLayout->setSpacing(4);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(2, 0, 2, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        friend_PB = new QPushButton(Index);
        friend_PB->setObjectName(QString::fromUtf8("friend_PB"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(friend_PB->sizePolicy().hasHeightForWidth());
        friend_PB->setSizePolicy(sizePolicy);
        friend_PB->setMaximumSize(QSize(16777215, 16777214));
        QFont font;
        font.setFamily(QString::fromUtf8("AcadEref"));
        font.setPointSize(16);
        friend_PB->setFont(font);
        friend_PB->setIconSize(QSize(16, 11));

        verticalLayout->addWidget(friend_PB);

        file_PB = new QPushButton(Index);
        file_PB->setObjectName(QString::fromUtf8("file_PB"));
        sizePolicy.setHeightForWidth(file_PB->sizePolicy().hasHeightForWidth());
        file_PB->setSizePolicy(sizePolicy);
        file_PB->setFont(font);
        file_PB->setIconSize(QSize(16, 11));

        verticalLayout->addWidget(file_PB);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        stackedWidget = new QStackedWidget(Index);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        friendPage = new Friend();
        friendPage->setObjectName(QString::fromUtf8("friendPage"));
        stackedWidget->addWidget(friendPage);
        filePage = new File();
        filePage->setObjectName(QString::fromUtf8("filePage"));
        stackedWidget->addWidget(filePage);

        gridLayout->addWidget(stackedWidget, 0, 1, 1, 1);


        retranslateUi(Index);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Index);
    } // setupUi

    void retranslateUi(QWidget *Index)
    {
        Index->setWindowTitle(QCoreApplication::translate("Index", "\351\246\226\351\241\265", nullptr));
        friend_PB->setText(QCoreApplication::translate("Index", "\345\245\275\n"
"\345\217\213", nullptr));
        file_PB->setText(QCoreApplication::translate("Index", "\346\226\207\n"
"\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Index: public Ui_Index {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INDEX_H
