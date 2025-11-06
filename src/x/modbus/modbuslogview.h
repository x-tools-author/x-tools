/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QWidget>

namespace Ui {
class ModbusLogView;
} // namespace Ui

namespace xModbus {

class ModbusLogModel;
class ModbusLogModelFilter;
class ModbusLogView : public QWidget
{
    Q_OBJECT
public:
    explicit ModbusLogView(QWidget *parent = nullptr);
    ~ModbusLogView() override;

    QJsonObject save();
    void load(const QJsonObject &obj);

private:
    Ui::ModbusLogView *ui;
    ModbusLogModel *m_logModel;
    ModbusLogModelFilter *m_logModelFilter{nullptr};

private:
    void onLogTypeChanged();
    void onFilterTextChanged(const QString &text);
    void onClearLogClicked();
    void onSaveLogClicked();
    void onOpenLogClicked();
    void onIgnoreDataLogClicked();
    void onUsingColorClicked();
};

} // namespace xModbus