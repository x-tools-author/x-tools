/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QEventLoop>

#include "SAKOtherAnalyzerThread.hh"

SAKOtherAnalyzerThread::SAKOtherAnalyzerThread(QObject *parent)
    :QThread(parent)
{
    mParameters.enable = false;
    mParameters.fixed = false;
    mParameters.length = 0;
    mParameters.startArray = QByteArray();
    mParameters.endArray = QByteArray();
}

SAKOtherAnalyzerThread::~SAKOtherAnalyzerThread()
{
    requestInterruption();
    mThreadCondition.wakeAll();
    exit();
    wait();
}

void SAKOtherAnalyzerThread::run()
{
    QEventLoop eventLoop;
    const int maxTempLength = 2048;
    ParametersContext parameters;
    while (1) {
        // Handle signal and slots
        eventLoop.processEvents();

        // Get parameters
        mParametersMutex.lock();
        parameters = mParameters;
        mParametersMutex.unlock();

        // Lock data
        mWaitingAnalyzingBytesMutex.lock();

        // If the bytes of temp data is more than maxTempLength(2048) bytes,
        // a frame which length is maxTempLength will be emit.
        if (mWaitingAnalyzingBytes.length() >= maxTempLength){
            QByteArray temp = QByteArray(mWaitingAnalyzingBytes.data(), maxTempLength);
            mWaitingAnalyzingBytes.remove(0, temp.length());
            emit bytesAnalyzed(temp);
        }

        // The length of frame is fixed
        if (mParameters.fixed){
            if (mParameters.length > 0){
                while (mWaitingAnalyzingBytes.length() >= mParameters.length) {
                    QByteArray temp = QByteArray(mWaitingAnalyzingBytes.data(), parameters.length);
                    mWaitingAnalyzingBytes.remove(0, temp.length());
                    emit bytesAnalyzed(temp);
                }
            }else{ // If parameters is error(the length is less than 0 or equal to 0), temp data will be clear!
                if (mWaitingAnalyzingBytes.length()){
                    emit bytesAnalyzed(mWaitingAnalyzingBytes);
                    mWaitingAnalyzingBytes.clear();
                }
            }
        }else{ // Extract data according to the flags
            // If both of start-bytes and end-bytes are empty, temp data will be clear
            if (parameters.startArray.isEmpty() && parameters.endArray.isEmpty()){
                if (mWaitingAnalyzingBytes.length()){
                    emit bytesAnalyzed(mWaitingAnalyzingBytes);
                    mWaitingAnalyzingBytes.clear();
                }
            }else{
                while(1){
                    // Ensure that bytes is enough
                    if (mWaitingAnalyzingBytes.length() < (parameters.startArray.length() + parameters.endArray.length())){
                        break;
                    }

                    // Match start-bytes
                    bool startBytesMatched = true;
                    if (parameters.startArray.isEmpty()){
                        startBytesMatched = true;
                    }else{
                        int ret = mWaitingAnalyzingBytes.indexOf(parameters.startArray, 0);
                        if (ret >= 0){
                            startBytesMatched = true;
                            // Remove error data
                            QByteArray temp = QByteArray(mWaitingAnalyzingBytes.data(), ret);
                            mWaitingAnalyzingBytes.remove(0, ret);
                            emit bytesAnalyzed(temp);
                        }else{
                            startBytesMatched = false;
                        }
                    }

                    // Match end-bytes
                    bool endBytesMatched = true;
                    quint32 frameLength = 0;
                    if (parameters.endArray.isEmpty()){
                        endBytesMatched = true;
                    }else {
                        int ret = mWaitingAnalyzingBytes.indexOf(parameters.endArray, parameters.startArray.length());
                        if (ret >= 0){
                            endBytesMatched = true;
                            frameLength = ret + parameters.endArray.length();
                        }else{
                            endBytesMatched = false;
                        }
                    }

                    // A completed data-frame has been extracted
                    if (startBytesMatched && endBytesMatched){
                        QByteArray temp(mWaitingAnalyzingBytes.data(), frameLength);
                        emit bytesAnalyzed(temp);
                        mWaitingAnalyzingBytes.remove(0, temp.length());
                    }
                }
            }
        }

        // Unlock data
        mWaitingAnalyzingBytesMutex.unlock();

        if (!isInterruptionRequested()){
            // Do something make cpu happy
            mThreadMutex.lock();
            mThreadCondition.wait(&mThreadMutex, 20);
            mThreadMutex.unlock();
        }else{
            break;
        }
    }
}

void SAKOtherAnalyzerThread::clearData()
{
    mWaitingAnalyzingBytesMutex.lock();
    mWaitingAnalyzingBytes.clear();
    mWaitingAnalyzingBytesMutex.unlock();
}

void SAKOtherAnalyzerThread::inputBytes(QByteArray array)
{
    // Ignore the empty data
    if (array.length()){
        mParametersMutex.lock();
        ParametersContext parameters = mParameters;
        mParametersMutex.unlock();

        if (parameters.enable){
            mWaitingAnalyzingBytesMutex.lock();
            mWaitingAnalyzingBytes.append(array);
            mWaitingAnalyzingBytesMutex.unlock();
        }else{
            emit bytesAnalyzed(array);
        }

        // Wake thread to analyze data
        mThreadCondition.wakeAll();
    }
}

void SAKOtherAnalyzerThread::setEnable(bool enable)
{
    mParametersMutex.lock();
    mParameters.enable = enable;
    mParametersMutex.unlock();
}

void SAKOtherAnalyzerThread::setFixed(bool fixed)
{
    mParametersMutex.lock();
    mParameters.fixed = fixed;
    mParametersMutex.unlock();
}

void SAKOtherAnalyzerThread::setLength(int length)
{
    mParametersMutex.lock();
    mParameters.length = length;
    mParametersMutex.unlock();
}

void SAKOtherAnalyzerThread::setStartArray(QByteArray array)
{
    mParametersMutex.lock();
    mParameters.startArray = array;
    mParametersMutex.unlock();
}

void SAKOtherAnalyzerThread::setEndArray(QByteArray array)
{
    mParametersMutex.lock();
    mParameters.endArray = array;
    mParametersMutex.unlock();
}
