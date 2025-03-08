/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMenu>

#include "page/common/tableview.h"

class PresetModel;
class PresetView : public TableView
{
    Q_OBJECT
public:
    explicit PresetView(QWidget *parent = nullptr);
    ~PresetView();
    QMenu *menu();

protected:
    QList<int> textItemColumns() const override;

private:
    QMenu *m_menu{nullptr};
    PresetModel *m_tableModel;

private:
    void onActionTriggered(int row);
    void onDataChanged();
};
