/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef CRCCALCULATOR_H
#define CRCCALCULATOR_H

#include <QWidget>
#include <QComboBox>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QCheckBox>
#include <QRadioButton>
#include <QTextEdit>
#include <QPushButton>

namespace Ui {
class CRCCalculator;
}

class CRCCalculator:public QWidget
{
    Q_OBJECT
public:
    CRCCalculator(QWidget* parent = nullptr);
    ~CRCCalculator();
private:
    Ui::CRCCalculator* ui;
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

    void initParameterModel();
    QJsonObject jsonObjectAt(int index);

    /// 字节按位翻转
    quint8 reverse(quint8 inByte);
private slots:
    void changedParameterModel(int index);
    void calculate();
};
#endif
