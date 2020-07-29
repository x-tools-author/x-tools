/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERANALYZERTHREAD_HH
#define SAKOTHERANALYZERTHREAD_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

/// @brief data analyzer
class SAKOtherAnalyzerThread:public QThread
{
    Q_OBJECT
public:
    /// @brief Analyzing parameters
    struct ParametersContext {
        bool enable; // true-enable analyzing, false-diable analyzing
        bool fixed; // true-the bytes of frame is fixed
        qint32 length; // the length of frame
        QByteArray startArray; // start bytes of frame
        QByteArray endArray; // end bytes of frame
    };

    SAKOtherAnalyzerThread(QObject *parent = Q_NULLPTR);
    ~SAKOtherAnalyzerThread();

    /**
     * @brief clearData: Clear the temp data
     */
    void clearData();

    /**
     * @brief inputBytes: Input bytes that need to be analyzed
     * @param array: The bytes that need to be analyzed
     */
    void inputBytes(QByteArray array);

    // Interfaces of setting parameters
    void setEnable(bool enable);
    void setFixed(bool fixed);
    void setLength(int length);
    void setStartArray(QByteArray array);
    void setEndArray(QByteArray array);
protected:
    void run() final;
private:
    QMutex mThreadMutex;
    QWaitCondition mThreadCondition;
    QByteArray mWaitingAnalyzingBytes;
    QMutex mWaitingAnalyzingBytesMutex;
    ParametersContext mParameters;
    QMutex mParametersMutex;
signals:
    // Bytes that analyzed will be emited
    void bytesAnalyzed(QByteArray bytes);
};

#endif
