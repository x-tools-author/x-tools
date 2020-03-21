/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKSAVEOUTPUTDATASETTINGS_HH
#define SAKSAVEOUTPUTDATASETTINGS_HH

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

namespace Ui {
    class SAKSaveOutputDataSettings;
}

class SAKSaveOutputDataThread;
class SAKSaveOutputDataSettings:public QDialog
{
    Q_OBJECT
public:
    SAKSaveOutputDataSettings(QWidget *parent = nullptr);
    ~SAKSaveOutputDataSettings();

    /**
     * @brief inputData 需要保存的数据由此输入
     * @param data 需要保存的数据
     */
    void inputData(QByteArray data);

    struct SaveOutputDataParamters {
        enum Format{
            Bin,
            Utf8,
            Hex
        }format;
        QString fileName;
    }parameters;
private:
    QString defaultPath;
    SAKSaveOutputDataThread *saveOutputDataThread;

private:
    Ui::SAKSaveOutputDataSettings *ui;

    QLineEdit    *pathLineEdit;
    QPushButton  *setFilePushButton;
    QRadioButton *binRadioButton;
    QRadioButton *utf8RadioButton;
    QRadioButton *hexRadioButton;
    QPushButton  *closePushButton;
    QPushButton  *clearFilePushButton;
private slots:
    void on_setFilePushButton_clicked();
    void on_clearFilePushButton_clicked();
signals:
    void writeDataToFile(QByteArray data, SaveOutputDataParamters parameters);
};
Q_DECLARE_METATYPE(SAKSaveOutputDataSettings::SaveOutputDataParamters);
#endif
