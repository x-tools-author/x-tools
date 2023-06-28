/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSTATISTICIANTOOL_HH
#define SAKSTATISTICIANTOOL_HH

#include "SAKBaseTool.hh"

class SAKStatisticianTool : public SAKBaseTool
{
    Q_OBJECT
    Q_PROPERTY(int frames READ frames NOTIFY framesChanged)
    Q_PROPERTY(int bytes READ bytes NOTIFY bytesChanged)
public:
    explicit SAKStatisticianTool(QObject *parent = nullptr);

    virtual void inputBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;

protected:
    virtual void run() final;

private:
    int mFrames{0};
    int mBytes{0};

private:
    int frames(){return mFrames;}
    int bytes(){return mBytes;}

signals:
    void framesChanged(int frames);
    void bytesChanged(int bytes);
};

#endif // SAKSTATISTICIANTOOL_HH
