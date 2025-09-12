/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "frameanalyzer.h"
#include "frameanalyzerui.h"

#include <QThread>
#include <QTimer>

class FrameAnalyzerThread : public QThread
{
public:
    FrameAnalyzerThread(FrameAnalyzer *frameAnalyzer)
        : QThread(frameAnalyzer)
        , m_frameAnalyzer(frameAnalyzer)
    {}

    ~FrameAnalyzerThread() override
    {
        if (isRunning()) {
            quit();
            wait();
        }
    }

protected:
    void run() override
    {
        QObject *obj = new QObject();
        QByteArray bytes;
        connect(m_frameAnalyzer,
                &FrameAnalyzer::input2thread,
                obj,
                [&bytes, this](std::shared_ptr<QtNodes::NodeData> nodeData,
                               QtNodes::PortIndex const) {
                    auto data = std::dynamic_pointer_cast<BaseNodeData>(nodeData);
                    if (!data) {
                        return;
                    }

                    bytes.append(data->bytes());
                    this->m_frameAnalyzer->analyze(bytes);
                });
        connect(m_frameAnalyzer, &FrameAnalyzer::invokeClearCache, obj, [&bytes]() {
            bytes.clear();
        });

        exec();
        obj->deleteLater();
    }

private:
    FrameAnalyzer *m_frameAnalyzer{nullptr};
};

FrameAnalyzer::FrameAnalyzer(QObject *parent)
    : BaseNode(parent)
{
    m_handleInDataInThread.store(true);
    FrameAnalyzerThread *thread = new FrameAnalyzerThread(this);
    thread->start();
}

FrameAnalyzer::~FrameAnalyzer() {}

QString FrameAnalyzer::caption() const
{
    return tr("Frame Analyzer");
}

QString FrameAnalyzer::name() const
{
    return QStringLiteral("FrameAnalyzer");
}

unsigned int FrameAnalyzer::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *FrameAnalyzer::embeddedWidget()
{
    if (QThread::currentThread() != this->thread()) {
        return m_embeddedWidget;
    }

    if (!m_embeddedWidget) {
        m_embeddedWidget = new FrameAnalyzerUi(this);
    }

    return m_embeddedWidget;
}

void FrameAnalyzer::analyze(QByteArray &bytes)
{
    FrameAnalyzerParametersKeys keys;
    QJsonObject parameters = save();
    bool fixed = parameters.value(keys.fixed).toBool();
    int length = parameters.value(keys.length).toInt();
    QString splitFlagStr = parameters.value(keys.splitFlag).toString("0A");
    QByteArray splitFlag = QByteArray::fromHex(splitFlagStr.toLatin1());
    int maxCacheBytes = parameters.value(keys.maxCacheBytes).toInt();

    if (bytes.isEmpty()) {
        return;
    }

    if (bytes.size() > maxCacheBytes) {
        bytes.clear();
        return;
    }

    if (fixed) {
        analyzeFixed(bytes, length);
    } else {
        analyzeSeparationMark(bytes, splitFlag);
    }
}

void FrameAnalyzer::analyzeFixed(QByteArray &bytes, int length)
{
    while (bytes.length() >= length) {
        QByteArray frame(bytes.data(), length);
        bytes = bytes.remove(0, length);
        setOutData(std::make_shared<BaseNodeData>(frame));
        emit dataUpdated(0);
    }
}

void FrameAnalyzer::analyzeSeparationMark(QByteArray &bytes, const QByteArray &splitFlag)
{
    if (splitFlag.isEmpty()) {
        QByteArray tmp = bytes;
        bytes.clear();

        setOutData(std::make_shared<BaseNodeData>(tmp));
        emit dataUpdated(0);
        return;
    }

    while (true) {
        auto ret = bytes.indexOf(splitFlag);
        if (ret == -1) {
            return;
        }

        int len = ret + splitFlag.size();
        QByteArray frame(bytes.constData(), len);
        bytes.remove(0, len);
        setOutData(std::make_shared<BaseNodeData>(frame));
        emit dataUpdated(0);
    }
}

void FrameAnalyzer::clearCacheBytes()
{
    emit invokeClearCache();
}