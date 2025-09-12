/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "aescipher.h"
#include "aescipherui.h"

#include <QtAES/QAESEncryption>

#include <QThread>

#include "nodeeditor/nodes/common/basenodedata.h"
#include "qaesencryption.h"

class AesCipherThread : public QThread
{
public:
    AesCipherThread(AesCipher *aesCipher, QObject *parent = nullptr)
        : QThread(parent)
        , m_aesCipher(aesCipher)
    {}
    ~AesCipherThread()
    {
        exit();
        wait();
    }

private:
    AesCipher *m_aesCipher;

protected:
    void run() override
    {
        typedef std::shared_ptr<QtNodes::NodeData> InData;
        typedef QtNodes::PortIndex PortIndex;
        QObject *obj = new QObject();
        connect(m_aesCipher, &AesCipher::input2thread, obj, [this](InData data, PortIndex index) {
            auto parameters = m_aesCipher->save();
            AesCipherParametersKeys keys;
            int workWay = parameters.value(keys.workWay).toInt();
            QByteArray key = parameters.value(keys.key).toString().toUtf8();
            QByteArray iv = parameters.value(keys.iv).toString().toUtf8();
            int mode = parameters.value(keys.mode).toInt();
            int padding = parameters.value(keys.padding).toInt();
            int level = parameters.value(keys.level).toInt();

            if (level == static_cast<int>(QAESEncryption::AES_128)) {
                if (key.size() != 16) {
                    qWarning() << QString::fromLatin1(QByteArray("(Key size is not 16 bytes)"));
                    return;
                }
            } else if (level == static_cast<int>(QAESEncryption::AES_192)) {
                if (key.size() != 24) {
                    qWarning() << QString::fromLatin1(QByteArray("(Key size is not 24 bytes)"));
                    return;
                }
            } else {
                if (key.size() != 32) {
                    qWarning() << QString::fromLatin1(QByteArray("(Key size is not 32 bytes)"));
                    return;
                }
            }

            if (iv.size() != 16) {
                qWarning() << QString::fromLatin1(QByteArray("(IV size is not 16 bytes)"));
                return;
            }

            auto baseNodeData = std::dynamic_pointer_cast<BaseNodeData>(data);
            QByteArray bytes = baseNodeData->bytes();
            if (bytes.isEmpty()) {
                return;
            }

            QAESEncryption *aes = new QAESEncryption(static_cast<QAESEncryption::Aes>(level),
                                                     static_cast<QAESEncryption::Mode>(mode),
                                                     static_cast<QAESEncryption::Padding>(padding));

            QByteArray result;
            if (workWay == static_cast<int>(AesCipher::WorkWay::Encrypt)) {
                result = aes->encode(bytes, key, iv);
            } else {
                result = aes->decode(bytes, key, iv);
                result = aes->removePadding(result);
            }

            m_aesCipher->setOutData(std::make_shared<BaseNodeData>(result));
            emit m_aesCipher->dataUpdated(index);
        });

        exec();
        obj->deleteLater();
    }
};

AesCipher::AesCipher(QObject *parent)
    : BaseNode(parent)
    , m_embeddedWidget(nullptr)
{
    AesCipherThread *aesCipherThread = new AesCipherThread(this, this);
    aesCipherThread->start();
    m_handleInDataInThread.store(true);
}

AesCipher::~AesCipher() {}

QString AesCipher::caption() const
{
    return tr("AES Cipher");
}

QString AesCipher::name() const
{
    return QString("AES Cipher");
}

unsigned int AesCipher::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *AesCipher::embeddedWidget()
{
    if (QThread::currentThread() != thread()) {
        return m_embeddedWidget;
    }

    if (!m_embeddedWidget) {
        m_embeddedWidget = new AesCipherUi(this);
    }
    return m_embeddedWidget;
}
