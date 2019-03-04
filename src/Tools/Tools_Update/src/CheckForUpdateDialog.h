/**
 * @copybrief   Copyright © 2018, WuHai all rights reserve.
 * @author      wuhai
 * @em          wuhai1024@outlook.com
 * @date        The file is created on Dec. 29, 2018 by wuhai.
 * @brief       The device info.
 * @version
 *      20181229  --v1.0.0 create the file.
 */
#ifndef CHECKFORUPDATEDIALOG_H
#define CHECKFORUPDATEDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QPushButton>
#include <QProgressBar>

class HttpAnalyzer;

namespace Ui {
class CheckForUpdateDialog;
}

class CheckForUpdateDialog:public QDialog
{
    Q_OBJECT
public:
    CheckForUpdateDialog(HttpAnalyzer* httpAnalyzer);
    ~CheckForUpdateDialog();

    void checkForUpdate();
private:
    Ui::CheckForUpdateDialog* ui = nullptr;
    QPushButton* cancelBt = nullptr;
    HttpAnalyzer* analyzer = nullptr;

    QLabel* infoLabel = nullptr;
    QProgressBar* progressBar = nullptr;
private slots:
    void cancel();
    void showErrorDialog(QString errStr);
    void noNewVersion();
signals:
    void cancelChecking();
};

#endif
