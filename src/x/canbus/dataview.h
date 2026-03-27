/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

#include "utilities/serializable.h"

namespace Ui {
class DataView;
}

namespace xCanBus {

class DataModel;
class DataFilter;
class DataView : public QWidget, public xTools::Serializable
{
    Q_OBJECT
public:
    explicit DataView(QWidget *parent = nullptr);
    ~DataView() override;
    QJsonObject save() override;
    void load(const QJsonObject &obj) override;

    DataModel *model();

private:
    Ui::DataView *ui{nullptr};
    DataModel *m_model{nullptr};
    DataFilter *m_filter{nullptr};

private:
    void onClearBtnClicked();
    void onRowInserted();
};

} // namespace xCanBus