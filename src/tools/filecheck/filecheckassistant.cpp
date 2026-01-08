/***************************************************************************************************
 * Copyright 2018-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "filecheckassistant.h"
#include "ui_filecheckassistant.h"

#include <QDebug>
#include <QFileDialog>
#include <QMetaEnum>

#include "hashcalculator.h"

FileCheckAssistant::FileCheckAssistant(QWidget* parent)
    : QWidget(parent)
    , m_fileName(QString("C:/Windows/explorer.exe"))
    , m_algorithm(QCryptographicHash::Md5)
    , m_calculator(Q_NULLPTR)
    , ui(new Ui::FileCheckAssistant)
{
    ui->setupUi(this);

    // Initializing data member about ui
    m_filePathlineEdit = ui->filePathlineEdit;
    m_algorithmComboBox = ui->algorithmComboBox;
    m_resultLineEdit = ui->resultLineEdit;
    m_calculatorProgressBar = ui->calculatorProgressBar;
    m_openPushButton = ui->openPushButton;
    m_startStopPushButton = ui->startStopPushButton;
    m_upperCheckBox = ui->upperCheckBox;
    m_messageLabel = ui->messageLabel;
    m_remainTimeLabel = ui->remainTimeLabel;
    m_filePathlineEdit->setText(m_fileName);

    // Appending algorithms to combo box
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
    QMetaEnum algorithms = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
    for (int i = 0; i < algorithms.keyCount(); i++) {
        if (QString(algorithms.key(i)) == QString("NumAlgorithms")) {
            continue;
        }

        QString key = QString(algorithms.key(i));
        int value = algorithms.value(i);
        m_algorithmComboBox->addItem(key, value);
    }
#else
#ifndef QT_CRYPTOGRAPHICHASH_ONLY_SHA1
    m_algorithmComboBox->addItem("Md4", static_cast<int>(QCryptographicHash::Md4));
    m_algorithmComboBox->addItem("Md5", static_cast<int>(QCryptographicHash::Md5));
#endif
    m_algorithmComboBox->addItem("Sha1", static_cast<int>(QCryptographicHash::Sha1));
#ifndef QT_CRYPTOGRAPHICHASH_ONLY_SHA1
    m_algorithmComboBox->addItem("Sha224", static_cast<int>(QCryptographicHash::Sha224));
    m_algorithmComboBox->addItem("Sha256", static_cast<int>(QCryptographicHash::Sha256));
    m_algorithmComboBox->addItem("Sha384", static_cast<int>(QCryptographicHash::Sha384));
    m_algorithmComboBox->addItem("Sha512", static_cast<int>(QCryptographicHash::Sha512));
    m_algorithmComboBox->addItem("Sha3_224", static_cast<int>(QCryptographicHash::Sha3_224));
    m_algorithmComboBox->addItem("Sha3_256", static_cast<int>(QCryptographicHash::Sha3_256));
    m_algorithmComboBox->addItem("Sha3_384", static_cast<int>(QCryptographicHash::Sha3_384));
    m_algorithmComboBox->addItem("Sha3_512", static_cast<int>(QCryptographicHash::Sha3_512));
#endif
#endif

    m_algorithmComboBox->setCurrentText("Md5");

    m_filePathlineEdit->setReadOnly(true);
    m_resultLineEdit->setReadOnly(true);
    m_calculatorProgressBar->setMinimum(0);
    m_calculatorProgressBar->setMaximum(100);
    m_calculatorProgressBar->setValue(0);

    // It will clean the message which was showed on the info label when the timer is timeout
    m_clearMessageTimer.setInterval(8000);
    connect(&m_clearMessageTimer, &QTimer::timeout, this, &FileCheckAssistant::clearMessage);

    m_upperCheckBox->setChecked(true);
    setWindowTitle(tr("File Check Assistant"));

    connect(m_openPushButton, SIGNAL(clicked()), this, SLOT(onOpenPushButtonClicked()));
    connect(m_startStopPushButton, SIGNAL(clicked()), this, SLOT(onStartStopPushButtonClicked()));
    connect(m_upperCheckBox, SIGNAL(clicked()), this, SLOT(onUpperCheckBoxClicked()));
    connect(m_algorithmComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onAlgorithmChanged(int)));
}

FileCheckAssistant::~FileCheckAssistant()
{
    delete ui;
    if (m_calculator) {
        m_calculator->blockSignals(true);
        m_calculator->requestInterruption();
        m_calculator->exit();
        m_calculator->wait();
        m_calculator->deleteLater();
        m_calculator = Q_NULLPTR;
    }
}

void FileCheckAssistant::setUiEnable(bool enable)
{
    m_algorithmComboBox->setEnabled(enable);
    m_openPushButton->setEnabled(enable);
}

QString FileCheckAssistant::fileName()
{
    return m_fileName;
}

QCryptographicHash::Algorithm FileCheckAssistant::algorithm()
{
    return m_algorithm;
}

void FileCheckAssistant::updateResult(QByteArray result)
{
    QString resultString = QString(result.toHex());
    if (m_upperCheckBox->isChecked()) {
        m_resultLineEdit->setText(resultString.toUpper());
    } else {
        m_resultLineEdit->setText(resultString);
    }
}

void FileCheckAssistant::outputMessage(QString msg, bool isErrMsg)
{
    if (isErrMsg) {
        QApplication::beep();
        msg = QString("<font color=red>%1</font>").arg(msg);
    } else {
        msg = QString("<font color=green>%1</font>").arg(msg);
    }

    m_messageLabel->setText(msg);
    m_clearMessageTimer.start();
}

void FileCheckAssistant::updateProgressBar(int currentValue)
{
    m_calculatorProgressBar->setValue(currentValue);
}

void FileCheckAssistant::changeRemainTime(QString remainTime)
{
    QString str = tr("Remaining time");
    m_remainTimeLabel->setText(QString("%1 %2").arg(str, remainTime));
}

void FileCheckAssistant::finished()
{
    QApplication::beep();
    onStartStopPushButtonClicked();
}

void FileCheckAssistant::clearMessage()
{
    m_clearMessageTimer.stop();
    m_messageLabel->clear();
    m_remainTimeLabel->clear();
}

void FileCheckAssistant::onOpenPushButtonClicked()
{
    QString tmp = ui->filePathlineEdit->text();
    m_fileName = QFileDialog::getOpenFileName();
    if (m_fileName.isEmpty()) {
        m_fileName = tmp;
    }
    m_filePathlineEdit->setText(m_fileName);
    if (!m_fileName.isEmpty()) {
        m_startStopPushButton->setEnabled(true);
    }

    m_calculatorProgressBar->setValue(0);
    m_resultLineEdit->clear();
    m_messageLabel->clear();
}

void FileCheckAssistant::onStartStopPushButtonClicked()
{
    if (m_calculator) {
        m_calculator->blockSignals(true);
        m_calculator->requestInterruption();
        m_calculator->exit();
        m_calculator->wait();
        m_calculator->deleteLater();
        m_calculator = Q_NULLPTR;
        m_startStopPushButton->setText(tr("Calculate"));
        setUiEnable(true);
    } else {
        m_calculator = new HashCalculator(this);
        connect(m_calculator, &QThread::finished, this, &FileCheckAssistant::finished);
        m_calculator->start();
        m_startStopPushButton->setText(tr("StopCalculating"));
        setUiEnable(false);
        ui->remainTimeLabel->clear();
    }
}

void FileCheckAssistant::onUpperCheckBoxClicked()
{
    QString temp = m_resultLineEdit->text();
    if (m_upperCheckBox->isChecked()) {
        m_resultLineEdit->setText(temp.toUpper());
    } else {
        m_resultLineEdit->setText(temp.toLower());
    }
}

void FileCheckAssistant::onAlgorithmChanged(int index)
{
    int value = ui->algorithmComboBox->currentData().toInt();
    m_algorithm = static_cast<QCryptographicHash::Algorithm>(value);
    m_resultLineEdit->clear();
    m_calculatorProgressBar->setValue(0);
}
