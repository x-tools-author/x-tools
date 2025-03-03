/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "crcassistant.h"
#include "ui_crcassistant.h"

#include <QComboBox>
#include <QDebug>
#include <QDesktopServices>
#include <QMetaEnum>

#include "common/crc.h"

CrcAssistant::CrcAssistant(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::CrcAssistant)
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
            &CrcAssistant::changedParameterModel);
    connect(m_calculatedBt, &QPushButton::clicked, this, &CrcAssistant::calculate);
    connect(m_inputTextEdit, &QTextEdit::textChanged, this, &CrcAssistant::textFormatControl);
}

CrcAssistant::~CrcAssistant()
{
    delete ui;
}

void CrcAssistant::initParameterModel()
{
    m_parameterComboBox->clear();
    QList<int> algorithms = CRC::supportedAlgorithms();
    for (auto algorithm : algorithms) {
        auto cookedAlgorithm = static_cast<CRC::Algorithm>(algorithm);
        QString name = CRC::algorithmName(cookedAlgorithm);
        m_parameterComboBox->addItem(name, algorithm);
    }

    m_parameterComboBox->setCurrentIndex(0);
    int algorithm = m_parameterComboBox->currentData().toInt();
    auto cookedAlgorithm = static_cast<CRC::Algorithm>(algorithm);

    int bitsWidth = CRC::bitsWidth(static_cast<CRC::Algorithm>(algorithm));
    m_widthComboBox->setCurrentIndex(m_widthComboBox->findText(QString::number(bitsWidth)));
    m_labelPolyFormula->setText(CRC::algorithmName(cookedAlgorithm));
}

void CrcAssistant::calculate()
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

    int algorithm = m_parameterComboBox->currentData().toInt();
    QByteArray result = CRC::calculate(inputArray, algorithm);
    QString crcHexString = QString::fromLatin1(result.toHex());
    QString crcBinString = QString::fromLatin1(result.toHex());
    m_hexCRCOutput->setText(crcHexString);
    m_binCRCOutput->setText(crcBinString);
}

void CrcAssistant::textFormatControl()
{
    if (m_asciiRadioBt->isChecked()) {
        return;
    }
    disconnect(m_inputTextEdit, &QTextEdit::textChanged, this, &CrcAssistant::textFormatControl);

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

    connect(m_inputTextEdit, &QTextEdit::textChanged, this, &CrcAssistant::textFormatControl);
}

void CrcAssistant::changedParameterModel(int index)
{
    Q_UNUSED(index)
    int bitsWidth = m_widthComboBox->currentText().toInt();
    int algorithm = m_parameterComboBox->currentData().toInt();
    auto cookedAlgorithm = static_cast<CRC::Algorithm>(algorithm);

    m_widthComboBox->setCurrentIndex(m_widthComboBox->findText(QString::number(bitsWidth)));
    QString strTmp = QString::number(static_cast<int>(CRC::poly(cookedAlgorithm)), 16);
    m_polyLineEdit->setText(QString("0x%1").arg(strTmp, bitsWidth / 4, '0'));

    strTmp = QString::number(static_cast<int>(CRC::initialValue(cookedAlgorithm)), 16);
    m_initLineEdit->setText(QString("0x%1").arg(strTmp, bitsWidth / 4, '0'));

    strTmp = QString::number(static_cast<int>(CRC::xorValue(cookedAlgorithm)), 16);
    m_xorLineEdit->setText(QString("0x%1").arg(strTmp, bitsWidth / 4, '0'));

    m_refinCheckBox->setChecked(CRC::isInputReversal(cookedAlgorithm));
    m_refoutCheckBox->setChecked(CRC::isOutputReversal(cookedAlgorithm));
    m_labelPolyFormula->setText(CRC::friendlyPoly(cookedAlgorithm));
}

bool CrcAssistant::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (watched == m_labelInfo) {
            QDesktopServices::openUrl(QUrl(QString("http://www.ip33.com/crc.html")));
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}
