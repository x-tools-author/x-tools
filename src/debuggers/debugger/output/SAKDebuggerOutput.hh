/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGEOUTPUTMANAGER_HH
#define SAKDEBUGPAGEOUTPUTMANAGER_HH

#include <QMutex>
#include <QTimer>
#include <QLabel>
#include <QTimer>
#include <QThread>
#include <QComboBox>
#include <QCheckBox>
#include <QSettings>
#include <QPushButton>
#include <QTextBrowser>
#include <QWaitCondition>

class SAKDebuggerOutputLog;
class SAKDebuggerOutputSave2File;
class SAKDebuggerOutputHighlighter;
/// @brief output data controller
class SAKDebuggerOutput:public QThread
{
    Q_OBJECT
public:
    SAKDebuggerOutput(QPushButton *menuBt, QComboBox *formatCB,
                      QSettings *settings, QString settingGroup,
                      QTextBrowser *textBrower,
                      QObject *parent = Q_NULLPTR);
    ~SAKDebuggerOutput();
    struct QMDStructSettingsKeyContext {
            QString showDate;
            QString showTime;
            QString showMs;
            QString showRx;
            QString showTx;
            QString wrapAnywhere;
            QString textFormat;
            QString faceWithoutMakeup;
        };

    struct QMDStructOutputParametersContext {
        bool showDate;
        bool showTime;
        bool showRx;
        bool showTx;
        bool showMs;
        bool wrapAnywhere;

        int textFormat;
        bool faceWithoutMakeup;
    };

    struct QMDStructDataContext {
        bool isRxData;
        QByteArray data;
        QMDStructOutputParametersContext ctx;
    };

    void onBytesRead(QByteArray bytes);
    void onBytesWritten(QByteArray bytes);
    void outputMessage(QString msg, bool isInfo = true);
protected:
    void run() override;
private:
    QPushButton *mMenuPushButton;
    QComboBox *mFormatComboBox;
    QSettings *mSettings;
    QString mSettingsGroup;
    QTextBrowser *mTextBrower;
    QMDStructSettingsKeyContext mSettingsKeyCtx;
    QMDStructOutputParametersContext mOutputParametersCtx;
    QMutex mOutputParametersCtxMutex;
    QVector<QMDStructDataContext> mDataVaector;
    QMutex mDataVaectorMutex;

    SAKDebuggerOutputSave2File *m_save2File;
    SAKDebuggerOutputLog *m_log;
    SAKDebuggerOutputHighlighter *mHhighlighter;
private:
    void appendData(bool isRxData, QByteArray data);
    QMDStructOutputParametersContext outputParametersContext();
    QString dateTimeString(QMDStructDataContext ctx);
    QString formattingData(QMDStructDataContext ctx);
    void save();
signals:
    void bytesCooked(QString dataString, bool faceWithoutMakeup);
};

#endif
