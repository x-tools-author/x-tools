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

namespace Ui {
    class SAKToolCRCCalculator;
}

class SAKCommonCrcInterface;
class SAKToolCRCCalculator:public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKToolCRCCalculator(QWidget* parent = Q_NULLPTR);
    ~SAKToolCRCCalculator();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    const char *mLogCategory;
    SAKCommonCrcInterface *mCrcInterface;
private:
    void initParameterModel();
private slots:
    void calculate();
    void textFormatControl();
    void changedParameterModel(int index);
private:
    Ui::SAKToolCRCCalculator* mUi;
    QComboBox* mWidthComboBox;
    QComboBox* mParameterComboBox;
    QCheckBox* mRefinCheckBox;
    QCheckBox* mRefoutCheckBox;
    QLineEdit* mPolyLineEdit;
    QLineEdit* mInitLineEdit;
    QLineEdit* mXorLineEdit;
    QRadioButton* mHexRadioBt;
    QRadioButton* mAsciiRadioBt;
    QLineEdit* mHexCRCOutput;
    QLineEdit* mBinCRCOutput;
    QTextEdit* mInputTextEdit;
    QPushButton* mCalculatedBt;
    QLabel *mLabelPolyFormula;
    QLabel *mLabelInfo;
};
#endif
