/***************************************************************************************************
 * Copyright 2018-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakfilecheckassistant.h"
#include "ui_sakfilecheckassistant.h"

#include <QDebug>
#include <QFileDialog>
#include <QMetaEnum>

#include "sakcryptographichashcalculator.h"

SAKFileCheckAssistant::SAKFileCheckAssistant(QWidget* parent)
    : QWidget(parent)
    , m_fileName(QString("C:/Windows/explorer.exe"))
    , m_algorithm(QCryptographicHash::Md5)
    , m_calculator(Q_NULLPTR)
    , ui(new Ui::SAKFileCheckAssistant)
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
#if QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
    QMetaEnum algorithms = QMetaEnum::fromType<SAKToolFileCheckAssistant::Algorithm>();
#else
    QMetaEnum algorithms = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
#endif
    QStringList algorithmsStringList;
    for (int i = 0; i < algorithms.keyCount(); i++) {
        algorithmsStringList.append(QString(algorithms.key(i)));
    }
    m_algorithmComboBox->addItems(algorithmsStringList);
    m_algorithmComboBox->setCurrentText("Md5");

    m_filePathlineEdit->setReadOnly(true);
    m_resultLineEdit->setReadOnly(true);
    m_calculatorProgressBar->setMinimum(0);
    m_calculatorProgressBar->setMaximum(100);
    m_calculatorProgressBar->setValue(0);

    // It will clean the message which was showed on the info label when the timer
    // is timeout
    m_clearMessageTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&m_clearMessageTimer, &QTimer::timeout, this, &SAKFileCheckAssistant::clearMessage);

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

SAKFileCheckAssistant::~SAKFileCheckAssistant()
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

void SAKFileCheckAssistant::setUiEnable(bool enable)
{
    m_algorithmComboBox->setEnabled(enable);
    m_openPushButton->setEnabled(enable);
}

QString SAKFileCheckAssistant::fileName()
{
    return m_fileName;
}

QCryptographicHash::Algorithm SAKFileCheckAssistant::algorithm()
{
    return m_algorithm;
}

void SAKFileCheckAssistant::updateResult(QByteArray result)
{
    QString resultString = QString(result.toHex());
    if (m_upperCheckBox->isChecked()) {
        m_resultLineEdit->setText(resultString.toUpper());
    } else {
        m_resultLineEdit->setText(resultString);
    }
}

void SAKFileCheckAssistant::outputMessage(QString msg, bool isErrMsg)
{
    if (isErrMsg) {
        QApplication::beep();
        msg = QString("<font color=red>%1</font>").arg(msg);
    } else {
        msg = QString("<font color=blue>%1</font>").arg(msg);
    }

    m_messageLabel->setText(msg);
    m_clearMessageTimer.start();
}

void SAKFileCheckAssistant::updateProgressBar(int currentValue)
{
    m_calculatorProgressBar->setValue(currentValue);
}

void SAKFileCheckAssistant::changeRemainTime(QString remainTime)
{
    QString str = tr("Remaining time");
    m_remainTimeLabel->setText(QString("%1 %2").arg(str, remainTime));
}

void SAKFileCheckAssistant::finished()
{
    onStartStopPushButtonClicked();
}

void SAKFileCheckAssistant::clearMessage()
{
    m_clearMessageTimer.stop();
    m_messageLabel->clear();
    m_remainTimeLabel->clear();
}

void SAKFileCheckAssistant::onOpenPushButtonClicked()
{
    m_fileName = QFileDialog::getOpenFileName();
    m_filePathlineEdit->setText(m_fileName);
    if (!m_fileName.isEmpty()) {
        m_startStopPushButton->setEnabled(true);
    }

    m_calculatorProgressBar->setValue(0);
    m_resultLineEdit->clear();
    m_messageLabel->clear();
}

void SAKFileCheckAssistant::onStartStopPushButtonClicked()
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
        m_calculator = new SAKCryptographicHashCalculator(this);
        connect(m_calculator, &QThread::finished, this, &SAKFileCheckAssistant::finished);
        m_calculator->start();
        m_startStopPushButton->setText(tr("StopCalculating"));
        setUiEnable(false);
    }
}

void SAKFileCheckAssistant::onUpperCheckBoxClicked()
{
    QString temp = m_resultLineEdit->text();
    if (m_upperCheckBox->isChecked()) {
        m_resultLineEdit->setText(temp.toUpper());
    } else {
        m_resultLineEdit->setText(temp.toLower());
    }
}

void SAKFileCheckAssistant::onAlgorithmChanged(int index)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
    QMetaEnum algorithms = QMetaEnum::fromType<SAKToolFileCheckAssistant::Algorithm>();
#else
    QMetaEnum algorithms = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
#endif
    m_algorithm = static_cast<QCryptographicHash::Algorithm>(algorithms.value(index));
    m_resultLineEdit->clear();
    m_calculatorProgressBar->setValue(0);
}