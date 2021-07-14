/*
 * Copyright 2021 Qter(qsaker@foxmial.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOUTPUTLOGDIALOG_HH
#define SAKOUTPUTLOGDIALOG_HH

#include <QDialog>
#include <QPushButton>
#include <QTextBrowser>

namespace Ui {
    class SAKOutputLogDialog;
}

class SAKOutputLogDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SAKOutputLogDialog(QWidget *parent = Q_NULLPTR);
    ~SAKOutputLogDialog();
    void outputMessage(QString msg, bool isInfo = true);
private:
    Ui::SAKOutputLogDialog *mUi;
    QPushButton *mClosePushButton;
    QPushButton *mSaveLogPushButton;
    QPushButton *mClearLogPushButton;
    QTextBrowser *mLogTextBrowser;
private:
    void onClosePushButtonClicked();
    void onSaveLogPushButtonClicked();
    void onClearLogPushButtonClicked();
};

#endif // SAKOUTPUTLOGDIALOG_HH
