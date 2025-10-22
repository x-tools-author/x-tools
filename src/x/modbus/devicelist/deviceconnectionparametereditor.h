/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QDialog>

namespace Ui {
class DeviceConnectionParameterEditor;
}

namespace xModbus {

class DeviceConnectionParameterEditor : public QDialog
{
    Q_OBJECT
public:
    explicit DeviceConnectionParameterEditor(QWidget *parent = nullptr);
    ~DeviceConnectionParameterEditor() override;

    void load(const QJsonObject &parameters);
    QJsonObject save() const;

private:
    Ui::DeviceConnectionParameterEditor *ui;

private:
    void onRefreshButtonClicked();

    void onDeviceTypeChanged();
};

} // namespace xModbus