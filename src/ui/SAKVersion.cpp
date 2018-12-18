/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "SAKVersion.h"
#include "ui_SAKVersion.h"

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

SAKVersion::SAKVersion()
    :ui(new Ui::SAKVersion)
{
    ui->setupUi(this);

    version = ui->labelVersion;
    datetime = ui->labelDatetime;
    author = ui->labelAuthor;
    email = ui->labelEmail;
    blog = ui->labelBlog;
    blog->installEventFilter(this);
    blog->setCursor(Qt::PointingHandCursor);

    version->setText(QString("1.0.0"));
    datetime->setText(QString(__DATE__) + " " + QString(__TIME__));
    author->setText(QString("作者还没想好名字"));
    email->setText(QString("wuhai1024@outlook.com"));
    blog->setText(QString("www.wuhai.pro"));

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Tool);
    setModal(true);
}


SAKVersion::~SAKVersion()
{

}

bool SAKVersion::eventFilter(QObject *o, QEvent *e)
{
    if (o == blog){
        if (e->type() == QEvent::MouseButtonPress){
            QDesktopServices::openUrl(QUrl(blog->text()));
            return true;
        }
    }

    return QDialog::eventFilter(o, e);
}
