/**
 * @copybrief   Copyright © 2018, WuHai all rights reserve.
 * @author      wuhai
 * @em          wuhai1024@outlook.com
 * @date        The file is created on Dec. 29, 2018 by wuhai.
 * @brief       The device info.
 * @version
 *      20181229  --v1.0.0 create the file.
 */
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "ConfirmUpdateDialog.h"
#include "ui_ConfirmUpdateDialog.h"


ConfirmUpdateDialog::ConfirmUpdateDialog()
    :ui(new Ui::ConfirmUpdateDialog)
{
    ui->setupUi(this);

    newVersion = ui->labelUpdateVersion;
    oldVersion = ui->labelCurrentVersion;
    desc       = ui->labelDescription;

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Tool);
    setModal(true);

    connect(this, &ConfirmUpdateDialog::accepted, this, &ConfirmUpdateDialog::need2download);
}

ConfirmUpdateDialog::~ConfirmUpdateDialog()
{

}

void ConfirmUpdateDialog::setInfo(QString version, QUrl url, QString description)
{
    newVersion->setText(version);
    address = url;
    desc->setText(description);
}
