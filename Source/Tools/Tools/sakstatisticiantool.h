/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKSTATISTICIANTOOL_H
#define SAKSTATISTICIANTOOL_H

#include "sakbasetool.h"

class SAKStatisticianTool : public SAKBaseTool
{
    Q_OBJECT
    Q_PROPERTY(int frames READ frames NOTIFY framesChanged)
    Q_PROPERTY(int bytes READ bytes NOTIFY bytesChanged)
public:
    explicit SAKStatisticianTool(QObject *parent = nullptr);

    void inputBytes(const QByteArray &bytes) override;

protected:
    virtual void run() final;

private:
    int mFrames{0};
    int mBytes{0};

private:
    int frames() { return mFrames; }
    int bytes() { return mBytes; }

signals:
    void framesChanged(int frames);
    void bytesChanged(int bytes);
};

#endif // SAKSTATISTICIANTOOL_H
