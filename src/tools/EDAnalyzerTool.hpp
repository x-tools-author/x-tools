/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDANALYZERTOOL_H
#define EDANALYZERTOOL_H

#include <QTimer>
#include <QMutex>

#include "EDBaseTool.hpp"

class EDAnalyzerTool : public EDBaseTool
{
    Q_OBJECT
public:
    explicit EDAnalyzerTool(QObject *parent = Q_NULLPTR);

    Q_INVOKABLE void setFixed(bool fixed);
    Q_INVOKABLE void setFrameBytes(int bytes);
    Q_INVOKABLE void setSeparationMark(const QByteArray &mark);
    Q_INVOKABLE void setMaxTempBytes(int maxBytes);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void inputBytesHandler(const QByteArray &bytes) final;
    virtual void uninitialize() final;

private:
    struct Parameters {
        bool fixed;
        int frameBytes;
        QByteArray separationMark;
        int maxTempBytes{1024};
    }mParameters;
    QTimer *mHandleTimer{Q_NULLPTR};
    QByteArray mInputtedBytes;
    QMutex mInputtedBytesMutex;



private:
    void analyze();
    void analyzeFixed();
    void analyzeFlag();
};

#endif // EDANALYZERTOOL_H
