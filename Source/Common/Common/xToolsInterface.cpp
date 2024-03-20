/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsInterface.h"

#include <QClipboard>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocale>
#include <QTextDocument>
#ifdef X_TOOLS_IMPORT_MODULE_QML
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#include <QQuickTextDocument>
#endif
#endif
#include <QAbstractTableModel>
#include <QLineEdit>
#include <QList>
#include <QRegularExpressionValidator>
#include <QSettings>
#include <QStandardItemModel>

#include "xToolsDataStructure.h"

xToolsInterface::xToolsInterface(QObject *parent)
    : QObject{parent}
{}

void xToolsInterface::setMaximumBlockCount(QVariant doc, int maximum)
{
    auto obj = doc.value<QObject *>();
    if (obj) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#ifdef X_TOOLS_IMPORT_MODULE_QML
        auto quickTextDoc = qobject_cast<QQuickTextDocument *>(obj);
        if (quickTextDoc) {
            auto textDoc = quickTextDoc->textDocument();
            textDoc->setMaximumBlockCount(maximum);
        }
#else
        Q_UNUSED(doc)
        Q_UNUSED(maximum)
#endif
#else
        Q_UNUSED(doc)
        Q_UNUSED(maximum)
#endif
    }
}

void xToolsInterface::setAppFont(const QString &fontFamily)
{
    qGuiApp->setFont(fontFamily);
}

void xToolsInterface::setClipboardText(const QString &text)
{
    QGuiApplication::clipboard()->setText(text);
}

bool xToolsInterface::isQtHighDpiScalePolicy(int policy)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QList<int> policyList;
    policyList << int(Qt::HighDpiScaleFactorRoundingPolicy::Round)
               << int(Qt::HighDpiScaleFactorRoundingPolicy::Ceil)
               << int(Qt::HighDpiScaleFactorRoundingPolicy::Floor)
               << int(Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor)
               << int(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    return policyList.contains(policy);
#else
    Q_UNUSED(false)
    return false;
#endif
}

QString xToolsInterface::arrayToString(const QByteArray &array, int format)
{
    auto cookedArray = [](const QByteArray &array, int base, int len) -> QString {
        QString str, numStr;
        for (int i = 0; i < array.length(); i++) {
            if (base == 10 || base == 8) {
                numStr = QString::number(array.at(i), base);
                str.append(QString("%1 ").arg(numStr));
            } else {
                numStr = QString::number(quint8(array.at(i)), base);
                str.append(QString("%1 ").arg(numStr, len, '0'));
            }
        }
        return str;
    };

    if (xToolsDataStructure::TextFormatBin == format) {
        return cookedArray(array, 2, 8);
    } else if (xToolsDataStructure::TextFormatOct == format) {
        return cookedArray(array, 8, 3);
    } else if (xToolsDataStructure::TextFormatDec == format) {
        return cookedArray(array, 10, 3);
    } else if (xToolsDataStructure::TextFormatHex == format) {
        return cookedArray(array, 16, 2);
    } else if (xToolsDataStructure::TextFormatAscii == format) {
        return QString::fromLatin1(array);
    } else {
        return QString::fromUtf8(array);
    }
}

QString xToolsInterface::dateTimeString(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
}

QString xToolsInterface::cookedFileName(const QString &fileName)
{
    QString cookedFileName = fileName;
#ifdef Q_OS_WIN
    cookedFileName = cookedFileName.remove("file:///");
#endif

    return cookedFileName;
}

QString xToolsInterface::string2hexString(const QString &str)
{
    return QString::fromLatin1(str.toUtf8().toHex());
}

QString xToolsInterface::hexString2String(const QString &str)
{
    QByteArray arr = QByteArray::fromHex(str.toUtf8());
    return QString::fromUtf8(arr);
}

QString xToolsInterface::buildDateTime(const QString &format)
{
    QString str = QString(__DATE__);
    QDate date = QDate::fromString(str, "MMM d yyyy");
    if (!date.isValid()) {
        date = QDate::fromString(str, "MMM  d yyyy");
    }
    QTime time = QTime::fromString(__TIME__, "hh:mm:ss");
    return QDateTime(date, time).toString(format);
}

QString xToolsInterface::dateFormat()
{
    return QLocale::system().dateFormat();
}

QString xToolsInterface::timeFormat()
{
    return QLocale::system().timeFormat();
}

QByteArray xToolsInterface::string2array(const QString &input, int format)
{
    QByteArray data;
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    auto behavior = QString::SkipEmptyParts;
#else
    auto behavior = Qt::SkipEmptyParts;
#endif

    if (format == xToolsDataStructure::TextFormatBin) {
        QStringList strList = input.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 2);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == xToolsDataStructure::TextFormatOct) {
        QStringList strList = input.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 8);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == xToolsDataStructure::TextFormatDec) {
        QStringList strList = input.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 10);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == xToolsDataStructure::TextFormatHex) {
        QStringList strList = input.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 16);
            data.append(reinterpret_cast<char *>(&value), 1);
        }
    } else if (format == xToolsDataStructure::TextFormatAscii) {
        data = input.toLatin1();
    } else {
        data = input.toUtf8();
    }

    return data;
}

QByteArray xToolsInterface::arrayAppendArray(const QByteArray &a1, const QByteArray &a2)
{
    return a1 + a2;
}

QByteArray xToolsInterface::arrayToHex(const QByteArray &array, char separator)
{
    if (array.isEmpty()) {
        return array;
    }

    auto toHex = [](quint8 value) -> char { return "0123456789abcdef"[value & 0xF]; };

    const int length = separator ? (array.size() * 3 - 1) : (array.size() * 2);
    QByteArray hex(length, Qt::Uninitialized);
    char *hexData = hex.data();
    const uchar *data = reinterpret_cast<const uchar *>(array.constData());
    for (int i = 0, o = 0; i < array.size(); ++i) {
        hexData[o++] = toHex(data[i] >> 4);
        hexData[o++] = toHex(data[i] & 0xf);

        if ((separator) && (o < length))
            hexData[o++] = separator;
    }
    return hex;
}

void xToolsInterface::setLineEditValidator(QLineEdit *lineEdit,
                                        SAKEnumValidatorType type,
                                        int maxLength)
{
    QMap<int, QRegularExpression> regExpMap;
    regExpMap.insert(ValidatorBin, QRegularExpression("([01][01][01][01][01][01][01][01][ ])*"));
    regExpMap.insert(ValidatorOtc, QRegularExpression("([0-7][0-7][ ])*"));
    regExpMap.insert(ValidatorDec, QRegularExpression("([-+.e0-9])*"));
    regExpMap.insert(ValidatorHex, QRegularExpression("([0-9a-fA-F][0-9a-fA-F][ ])*"));
    regExpMap.insert(ValidatorAscii, QRegularExpression("([ -~])*"));
    regExpMap.insert(ValidatorFloat, QRegularExpression("^[-+]?[0-9]*\\.?[0-9]+$"));

    if (lineEdit) {
        if (lineEdit->validator()) {
            delete lineEdit->validator();
        }

        if (regExpMap.contains(type)) {
            if (type != ValidatorNone) {
                auto regExpValidator = new QRegularExpressionValidator(regExpMap.value(type),
                                                                       lineEdit);
                lineEdit->setValidator(regExpValidator);
                lineEdit->setMaxLength(maxLength);
            }
        }
    }
}

#ifdef X_TOOLS_IMPORT_MODULE_SERIALPORT
void xToolsInterface::addSerialPortNametItemsToComboBox(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->clear();
        QList<QSerialPortInfo> coms = QSerialPortInfo::availablePorts();
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        for (auto &var : coms) {
            QStandardItem *item = new QStandardItem(var.portName());
            item->setToolTip(var.description());
            itemModel->appendRow(item);
        }

        comboBox->setModel(itemModel);
    }
}
#endif

#ifdef X_TOOLS_IMPORT_MODULE_SERIALPORT
void xToolsInterface::addSerialPortBaudRateItemsToComboBox(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->clear();
        QList<qint32> bd = QSerialPortInfo::standardBaudRates();
        for (auto &var : bd) {
            comboBox->addItem(QString::number(var), QVariant::fromValue(var));
        }

        comboBox->setCurrentText("9600");
    }
}
#endif

#ifdef X_TOOLS_IMPORT_MODULE_SERIALPORT
void xToolsInterface::addSerialPortDataBitItemsToComboBox(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->clear();
        comboBox->addItem("8", QVariant::fromValue(int(QSerialPort::Data8)));
        comboBox->addItem("7", QVariant::fromValue(int(QSerialPort::Data7)));
        comboBox->addItem("6", QVariant::fromValue(int(QSerialPort::Data6)));
        comboBox->addItem("5", QVariant::fromValue(int(QSerialPort::Data5)));
    }
}
#endif

#ifdef X_TOOLS_IMPORT_MODULE_SERIALPORT
void xToolsInterface::addSerialPortStopBitItemsToComboBox(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->clear();
        comboBox->addItem("1", QVariant::fromValue(int(QSerialPort::OneStop)));
#ifdef Q_OS_WINDOWS
        comboBox->addItem("1.5", QVariant::fromValue(int(QSerialPort::OneAndHalfStop)));
#endif
        comboBox->addItem("2", QVariant::fromValue(int(QSerialPort::TwoStop)));
    }
}
#endif

#ifdef X_TOOLS_IMPORT_MODULE_SERIALPORT
void xToolsInterface::addSerialPortParityItemsToComboBox(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->clear();
        comboBox->addItem(tr("No"), QVariant::fromValue(int(QSerialPort::NoParity)));
        comboBox->addItem(tr("Even"), QVariant::fromValue(int(QSerialPort::EvenParity)));
        comboBox->addItem(tr("Odd"), QVariant::fromValue(int(QSerialPort::OddParity)));
        comboBox->addItem(tr("Space"), QVariant::fromValue(int(QSerialPort::SpaceParity)));
        comboBox->addItem(tr("Mark"), QVariant::fromValue(int(QSerialPort::MarkParity)));
    }
}
#endif

#ifdef X_TOOLS_IMPORT_MODULE_SERIALPORT
void xToolsInterface::addSerialPortFlowControlItemsToComboBox(QComboBox *comboBox)
{
    if (comboBox) {
        comboBox->clear();
        comboBox->addItem(tr("No"), QVariant::fromValue(int(QSerialPort::NoFlowControl)));
        comboBox->addItem(tr("Hardware"), QVariant::fromValue(int(QSerialPort::HardwareControl)));
        comboBox->addItem(tr("Software"), QVariant::fromValue(int(QSerialPort::SoftwareControl)));
    }
}
#endif

void xToolsInterface::setComboBoxIndexFromSettings(QSettings *settings,
                                                QString key,
                                                QComboBox *comboBox)
{
    int index = settings->value(key).toInt();
    if (index >= 0 && index < comboBox->count()) {
        comboBox->setCurrentIndex(index);
    }
}

void xToolsInterface::setSettingsValueFromComboBoxIndex(QSettings *settings,
                                                     QString key,
                                                     QComboBox *comboBox)
{
    int currentIndex = comboBox->currentIndex();
    settings->setValue(key, currentIndex);
}

void xToolsInterface::setLineEditTextFromSettings(QSettings *settings, QString key, QLineEdit *lineEdit)
{
    QString text = settings->value(key).toString();
    if (!text.isEmpty()) {
        lineEdit->setText(text);
    }
}

void xToolsInterface::setSettingsValueFromLineEditText(QSettings *settings,
                                                    QString key,
                                                    QLineEdit *lineEdit)
{
    QString value = lineEdit->text();
    settings->setValue(key, value);
}

void xToolsInterface::setCheckBoxValueFromSettings(QSettings *settings,
                                                QString key,
                                                QCheckBox *checkBox)
{
    checkBox->setChecked(settings->value(key).toBool());
}

void xToolsInterface::setSettingsValueFromCheckBox(QSettings *settings,
                                                QString key,
                                                QCheckBox *checkBox)
{
    settings->setValue(key, checkBox->isChecked());
}
