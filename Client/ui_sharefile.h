/********************************************************************************
** Form generated from reading UI file 'sharefile.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAREFILE_H
#define UI_SHAREFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShareFile
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *friend_LW;
    QVBoxLayout *verticalLayout;
    QPushButton *allSelect_PB;
    QPushButton *cancleSelec_PB;
    QSpacerItem *verticalSpacer;
    QPushButton *confirm_PB;

    void setupUi(QWidget *ShareFile)
    {
        if (ShareFile->objectName().isEmpty())
            ShareFile->setObjectName(QString::fromUtf8("ShareFile"));
        ShareFile->resize(629, 463);
        horizontalLayout = new QHBoxLayout(ShareFile);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        friend_LW = new QListWidget(ShareFile);
        friend_LW->setObjectName(QString::fromUtf8("friend_LW"));
        friend_LW->setSelectionMode(QAbstractItemView::MultiSelection);

        horizontalLayout->addWidget(friend_LW);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        allSelect_PB = new QPushButton(ShareFile);
        allSelect_PB->setObjectName(QString::fromUtf8("allSelect_PB"));
        QFont font;
        font.setFamily(QString::fromUtf8("AcadEref"));
        font.setPointSize(12);
        allSelect_PB->setFont(font);

        verticalLayout->addWidget(allSelect_PB);

        cancleSelec_PB = new QPushButton(ShareFile);
        cancleSelec_PB->setObjectName(QString::fromUtf8("cancleSelec_PB"));
        cancleSelec_PB->setFont(font);

        verticalLayout->addWidget(cancleSelec_PB);

        verticalSpacer = new QSpacerItem(28, 168, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        confirm_PB = new QPushButton(ShareFile);
        confirm_PB->setObjectName(QString::fromUtf8("confirm_PB"));
        confirm_PB->setFont(font);

        verticalLayout->addWidget(confirm_PB);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(ShareFile);

        QMetaObject::connectSlotsByName(ShareFile);
    } // setupUi

    void retranslateUi(QWidget *ShareFile)
    {
        ShareFile->setWindowTitle(QCoreApplication::translate("ShareFile", "Form", nullptr));
        allSelect_PB->setText(QCoreApplication::translate("ShareFile", "\345\205\250\351\200\211", nullptr));
        cancleSelec_PB->setText(QCoreApplication::translate("ShareFile", "\345\217\226\346\266\210\351\200\211\344\270\255", nullptr));
        confirm_PB->setText(QCoreApplication::translate("ShareFile", "\347\241\256\350\256\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShareFile: public Ui_ShareFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAREFILE_H
