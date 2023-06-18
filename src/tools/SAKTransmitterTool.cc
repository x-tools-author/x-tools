/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QTimer>
#include "SAKTransmitterTool.hh"

SAKTransmitterTool::SAKTransmitterTool(const char *logCategory, QObject *parent)
    : SAKTableModelTool{logCategory, parent}
{
    connect(mTableModel, &QAbstractTableModel::dataChanged,
            this, &SAKTransmitterTool::onDataChanged);
}

int SAKTransmitterTool::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    int ret = mToolVector.length();
    return ret;
}

bool SAKTransmitterTool::removeRows(int row, int count,
                                    const QModelIndex &parent)
{
    Q_UNUSED(parent)
    bool isValidRow = (row >= 0 && row < mToolVector.count());
    Q_ASSERT_X(isValidRow, __FUNCTION__, "invalid row");

    bool isValidCount = (count >= 0 && count <= mToolVector.count());
    Q_ASSERT_X(isValidCount, __FUNCTION__, "invalid count");

    if (isValidCount && isValidRow) {
        while (count--) {
            auto tool = mToolVector.takeAt(row);
            tool->exit();
            tool->wait();
            tool->deleteLater();
            tool = nullptr;
        }

        return true;
    }

    return false;
}


bool SAKTransmitterTool::insertRows(int row, int count,
                                    const QModelIndex &parent)
{
    Q_UNUSED(parent)

    auto initTool = [=](SAKCommunicationTool* tool){
        tool->setParent(this);
        connect(this, &SAKCommunicationTool::bytesInputted,
                tool, &SAKCommunicationTool::inputBytes);
        connect(tool, &SAKCommunicationTool::bytesOutputted,
                this, &SAKCommunicationTool::bytesOutputted);
        connect(this, &SAKCommunicationTool::started,
                tool, [=](){tool->start();});
        connect(this, &SAKCommunicationTool::finished,
                tool, [=](){tool->exit();});

        connect(tool, &SAKCommunicationTool::finished, this, [=](){
            // Reboot the transmitter thread if tool box is wroking.
            if (this->isRunning()) {
                QTimer::singleShot(1*1000, tool, [=](){
                    qCDebug(mLoggingCategory) << "reboot...";
                    tool->start();
                });
            }
        });
    };

    for (int i = 0; i < count; i++) {
        mToolVectorMutex.lock();
        auto tool = createTool();
        initTool(tool);
        mToolVector.insert(row, tool);
        mToolVectorMutex.unlock();
    }

    return true;
}

void SAKTransmitterTool::onDataChanged(const QModelIndex &topLeft,
                                       const QModelIndex &bottomRight,
                                       const QList<int> &roles)
{
    Q_UNUSED(bottomRight)
    Q_UNUSED(roles)
    Q_UNUSED(topLeft)
#if 0
    int index = topLeft.row();
    if (index >= 0 && index < mToolVector.count()) {
        auto tool = mToolVector.at(index);
        tool->exit();
        tool->wait();
        tool->start();
    }
#endif
}
