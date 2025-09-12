/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "datapersistence.h"
#include "datapersistenceui.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <QTimer>
#include <QVariantMap>

#include "common/common.h"

class DataPersistenceThread : public QThread
{
public:
    DataPersistenceThread(DataPersistence *dataPersistence, QObject *parent = nullptr)
        : QThread(parent)
        , m_dataPersistence(dataPersistence)
    {}

private:
    DataPersistence *m_dataPersistence{nullptr};

protected:
    void run() override
    {
        QTimer timer;
        QList<QByteArray> bytesList;
        connect(m_dataPersistence,
                &DataPersistence::input2thread,
                &timer,
                [&bytesList](std::shared_ptr<QtNodes::NodeData> nodeData,
                             QtNodes::PortIndex const portIndex) {
                    auto data = std::dynamic_pointer_cast<BaseNodeData>(nodeData);
                    if (data) {
                        bytesList.append(data->bytes());
                    }
                });

        connect(&timer, &QTimer::timeout, &timer, [&, this]() {
            QJsonObject parameters = this->m_dataPersistence->save();
            DataPersistence::Data data = this->m_dataPersistence->jsonObject2Data(parameters);
            if (data.fileName.isEmpty()) {
                return;
            }

            QFile file(data.fileName);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
                qWarning() << "Failed to open file:" << data.fileName;
                return;
            }

            if (data.binary) {
                QDataStream stream(&file);
                for (const auto &bytes : bytesList) {
                    stream.writeRawData(bytes.data(), bytes.size());
                }
            } else {
                QString dateTimeString = "";
                QDateTime dateTime = QDateTime::currentDateTime();
                if (data.saveDate) {
                    dateTimeString += dateTime.toString("yyyy-MM-dd");
                }
                if (data.saveTime) {
                    if (!dateTimeString.isEmpty()) {
                        dateTimeString += " ";
                    }

                    if (data.saveMs) {
                        dateTimeString += dateTime.toString("hh:mm:ss.zzz");
                    } else {
                        dateTimeString += dateTime.toString("hh:mm:ss");
                    }
                }

                dateTimeString = dateTimeString.trimmed();
                QTextStream stream(&file);
                for (const auto &bytes : bytesList) {
                    QString text = bytes2string(bytes, data.format);
                    if (dateTimeString.isEmpty()) {
                        stream << text << Qt::endl;
                    } else {
                        stream << QString("[%1]").arg(dateTimeString) + " " + text << Qt::endl;
                    }
                }
            }

            file.close();
            bytesList.clear();
            timer.start();
        });

        timer.setInterval(1000);
        timer.setSingleShot(true);
        timer.start();
        exec();
        timer.stop();
        timer.deleteLater();
    }
};

DataPersistence::DataPersistence(QObject *parent)
    : BaseNode(parent)
    , m_embeddedWidget(nullptr)
{
    m_handleInDataInThread.store(true);
    m_thread = new DataPersistenceThread(this);
    m_thread->start();
}

DataPersistence::~DataPersistence()
{
    if (m_thread) {
        m_thread->exit();
        m_thread->wait();
        m_thread->deleteLater();
    }
}

QString DataPersistence::caption() const
{
    return tr("Data Persistence");
}

QString DataPersistence::name() const
{
    return QString("DataPersistence");
}

unsigned int DataPersistence::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 0;
    } else {
        return 0;
    }
}

QWidget *DataPersistence::embeddedWidget()
{
    if (QThread::currentThread() != qApp->thread()) {
        return m_embeddedWidget;
    }

    if (m_embeddedWidget == nullptr) {
        m_embeddedWidget = new DataPersistenceUi(this);
    }

    return m_embeddedWidget;
}

DataPersistence::Data DataPersistence::jsonObject2Data(const QJsonObject &parameters)
{
    Data data;
    DataKeys keys = DataKeys();
    QVariantMap parametersMap = parameters.toVariantMap();

    data.fileName = parametersMap.value(keys.fileName, "").toString();
    data.format = parametersMap.value(keys.format, static_cast<int>(TextFormat::Hex)).toInt();
    data.saveDate = parametersMap.value(keys.saveDate, false).toBool();
    data.saveTime = parametersMap.value(keys.saveTime, true).toBool();
    data.saveMs = parametersMap.value(keys.saveMs, false).toBool();
    data.binary = parametersMap.value(keys.binary, false).toBool();

    return data;
}

QJsonObject DataPersistence::data2JsonObject(const DataPersistence::Data &data)
{
    QJsonObject parameters = QJsonObject();
    DataKeys keys = DataKeys();

    parameters.insert(keys.fileName, data.fileName);
    parameters.insert(keys.format, data.format);
    parameters.insert(keys.saveDate, data.saveDate);
    parameters.insert(keys.saveTime, data.saveTime);
    parameters.insert(keys.saveMs, data.saveMs);
    parameters.insert(keys.binary, data.binary);

    return parameters;
}