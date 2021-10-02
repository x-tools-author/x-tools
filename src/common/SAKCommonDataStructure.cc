/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QMetaEnum>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "SAKCommonDataStructure.hh"

SAKCommonDataStructure::SAKCommonDataStructure(QObject* parent)
    :QObject (parent)
{

}

SAKCommonDataStructure::~SAKCommonDataStructure()
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
            formatMap.insert(InputFormatAscii, QString("ASCII"));
            formatMap.insert(InputFormatLocal, QString("SYSTEM"));
            setComboBoxItems(comboBox, formatMap, InputFormatLocal);
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

QString SAKCommonDataStructure::formattingString(QString &origingString,
                                                 SAKEnumTextFormatInput format)
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
    }else if(format == SAKCommonDataStructure::InputFormatLocal) {
        cookedString = origingString;
    }else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknown input model!");
    }

    return cookedString;
}

QByteArray SAKCommonDataStructure::stringToByteArray(QString &origingString,
                                                     SAKEnumTextFormatInput format)
{
    QByteArray data;
    if (format == SAKCommonDataStructure::InputFormatBin){
        QStringList strList = origingString.split(' ');
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 2);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
    }else if (format == SAKCommonDataStructure::InputFormatOct){
        QStringList strList = origingString.split(' ');
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 8);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
    }else if (format == SAKCommonDataStructure::InputFormatDec){
        QStringList strList = origingString.split(' ');
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 10);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
    }else if (format == SAKCommonDataStructure::InputFormatHex){
        QStringList strList = origingString.split(' ');
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 16);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
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

QByteArray SAKCommonDataStructure::stringToByteArray(QString &origingString, int format)
{
    auto cookedFormat =
            static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(format);
    return stringToByteArray(origingString, cookedFormat);
}

QString SAKCommonDataStructure::byteArrayToString(QByteArray &origingData,
                                                  SAKEnumTextFormatOutput format)
{
    QString str;
    if (format == SAKCommonDataStructure::OutputFormatBin){
        for (int i = 0; i < origingData.length(); i++){
            str.append(QString("%1 ").arg(
                           QString::number(static_cast<uint8_t>(origingData.at(i)), 2),
                           8,
                           '0'
                           ));
        }
    }else if (format == SAKCommonDataStructure::OutputFormatOct){
        for (int i = 0; i < origingData.length(); i++){
            str.append(QString("%1 ").arg(
                           QString::number(static_cast<uint8_t>(origingData.at(i)), 8),
                           3,
                           '0')
                       );
        }
    }else if (format == SAKCommonDataStructure::OutputFormatDec){
        for (int i = 0; i < origingData.length(); i++){
            str.append(QString("%1 ").arg(
                           QString::number(static_cast<uint8_t>(origingData.at(i)), 10)
                           ));
        }
    }else if (format == SAKCommonDataStructure::OutputFormatHex){
        for (int i = 0; i < origingData.length(); i++){
            str.append(QString("%1 ").arg(
                           QString::number(static_cast<uint8_t>(origingData.at(i)), 16),
                           2,
                           '0'));
        }
    }else if (format == SAKCommonDataStructure::OutputFormatAscii){
        str.append(QString::fromLatin1(origingData));
    }else if (format == SAKCommonDataStructure::OutputFormatUtf8){
        str.append(QString::fromUtf8(origingData));
    }else if (format == SAKCommonDataStructure::OutputFormatUtf16){
        str.append(QString::fromUtf16(
                       reinterpret_cast<const char16_t*>(origingData.constData()),
                       origingData.length()/sizeof(char16_t)
                       ));
    }else if (format == SAKCommonDataStructure::OutputFormatUcs4){
        str.append(QString::fromUcs4(
                       reinterpret_cast<const char32_t*>(origingData.constData()),
                       origingData.length()/sizeof(char32_t)
                       ));
    }else if (format == SAKCommonDataStructure::OutputFormatLocal){
        str.append(QString::fromLocal8Bit(origingData));
    }else {
        str.append(QString::fromUtf8(origingData));
        Q_ASSERT_X(false, __FUNCTION__, "Unknown output mode!");
    }

    return str;
}

void SAKCommonDataStructure::setLineEditTextFormat(
        QLineEdit *lineEdit,
        SAKEnumTextFormatInput format
        )
{
    QMap<int, QRegularExpressionValidator*> regExpMap;
    QRegularExpression binRegExp = QRegularExpression("([01][01][01][01][01][01][01][01][ ])*");
    QRegularExpression otcRegExp = QRegularExpression("([0-7][0-7][ ])*");
    QRegularExpression decRegExp = QRegularExpression("([0-9][0-9][ ])*");
    QRegularExpression hexRegExp = QRegularExpression("([0-9a-fA-F][0-9a-fA-F][ ])*");
    QRegularExpression asciiRegExp = QRegularExpression("([ -~])*");
    regExpMap.insert(SAKCommonDataStructure::InputFormatBin,
                     new QRegularExpressionValidator(binRegExp));
    regExpMap.insert(SAKCommonDataStructure::InputFormatOct,
                     new QRegularExpressionValidator(otcRegExp));
    regExpMap.insert(SAKCommonDataStructure::InputFormatDec,
                     new QRegularExpressionValidator(decRegExp));
    regExpMap.insert(SAKCommonDataStructure::InputFormatHex,
                     new QRegularExpressionValidator(hexRegExp));
    regExpMap.insert(SAKCommonDataStructure::InputFormatAscii,
                     new QRegularExpressionValidator(asciiRegExp));
    regExpMap.insert(SAKCommonDataStructure::InputFormatLocal,
                     Q_NULLPTR);

    if (lineEdit) {
        if (lineEdit->validator()) {
            delete lineEdit->validator();
            lineEdit->setValidator(Q_NULLPTR);
        }

        if (regExpMap.contains(format)) {
            QRegularExpressionValidator *validator = regExpMap.value(format);
            lineEdit->setValidator(validator);
        } else {
            lineEdit->setValidator(Q_NULLPTR);
        }
    }
}

void SAKCommonDataStructure::setLineEditTextFormat(QLineEdit *lineEdit, int format)
{
    auto cookedFormat = static_cast<SAKEnumTextFormatInput>(format);
    SAKCommonDataStructure::setLineEditTextFormat(lineEdit, cookedFormat);
}

QString SAKCommonDataStructure::suffix(SAKEmnuSuffixsType type)
{
    return suffix(int(type));
}

QString SAKCommonDataStructure::suffix(int type)
{
    switch (type) {
    case SuffixsTypeNone: return "";
    case SuffixsTypeR: return "\r";
    case SuffixsTypeN: return "\n";
    case SuffixsTypeRN: return "\r\n";
    case SuffixsTypeNR: return "\n\r";
    default: return "";
    }
}

QString SAKCommonDataStructure::friendlySuffix(SAKEmnuSuffixsType type)
{
    switch (type) {
    case SuffixsTypeNone: return tr("None");
    case SuffixsTypeR: return "\\r";
    case SuffixsTypeN: return "\\n";
    case SuffixsTypeRN: return "\\r\\n";
    case SuffixsTypeNR: return "\\n\\r";
    default: return tr("None");
    }
}

void SAKCommonDataStructure::setupSuffix(QComboBox *comboBox)
{
    if (comboBox){
        QMap<int, QString> formatMap;
        formatMap.insert(SuffixsTypeNone, friendlySuffix(SuffixsTypeNone));
        formatMap.insert(SuffixsTypeR, friendlySuffix(SuffixsTypeR));
        formatMap.insert(SuffixsTypeN, friendlySuffix(SuffixsTypeN));
        formatMap.insert(SuffixsTypeRN, friendlySuffix(SuffixsTypeRN));
        formatMap.insert(SuffixsTypeNR, friendlySuffix(SuffixsTypeNR));
        setComboBoxItems(comboBox, formatMap, SuffixsTypeNone);
    }
}

void SAKCommonDataStructure::formattingInputText(QTextEdit *textEdit, int model)
{
    if (textEdit){
        textEdit->blockSignals(true);
        QString plaintext = textEdit->toPlainText();
        if (!plaintext.isEmpty()){
            auto cookedModel =
                    static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(model);
            QString cookedString =
                    SAKCommonDataStructure::formattingString(plaintext,
                                                             cookedModel);
            textEdit->setText(cookedString);
            textEdit->moveCursor(QTextCursor::End);
        }
        textEdit->blockSignals(false);
    }
}

void SAKCommonDataStructure::setComboBoxItems(QComboBox *comboBox,
                                              QMap<int, QString> &formatMap,
                                              int currentData)
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
