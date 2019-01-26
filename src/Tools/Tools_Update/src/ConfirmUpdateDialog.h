/**
 * @copybrief   Copyright © 2018, WuHai all rights reserve.
 * @author      wuhai
 * @em          wuhai1024@outlook.com
 * @date        The file is created on Dec. 29, 2018 by wuhai.
 * @brief       The device info.
 * @version
 *      20181229  --v1.0.0 create the file.
 */

#ifndef CONFIRMUPDATEDIALOG_H
#define CONFIRMUPDATEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QUrl>

namespace Ui {
class ConfirmUpdateDialog;
}

class ConfirmUpdateDialog:public QDialog
{
    Q_OBJECT
public:
    ConfirmUpdateDialog();
    ~ConfirmUpdateDialog();
    void setInfo(QString version, QUrl url, QString description);
    void setCurrentVersion(QString version){oldVersion->setText(version);}
private:
    Ui::ConfirmUpdateDialog* ui = nullptr;
    QLabel* newVersion;
    QLabel* oldVersion;
    QLabel* desc;

    QUrl address;
private slots:
    void need2download(){emit download(address);}
signals:
    void download(QUrl url);
};

#endif
