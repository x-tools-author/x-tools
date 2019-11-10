/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKVersion.hh"
#include "ui_SAKVersion.h"

#include <QUrl>
#include <QLineEdit>
#include <QDateTime>
#include <QDesktopServices>

static const QDate buildDate = QLocale( QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");

SAKVersion* SAKVersion::sakVersionSingleton = nullptr;
SAKVersion::SAKVersion()
    :ui(new Ui::SAKVersion)
{
    Q_ASSERT_X(!sakVersionSingleton, __FUNCTION__, "该类只能有一个实例， 请使用 SAKVersion::instance()获取实例。");
    sakVersionSingleton = this;
    ui->setupUi(this);

    version = ui->labelVersion;
    datetime = ui->labelDatetime;
    author = ui->labelAuthor;
    email = ui->labelEmail;
    qq = ui->labelQQ;
    blog = ui->labelBlog;
    blog->installEventFilter(this);
    blog->setCursor(Qt::PointingHandCursor);
    copyrightLabel = ui->copyrightLabel;

    copyQQ = ui->pushButtonCopy;

    version->setText(QString("2.2.0"));
    datetime->setText(buildDate.toString("yyyy/MM/dd") + " " + QString(__TIME__));
    author->setText(QString("Qter"));
    email->setText(QString("wuuhii@outlook.com"));
    qq->setText(QString("952218522"));
    blog->setText(QString("http://wuhai.pro/"));
    copyrightLabel->setText(QString("Copyright(C) 2018-%1 Qter. All rights reserved.").arg(buildDate.toString("yyyy")));

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Tool);
    setModal(true);

    connect(copyQQ, SIGNAL(clicked()), this, SLOT(copyQQNum()));
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

void SAKVersion::copyQQNum()
{
    QLineEdit temp;
    temp.setText(qq->text());
    temp.selectAll();
    temp.copy();
}
