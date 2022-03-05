/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
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
                      QWidget *uiParent = Q_NULLPTR,
                      QObject *parent = Q_NULLPTR);
    ~SAKDebuggerOutput();
    struct SAKStructSettingsKeyContext {
            QString showDate;
            QString showTime;
            QString showMs;
            QString showRx;
            QString showTx;
            QString wrapAnywhere;
            QString textFormat;
            QString faceWithoutMakeup;
        };

    struct SAKStructOutputParametersContext {
        bool showDate;
        bool showTime;
        bool showRx;
        bool showTx;
        bool showMs;
        bool wrapAnywhere;

        int textFormat;
        bool faceWithoutMakeup;
    };

    struct SAKStructDataContext {
        bool isRxData;
        QByteArray data;
        QString flag;
        SAKStructOutputParametersContext ctx;
    };

    void onBytesRead(QByteArray bytes, const QString &from);
    void onBytesWritten(QByteArray bytes, const QString &to);
    void outputMessage(QString msg, bool isInfo = true);
protected:
    void run() override;
private:
    bool mIsFirst;
    QMutex mDataVectorMutex;
    QMutex mOutputParametersCtxMutex;
    QString mSettingsGroup;
    QComboBox *mFormatComboBox;
    QSettings *mSettings;
    QPushButton *mMenuPushButton;
    QTextBrowser *mTextBrower;
    QVector<SAKStructDataContext> mDataVector;
    SAKStructSettingsKeyContext mSettingsKeyCtx;
    SAKStructOutputParametersContext mOutputParametersCtx;

    SAKDebuggerOutputLog *mLog;
    SAKDebuggerOutputSave2File *mSave2File;
    SAKDebuggerOutputHighlighter *mHhighlighter;
private:
    void save();
    void appendData(bool isRxData, QByteArray data, const QString &flag);
    QString dateTimeString(SAKStructDataContext ctx);
    QString formattingData(SAKStructDataContext ctx);
    SAKStructOutputParametersContext outputParametersContext();
signals:
    void bytesCooked(QString dataString, bool faceWithoutMakeup);
};

#endif
