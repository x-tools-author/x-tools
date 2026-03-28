/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCanBusFrame>
#include <QWidget>

#include "framelistcommon.h"

namespace xCanBus {

class FrameListItemEditorPrivate;
class FrameListItemEditor : public QWidget
{
    Q_OBJECT
    FrameListItemEditorPrivate *d{nullptr};

public:
    explicit FrameListItemEditor(QWidget *parent = nullptr);
    ~FrameListItemEditor() override;

    FrameItem frameItem() const;
    void setFrameItem(const FrameItem &frameItem);

signals:
    void rejected();
    void accepted();
};

} // namespace xCanBus