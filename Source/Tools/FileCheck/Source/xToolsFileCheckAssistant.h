/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QCryptographicHash>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

namespace Ui {
class xToolsFileCheckAssistant;
}

class xToolsCryptographicHashCalculator;
class xToolsFileCheckAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE xToolsFileCheckAssistant(QWidget* parent = Q_NULLPTR);
    ~xToolsFileCheckAssistant();

    void setUiEnable(bool enable);
    QString fileName();
    QCryptographicHash::Algorithm algorithm();
    void updateResult(QByteArray result);
    void outputMessage(QString msg, bool isErrMsg = false);
    void updateProgressBar(int currentValue);
    void changeRemainTime(QString remainTime);
#if QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
    // The QtCryptographicHashCalculator::Algorithm enum is not export with Q_ENUM in Qt5.6.0
    enum Algorithm {
#ifndef QT_CRYPTOGRAPHICHASH_ONLY_SHA1
        Md4,
        Md5,
#endif
        Sha1 = 2,
#ifndef QT_CRYPTOGRAPHICHASH_ONLY_SHA1
        Sha224,
        Sha256,
        Sha384,
        Sha512,
        Sha3_224,
        Sha3_256,
        Sha3_384,
        Sha3_512
#endif
    };
    Q_ENUM(Algorithm)
#endif
private:
    void finished();
    void clearMessage();

private:
    QString m_fileName;
    QCryptographicHash::Algorithm m_algorithm;
    xToolsCryptographicHashCalculator* m_calculator;
    QTimer m_clearMessageTimer;

private:
    Ui::xToolsFileCheckAssistant* ui;
    QLineEdit* m_filePathlineEdit;
    QComboBox* m_algorithmComboBox;
    QLineEdit* m_resultLineEdit;
    QProgressBar* m_calculatorProgressBar;
    QPushButton* m_openPushButton;
    QPushButton* m_startStopPushButton;
    QCheckBox* m_upperCheckBox;
    QLabel* m_messageLabel;
    QLabel* m_remainTimeLabel;

private slots:
    void onOpenPushButtonClicked();
    void onStartStopPushButtonClicked();
    void onUpperCheckBoxClicked();
    void onAlgorithmChanged(int index);
};
