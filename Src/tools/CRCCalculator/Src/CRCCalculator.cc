/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
  *
 * Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
 */
#include "CRCCalculator.hh"
#include "ui_CRCCalculator.h"

#include <QFile>
#include <QDebug>
#include <QComboBox>
#include <QMetaEnum>
#include <QDesktopServices>
#include <QLoggingCategory>

CRCCalculator::CRCCalculator(QWidget* parent)
    :QDialog (parent)
    ,ui(new Ui::CRCCalculator)
{
    ui->setupUi(this);
    widthComboBox = ui->comboBoxWidth;
    widthComboBox->addItem(QString::number(8));
    widthComboBox->addItem(QString::number(16));
    widthComboBox->addItem(QString::number(32));
    widthComboBox->setEnabled(false);

    parameterComboBox = ui->comboBoxName;
    parameterComboBox->clear();

    refinCheckBox = ui->checkBoxRefIn;
    refoutCheckBox = ui->checkBoxRefOut;
    polyLineEdit = ui->lineEditPoly;
    initLineEdit = ui->lineEditInit;
    xorLineEdit = ui->lineEditXOROUT;

    refinCheckBox->setEnabled(false);
    refoutCheckBox->setEnabled(false);
    polyLineEdit->setReadOnly(true);
    initLineEdit->setReadOnly(true);
    xorLineEdit->setReadOnly(true);

    hexRadioBt = ui->radioButtonHex;
    asciiRadioBt = ui->radioButtonASCII;

    hexCRCOutput = ui->lineEditOutputHex;
    binCRCOutput = ui->lineEditOutputBin;
    hexCRCOutput->setReadOnly(true);
    binCRCOutput->setReadOnly(true);

    inputTextEdit = ui->textEdit;

    calculatedBt = ui->pushButtonCalculate;
    labelPolyFormula = ui->labelPolyFormula;
    labelInfo = ui->labelInfo;
    labelInfo->installEventFilter(this);
    labelInfo->setCursor(QCursor(Qt::PointingHandCursor));

    /// 读入crc校验算法模型
    initParameterModel();

    connect(parameterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changedParameterModel(int)));
    connect(calculatedBt, SIGNAL(clicked()), this, SLOT(calculate()));
    connect(inputTextEdit, SIGNAL(textChanged()), this, SLOT(textFormatControl()));

    setAttribute(Qt::WA_DeleteOnClose, true);
    setModal(true);
}

CRCCalculator::~CRCCalculator()
{
    QLoggingCategory category(logCategory);
    qCInfo(category) << "Goodbye CRCCalculator";
    delete  ui;
}

void CRCCalculator::initParameterModel()
{
    parameterComboBox->clear();
    QStringList list = crcInterface.getSupportParameterModels();
    parameterComboBox->addItems(list);

    QMetaEnum models = QMetaEnum::fromType<CRCInterface::CRCModel>();
    bool ok = false;
    int ret = models.keyToValue(parameterComboBox->currentText().toLatin1().constData(), &ok);
    CRCInterface::CRCModel model = CRCInterface::CRC_8;
    if (ok){
        model = static_cast<CRCInterface::CRCModel>(ret);
    }

    int bitsWidth = crcInterface.getBitsWidth(model);
    widthComboBox->setCurrentIndex(widthComboBox->findText(QString::number(bitsWidth)));
    labelPolyFormula->setText(crcInterface.getPolyFormula(model));
}

void CRCCalculator::changedParameterModel(int index)
{
    Q_UNUSED(index);

    QMetaEnum models = QMetaEnum::fromType<CRCInterface::CRCModel>();
    bool ok = false;
    CRCInterface::CRCModel model = CRCInterface::CRC_8;
    int ret = models.keyToValue(parameterComboBox->currentText().toLatin1().constData(), &ok);
    if (ok){
        model = static_cast<CRCInterface::CRCModel>(ret);
    }else{
        QLoggingCategory category(logCategory);
        qCWarning(category) << "Unknow parameter model!";
        Q_ASSERT_X(false, __FUNCTION__, "Unknow parameter model!");
        return;
    }

    int bitsWidth = crcInterface.getBitsWidth(model);
    widthComboBox->setCurrentIndex(widthComboBox->findText(QString::number(bitsWidth)));
    polyLineEdit->setText(QString("0x%1").arg(QString::number(static_cast<int>(crcInterface.getPoly(model)), 16), bitsWidth/4, '0'));
    initLineEdit->setText(QString("0x%1").arg(QString::number(static_cast<int>(crcInterface.getInitValue(model)), 16), bitsWidth/4, '0'));
    xorLineEdit->setText(QString("0x%1").arg(QString::number(static_cast<int>(crcInterface.getXorValue(model)), 16), bitsWidth/4, '0'));
    refinCheckBox->setChecked(crcInterface.getInputReversal(model));
    refoutCheckBox->setChecked(crcInterface.getOutputReversal(model));
    labelPolyFormula->setText(crcInterface.getPolyFormula(model));
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

    if (inputArray.isEmpty()){
        return;
    }

    int bitsWidth = widthComboBox->currentText().toInt();    
    QMetaEnum models = QMetaEnum::fromType<CRCInterface::CRCModel>();
    bool ok = false;
    int ret = models.keyToValue(parameterComboBox->currentText().toLatin1().constData(), &ok);
    CRCInterface::CRCModel model = CRCInterface::CRC_8;
    if (ok){
        model = static_cast<CRCInterface::CRCModel>(ret);
    }

    QString crcHexString = "error";
    QString crcBinString = "error";

    if (bitsWidth == 8){
        uint8_t crc = 0;
        crcInterface.crcCalculate<uint8_t>(reinterpret_cast<uint8_t*>(inputArray.data()), static_cast<uint64_t>(inputArray.length()), crc, model);
        crcHexString = QString("0x%1").arg(QString::number(crc, 16), 2, '0');
        crcBinString = QString("%1").arg(QString::number(crc, 2), 8, '0');
    }else if (bitsWidth == 16){
        uint16_t crc = 0;
        crcInterface.crcCalculate<uint16_t>(reinterpret_cast<uint8_t*>(inputArray.data()), static_cast<uint64_t>(inputArray.length()), crc, model);
        crcHexString = QString("0x%1").arg(QString::number(crc, 16), 4, '0');
        crcBinString = QString("%1").arg(QString::number(crc, 2), 16, '0');
    }else if (bitsWidth == 32){
        uint32_t crc = 0;
        crcInterface.crcCalculate<uint32_t>(reinterpret_cast<uint8_t*>(inputArray.data()), static_cast<uint64_t>(inputArray.length()), crc, model);
        crcHexString = QString("0x%1").arg(QString::number(crc, 16), 8, '0');
        crcBinString = QString("%1").arg(QString::number(crc, 2), 32, '0');
    }else {
        qWarning() << "Not supported bits width!";
    }

    hexCRCOutput->setText(crcHexString);
    binCRCOutput->setText(crcBinString);
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

bool CRCCalculator::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick){
        if (watched == labelInfo){
            QDesktopServices::openUrl(QUrl(QString("http://www.ip33.com/crc.html")));
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}
