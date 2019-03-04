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
