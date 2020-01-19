/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QComboBox>
#include <QMetaEnum>
#include <QDesktopServices>
#include <QLoggingCategory>

#include "SAKCRCCalculator.hh"
#include "ui_SAKCRCCalculator.h"

SAKCRCCalculator::SAKCRCCalculator(QWidget* parent)
    :QDialog (parent)
    ,ui(new Ui::SAKCRCCalculator)
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

SAKCRCCalculator::~SAKCRCCalculator()
{
    QLoggingCategory category(logCategory);
    qCInfo(category) << "Goodbye CRCCalculator";
    delete  ui;
}

void SAKCRCCalculator::initParameterModel()
{
    parameterComboBox->clear();
    QStringList list = crcInterface.supportedParameterModels();
    parameterComboBox->addItems(list);

    QMetaEnum models = QMetaEnum::fromType<SAKCRCInterface::CRCModel>();
    bool ok = false;
    int ret = models.keyToValue(parameterComboBox->currentText().toLatin1().constData(), &ok);
    SAKCRCInterface::CRCModel model = SAKCRCInterface::CRC_8;
    if (ok){
        model = static_cast<SAKCRCInterface::CRCModel>(ret);
    }

    int bitsWidth = crcInterface.getBitsWidth(model);
    widthComboBox->setCurrentIndex(widthComboBox->findText(QString::number(bitsWidth)));
    labelPolyFormula->setText(crcInterface.getPolyFormula(model));
}

void SAKCRCCalculator::changedParameterModel(int index)
{
    Q_UNUSED(index)

    QMetaEnum models = QMetaEnum::fromType<SAKCRCInterface::CRCModel>();
    bool ok = false;
    SAKCRCInterface::CRCModel model = SAKCRCInterface::CRC_8;
    int ret = models.keyToValue(parameterComboBox->currentText().toLatin1().constData(), &ok);
    if (ok){
        model = static_cast<SAKCRCInterface::CRCModel>(ret);
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

void SAKCRCCalculator::calculate()
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
    QMetaEnum models = QMetaEnum::fromType<SAKCRCInterface::CRCModel>();
    bool ok = false;
    int ret = models.keyToValue(parameterComboBox->currentText().toLatin1().constData(), &ok);
    SAKCRCInterface::CRCModel model = SAKCRCInterface::CRC_8;
    if (ok){
        model = static_cast<SAKCRCInterface::CRCModel>(ret);
    }

    QString crcHexString = "error";
    QString crcBinString = "error";

    if (bitsWidth == 8){
        uint8_t crc = crcInterface.crcCalculate<uint8_t>(reinterpret_cast<uint8_t*>(inputArray.data()), static_cast<uint64_t>(inputArray.length()), model);
        crcHexString = QString("0x%1").arg(QString::number(crc, 16), 2, '0');
        crcBinString = QString("%1").arg(QString::number(crc, 2), 8, '0');
    }else if (bitsWidth == 16){
        uint16_t crc = crcInterface.crcCalculate<uint8_t>(reinterpret_cast<uint8_t*>(inputArray.data()), static_cast<uint64_t>(inputArray.length()), model);
        crcHexString = QString("0x%1").arg(QString::number(crc, 16), 4, '0');
        crcBinString = QString("%1").arg(QString::number(crc, 2), 16, '0');
    }else if (bitsWidth == 32){
        uint32_t crc = crcInterface.crcCalculate<uint8_t>(reinterpret_cast<uint8_t*>(inputArray.data()), static_cast<uint64_t>(inputArray.length()), model);
        crcHexString = QString("0x%1").arg(QString::number(crc, 16), 8, '0');
        crcBinString = QString("%1").arg(QString::number(crc, 2), 32, '0');
    }else {
        qWarning() << "Not supported bits width!";
    }

    hexCRCOutput->setText(crcHexString);
    binCRCOutput->setText(crcBinString);
}

void SAKCRCCalculator::textFormatControl()
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

bool SAKCRCCalculator::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick){
        if (watched == labelInfo){
            QDesktopServices::openUrl(QUrl(QString("http://www.ip33.com/crc.html")));
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}
