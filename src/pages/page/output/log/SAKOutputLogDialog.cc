/*
 * Copyright 2021 Qter(qsaker@foxmial.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QFile>
#include <QDateTime>
#include <QFileDialog>
#include <QTextStream>

#include "SAKOutputLogDialog.hh"
#include "ui_SAKOutputLogDialog.h"

SAKOutputLogDialog::SAKOutputLogDialog(QWidget *parent)
    :QDialog(parent)
    ,mUi(new Ui::SAKOutputLogDialog)
{
    mUi->setupUi(this);
    mClosePushButton = mUi->closePushButton;
    mSaveLogPushButton = mUi->saveLogPushButton;
    mClearLogPushButton = mUi->clearLogPushButton;
    mLogTextBrowser = mUi->textBrowser;
    connect(mClosePushButton, &QPushButton::clicked, this, &SAKOutputLogDialog::onClosePushButtonClicked);
    connect(mSaveLogPushButton, &QPushButton::clicked, this, &SAKOutputLogDialog::onSaveLogPushButtonClicked);
    connect(mClearLogPushButton, &QPushButton::clicked, this, &SAKOutputLogDialog::onClearLogPushButtonClicked);
    setModal(true);
}

SAKOutputLogDialog::~SAKOutputLogDialog()
{
    delete mUi;
}

void SAKOutputLogDialog::outputMessage(QString msg, bool isInfo)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss ");
    QString temp;
    temp.append(time);
    temp.append(msg);
    time = QString("<font color=silver>%1</font>").arg(time);

    msg.prepend(time);
    mLogTextBrowser->append(QString("<font color=%1>%2</font>").arg(isInfo ? "black" : "red", msg));
}

void SAKOutputLogDialog::onClosePushButtonClicked()
{
    close();
}

void SAKOutputLogDialog::onSaveLogPushButtonClicked()
{
    auto fileName = QFileDialog::getSaveFileName(this, tr("Save Log to File"), QString(QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".txt"), QString("*.txt"));
    if (fileName.length()){
        QFile file(fileName);
        if (file.open(QFile::ReadWrite|QFile::Text)){
            QTextStream out(&file);
            out << mLogTextBrowser->toPlainText();
            file.close();
        }
    }
}

void SAKOutputLogDialog::onClearLogPushButtonClicked()
{
    mLogTextBrowser->clear();
}
