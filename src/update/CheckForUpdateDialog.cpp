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
#include "CheckForUpdateDialog.h"
#include "ui_CheckForUpdateDialog.h"
#include "HttpAnalyzer.h"

#include <QMessageBox>

CheckForUpdateDialog::CheckForUpdateDialog(HttpAnalyzer *httpAnalyzer)
    :ui(new Ui::CheckForUpdateDialog)
    ,analyzer(httpAnalyzer)
{
    ui->setupUi(this);
    infoLabel = ui->labelInfo;
    progressBar = ui->progressBar;

    cancelBt = ui->pushButtonCancel;
    connect(cancelBt, &QPushButton::clicked, this, &CheckForUpdateDialog::cancel);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Tool);
    setModal(true);

    connect(this, &CheckForUpdateDialog::cancelChecking, analyzer, &HttpAnalyzer::httpCancel);
    connect(analyzer, &HttpAnalyzer::badUrl, this, &CheckForUpdateDialog::showErrorDialog);
    connect(analyzer, &HttpAnalyzer::noNewVersion, this, &CheckForUpdateDialog::noNewVersion);

    analyzer->start();
}

CheckForUpdateDialog::~CheckForUpdateDialog()
{

}

void CheckForUpdateDialog::checkForUpdate()
{
    show();
    analyzer->startRequest();
}

void CheckForUpdateDialog::cancel()
{
    close();
    emit cancelChecking();
}

void CheckForUpdateDialog::showErrorDialog(QString errStr)
{
    cancel();
    QMessageBox::warning(nullptr, tr("检查更新错误"), tr("错误信息：") + errStr);
}

void CheckForUpdateDialog::noNewVersion()
{
    infoLabel->setText(tr("暂无更新版本"));
    progressBar->setRange(0, 100);
    progressBar->setValue(100);
}
