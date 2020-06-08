/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
