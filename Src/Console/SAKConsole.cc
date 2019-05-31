/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
 */
#include "SAKConsole.hh"
#include "SAKGlobal.hh"
#include "SAKLogOutput.hh"
#include "ui_SAKConsole.h"

#include <QFile>
#include <QFileDialog>
#include <QDateTime>
#include <QTextStream>
#include <QLoggingCategory>

SAKConsole* SAKConsole::instance = nullptr;
SAKConsole::SAKConsole(QWidget *parent)
    :QWidget (parent)
    ,appLogOutput (nullptr)
    ,ui (new Ui::SAKConsole)
{
    instance = this;
    ui->setupUi(this);

    saveLogBt = ui->pushButtonSave;
    textBrowser = ui->textBrowser;
    textBrowser->setLineWrapMode(QTextEdit::NoWrap);
    connect(saveLogBt, &QPushButton::clicked, this, &SAKConsole::saveLogToFile);

    appLogOutput = new SAKLogOutput;
    connect(this, &SAKConsole::outputMessage, appLogOutput, &SAKLogOutput::outputMessage);
    appLogOutput->start();


}

void SAKConsole::messageRedirection(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (instance){
        instance->addMessage(type, context, msg);
        instance->cookingMsg(type, context, msg);
    }  
}

void SAKConsole::cookingMsg(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString category = qtMsgTypeString(type);

    QString log = QDateTime::currentDateTime().toString("yyyyMMdd-hh:mm:ss");
    log.append(" ");
    log.append(QString(category));
    log.append(" ");
    log.append(context.category);
    log.append(":");
    log.append(msg);

    emit outputMessage(log);
}

QString SAKConsole::qtMsgTypeString(QtMsgType type)
{
    QString string;
    switch (type) {
    case QtDebugMsg:
        string = QString("QtDebugMsg");
        break;
    case QtInfoMsg:
        string =  QString("QtInfoMsg");
        break;
    case QtWarningMsg:
        string = QString("QtWarningMsg");
        break;
    case QtCriticalMsg:
        string = QString("QtCriticalMsg");
        break;
    case QtFatalMsg:
        string = QString("QtFatalMsg");
        break;
    }

    string.resize(12, ' ');
    string.prepend("[");
    string.append("]");

    return string;
}

void SAKConsole::addMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString msgTemp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    msgTemp = QString("<font color=%1>%2</font>").arg(QString("silver")).arg(msgTemp);

    msgTemp.append(" ");
    msgTemp.append(qtMsgTypeString(type));
    msgTemp.append(" ");
    msgTemp.append(context.category);
    msgTemp.append(" >> ");
    msgTemp.append(msg);

    QString color;
    switch (type) {
    case QtWarningMsg:
    case QtCriticalMsg:
    case QtFatalMsg:
        msgTemp = QString("<font color=%1>%2</font>").arg(QString("red")).arg(msgTemp);
        break;
    default:
        break;
    }

    textBrowser->append(msgTemp);
}

void SAKConsole::saveLogToFile()
{
    QLoggingCategory category(logCategory);
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("保存输出"),
                                                    QString("./%1").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")),
                                                    QString("text(*.txt);;log(*.log)"));

    if (filename.isEmpty()){
        qCWarning(category) << "The operation has been terminated by user!";
        return;
    }

    QFile file(filename);
    if (file.open(QFile::WriteOnly)){
        QTextStream out(&file);
        QString text = textBrowser->toPlainText();
        out << text;
        file.close();
        qCInfo(category) << "Saving file sucessfully:" << filename;
    }else{
        qCWarning(category) << "Saving file failed:" << file.errorString();
    }
}
