/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "CRCCalculator.h"
#include "ui_CRCCalculator.h"

#include <QFile>
#include <QDebug>
#include <QComboBox>

CRCCalculator::CRCCalculator(QWidget* parent)
    :QWidget (parent)
    ,ui(new Ui::CRCCalculator)
{
    ui->setupUi(this);
    widthComboBox = ui->comboBoxWidth;
    for (int i = 1; i < 33; i++){
        widthComboBox->addItem(QString::number(i));
    }
    parameterComboBox = ui->comboBoxName;
    parameterComboBox->clear();

    refinCheckBox = ui->checkBoxRefIn;
    refoutCheckBox = ui->checkBoxRefOut;
    polyLineEdit = ui->lineEditPoly;
    initLineEdit = ui->lineEditInit;
    xorLineEdit = ui->lineEditXOROUT;

    hexRadioBt = ui->radioButtonHex;
    asciiRadioBt = ui->radioButtonASCII;

    hexCRCOutput = ui->lineEditOutputHex;
    binCRCOutput = ui->lineEditOutputBin;

    inputTextEdit = ui->textEdit;

    calculatedBt = ui->pushButtonCalculate;
    calculatedBt->setEnabled(false);

    /// 读入crc校验算法模型
    initParameterModel();

    connect(parameterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changedParameterModel(int)));
    connect(calculatedBt, SIGNAL(clicked()), this, SLOT(calculate()));

#if 0
    setWindowTitle(windowTitle() + tr("(功能未实现)"));
#endif
    connect(inputTextEdit, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
}

CRCCalculator::~CRCCalculator()
{
    delete  ui;
}

void CRCCalculator::initParameterModel()
{
    QFile file(":/ParameterModel.json");
    if (file.open(QFile::ReadOnly)){
        QByteArray jsonFile = file.readAll();
        file.close();

        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile, &error);
        if (error.error == QJsonParseError::NoError){
            if(jsonDoc.isArray()){
            /**
                "index" :   索引号
                "name"  :   crc算法名称
                "width" :   位宽,
                "poly"  :   多项式
                "init"  :   初始值
                "xor"   :   异或值
                "refin" :   输入翻转
                "refout":   输出翻转
            */
                QJsonArray jsonArray = jsonDoc.array();
                for (int i = 0; i < jsonArray.count(); i++){
                    QJsonValue value = jsonArray.at(i);
                    QJsonObject obj = value.toObject();
                    QString str = obj.value("name").toString();
                    parameterComboBox->addItem(str);
                }
            }
        }else {
            qWarning() << "Parse error:" << error.errorString();
        }

    }else {
        qWarning() << "Can not open json file:" << file.errorString();
    }
}

QJsonObject CRCCalculator::jsonObjectAt(int index)
{
    QFile file(":/ParameterModel.json");
    if (file.open(QFile::ReadOnly)){
        QByteArray jsonFile = file.readAll();
        file.close();

        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile, &error);
        if (error.error == QJsonParseError::NoError){
            if(jsonDoc.isArray()){
                QJsonArray jsonArray = jsonDoc.array();
                return jsonArray.at(index).toObject();
            }
        }else {
            qWarning() << "Parse error:" << error.errorString();
        }

    }else {
        qWarning() << "Can not open json file:" << file.errorString();
    }

    QJsonObject obj;
    return obj;
}

void CRCCalculator::changedParameterModel(int index)
{
    /// 不支持自定义选项
    QJsonObject jsonObj = jsonObjectAt(index);
    if (jsonObj.value("name").toString().compare("自定义") == 0){
        calculatedBt->setEnabled(false);
        widthComboBox->setEnabled(true);
        refinCheckBox->setEnabled(true);
        refoutCheckBox->setEnabled(true);
        polyLineEdit->setEnabled(true);
        initLineEdit->setEnabled(true);
        xorLineEdit->setEnabled(true);
    }else {
        calculatedBt->setEnabled(true);
        widthComboBox->setEnabled(false);
        refinCheckBox->setEnabled(false);
        refoutCheckBox->setEnabled(false);
        polyLineEdit->setEnabled(false);
        initLineEdit->setEnabled(false);
        xorLineEdit->setEnabled(false);
    }


    if (!jsonObj.isEmpty()){
        widthComboBox->setCurrentIndex(widthComboBox->findText(QString::number(jsonObj.value("width").toInt())));
        polyLineEdit->setText(jsonObj.value("poly").toString());
        initLineEdit->setText(jsonObj.value("init").toString());
        xorLineEdit->setText(jsonObj.value("xor").toString());
        refinCheckBox->setChecked(jsonObj.value("refin").toBool());
        refoutCheckBox->setChecked(jsonObj.value("refout").toBool());
    }else {
        qWarning("error: empty json object");
    }
}

void CRCCalculator::calculate()
{
    QByteArray inputArray;
    if (hexRadioBt->isChecked()){
        QString str = inputTextEdit->toPlainText();
        QStringList strList = str.split(' ');
        char ch;
        for (int i = 0; i < strList.length(); i++){
            if (strList.at(i).isEmpty()){
                continue;
            }
            ch = static_cast<char>(strList.at(i).toInt(nullptr, 16));
            inputArray.append(ch);
        }
    }else{
        inputArray = inputTextEdit->toPlainText().toLatin1();
    }

    int bitsWidth = widthComboBox->currentText().toInt();
    int poly = polyLineEdit->text().toInt(nullptr, 16);
    int startValue = initLineEdit->text().toInt(nullptr, 16);
    int xorValue = xorLineEdit->text().toInt(nullptr, 16);

    QString crcHexString = "error";
    QString crcBinString = "error";

    if (bitsWidth == 4){

    }else if (bitsWidth == 5){

    }else if (bitsWidth == 6){

    }else if (bitsWidth == 7){

    }else if (bitsWidth == 8){

    }else if (bitsWidth == 16){
        quint16 polyTemp = static_cast<quint16>(poly);
        reverseInt(polyTemp, polyTemp);
        quint16 crc = crc16(reinterpret_cast<const quint8*>(inputArray.constData()),
                            static_cast<quint64>(inputArray.length()),
                            static_cast<quint16>(startValue),
                            polyTemp);
        crc ^= xorValue;
        crcHexString = QString("0x%1").arg(QString::number(crc, 16), 2, '0');
        crcBinString = QString("0x%1").arg(QString::number(crc, 2), 8, '0');
    }else if (bitsWidth == 32){

    }else {
        qWarning() << "Nonsupport width!";
    }

    hexCRCOutput->setText(crcHexString);
    binCRCOutput->setText(crcBinString);
}

quint16 CRCCalculator::crc16(const quint8 *inputPtr, quint64 numBytes, quint16 startValue, quint16 poly) {

    quint16 crc;
    quint64 a;
    const unsigned char *ptr;

    quint16 crc16Table[256];

    quint16 i;
    quint16 j;
    quint16 tableValue;
    quint16 c;
    for (i=0; i<256; i++) {
        tableValue = 0;
        c   = i;
        for (j=0; j<8; j++) {
            if ( (tableValue ^ c) & 0x0001 ){
                tableValue = ( tableValue >> 1 ) ^ poly;
            }else{
                tableValue =   tableValue >> 1;
            }
            c = c >> 1;
        }
        crc16Table[i] = tableValue;
    }

    crc = startValue;
    ptr = inputPtr;

    if ( ptr != nullptr ){
        for (a=0; a< numBytes; a++) {
            crc = (crc >> 8) ^ crc16Table[ (crc ^ static_cast<quint16>(*ptr++)) & 0x00FF ];
        }
    }
    return crc;
}

template<class Tint> void CRCCalculator::reverseInt(Tint &input, Tint &output)
{
    int bitsWidth = sizeof (input)*8;

    QString inputStr = QString("%1").arg(QString::number(input, 2), bitsWidth, '0');
    QString outputStr;
    outputStr.resize(bitsWidth);
    for (int i = 0; i < bitsWidth; i++){
        outputStr.replace(i, 1, inputStr.at(bitsWidth-1-i));
    }

    output = static_cast<Tint>(outputStr.toULongLong(nullptr, 2));
}

void CRCCalculator::textFormatControl()
{
    if (asciiRadioBt->isChecked()){
        return;
    }
    disconnect(inputTextEdit, SIGNAL(textChanged()), this, SLOT(textFormatControl()));

    QString strTemp;
    QString plaintext = inputTextEdit->toPlainText();
    plaintext.remove(QRegExp("[^0-9a-fA-F]"));
    for (int i = 0; i < plaintext.length(); i++){
        if ((i != 0) && (i % 2 == 0)){
            strTemp.append(QChar(' '));
        }
        strTemp.append(plaintext.at(i));
    }
    inputTextEdit->setText(strTemp.toUpper());
    inputTextEdit->moveCursor(QTextCursor::End);

    connect(inputTextEdit, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
}
