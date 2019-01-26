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

#include <QDialog>
#include <QPushButton>

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
private slots:
    void cancel();
    void showErrorDialog(QString errStr);
signals:
    void cancelChecking();
};

#endif
