/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
  *
 * Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
 */
#ifndef CRCCALCULATOR_HH
#define CRCCALCULATOR_HH

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

#include "CRCInterface.hh"

namespace Ui {
class CRCCalculator;
}

class CRCCalculator:public QDialog
{
    Q_OBJECT
public:
    CRCCalculator(QWidget* parent = nullptr);
    ~CRCCalculator();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    const char *logCategory = "CRCCalculator";
    CRCInterface crcInterface;

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
    QLabel *labelPolyFormula = nullptr;
    QLabel *labelInfo = nullptr;

    void initParameterModel();
private slots:
    void changedParameterModel(int index);
    void calculate();
    void textFormatControl();
};
#endif
