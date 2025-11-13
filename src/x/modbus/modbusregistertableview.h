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
#include <QMenu>
#include <QStandardItem>
#include <QWidget>

namespace Ui {
class ModbusRegisterTableView;
}

namespace xModbus {

class ModbusRegisterTable;
class ModbusRegisterDelegate;
class ModbusRegisterTableFilter;
class ModbusRegisterTableView : public QWidget
{
    Q_OBJECT
public:
    explicit ModbusRegisterTableView(QWidget *parent = nullptr);
    ~ModbusRegisterTableView() override;

    QJsonObject save() const;
    void load(const QJsonObject &obj);
    void selectRow(int row);
    void setServerAddressColumnVisible(bool visible);
    ModbusRegisterTable *registerTable() const;

private:
    ModbusRegisterTable *m_registerTable{nullptr};
    ModbusRegisterTableFilter *m_registerTableFilter{nullptr};
    Ui::ModbusRegisterTableView *ui{nullptr};
    ModbusRegisterDelegate *m_registerDelegate{nullptr};

    QMenu *m_columnMenu{nullptr};

private:
    void onFilterTextChanged(const QString &text);
    void onAddRegisterButtonClicked();
    void onRemoveRegisterButtonClicked();
    void onClearRegistersButtonClicked();
    void onSaveRegistersButtonClicked();
    void onLoadRegistersButtonClicked();

    void resetColumnMenu();
    QJsonArray saveRegisters() const;
    void loadRegisters(const QJsonArray &array);
};

} // namespace xModbus