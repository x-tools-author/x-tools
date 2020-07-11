/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
    SAKSaveOutputDataSettings(QWidget *parent = Q_NULLPTR);
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
