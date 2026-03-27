/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>
#include <QCanBusFrame>
#include <QList>

#define FRAME_LIST_MODEL_COLUMN_SEND 0        // clicked to send the frame
#define FRAME_LIST_MODEL_COLUMN_NAME 1        // frame name
#define FRAME_LIST_MODEL_COLUMN_CYCLE 2       // cycle sending the frame
#define FRAME_LIST_MODEL_COLUMN_INTERVAL 3    // cycle sending interval
#define FRAME_LIST_MODEL_COLUMN_RESPONSE 4    // response frame
#define FRAME_LIST_MODEL_COLUMN_RESPONSE_ID 5 // response frame id
#define FRAME_LIST_MODEL_COLUMN_FRAME 6       // frame

namespace xCanBus {

class FrameListModelPrivate;
class FrameListModel : public QAbstractTableModel
{
    Q_OBJECT
    FrameListModelPrivate *d{nullptr};

public:
    explicit FrameListModel(QObject *parent = nullptr);
    ~FrameListModel() override;

    void addFrame(const QCanBusFrame &frame, bool isRx);
    void clear();

public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

} // namespace xCanBus