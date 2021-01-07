/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QMetaEnum>
#include <QStandardItemModel>
#include "SAKCommonDataStructure.hh"

SAKCommonDataStructure::SAKCommonDataStructure(QObject* parent)
    :QObject (parent)
{

}

void SAKCommonDataStructure::setComboBoxTextOutputFormat(QComboBox *comboBox)
{
    if (comboBox){
        QMap<int, QString> formatMap;
        formatMap.insert(OutputFormatBin,   QString("BIN"));
        formatMap.insert(OutputFormatOct,   QString("OCT"));
        formatMap.insert(OutputFormatDec,   QString("DEC"));
        formatMap.insert(OutputFormatHex,   QString("HEX"));
        formatMap.insert(OutputFormatUtf8,  QString("UTF8"));
        formatMap.insert(OutputFormatUcs4,  QString("UCS4"));
        formatMap.insert(OutputFormatAscii, QString("ASCII"));
        formatMap.insert(OutputFormatUtf16, QString("UTF16"));
        formatMap.insert(OutputFormatLocal, QString("SYSTEM"));
        setComboBoxItems(comboBox, formatMap, OutputFormatHex);
    }
}

void SAKCommonDataStructure::setComboBoxTextInputFormat(QComboBox *comboBox)
{
    if (comboBox){
        if (comboBox){
            QMap<int, QString> formatMap;
            formatMap.insert(InputFormatBin,   QString("BIN"));
            formatMap.insert(InputFormatOct,   QString("OTC"));
            formatMap.insert(InputFormatDec,   QString("DEC"));
            formatMap.insert(InputFormatHex,   QString("HEX"));
            formatMap.insert(InputFormatUtf8,  QString("UTF8"));
            formatMap.insert(InputFormatAscii, QString("ASCII"));
            formatMap.insert(InputFormatLocal, QString("SYSTEM"));
            setComboBoxItems(comboBox, formatMap, InputFormatUtf8);
        }
    }
}

void SAKCommonDataStructure::setComboBoxTextWebSocketSendingType(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->addItem(tr("BIN"), SAKCommonDataStructure::WebSocketSendingTypeBin);
        comboBox->addItem(tr("TEXT"), SAKCommonDataStructure::WebSocketSendingTypeText);
    }
}

QString SAKCommonDataStructure::formattingString(QString &origingString, SAKEnumTextInputFormat format)
{
    QString cookedString;
    if (format == SAKCommonDataStructure::InputFormatBin){
        origingString.remove(QRegularExpression("[^0-1]"));
        for (int i = 0; i < origingString.length(); i++){
            if ((i != 0) && (i % 8 == 0)){
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    }else if(format == SAKCommonDataStructure::InputFormatOct) {
        origingString.remove(QRegularExpression("[^0-7]"));
        for (int i = 0; i < origingString.length(); i++){
            if ((i != 0) && (i % 2 == 0)){
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    }else if(format == SAKCommonDataStructure::InputFormatDec) {
        origingString.remove(QRegularExpression("[^0-9]"));
        for (int i = 0; i < origingString.length(); i++){
            if ((i != 0) && (i % 2 == 0)){
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    }else if(format == SAKCommonDataStructure::InputFormatHex) {
        origingString.remove(QRegularExpression("[^0-9a-fA-F]"));
        for (int i = 0; i < origingString.length(); i++){
            if ((i != 0) && (i % 2 == 0)){
                cookedString.append(QChar(' '));
            }
            cookedString.append(origingString.at(i));
        }
    }else if(format == SAKCommonDataStructure::InputFormatAscii) {
        for (int i = 0; i < origingString.count(); i++){
            if (origingString.at(i).unicode() <= 127){
                cookedString.append(origingString.at(i));
            }
        }
    }else if(format == SAKCommonDataStructure::InputFormatUtf8) {
        cookedString = origingString;
    }else if(format == SAKCommonDataStructure::InputFormatLocal) {
        cookedString = origingString;
    }else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknown input model!");
    }

    return cookedString;
}

QByteArray SAKCommonDataStructure::stringToByteArray(QString &origingString, SAKEnumTextInputFormat format)
{
    QByteArray data;
    if (format == SAKCommonDataStructure::InputFormatBin){
        QStringList strList = origingString.split(' ');
        for (QString str:strList){
            qint8 value = QString(str).toInt(Q_NULLPTR, 2);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
    }else if (format == SAKCommonDataStructure::InputFormatOct){
        QStringList strList = origingString.split(' ');
        for (QString str:strList){
            qint8 value = QString(str).toInt(Q_NULLPTR, 8);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
    }else if (format == SAKCommonDataStructure::InputFormatDec){
        QStringList strList = origingString.split(' ');
        for (QString str:strList){
            qint8 value = QString(str).toInt(Q_NULLPTR, 10);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
    }else if (format == SAKCommonDataStructure::InputFormatHex){
        QStringList strList = origingString.split(' ');
        for (QString str:strList){
            qint8 value = QString(str).toInt(Q_NULLPTR, 16);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
    }else if (format == SAKCommonDataStructure::InputFormatUtf8){
        data = origingString.toUtf8();
    }else if (format == SAKCommonDataStructure::InputFormatAscii){
        data = origingString.toLatin1();
    }else if (format == SAKCommonDataStructure::InputFormatLocal){
        data = origingString.toLocal8Bit();
    }else {
        data = origingString.toUtf8();
        Q_ASSERT_X(false, __FUNCTION__, "Unknown input mode!");
    }

    return data;
}

QString SAKCommonDataStructure::byteArrayToString(QByteArray &origingData, SAKEnumTextOutputFormat format)
{
    QString str;
    if (format == SAKCommonDataStructure::OutputFormatBin){
        for (int i = 0; i < origingData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 2), 8, '0'));
        }
    }else if (format == SAKCommonDataStructure::OutputFormatOct){
        for (int i = 0; i < origingData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 8), 3, '0'));
        }
    }else if (format == SAKCommonDataStructure::OutputFormatDec){
        for (int i = 0; i < origingData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 10)));
        }
    }else if (format == SAKCommonDataStructure::OutputFormatHex){
        for (int i = 0; i < origingData.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(origingData.at(i)), 16), 2, '0'));
        }
    }else if (format == SAKCommonDataStructure::OutputFormatAscii){
        str.append(QString::fromLatin1(origingData));
    }else if (format == SAKCommonDataStructure::OutputFormatUtf8){
        str.append(QString::fromUtf8(origingData));
    }else if (format == SAKCommonDataStructure::OutputFormatUtf16){
        str.append(QString::fromUtf16(reinterpret_cast<const char16_t*>(origingData.constData()),origingData.length()/sizeof(char16_t)));
    }else if (format == SAKCommonDataStructure::OutputFormatUcs4){
        str.append(QString::fromUcs4(reinterpret_cast<const char32_t*>(origingData.constData()),origingData.length()/sizeof(char32_t)));
    }else if (format == SAKCommonDataStructure::OutputFormatLocal){
        str.append(QString::fromLocal8Bit(origingData));
    }else {
        str.append(QString::fromUtf8(origingData));
        Q_ASSERT_X(false, __FUNCTION__, "Unknown output mode!");
    }

    return str;
}

void SAKCommonDataStructure::setComboBoxItems(QComboBox *comboBox, QMap<int, QString> &formatMap, int currentData)
{
    if (comboBox){
        comboBox->clear();
        QMapIterator<int, QString> mapIterator(formatMap);
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        while (mapIterator.hasNext()) {
            mapIterator.next();
            QStandardItem *item = new QStandardItem(mapIterator.value());
            item->setToolTip(mapIterator.value());
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
        comboBox->setCurrentIndex(currentData);

        // Reset the iterator...
        while (mapIterator.hasPrevious()) {
            mapIterator.previous();
        }

        // Set item data of combo box
        int index = 0;
        while (mapIterator.hasNext()) {
            mapIterator.next();
            comboBox->setItemData(index, QVariant::fromValue(mapIterator.key()));
            index += 1;
        }

        // Try to set the current index.
        for (int i = 0; i < comboBox->count(); i++){
            if (comboBox->itemData(i).toInt() == currentData){
                comboBox->setCurrentIndex(i);
            }
        }
    }
}
