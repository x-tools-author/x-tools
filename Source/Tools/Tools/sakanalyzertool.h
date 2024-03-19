/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKANALYZERTOOL_H
#define SAKANALYZERTOOL_H

#include <QMutex>
#include <QTimer>

#include "sakbasetool.h"

class SAKAnalyzerTool : public SAKBaseTool
{
    Q_OBJECT
public:
    explicit SAKAnalyzerTool(QObject *parent = Q_NULLPTR);

    Q_INVOKABLE void setFixed(bool fixed);
    Q_INVOKABLE void setFrameBytes(int bytes);
    Q_INVOKABLE void setSeparationMark(const QByteArray &mark);
    Q_INVOKABLE void setMaxTempBytes(int maxBytes);

    void inputBytes(const QByteArray &bytes) override;

protected:
    virtual void run() final;

private:
    struct Parameters
    {
        bool fixed;
        int frameBytes;
        QByteArray separationMark;
        int maxTempBytes{1024};
    } m_parameters;
    QMutex m_parametersMutex;
    QByteArray m_inputtedBytes;
    QMutex m_inputtedBytesMutex;
    QVariant m_context;

private:
    void analyze();
    void analyzeFixed();
    void analyzeSeparationMark();
};

#endif // SAKANALYZERTOOL_H
