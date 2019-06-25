/********************************************************************************
** Form generated from reading UI file 'QtCryptographicHashController.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTCRYPTOGRAPHICHASHCONTROLLER_H
#define UI_QTCRYPTOGRAPHICHASHCONTROLLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtCryptographicHashController
{
public:
    QGridLayout *gridLayout;
    QLabel *remainTimeLabel;
    QLabel *messageLabel;
    QCheckBox *upperCheckBox;
    QProgressBar *calculatorProgressBar;
    QLineEdit *resultLineEdit;
    QLineEdit *filePathlineEdit;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_2;
    QLabel *label_3;
    QComboBox *algorithmComboBox;
    QPushButton *startStopPushButton;
    QPushButton *openPushButton;

    void setupUi(QWidget *QtCryptographicHashController)
    {
        if (QtCryptographicHashController->objectName().isEmpty())
            QtCryptographicHashController->setObjectName(QString::fromUtf8("QtCryptographicHashController"));
        QtCryptographicHashController->resize(446, 165);
        gridLayout = new QGridLayout(QtCryptographicHashController);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        remainTimeLabel = new QLabel(QtCryptographicHashController);
        remainTimeLabel->setObjectName(QString::fromUtf8("remainTimeLabel"));

        gridLayout->addWidget(remainTimeLabel, 4, 0, 1, 2);

        messageLabel = new QLabel(QtCryptographicHashController);
        messageLabel->setObjectName(QString::fromUtf8("messageLabel"));
        messageLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(messageLabel, 5, 0, 1, 5);

        upperCheckBox = new QCheckBox(QtCryptographicHashController);
        upperCheckBox->setObjectName(QString::fromUtf8("upperCheckBox"));

        gridLayout->addWidget(upperCheckBox, 4, 4, 1, 1);

        calculatorProgressBar = new QProgressBar(QtCryptographicHashController);
        calculatorProgressBar->setObjectName(QString::fromUtf8("calculatorProgressBar"));
        calculatorProgressBar->setLayoutDirection(Qt::LeftToRight);
        calculatorProgressBar->setAutoFillBackground(false);
        calculatorProgressBar->setValue(24);
        calculatorProgressBar->setTextDirection(QProgressBar::TopToBottom);

        gridLayout->addWidget(calculatorProgressBar, 3, 1, 1, 4);

        resultLineEdit = new QLineEdit(QtCryptographicHashController);
        resultLineEdit->setObjectName(QString::fromUtf8("resultLineEdit"));

        gridLayout->addWidget(resultLineEdit, 2, 1, 1, 4);

        filePathlineEdit = new QLineEdit(QtCryptographicHashController);
        filePathlineEdit->setObjectName(QString::fromUtf8("filePathlineEdit"));

        gridLayout->addWidget(filePathlineEdit, 0, 1, 1, 4);

        label = new QLabel(QtCryptographicHashController);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_4 = new QLabel(QtCryptographicHashController);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_2 = new QLabel(QtCryptographicHashController);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(QtCryptographicHashController);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        algorithmComboBox = new QComboBox(QtCryptographicHashController);
        algorithmComboBox->setObjectName(QString::fromUtf8("algorithmComboBox"));

        gridLayout->addWidget(algorithmComboBox, 1, 1, 1, 4);

        startStopPushButton = new QPushButton(QtCryptographicHashController);
        startStopPushButton->setObjectName(QString::fromUtf8("startStopPushButton"));

        gridLayout->addWidget(startStopPushButton, 4, 2, 1, 1);

        openPushButton = new QPushButton(QtCryptographicHashController);
        openPushButton->setObjectName(QString::fromUtf8("openPushButton"));

        gridLayout->addWidget(openPushButton, 4, 3, 1, 1);


        retranslateUi(QtCryptographicHashController);

        QMetaObject::connectSlotsByName(QtCryptographicHashController);
    } // setupUi

    void retranslateUi(QWidget *QtCryptographicHashController)
    {
        QtCryptographicHashController->setWindowTitle(QApplication::translate("QtCryptographicHashController", "Form", nullptr));
        remainTimeLabel->setText(QString());
        messageLabel->setText(QString());
        upperCheckBox->setText(QApplication::translate("QtCryptographicHashController", "\347\273\223\346\236\234\345\244\247\345\206\231", nullptr));
        filePathlineEdit->setText(QApplication::translate("QtCryptographicHashController", "E:/ISO/Linux/Debian/debian-live-9.9.0-amd64-kde.iso", nullptr));
        label->setText(QApplication::translate("QtCryptographicHashController", "\346\240\241\351\252\214\346\226\207\344\273\266", nullptr));
        label_4->setText(QApplication::translate("QtCryptographicHashController", "\346\243\200\351\252\214\350\277\233\345\272\246", nullptr));
        label_2->setText(QApplication::translate("QtCryptographicHashController", "\346\240\241\351\252\214\347\256\227\346\263\225", nullptr));
        label_3->setText(QApplication::translate("QtCryptographicHashController", "\346\240\241\351\252\214\347\273\223\346\236\234", nullptr));
        startStopPushButton->setText(QApplication::translate("QtCryptographicHashController", "\345\274\200\345\247\213\350\256\241\347\256\227", nullptr));
        openPushButton->setText(QApplication::translate("QtCryptographicHashController", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtCryptographicHashController: public Ui_QtCryptographicHashController {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTCRYPTOGRAPHICHASHCONTROLLER_H
