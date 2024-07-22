/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMutex>
#include <QTimer>
#include <QVariant>

#include "xToolsBaseTool.h"

class xToolsAnalyzerTool : public xToolsBaseTool
{
    Q_OBJECT
public:
    explicit xToolsAnalyzerTool(QObject *parent = Q_NULLPTR);

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
