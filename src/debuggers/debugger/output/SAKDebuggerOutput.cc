/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QMenu>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QTextStream>
#include <QFileDialog>
#include <QStandardPaths>

#include "SAKDebuggerOutput.hh"
#include "SAKDebuggerOutputLog.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerOutputSave2File.hh"
#include "SAKDebuggerOutputHighlighter.hh"

SAKDebuggerOutput::SAKDebuggerOutput(QPushButton *menuBt, QComboBox *formatCB,
                                     QSettings *settings, QString settingGroup,
                                     QTextBrowser *textBrower,
                                     QWidget *uiParent, QObject *parent)
    :QThread(parent)
    ,mSettingsGroup(settingGroup)
    ,mFormatComboBox(formatCB)
    ,mSettings(settings)
    ,mMenuPushButton(menuBt)
    ,mTextBrower(textBrower)
{
    mSettingsKeyCtx.showDate = mSettingsGroup + "/" + "showDate";
    mSettingsKeyCtx.showTime = mSettingsGroup + "/" + "showTime";
    mSettingsKeyCtx.showRx = mSettingsGroup + "/" + "showRx";
    mSettingsKeyCtx.showTx = mSettingsGroup + "/" + "showTx";
    mSettingsKeyCtx.showMs = mSettingsGroup + "/" + "showMs";
    mSettingsKeyCtx.wrapAnywhere = mSettingsGroup + "/" + "wrapAnywhere";
    mSettingsKeyCtx.textFormat = mSettingsGroup + "/" + "textFormat";
    mSettingsKeyCtx.faceWithoutMakeup = mSettingsGroup + "/" + "faceWithoutMakeup";


    mFormatComboBox->blockSignals(true);
    mFormatComboBox->addItem("Bin", SAKCommonDataStructure::OutputFormatBin);
    mFormatComboBox->addItem("Otc", SAKCommonDataStructure::OutputFormatOct);
    mFormatComboBox->addItem("Dec", SAKCommonDataStructure::OutputFormatDec);
    mFormatComboBox->addItem("Hex", SAKCommonDataStructure::OutputFormatHex);
    mFormatComboBox->addItem("Ucs4", SAKCommonDataStructure::OutputFormatUcs4);
    mFormatComboBox->addItem("Utf8", SAKCommonDataStructure::OutputFormatUtf8);
    mFormatComboBox->addItem("Utf16", SAKCommonDataStructure::OutputFormatUtf16);
    mFormatComboBox->addItem("Ascii", SAKCommonDataStructure::OutputFormatAscii);
    mFormatComboBox->addItem(tr("System"), SAKCommonDataStructure::OutputFormatLocal);


    // Readin settings
    auto textFromatOutput = mSettings->value(mSettingsKeyCtx.textFormat).toString();
    int index = mFormatComboBox->findText(textFromatOutput);
    if (index != -1) {
        mFormatComboBox->setCurrentIndex(index);
    }
    mOutputParametersCtx.textFormat = mFormatComboBox->currentData().toInt();
    mFormatComboBox->blockSignals(false);


    connect(mFormatComboBox, &QComboBox::currentTextChanged,
            this, [&](const QString &text){
        mSettings->setValue(mSettingsKeyCtx.textFormat, text);
        mOutputParametersCtxMutex.lock();
        mOutputParametersCtx.textFormat = mFormatComboBox->currentData().toInt();
        mOutputParametersCtxMutex.unlock();
    });


    QMenu *btMenu = new QMenu(mMenuPushButton);
    auto menu = new QMenu(tr("Output Parameters"), btMenu);
    btMenu->addMenu(menu);
    QAction *action = menu->addAction(tr("Auto Wrap"));
    action->setCheckable(true);
    bool wrapAnywhere = mSettings->value(mSettingsKeyCtx.wrapAnywhere).isNull()
            ? true
            : mSettings->value(mSettingsKeyCtx.wrapAnywhere).toBool();
    action->setChecked(wrapAnywhere);
    connect(action, &QAction::triggered, this, [=](){
        mTextBrower->setWordWrapMode(action->isChecked()
                                     ? QTextOption::WrapAnywhere
                                     : QTextOption::NoWrap);
        mSettings->setValue(mSettingsKeyCtx.wrapAnywhere, action->isChecked());
    });
    menu->addAction(action);
    menu->addSeparator();


    QStringList paras;
    QVector<QAction*> actionVector;
    paras << tr("Show Date")
          << tr("Show Time")
          << tr("Show MS")
          << tr("Show Rx Data")
          << tr("Show Tx Data");
    for (int i = 0; i < paras.length(); i++) {
        auto name = paras.at(i);
        action = menu->addAction(name);
        action->setCheckable(true);

        if (i < 3) {
            actionVector.append(action);
        }

        QString key = QString("");
        switch (i) {
        case 0: key = mSettingsKeyCtx.showDate; break;
        case 1: key = mSettingsKeyCtx.showTime; break;
        case 2: key = mSettingsKeyCtx.showMs; break;
        case 3: key = mSettingsKeyCtx.showRx; break;
        case 4: key = mSettingsKeyCtx.showTx; break;
        default: Q_ASSERT_X(false, __FUNCTION__, "Unknow index"); break;
        }

        bool checked = mSettings->value(key).isNull()
                ? true
                : mSettings->value(key).toBool();
        action->setChecked(checked);
        switch (i) {
        case 0: mOutputParametersCtx.showDate = checked; break;
        case 1: mOutputParametersCtx.showTime = checked; break;
        case 2: mOutputParametersCtx.showMs = checked; break;
        case 3: mOutputParametersCtx.showRx = checked; break;
        case 4: mOutputParametersCtx.showTx = checked; break;
        default: Q_ASSERT_X(false, __FUNCTION__, "Unknow index"); break;
        }

        connect(action, &QAction::triggered, this, [=](bool checked){
            mSettings->setValue(key, action->isChecked());
            mOutputParametersCtxMutex.lock();
            switch (i) {
            case 0: mOutputParametersCtx.showDate = checked; break;
            case 1: mOutputParametersCtx.showTime = checked; break;
            case 2: mOutputParametersCtx.showMs = checked; break;
            case 3: mOutputParametersCtx.showRx = checked; break;
            case 4: mOutputParametersCtx.showTx = checked; break;
            default: Q_ASSERT_X(false, __FUNCTION__, "Unknow index"); break;
            }
            mOutputParametersCtxMutex.unlock();
        });
    }
    menu->addSeparator();
    action = menu->addAction(tr("Face Without Makeup "));
    action->setToolTip(tr("Just output data which is read or written!"));
    action->setCheckable(true);
    bool enable = mSettings->value(mSettingsKeyCtx.faceWithoutMakeup).toBool();
    action->setChecked(enable);
    mOutputParametersCtx.faceWithoutMakeup = enable;
    auto setFaceWithoutMakeup = [=](bool enable){
        for (int i = 0; i < actionVector.length(); i++) {
            QAction *a = actionVector.at(i);
            a->setEnabled(!enable);
        }

        this->mOutputParametersCtxMutex.lock();
        this->mOutputParametersCtx.faceWithoutMakeup = enable;
        this->mOutputParametersCtxMutex.unlock();
        mSettings->setValue(mSettingsKeyCtx.faceWithoutMakeup, enable);
    };

    setFaceWithoutMakeup(enable);
    connect(action, &QAction::triggered, this, [=](){
        bool enable = action->isChecked();
        setFaceWithoutMakeup(enable);
    });

    mLog = new SAKDebuggerOutputLog(uiParent);
    mSave2File = new SAKDebuggerOutputSave2File(mSettings, settingGroup, uiParent);
    mHhighlighter = new SAKDebuggerOutputHighlighter(mTextBrower->document(), uiParent);

    action = btMenu->addAction(tr("Save Output"));
    connect(action, &QAction::triggered,
            this, &SAKDebuggerOutput::save);
    btMenu->addSeparator();
    action = btMenu->addAction(tr("Log"));
    connect(action, &QAction::triggered,
            mLog, &SAKDebuggerOutputLog::show);
    action = btMenu->addAction(tr("Highlighter"));
    connect(action, &QAction::triggered,
            mHhighlighter, &SAKDebuggerOutputHighlighter::show);
    action = btMenu->addAction(tr("Write to Disk"));
    connect(action, &QAction::triggered,
            mSave2File, &SAKDebuggerOutputSave2File::show);
    mMenuPushButton->setMenu(btMenu);


    connect(this, &SAKDebuggerOutput::bytesCooked,
            this, [=](QString dataString, bool faceWithoutMakeup) {
        if (faceWithoutMakeup) {
            mTextBrower->textCursor().movePosition(QTextCursor::End);
            mTextBrower->insertHtml(dataString);
        } else {
            mTextBrower->append(dataString);
        }
    }, Qt::QueuedConnection);


    connect(this, &SAKDebuggerOutput::finished, this, [&](){
        mDataVectorMutex.lock();
        if (mDataVector.length()) {
            start();
        }
        mDataVectorMutex.unlock();
    });

    textBrower->document()->setMaximumBlockCount(2000);
}

SAKDebuggerOutput::~SAKDebuggerOutput()
{
    mLog->deleteLater();
    mSave2File->deleteLater();
    mHhighlighter->deleteLater();

    exit();
    wait();
}

void SAKDebuggerOutput::onBytesRead(QByteArray bytes)
{
    appendData(true, bytes);
    mSave2File->bytesRead(bytes);
}

void SAKDebuggerOutput::onBytesWritten(QByteArray bytes)
{
    appendData(false, bytes);
    mSave2File->bytesWritten(bytes);
}

void SAKDebuggerOutput::outputMessage(QString msg, bool isInfo)
{
    mLog->outputMessage(msg, isInfo);
}

void SAKDebuggerOutput::run()
{
    while (1) {
        mDataVectorMutex.lock();
        if (mDataVector.length()) {
            auto dataCtx = mDataVector.takeFirst();
            mDataVectorMutex.unlock();

            bool faceWithoutMakeup = dataCtx.ctx.faceWithoutMakeup;
            if (dataCtx.isRxData && (!dataCtx.ctx.showRx)) {
                continue;
            } else if ((!dataCtx.isRxData) && (!dataCtx.ctx.showTx)) {
                continue;
            }

            QString dateTimeStr = faceWithoutMakeup ? "" : dateTimeString(dataCtx);
            QString dataString = formattingData(dataCtx);
            QString rxColor = "red";
            QString txColor = "blue";
            QString color = dataCtx.isRxData ? rxColor : txColor;
            QString rxTx = dataCtx.isRxData
                    ? QString("<font color=%1>Rx</font>").arg(rxColor)
                    : QString("<font color=%2>Tx</font>").arg(txColor);

            QString frameString;
            if (faceWithoutMakeup) {
                frameString = QString("<font color=%1>%2</font>")
                        .arg(color, dataString);
            } else {
                QString space = dateTimeStr.isEmpty() ? "" : " ";
                frameString =
                        QString("<font color=silver>[%1%2%3]</font>%4")
                        .arg(dateTimeStr, space, rxTx, dataString);
            }

            emit bytesCooked(frameString, dataCtx.ctx.faceWithoutMakeup);
        } else {
            mDataVectorMutex.unlock();
            break;
        }
    }
}

void SAKDebuggerOutput::save()
{
    QString dtstr = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    auto location = QStandardPaths::DesktopLocation;
    QString defaultPath = QStandardPaths::writableLocation(location) + "/" + dtstr;
    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR,
                                                    tr("Save Output Data to File"),
                                                    defaultPath,
                                                    tr("text (*.txt)"));
    if (fileName.length()) {
        QFile file(fileName);
        if (file.open(QFile::WriteOnly)) {
            QTextStream out(&file);
            out << mTextBrower->toPlainText();
            file.close();
        }
    }
}

void SAKDebuggerOutput::appendData(bool isRxData, QByteArray data)
{
    SAKStructDataContext ctx;
    ctx.data = data;
    ctx.isRxData = isRxData;
    ctx.ctx = outputParametersContext();

    mDataVectorMutex.lock();
    mDataVector.append(ctx);
    mDataVectorMutex.unlock();

    if (!isRunning()) {
        start();
    }
}

QString SAKDebuggerOutput::dateTimeString(SAKStructDataContext ctx)
{
    QString dateTimeString;
    if (ctx.ctx.showDate) {
        dateTimeString += QDate::currentDate().toString("yyyy-MM-dd");
    }

    if (ctx.ctx.showTime) {
        QString format = ctx.ctx.showMs ? "hh:mm:ss.zzz" : "hh:mm:ss";
        if (dateTimeString.length()) {
            format.prepend(" ");
        }
        dateTimeString += QTime::currentTime().toString(format);
    }

    return dateTimeString;
}

QString SAKDebuggerOutput::formattingData(SAKStructDataContext ctx)
{
    QString str;
    QByteArray origingData = ctx.data;
    int format = ctx.ctx.textFormat;
    if (format == SAKCommonDataStructure::OutputFormatBin) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(QString("%1 ")
                       .arg(QString::number(static_cast<uint8_t>(origingData.at(i)),
                                            2),
                            8,
                            '0'));
        }
    } else if (format == SAKCommonDataStructure::OutputFormatOct) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(QString("%1 ")
                       .arg(QString::number(static_cast<uint8_t>(origingData.at(i)),
                                            8),
                            3,
                            '0'));
        }
    } else if (format == SAKCommonDataStructure::OutputFormatDec) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(QString("%1 ")
                       .arg(QString::number(static_cast<uint8_t>(origingData.at(i)),
                                            10)));
        }
    } else if (format == SAKCommonDataStructure::OutputFormatHex) {
        for (int i = 0; i < origingData.length(); i++) {
            str.append(QString("%1 ")
                       .arg(QString::number(static_cast<uint8_t>(origingData.at(i)),
                                            16),
                            2,
                            '0'));
        }
    } else if (format == SAKCommonDataStructure::OutputFormatAscii) {
        str.append(QString::fromLatin1(origingData));
    } else if (format == SAKCommonDataStructure::OutputFormatUtf8) {
        str.append(QString::fromUtf8(origingData));
    } else if (format == SAKCommonDataStructure::OutputFormatUtf16) {
        const char *data = origingData.constData();
        int len = origingData.length()/int(sizeof(char16_t));
        str.append(QString::fromUtf16(reinterpret_cast<const char16_t*>(data), len));
    } else if (format == SAKCommonDataStructure::OutputFormatUcs4) {
        const char *data = origingData.constData();
        int len = origingData.length()/int(sizeof(char32_t));
        str.append(QString::fromUcs4(reinterpret_cast<const char32_t*>(data), len));
    } else if (format == SAKCommonDataStructure::OutputFormatLocal) {
        str.append(QString::fromLocal8Bit(origingData));
    } else {
        str.append(QString::fromUtf8(origingData));
        Q_ASSERT_X(false, __FUNCTION__, "Unknown output mode!");
    }

    return str;
}

SAKDebuggerOutput::SAKStructOutputParametersContext
SAKDebuggerOutput::outputParametersContext()
{
    mOutputParametersCtxMutex.lock();
    auto ctx = mOutputParametersCtx;
    mOutputParametersCtxMutex.unlock();
    return ctx;
}
