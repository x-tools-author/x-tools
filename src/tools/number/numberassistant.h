/***************************************************************************************************
 * Copyright 2020-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace Ui {
class NumberAssistant;
}

class xToolsInterface;
class NumberAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE NumberAssistant(QWidget* parent = Q_NULLPTR);
    ~NumberAssistant();

private:
    enum CookedDataType {
        CookedDataTypeInt8,
        CookedDataTypeUint8,
        CookedDataTypeInt16,
        CookedDataTypeUint16,
        CookedDataTypeInt32,
        CookedDataTypeUint32,
        CookedDataTypeInt64,
        CookedDataTypeUint64,
        CookedDataTypeFloat,
        CookedDataTypeDouble
    };

private:
    Ui::NumberAssistant* ui;
    xToolsInterface* m_interface;

private:
    void updateCookedData();
    void updateRawData();
    int bytesOfType(int type);
    void onCookedDataTypeChanged();
};
