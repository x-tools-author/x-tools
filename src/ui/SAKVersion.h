/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAKVERSION_H
#define SAKVERSION_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class SAKVersion;
}

class SAKVersion:public QDialog
{
    Q_OBJECT
public:
    SAKVersion();
    ~SAKVersion();
private:
    Ui::SAKVersion* ui          = nullptr;
    QLabel*         version     = nullptr;
    QLabel*         datetime    = nullptr;
    QLabel*         author      = nullptr;
    QLabel*         email       = nullptr;
    QLabel*         blog        = nullptr;
protected:
    bool eventFilter(QObject *o, QEvent *e) final;
private:
    void openBlogUrl();
};

#endif
