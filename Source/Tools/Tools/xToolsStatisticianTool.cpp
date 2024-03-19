/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakstatisticiantool.h"

SAKStatisticianTool::SAKStatisticianTool(QObject *parent)
    : SAKBaseTool{parent}
{
    connect(this, &SAKStatisticianTool::started, this, [=]() {
        this->mFrames = 0;
        this->mBytes = 0;

        emit framesChanged(this->mFrames);
        emit bytesChanged(this->mBytes);
    });
}

void SAKStatisticianTool::inputBytes(const QByteArray &bytes)
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
