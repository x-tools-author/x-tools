/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsFileCheckAssistant.h"
#include "ui_xToolsFileCheckAssistant.h"

#include <QDebug>
#include <QFileDialog>
#include <QMetaEnum>

#include "xToolsCryptographicHashCalculator.h"

xToolsFileCheckAssistant::xToolsFileCheckAssistant(QWidget* parent)
    : QWidget(parent)
    , m_fileName(QString("C:/Windows/explorer.exe"))
    , m_algorithm(QCryptographicHash::Md5)
    , m_calculator(Q_NULLPTR)
    , ui(new Ui::xToolsFileCheckAssistant)
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
    QMetaEnum algorithms = QMetaEnum::fromType<xToolsFileCheckAssistant::Algorithm>();
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
    m_clearMessageTimer.setInterval(X_TOOLS_CLEAR_MESSAGE_INTERVAL);
    connect(&m_clearMessageTimer, &QTimer::timeout, this, &xToolsFileCheckAssistant::clearMessage);

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

xToolsFileCheckAssistant::~xToolsFileCheckAssistant()
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

void xToolsFileCheckAssistant::setUiEnable(bool enable)
{
    m_algorithmComboBox->setEnabled(enable);
    m_openPushButton->setEnabled(enable);
}

QString xToolsFileCheckAssistant::fileName()
{
    return m_fileName;
}

QCryptographicHash::Algorithm xToolsFileCheckAssistant::algorithm()
{
    return m_algorithm;
}

void xToolsFileCheckAssistant::updateResult(QByteArray result)
{
    QString resultString = QString(result.toHex());
    if (m_upperCheckBox->isChecked()) {
        m_resultLineEdit->setText(resultString.toUpper());
    } else {
        m_resultLineEdit->setText(resultString);
    }
}

void xToolsFileCheckAssistant::outputMessage(QString msg, bool isErrMsg)
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

void xToolsFileCheckAssistant::updateProgressBar(int currentValue)
{
    m_calculatorProgressBar->setValue(currentValue);
}

void xToolsFileCheckAssistant::changeRemainTime(QString remainTime)
{
    QString str = tr("Remaining time");
    m_remainTimeLabel->setText(QString("%1 %2").arg(str, remainTime));
}

void xToolsFileCheckAssistant::finished()
{
    onStartStopPushButtonClicked();
}

void xToolsFileCheckAssistant::clearMessage()
{
    m_clearMessageTimer.stop();
    m_messageLabel->clear();
    m_remainTimeLabel->clear();
}

void xToolsFileCheckAssistant::onOpenPushButtonClicked()
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

void xToolsFileCheckAssistant::onStartStopPushButtonClicked()
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
        m_calculator = new xToolsCryptographicHashCalculator(this);
        connect(m_calculator, &QThread::finished, this, &xToolsFileCheckAssistant::finished);
        m_calculator->start();
        m_startStopPushButton->setText(tr("StopCalculating"));
        setUiEnable(false);
    }
}

void xToolsFileCheckAssistant::onUpperCheckBoxClicked()
{
    QString temp = m_resultLineEdit->text();
    if (m_upperCheckBox->isChecked()) {
        m_resultLineEdit->setText(temp.toUpper());
    } else {
        m_resultLineEdit->setText(temp.toLower());
    }
}

void xToolsFileCheckAssistant::onAlgorithmChanged(int index)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
    QMetaEnum algorithms = QMetaEnum::fromType<xToolsFileCheckAssistant::Algorithm>();
#else
    QMetaEnum algorithms = QMetaEnum::fromType<QCryptographicHash::Algorithm>();
#endif
    m_algorithm = static_cast<QCryptographicHash::Algorithm>(algorithms.value(index));
    m_resultLineEdit->clear();
    m_calculatorProgressBar->setValue(0);
}
