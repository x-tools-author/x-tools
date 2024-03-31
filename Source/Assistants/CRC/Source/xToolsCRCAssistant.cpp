/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsCRCAssistant.h"
#include "ui_xToolsCRCAssistant.h"

#include <QComboBox>
#include <QDebug>
#include <QDesktopServices>
#include <QMetaEnum>

#include "xToolsCrcInterface.h"

xToolsCRCAssistant::xToolsCRCAssistant(QWidget* parent)
    : QWidget(parent)
    , m_crcInterface(new xToolsCrcInterface)
    , ui(new Ui::xToolsCRCAssistant)
{
    ui->setupUi(this);
    m_widthComboBox = ui->comboBoxWidth;
    m_widthComboBox->addItem(QString::number(8));
    m_widthComboBox->addItem(QString::number(16));
    m_widthComboBox->addItem(QString::number(32));
    m_widthComboBox->setEnabled(false);

    m_parameterComboBox = ui->comboBoxName;
    m_parameterComboBox->clear();

    m_refinCheckBox = ui->checkBoxRefIn;
    m_refoutCheckBox = ui->checkBoxRefOut;
    m_polyLineEdit = ui->lineEditPoly;
    m_initLineEdit = ui->lineEditInit;
    m_xorLineEdit = ui->lineEditXOROUT;

    m_refinCheckBox->setEnabled(false);
    m_refoutCheckBox->setEnabled(false);
    m_polyLineEdit->setReadOnly(true);
    m_initLineEdit->setReadOnly(true);
    m_xorLineEdit->setReadOnly(true);

    m_hexRadioBt = ui->radioButtonHex;
    m_asciiRadioBt = ui->radioButtonASCII;

    m_hexCRCOutput = ui->lineEditOutputHex;
    m_binCRCOutput = ui->lineEditOutputBin;
    m_hexCRCOutput->setReadOnly(true);
    m_binCRCOutput->setReadOnly(true);

    m_inputTextEdit = ui->textEdit;

    m_calculatedBt = ui->pushButtonCalculate;
    m_labelPolyFormula = ui->labelPolyFormula;
    m_labelInfo = ui->labelInfo;
    m_labelInfo->installEventFilter(this);
    m_labelInfo->setCursor(QCursor(Qt::PointingHandCursor));

    initParameterModel();
    connect(m_parameterComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &xToolsCRCAssistant::changedParameterModel);
    connect(m_calculatedBt, &QPushButton::clicked, this, &xToolsCRCAssistant::calculate);
    connect(m_inputTextEdit, &QTextEdit::textChanged, this, &xToolsCRCAssistant::textFormatControl);
}

xToolsCRCAssistant::~xToolsCRCAssistant()
{
    delete m_crcInterface;
    delete ui;
}

void xToolsCRCAssistant::initParameterModel()
{
    m_parameterComboBox->clear();
    QStringList list = m_crcInterface->supportedParameterModels();
    m_parameterComboBox->addItems(list);

    QMetaEnum models = QMetaEnum::fromType<xToolsCrcInterface::SAKEnumCrcAlgorithm>();
    bool ok = false;
    int ret = models.keyToValue(m_parameterComboBox->currentText().toLatin1().constData(), &ok);
    xToolsCrcInterface::SAKEnumCrcAlgorithm model = xToolsCrcInterface::CRC_8;
    if (ok) {
        model = static_cast<xToolsCrcInterface::SAKEnumCrcAlgorithm>(ret);
    }

    int bitsWidth = m_crcInterface->bitsWidth(model);
    m_widthComboBox->setCurrentIndex(m_widthComboBox->findText(QString::number(bitsWidth)));
    m_labelPolyFormula->setText(m_crcInterface->friendlyPoly(model));
}

void xToolsCRCAssistant::calculate()
{
    QByteArray inputArray;
    if (m_hexRadioBt->isChecked()) {
        QString str = m_inputTextEdit->toPlainText();
        QStringList strList = str.split(' ');
        char ch;
        for (int i = 0; i < strList.length(); i++) {
            if (strList.at(i).isEmpty()) {
                continue;
            }
            ch = static_cast<char>(strList.at(i).toInt(Q_NULLPTR, 16));
            inputArray.append(ch);
        }
    } else {
        inputArray = m_inputTextEdit->toPlainText().toLatin1();
    }

    if (inputArray.isEmpty()) {
        return;
    }

    int bitsWidth = m_widthComboBox->currentText().toInt();
    QMetaEnum models = QMetaEnum::fromType<xToolsCrcInterface::SAKEnumCrcAlgorithm>();
    bool ok = false;
    int ret = models.keyToValue(m_parameterComboBox->currentText().toLatin1().constData(), &ok);
    xToolsCrcInterface::SAKEnumCrcAlgorithm model = xToolsCrcInterface::CRC_8;
    if (ok) {
        model = static_cast<xToolsCrcInterface::SAKEnumCrcAlgorithm>(ret);
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknown crc parameters model!");
    }

    QString crcHexString = "error";
    QString crcBinString = "error";

    uint8_t* inputData = reinterpret_cast<uint8_t*>(inputArray.data());
    uint64_t inputLength = static_cast<uint64_t>(inputArray.length());
    if (bitsWidth == 8) {
        uint8_t crc = m_crcInterface->crcCalculate<uint8_t>(inputData, inputLength, model);
        crcHexString = QString("0x%1").arg(QString::number(crc, 16), 2, '0');
        crcBinString = QString("%1").arg(QString::number(crc, 2), 8, '0');
    } else if (bitsWidth == 16) {
        uint16_t crc = m_crcInterface->crcCalculate<uint16_t>(inputData, inputLength, model);
        crcHexString = QString("0x%1").arg(QString::number(crc, 16), 4, '0');
        crcBinString = QString("%1").arg(QString::number(crc, 2), 16, '0');
    } else if (bitsWidth == 32) {
        uint32_t crc = m_crcInterface->crcCalculate<uint32_t>(inputData, inputLength, model);
        crcHexString = QString("0x%1").arg(QString::number(crc, 16), 8, '0');
        crcBinString = QString("%1").arg(QString::number(crc, 2), 32, '0');
    } else {
        qWarning() << "Not supported bits width!";
    }

    m_hexCRCOutput->setText(crcHexString);
    m_binCRCOutput->setText(crcBinString);
}

void xToolsCRCAssistant::textFormatControl()
{
    if (m_asciiRadioBt->isChecked()) {
        return;
    }
    disconnect(m_inputTextEdit,
               &QTextEdit::textChanged,
               this,
               &xToolsCRCAssistant::textFormatControl);

    QString strTemp;
    QString plaintext = m_inputTextEdit->toPlainText();
    static QRegularExpression reg("[^0-9a-fA-F]");
    plaintext.remove(reg);
    for (int i = 0; i < plaintext.length(); i++) {
        if ((i != 0) && (i % 2 == 0)) {
            strTemp.append(QChar(' '));
        }
        strTemp.append(plaintext.at(i));
    }
    m_inputTextEdit->setText(strTemp.toUpper());
    m_inputTextEdit->moveCursor(QTextCursor::End);

    connect(m_inputTextEdit, &QTextEdit::textChanged, this, &xToolsCRCAssistant::textFormatControl);
}

void xToolsCRCAssistant::changedParameterModel(int index)
{
    Q_UNUSED(index)
    QMetaEnum models = QMetaEnum::fromType<xToolsCrcInterface::SAKEnumCrcAlgorithm>();
    bool ok = false;
    xToolsCrcInterface::SAKEnumCrcAlgorithm model = xToolsCrcInterface::CRC_8;
    int ret = models.keyToValue(m_parameterComboBox->currentText().toLatin1().constData(), &ok);
    if (ok) {
        model = static_cast<xToolsCrcInterface::SAKEnumCrcAlgorithm>(ret);
    } else {
        qWarning() << "Unknown parameter model!";
        Q_ASSERT_X(false, __FUNCTION__, "Unknown parameter model!");
        return;
    }

    int bitsWidth = m_crcInterface->bitsWidth(model);
    m_widthComboBox->setCurrentIndex(m_widthComboBox->findText(QString::number(bitsWidth)));
    QString strTmp = QString::number(static_cast<int>(m_crcInterface->poly(model)), 16);
    m_polyLineEdit->setText(QString("0x%1").arg(strTmp, bitsWidth / 4, '0'));

    strTmp = QString::number(static_cast<int>(m_crcInterface->initialValue(model)), 16);
    m_initLineEdit->setText(QString("0x%1").arg(strTmp, bitsWidth / 4, '0'));

    strTmp = QString::number(static_cast<int>(m_crcInterface->xorValue(model)), 16);
    m_xorLineEdit->setText(QString("0x%1").arg(strTmp, bitsWidth / 4, '0'));

    m_refinCheckBox->setChecked(m_crcInterface->isInputReversal(model));
    m_refoutCheckBox->setChecked(m_crcInterface->isOutputReversal(model));
    m_labelPolyFormula->setText(m_crcInterface->friendlyPoly(model));
}

bool xToolsCRCAssistant::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (watched == m_labelInfo) {
            QDesktopServices::openUrl(QUrl(QString("http://www.ip33.com/crc.html")));
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}
