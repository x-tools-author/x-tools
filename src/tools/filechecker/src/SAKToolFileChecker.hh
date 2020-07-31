/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTOOLFILECHECKER_HH
#define SAKTOOLFILECHECKER_HH

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QProgressBar>
#include <QCryptographicHash>

#include "SAKToolBase.hh"

namespace Ui {
    class SAKToolFileChecker;
}

class QtCryptographicHashCalculator;
class SAKToolFileChecker : public SAKToolBase
{
    Q_OBJECT
public:
    SAKToolFileChecker(QWidget *parent = Q_NULLPTR);

    void setUiEnable(bool enable);
    QString fileName(){return _fileName;}
    QCryptographicHash::Algorithm algorithm(){return _algorithm;}
    void updateResult(QByteArray result);
    void outputMessage(QString msg, bool isErrMsg = false);
    void updateProgressBar(int currentValue);
    void changeRemainTime(QString remainTime);
private:
    Ui::SAKToolFileChecker *ui;
    QString _fileName = QString("E:/ISO/Linux/Debian/debian-live-9.9.0-amd64-kde.iso");
    QCryptographicHash::Algorithm _algorithm = QCryptographicHash::Md5;
    QtCryptographicHashCalculator *calculator;
    QTimer clearMessageTimer;
    // ------------------------------------------------------------------------
    void finished();
    void clearMessage();
private:
    /*
     * ui控件指针
     */
    QLineEdit       *filePathlineEdit;
    QComboBox       *algorithmComboBox;
    QLineEdit       *resultLineEdit;
    QProgressBar    *calculatorProgressBar;
    QPushButton     *openPushButton;
    QPushButton     *startStopPushButton;
    QCheckBox       *upperCheckBox;
    QLabel          *messageLabel;
    QLabel          *remainTimeLabel;
private slots:
    void on_openPushButton_clicked();
    void on_algorithmComboBox_currentIndexChanged(int index);
    void on_startStopPushButton_clicked();
    void on_upperCheckBox_clicked();
};

#endif
