/*
 * Copyright © 2021 Qsaker(qsaker@outlook.com). All rights reserved.
 */
#include <QFile>
#include <QDateTime>
#include <QFileDialog>
#include <QTextStream>

#include "SAKDebuggerOutputLog.hh"
#include "ui_SAKDebuggerOutputLog.h"

SAKDebuggerOutputLog::SAKDebuggerOutputLog(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::SAKDebuggerOutputLog)
{
    ui->setupUi(this);
    m_closePushButton = ui->closePushButton;
    m_saveLogPushButton = ui->saveLogPushButton;
    m_clearLogPushButton = ui->clearLogPushButton;
    m_logTextBrowser = ui->textBrowser;
    connect(m_closePushButton, &QPushButton::clicked, this, &SAKDebuggerOutputLog::onClosePushButtonClicked);
    connect(m_saveLogPushButton, &QPushButton::clicked, this, &SAKDebuggerOutputLog::onSaveLogPushButtonClicked);
    connect(m_clearLogPushButton, &QPushButton::clicked, this, &SAKDebuggerOutputLog::onClearLogPushButtonClicked);
    setModal(true);
}

SAKDebuggerOutputLog::~SAKDebuggerOutputLog()
{
    delete ui;
}

void SAKDebuggerOutputLog::outputMessage(QString msg, bool isInfo)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss ");
    QString temp;
    temp.append(time);
    temp.append(msg);
    time = QString("<font color=silver>%1</font>").arg(time);

    msg.prepend(time);
    m_logTextBrowser->append(QString("<font color=%1>%2</font>").arg(isInfo ? "black" : "red", msg));
}

void SAKDebuggerOutputLog::onClosePushButtonClicked()
{
    close();
}

void SAKDebuggerOutputLog::onSaveLogPushButtonClicked()
{
    auto fileName = QFileDialog::getSaveFileName(this, tr("保存日志"), QString(QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".txt"), QString("*.txt"));
    if (fileName.length()){
        QFile file(fileName);
        if (file.open(QFile::ReadWrite|QFile::Text)){
            QTextStream out(&file);
            out << m_logTextBrowser->toPlainText();
            file.close();
        }
    }
}

void SAKDebuggerOutputLog::onClearLogPushButtonClicked()
{
    m_logTextBrowser->clear();
}
