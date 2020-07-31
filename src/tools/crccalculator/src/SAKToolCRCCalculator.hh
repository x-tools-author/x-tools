/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTOOLCRCCALCULATOR_HH
#define SAKTOOLCRCCALCULATOR_HH

#include <QLabel>
#include <QDialog>
#include <QComboBox>
#include <QTextEdit>
#include <QCheckBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QPushButton>
#include <QRadioButton>
#include <QJsonDocument>
#include <QJsonParseError>

#include "SAKToolBase.hh"
#include "SAKCRCInterface.hh"

namespace Ui {
    class SAKToolCRCCalculator;
}

/// @brief crc计算器
class SAKToolCRCCalculator:public SAKToolBase
{
    Q_OBJECT
public:
    SAKToolCRCCalculator(QWidget* parent = Q_NULLPTR);
    ~SAKToolCRCCalculator();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    const char *logCategory;
    SAKCRCInterface crcInterface;
private:
    void initParameterModel();
private slots:
    void calculate();
    void textFormatControl();
    void changedParameterModel(int index);
private:
    Ui::SAKToolCRCCalculator* ui;
    QComboBox* widthComboBox;
    QComboBox* parameterComboBox;

    QCheckBox* refinCheckBox;
    QCheckBox* refoutCheckBox;
    QLineEdit* polyLineEdit;
    QLineEdit* initLineEdit;
    QLineEdit* xorLineEdit;

    QRadioButton* hexRadioBt;
    QRadioButton* asciiRadioBt;

    QLineEdit* hexCRCOutput;
    QLineEdit* binCRCOutput;

    QTextEdit* inputTextEdit;

    QPushButton* calculatedBt;
    QLabel *labelPolyFormula;
    QLabel *labelInfo;
};
#endif
