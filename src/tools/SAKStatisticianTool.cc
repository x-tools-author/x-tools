/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKStatisticianTool.hh"

SAKStatisticianTool::SAKStatisticianTool(QObject *parent)
    : SAKBaseTool{"SAK.StatisticianTool", parent}
{
    connect(this, &SAKStatisticianTool::started, this, [=](){
        this->mFrames = 0;
        this->mBytes = 0;

        emit framesChanged(this->mFrames);
        emit bytesChanged(this->mBytes);
    });
}

void SAKStatisticianTool::inputBytes(const QByteArray &bytes,
                                     const QVariant &context)
{
    mFrames += 1;
    emit framesChanged(mFrames);

    mBytes += bytes.length();
    emit bytesChanged(mBytes);
}

void SAKStatisticianTool::run()
{
    exec();
}
