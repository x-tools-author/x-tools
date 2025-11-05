/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xModbus is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QToolBar>
#include <QWidget>

namespace Ui {
class xModbus;
}

namespace xModbus {
class ModbusRegisterTableView;
class xModbus : public QWidget
{
    Q_OBJECT
public:
    explicit xModbus(QWidget* parent = nullptr);
    ~xModbus();

    QJsonObject save();
    void load(const QJsonObject& obj);

private:
    Ui::xModbus* ui;
    QToolBar* m_toolBar{nullptr};

private:
    void onInvokeShowRegisterView(ModbusRegisterTableView* registerView);
};
} // namespace xModbus