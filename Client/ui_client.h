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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *pwd_LE;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *name_LE;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *regist_PB;
    QSpacerItem *horizontalSpacer;
    QPushButton *login_PB;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName(QString::fromUtf8("Client"));
        Client->resize(400, 500);
        Client->setMinimumSize(QSize(400, 500));
        Client->setMaximumSize(QSize(400, 500));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/regist.png"), QSize(), QIcon::Normal, QIcon::Off);
        Client->setWindowIcon(icon);
        Client->setStyleSheet(QString::fromUtf8("/* QQ\351\243\216\346\240\274Qt\346\240\267\345\274\217\350\241\250 */\n"
"QMainWindow, QDialog {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"                                stop:0 #1E90FF, stop:0.5 #00BFFF, stop:1 #87CEEB);\n"
"    font-family: \"Microsoft YaHei\", \"Segoe UI\", sans-serif;\n"
"}\n"
"\n"
"QWidget#centralWidget {\n"
"    background: rgba(255, 255, 255, 0.95);\n"
"    border-radius: 15px;\n"
"    margin: 20px;\n"
"    border: 1px solid #E0E0E0;\n"
"}\n"
"\n"
"QLabel {\n"
"    color: #333333;\n"
"    font-size: 14px;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QLineEdit {\n"
"    border: 2px solid #E0E0E0;\n"
"    border-radius: 20px;\n"
"    padding: 10px 15px;\n"
"    font-size: 14px;\n"
"    background: #FFFFFF;\n"
"    selection-background-color: #1E90FF;\n"
"    min-height: 25px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border-color: #1E90FF;\n"
"    background: #F0F8FF;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"               "
                        "                 stop:0 #4DA6FF, stop:1 #1E90FF);\n"
"    border: none;\n"
"    border-radius: 20px;\n"
"    color: white;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    padding: 10px 20px;\n"
"    min-height: 35px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #5CB0FF, stop:1 #2D9CFF);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #3D96FF, stop:1 #0D80FF);\n"
"}\n"
"\n"
"/* \346\263\250\345\206\214\346\214\211\351\222\256\347\211\271\346\256\212\346\240\267\345\274\217 */\n"
"QPushButton[text=\"\346\263\250\345\206\214\"] {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #FF9500, stop:1 #FF7F00);\n"
"}\n"
"\n"
"QPushButton[text=\"\346\263\250\345\206\214\"]:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                "
                        "stop:0 #FFA533, stop:1 #FF8C1A);\n"
"}\n"
"\n"
"QPushButton[text=\"\346\263\250\345\206\214\"]:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #E68500, stop:1 #CC6A00);\n"
"}\n"
"\n"
"/* \347\231\273\345\275\225\346\214\211\351\222\256\347\211\271\346\256\212\346\240\267\345\274\217 */\n"
"QPushButton[text=\"\347\231\273\345\275\225\"] {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #00CD00, stop:1 #00B200);\n"
"}\n"
"\n"
"QPushButton[text=\"\347\231\273\345\275\225\"]:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #00E600, stop:1 #00CC00);\n"
"}\n"
"\n"
"QPushButton[text=\"\347\231\273\345\275\225\"]:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                stop:0 #00B200, stop:1 #009900);\n"
"}"));
        gridLayout = new QGridLayout(Client);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(Client);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font;
        font.setFamily(QString::fromUtf8("AcadEref"));
        label_2->setFont(font);

        horizontalLayout_3->addWidget(label_2);

        pwd_LE = new QLineEdit(Client);
        pwd_LE->setObjectName(QString::fromUtf8("pwd_LE"));
        pwd_LE->setFont(font);
        pwd_LE->setEchoMode(QLineEdit::Password);
        pwd_LE->setClearButtonEnabled(true);

        horizontalLayout_3->addWidget(pwd_LE);


        gridLayout->addLayout(horizontalLayout_3, 3, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(Client);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);

        horizontalLayout_2->addWidget(label);

        name_LE = new QLineEdit(Client);
        name_LE->setObjectName(QString::fromUtf8("name_LE"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("AcadEref"));
        font1.setStrikeOut(false);
        name_LE->setFont(font1);
        name_LE->setTabletTracking(false);
        name_LE->setEchoMode(QLineEdit::Normal);
        name_LE->setReadOnly(false);
        name_LE->setCursorMoveStyle(Qt::LogicalMoveStyle);
        name_LE->setClearButtonEnabled(true);

        horizontalLayout_2->addWidget(name_LE);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(379, 146, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 4, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        regist_PB = new QPushButton(Client);
        regist_PB->setObjectName(QString::fromUtf8("regist_PB"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("AcadEref"));
        font2.setBold(true);
        font2.setWeight(75);
        regist_PB->setFont(font2);
        regist_PB->setAutoDefault(false);

        horizontalLayout->addWidget(regist_PB);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        login_PB = new QPushButton(Client);
        login_PB->setObjectName(QString::fromUtf8("login_PB"));
        login_PB->setFont(font2);

        horizontalLayout->addWidget(login_PB);


        gridLayout->addLayout(horizontalLayout, 5, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(148, 95, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 2, 0, 1, 1);


        retranslateUi(Client);

        regist_PB->setDefault(false);


        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QWidget *Client)
    {
        Client->setWindowTitle(QCoreApplication::translate("Client", "\347\231\273\345\275\225", nullptr));
        label_2->setText(QCoreApplication::translate("Client", "\345\257\206  \347\240\201", nullptr));
        pwd_LE->setText(QString());
        pwd_LE->setPlaceholderText(QCoreApplication::translate("Client", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        label->setText(QCoreApplication::translate("Client", "\347\224\250\346\210\267\345\220\215", nullptr));
        name_LE->setPlaceholderText(QCoreApplication::translate("Client", "\350\257\267\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215", nullptr));
        regist_PB->setText(QCoreApplication::translate("Client", "\346\263\250\345\206\214", nullptr));
        login_PB->setText(QCoreApplication::translate("Client", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
