/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDebug>
#include <QEventLoop>

#include "SAKProtocolAnalyzer.hh"

SAKProtocolAnalyzer::SAKProtocolAnalyzer(QObject *parent)
    :QThread(parent)
{
    mParameters.enable = false;
    mParameters.fixed = false;
    mParameters.length = 0;
    mParameters.startArray = QByteArray();
    mParameters.endArray = QByteArray();
}

SAKProtocolAnalyzer::~SAKProtocolAnalyzer()
{
    requestInterruption();
    wakeMe();
    exit();
    wait();
#ifdef QT_DEBUG
    qDebug() << __FUNCTION__ << "protocol analyzer has been destroyed";
#endif
}

void SAKProtocolAnalyzer::run()
{
    QEventLoop eventLoop;
    int maxTempLength = 2048;
    while (1) {
        /// @brief 处理外部中断事件
        if (isInterruptionRequested()){
            break;
        }

        /// @brief 获取参数
        mParametersMutex.lock();
        ParametersContext parameters = mParameters;
        mParametersMutex.unlock();

        /// @brief 分析数据
        mWaitingAnalyzingBytesMutex.lock();

        /// @brief 限制缓存数据大小
        if (mWaitingAnalyzingBytes.length() >= maxTempLength){
            QByteArray temp = QByteArray(mWaitingAnalyzingBytes.data(), maxTempLength);
            mWaitingAnalyzingBytes.remove(0, temp.length());
            emit bytesAnalized(temp);
        }

        if (mParameters.fixed){ // 根据协议长度提取
            if (mParameters.length > 0){
                while (mWaitingAnalyzingBytes.length() >= mParameters.length) {
                    QByteArray temp = QByteArray(mWaitingAnalyzingBytes.data(), parameters.length);
                    mWaitingAnalyzingBytes.remove(0, temp.length());
                    emit bytesAnalized(temp);
                }
            }else{  // 长度小于或者等于0，不再提取数据
                if (mWaitingAnalyzingBytes.length()){
                    emit bytesAnalized(mWaitingAnalyzingBytes);
                    mWaitingAnalyzingBytes.clear();
                }
            }
        }else{  // 根据首尾标志提取
            /// @brief 未指定首尾标志时，将数据全部发出
            if (parameters.startArray.isEmpty() && parameters.endArray.isEmpty()){
                if (mWaitingAnalyzingBytes.length()){
                    emit bytesAnalized(mWaitingAnalyzingBytes);
                    mWaitingAnalyzingBytes.clear();
                }
            }else{
                while(mWaitingAnalyzingBytes.length() > (parameters.startArray.length() + parameters.endArray.length())){
                    /// @brief 匹配起始字节
                    bool startBytesMatched = true;
                    for (int i = 0; i < parameters.startArray.length(); i++){
                        if (parameters.startArray.at(i) != mWaitingAnalyzingBytes.at(i)){
                            startBytesMatched = false;
                            mWaitingAnalyzingBytes.remove(i, 1);
                            break;
                        }
                    }

                    /// @brief 匹配结束字节
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

                    /// @brief 起始字节及结束字节都匹配完成后才对外发送数据
                    if (startBytesMatched && endBytesMatched){
                        QByteArray temp(mWaitingAnalyzingBytes.data(), frameLength);
                        emit bytesAnalized(temp);
                        mWaitingAnalyzingBytes.remove(0, temp.length());
                    }
                }
            }
        }
        mWaitingAnalyzingBytesMutex.unlock();

        if (!isInterruptionRequested()){
            /// @brief 处理事件吹动
            eventLoop.processEvents();
            /// @brief 每隔20毫秒处理一次数据
            mThreadMutex.lock();
            mThreadCondition.wait(&mThreadMutex, 20);
            mThreadMutex.unlock();
        }
    }
}

void SAKProtocolAnalyzer::clearData()
{
    mWaitingAnalyzingBytesMutex.lock();
    mWaitingAnalyzingBytes.clear();
    mWaitingAnalyzingBytesMutex.unlock();
}

void SAKProtocolAnalyzer::wakeMe()
{
    mThreadCondition.wakeAll();
}

void SAKProtocolAnalyzer::inputBytes(QByteArray array)
{
    /// @brief 不处理空数据
    if (array.isEmpty()){
        return;
    }

    mParametersMutex.lock();
    ParametersContext parameters = mParameters;
    mParametersMutex.unlock();

    if (parameters.enable){
        mWaitingAnalyzingBytesMutex.lock();
        mWaitingAnalyzingBytes.append(array);
        mWaitingAnalyzingBytesMutex.unlock();
    }else{
        /// @brief 不使用协议分析提取功能，直接将数据对外发送
        emit bytesAnalized(array);
    }
}

void SAKProtocolAnalyzer::setEnable(bool enable)
{
    mParametersMutex.lock();
    mParameters.enable = enable;
    mParametersMutex.unlock();
}

void SAKProtocolAnalyzer::setFixed(bool fixed)
{
    mParametersMutex.lock();
    mParameters.fixed = fixed;
    mParametersMutex.unlock();
}

void SAKProtocolAnalyzer::setLength(int length)
{
    mParametersMutex.lock();
    mParameters.length = length;
    mParametersMutex.unlock();
}

void SAKProtocolAnalyzer::setStartArray(QByteArray array)
{
    mParametersMutex.lock();
    mParameters.startArray = array;
    mParametersMutex.unlock();
}

void SAKProtocolAnalyzer::setEndArray(QByteArray array)
{
    mParametersMutex.lock();
    mParameters.endArray = array;
    mParametersMutex.unlock();
}
