/*
 * Copyright © 2021 Qsaker(qsaker@outlook.com). All rights reserved.
 */
#ifndef SAKDEBUGGEROUTPUTLOG_HH
#define SAKDEBUGGEROUTPUTLOG_HH

#include <QDialog>
#include <QPushButton>
#include <QTextBrowser>

namespace Ui {
    class SAKDebuggerOutputLog;
}

class SAKDebuggerOutputLog : public QDialog
{
    Q_OBJECT
public:
    explicit SAKDebuggerOutputLog(QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerOutputLog();
    void outputMessage(QString msg, bool isInfo = true);
private:
    Ui::SAKDebuggerOutputLog *ui;
    QPushButton *m_closePushButton;
    QPushButton *m_saveLogPushButton;
    QPushButton *m_clearLogPushButton;
    QTextBrowser *m_logTextBrowser;
private:
    void onClosePushButtonClicked();
    void onSaveLogPushButtonClicked();
    void onClearLogPushButtonClicked();
};

#endif // SAKOUTPUTLOGDIALOG_HH
