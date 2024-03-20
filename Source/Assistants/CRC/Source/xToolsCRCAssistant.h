/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>

namespace Ui {
class xToolsCRCAssistant;
}

class xToolsCrcInterface;
class xToolsCRCAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE xToolsCRCAssistant(QWidget* parent = Q_NULLPTR);
    ~xToolsCRCAssistant();

protected:
    bool eventFilter(QObject* watched, QEvent* event);

private:
    xToolsCrcInterface* m_crcInterface;

    Ui::xToolsCRCAssistant* ui;
    QComboBox* m_widthComboBox;
    QComboBox* m_parameterComboBox;
    QCheckBox* m_refinCheckBox;
    QCheckBox* m_refoutCheckBox;
    QLineEdit* m_polyLineEdit;
    QLineEdit* m_initLineEdit;
    QLineEdit* m_xorLineEdit;
    QRadioButton* m_hexRadioBt;
    QRadioButton* m_asciiRadioBt;
    QLineEdit* m_hexCRCOutput;
    QLineEdit* m_binCRCOutput;
    QTextEdit* m_inputTextEdit;
    QPushButton* m_calculatedBt;
    QLabel* m_labelPolyFormula;
    QLabel* m_labelInfo;

private:
    void initParameterModel();

    void calculate();
    void textFormatControl();
    void changedParameterModel(int index);
};

