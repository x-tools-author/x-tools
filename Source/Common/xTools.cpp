/***************************************************************************************************
 * Copyright 2020-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xTools.h"

#include <QMetaEnum>

namespace xTools {

xTools::xTools(QObject *parent)
    : QObject(parent)
{}

xTools::~xTools() {}

QByteArray xTools::byteArrray2Hex(const QByteArray &source, char separator)
{
    if (source.isEmpty()) {
        return source;
    }

    auto toHex = [](quint8 value) -> char { return "0123456789abcdef"[value & 0xF]; };

    const int length = separator ? (source.size() * 3 - 1) : (source.size() * 2);
    QByteArray hex(length, Qt::Uninitialized);
    char *hexData = hex.data();
    const uchar *data = reinterpret_cast<const uchar *>(source.constData());
    for (int i = 0, o = 0; i < source.size(); ++i) {
        hexData[o++] = toHex(data[i] >> 4);
        hexData[o++] = toHex(data[i] & 0xf);

        if ((separator) && (o < length))
            hexData[o++] = separator;
    }
    return hex;
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
QVariantList xTools::supportedHighDpiPolicies()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::HighDpiScaleFactorRoundingPolicy>();
    QVariantList list;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        list.append(metaEnum.value(i));
    }
    return list;
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
QString xTools::highDpiPolicyName(int policy)
{
    typedef Qt::HighDpiScaleFactorRoundingPolicy Policy;
    static QMap<Policy, QString> policyMap;
    if (policyMap.isEmpty()) {
        policyMap.insert(Policy::Unset, tr("System"));
        policyMap.insert(Policy::Round, tr("Round up for .5 and above"));
        policyMap.insert(Policy::Ceil, tr("Always round up"));
        policyMap.insert(Policy::Floor, tr("Always round down"));
        policyMap.insert(Policy::RoundPreferFloor, tr("Round up for .75 and above"));
        policyMap.insert(Policy::PassThrough, tr("Don't round"));
    }

    return policyMap.value(static_cast<Policy>(policy), "Unknown");
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
bool xTools::isValidHighDpiPolicy(int policy)
{
    auto policies = supportedHighDpiPolicies();
    return policies.contains(QVariant(policy));
}
#endif

} // namespace xTools
