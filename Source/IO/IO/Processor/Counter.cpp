/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsStatisticianTool.h"

xToolsStatisticianTool::xToolsStatisticianTool(QObject *parent)
    : xToolsBaseTool{parent}
{
    connect(this, &xToolsStatisticianTool::started, this, [=]() {
        this->m_frames = 0;
        this->m_bytes = 0;

        emit framesChanged(this->m_frames);
        emit bytesChanged(this->m_bytes);
    });
}

void xToolsStatisticianTool::inputBytes(const QByteArray &bytes)
{
    m_frames += 1;
    emit framesChanged(m_frames);

    m_bytes += bytes.length();
    emit bytesChanged(m_bytes);
}

void xToolsStatisticianTool::run()
{
    exec();
}
