/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QVariantMap>

#include "page/panels/common/panel.h"

namespace Ui {
class DataRecordsView;
}

class DataRecordsModel;
class DataRecordsModelFilter;
class DataRecordsView : public Panel
{
    Q_OBJECT
public:
    explicit DataRecordsView(QWidget *parent = nullptr);
    ~DataRecordsView();

    void onBytesRead(const QByteArray &bytes, const QString &flag) override;
    void onBytesWritten(const QByteArray &bytes, const QString &flag) override;

    void load(const QVariantMap &parameters) override;
    QVariantMap save() const override;

private:
    Ui::DataRecordsView *ui;
    DataRecordsModel *m_model;
    DataRecordsModelFilter *m_modelFilter;

private:
    void tryAddFlag(const QString &flag);

    void onTypeChanged();
    void onFlagChanged();
    void onFormatChanged();
    void onClear();
    void onRecordsCountChanged();
    void onSearchTextChanged(const QString &text);
};