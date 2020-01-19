/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef QTCRYPTOGRAPHICHASHCONTROLLER_HH
#define QTCRYPTOGRAPHICHASHCONTROLLER_HH

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QProgressBar>
#include <QCryptographicHash>

namespace Ui {
class QtCryptographicHashController;
}

class QtCryptographicHashCalculator;
class QtCryptographicHashController : public QWidget
{
    Q_OBJECT
public:
    QtCryptographicHashController(QWidget *parent = nullptr);

    void setUiEnable(bool enable);
    QString fileName(){return _fileName;}
    QCryptographicHash::Algorithm algorithm(){return _algorithm;}
    void updateResult(QByteArray result);
    void outputMessage(QString msg, bool isErrMsg = false);
    void updateProgressBar(int currentValue);
    void changeRemainTime(QString remainTime);
private:
    Ui::QtCryptographicHashController *ui;
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
