/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "userinput.h"
#include "userinputui.h"

#include <QJsonObject>
#include <QTimer>

#include "common/common.h"

UserInput::UserInput(QObject *parent)
    : BaseNode(parent)
{}

UserInput::~UserInput() {}

QString UserInput::caption() const
{
    return tr("User Input");
}

QString UserInput::name() const
{
    return QString("UserInput");
}

unsigned int UserInput::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 0;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *UserInput::embeddedWidget()
{
    if (m_embeddedWidget == nullptr) {
        m_embeddedWidget = new UserInputUi(this);
    }

    return m_embeddedWidget;
}

QJsonObject UserInput::saveData(const UserInput::Data &data)
{
    QJsonObject obj;
    DataKeys keys;
    obj[keys.format] = data.format;
    obj[keys.prefix] = data.prefix;
    obj[keys.suffix] = data.suffix;
    obj[keys.escape] = data.escape;
    obj[keys.interval] = data.interval;
    obj[keys.text] = data.text;
    return obj;
}

UserInput::Data UserInput::loadData(const QJsonObject &data)
{
    static Data defaultData{static_cast<int>(TextFormat::Hex),
                            static_cast<int>(Affixes::None),
                            static_cast<int>(Affixes::None),
                            static_cast<int>(EscapeCharacter::None),
                            1000,
                            QString("(null)")};
    if (data.isEmpty()) {
        return defaultData;
    }

    Data d;
    DataKeys keys;
    QVariantMap map = data.toVariantMap();
    d.format = map.value(keys.format, defaultData.format).toInt();
    d.prefix = map.value(keys.prefix, defaultData.prefix).toInt();
    d.suffix = map.value(keys.suffix, defaultData.suffix).toInt();
    d.escape = map.value(keys.escape, defaultData.escape).toInt();
    d.interval = map.value(keys.interval, defaultData.interval).toInt();
    d.text = map.value(keys.text, defaultData.text).toString();
    return d;
}

QByteArray UserInput::data2Bytes(const Data &data)
{
    QString text = cookedEscapeCharacter(data.text, data.escape);
    QByteArray bytes = string2bytes(text, data.format);
    QByteArray prefixBytes = cookedAffixes(data.prefix);
    QByteArray suffixBytes = cookedAffixes(data.suffix);

    bytes = prefixBytes + bytes + suffixBytes;
    return bytes;
}
