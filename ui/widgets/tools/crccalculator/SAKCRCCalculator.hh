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
#ifndef SAKCRCCALCULATOR_HH
#define SAKCRCCALCULATOR_HH

#include <QDialog>
#include <QComboBox>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QCheckBox>
#include <QRadioButton>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

#include "SAKCRCInterface.hh"

namespace Ui {
    class SAKCRCCalculator;
}

class SAKCRCCalculator:public QDialog
{
    Q_OBJECT
public:
    SAKCRCCalculator(QWidget* parent = nullptr);
    ~SAKCRCCalculator();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    const char *logCategory = "CRCCalculator";
    SAKCRCInterface crcInterface;

    Ui::SAKCRCCalculator* ui;
    QComboBox* widthComboBox = nullptr;
    QComboBox* parameterComboBox = nullptr;

    QCheckBox* refinCheckBox = nullptr;
    QCheckBox* refoutCheckBox = nullptr;
    QLineEdit* polyLineEdit = nullptr;
    QLineEdit* initLineEdit = nullptr;
    QLineEdit* xorLineEdit = nullptr;

    QRadioButton* hexRadioBt = nullptr;
    QRadioButton* asciiRadioBt = nullptr;

    QLineEdit* hexCRCOutput = nullptr;
    QLineEdit* binCRCOutput = nullptr;

    QTextEdit* inputTextEdit = nullptr;

    QPushButton* calculatedBt = nullptr;
    QLabel *labelPolyFormula = nullptr;
    QLabel *labelInfo = nullptr;

    void initParameterModel();
private slots:
    void changedParameterModel(int index);
    void calculate();
    void textFormatControl();
};
#endif
