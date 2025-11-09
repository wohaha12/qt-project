/********************************************************************************
** Form generated from reading UI file 'chat.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAT_H
#define UI_CHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Chat
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *Show;
    QHBoxLayout *horizontalLayout;
    QLineEdit *Msg;
    QPushButton *Send_PB;

    void setupUi(QWidget *Chat)
    {
        if (Chat->objectName().isEmpty())
            Chat->setObjectName(QString::fromUtf8("Chat"));
        Chat->resize(585, 379);
        verticalLayout = new QVBoxLayout(Chat);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        Show = new QTextEdit(Chat);
        Show->setObjectName(QString::fromUtf8("Show"));
        QFont font;
        font.setFamily(QString::fromUtf8("AcadEref"));
        font.setPointSize(14);
        Show->setFont(font);
        Show->setReadOnly(true);

        verticalLayout->addWidget(Show);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        Msg = new QLineEdit(Chat);
        Msg->setObjectName(QString::fromUtf8("Msg"));
        Msg->setFont(font);

        horizontalLayout->addWidget(Msg);

        Send_PB = new QPushButton(Chat);
        Send_PB->setObjectName(QString::fromUtf8("Send_PB"));
        Send_PB->setFont(font);

        horizontalLayout->addWidget(Send_PB);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Chat);

        QMetaObject::connectSlotsByName(Chat);
    } // setupUi

    void retranslateUi(QWidget *Chat)
    {
        Chat->setWindowTitle(QCoreApplication::translate("Chat", "\350\201\212\345\244\251", nullptr));
        Send_PB->setText(QCoreApplication::translate("Chat", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Chat: public Ui_Chat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_H
