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
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#ifndef SAVEOUTPUTDATASETTINGS_HH
#define SAVEOUTPUTDATASETTINGS_HH

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

namespace Ui {
    class SaveOutputDataSettings;
}

class SaveOutputDataThread;
class SaveOutputDataSettings:public QDialog
{
    Q_OBJECT
public:
    SaveOutputDataSettings(QWidget *parent = nullptr);
    ~SaveOutputDataSettings();

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
    SaveOutputDataThread *saveOutputDataThread;

private:
    Ui::SaveOutputDataSettings *ui;

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
Q_DECLARE_METATYPE(SaveOutputDataSettings::SaveOutputDataParamters);
#endif
