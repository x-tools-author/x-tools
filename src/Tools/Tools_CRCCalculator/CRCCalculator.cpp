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
    parameterComboBox->addItem(tr("自定义"));

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

#if 1
    setWindowTitle(windowTitle() + tr("(功能未实现)"));
#endif
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
    if (index == 0){
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

    if (index > 0){
        QJsonObject jsonObj = jsonObjectAt(index - 1);
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
}

quint8 CRCCalculator::reverse(quint8 inByte)
{
    /// 字节位翻转算法（查表法）
    const quint8 table[256] = {
        0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
        0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
        0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
        0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
        0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
        0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
        0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
        0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
        0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
        0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
        0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
        0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
        0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
        0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
        0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
        0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
    };

    return table[inByte];
}

void CRCCalculator::calculate()
{
    QByteArray inputArray;
    if (hexRadioBt->isChecked()){
        inputArray = inputTextEdit->toPlainText().toLatin1();
    }else{
        inputArray = inputTextEdit->toPlainText().toLatin1();
    }

    bool refin  = refinCheckBox->isChecked();
    bool refout = refoutCheckBox->isChecked();

    inputArray = QByteArray("Hello");
    if (refin){
        /// 输入翻转
        for (int i = 0; i < inputArray.length(); i++){
            //inputArray.data()[i] = static_cast<char>(reverse(static_cast<quint8>(inputArray.at(i))));
        }
    }

    int width = widthComboBox->currentText().toInt();
    int poly = polyLineEdit->text().toInt(nullptr, 16);
    int init = initLineEdit->text().toInt(nullptr, 16);
    int xorValue = xorLineEdit->text().toInt(nullptr, 16);

#if 1
    qInfo() << __LINE__ << "width" << width;
    qInfo() << __LINE__ << "poly"  << QString("0x%1").arg(QString::number(poly, 16), 2, '0');
    qInfo() << __LINE__ << "init"  << QString("0x%1").arg(QString::number(init, 16), 2, '0');
    qInfo() << __LINE__ << "xor"   << QString("0x%1").arg(QString::number(xorValue, 16), 2, '0');
    qInfo() << __LINE__ << "refin"   << (refin ? "true" : "false");
    qInfo() << __LINE__ << "refout"   << (refout ? "true" : "false");
#endif

    QString crcHexString = "error";
    QString crcBinString = "error";
    if (width == 4){

    }else if (width == 5){

    }else if (width == 6){

    }else if (width == 7){

    }else if (width == 8){
        uint8_t crc8 = static_cast<uint8_t>(init);
        int len = inputArray.length();
        uint8_t* data = reinterpret_cast<uint8_t*>(inputArray.data());
        while (len--) {
            crc8 ^= *data;
            for(int i = 0; i < 8; i++){
                if(crc8 & 0x01){
                    crc8 = (crc8 >> 1) ^ 0x8c/*static_cast<uint8_t>(poly)*/;
                }else{
                    crc8 >>= 1;
                }
            }
            data++;
        }

        qDebug() << QString("0x%1").arg(QString::number(crc8, 16), 2, '0');
        if (refout){
            crc8 = reverse(crc8);
            qDebug() << QString("0x%1").arg(QString::number(crc8, 16), 2, '0');
        }
        crc8 = crc8 ^ static_cast<uint8_t>(xorValue);
        qDebug() << QString("0x%1").arg(QString::number(crc8, 16), 2, '0');
        crcHexString = QString("0x%1").arg(QString::number(crc8, 16), 2, '0');
        crcBinString = QString("0x%1").arg(QString::number(crc8, 2), 8, '0');
    }else if (width == 16){

    }else if (width == 32){

    }else {
        qWarning() << "Nonsupport width!";
    }

    hexCRCOutput->setText(crcHexString);
    binCRCOutput->setText(crcBinString);
}
