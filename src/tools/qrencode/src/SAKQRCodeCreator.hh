/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
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
    SAKCRCCalculator(QWidget* parent = Q_NULLPTR);
    ~SAKCRCCalculator();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    const char *logCategory = "CRCCalculator";
    SAKCRCInterface crcInterface;

    Ui::SAKCRCCalculator* ui;
    QComboBox* widthComboBox = Q_NULLPTR;
    QComboBox* parameterComboBox = Q_NULLPTR;

    QCheckBox* refinCheckBox = Q_NULLPTR;
    QCheckBox* refoutCheckBox = Q_NULLPTR;
    QLineEdit* polyLineEdit = Q_NULLPTR;
    QLineEdit* initLineEdit = Q_NULLPTR;
    QLineEdit* xorLineEdit = Q_NULLPTR;

    QRadioButton* hexRadioBt = Q_NULLPTR;
    QRadioButton* asciiRadioBt = Q_NULLPTR;

    QLineEdit* hexCRCOutput = Q_NULLPTR;
    QLineEdit* binCRCOutput = Q_NULLPTR;

    QTextEdit* inputTextEdit = Q_NULLPTR;

    QPushButton* calculatedBt = Q_NULLPTR;
    QLabel *labelPolyFormula = Q_NULLPTR;
    QLabel *labelInfo = Q_NULLPTR;

    void initParameterModel();
private slots:
    void changedParameterModel(int index);
    void calculate();
    void textFormatControl();
};
#endif
