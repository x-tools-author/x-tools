/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QTimer>

namespace Ui {
class UpdateDialog;
}

class HttpWindow;

class UpdateDialog:public QDialog
{
    Q_OBJECT
public:
    UpdateDialog(QWidget *parent = nullptr);
    ~UpdateDialog();
private:
    Ui::UpdateDialog *ui = nullptr;
    HttpWindow *httpWindow = nullptr;
private slots:
    void checkForUpdate();
    void cancel();
    void updateSoftware();
    void updateProgressBar();
};

#endif
