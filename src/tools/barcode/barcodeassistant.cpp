/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "barcodeassistant.h"
#include "ui_barcodeassistant.h"

#if defined(X_USING_ZINT_SRC)
#include <backend_qt/qzint.h>
#else
#include <qzint.h>
#endif

#include <QCompleter>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QStringListModel>

#include "common/xtools.h"
#include "utilities/compatibility.h"

#if 0
#define X_ENABLE_COMPLETER
#endif

BarCodeAssistant::BarCodeAssistant(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BarCodeAssistant)
    , m_completer(nullptr)
    , m_filterModel(nullptr)
{
    ui->setupUi(this);
    setWindowTitle(tr("Barcode Assistant"));
    resize(720, 480);

    setupTypeComboBox();

    connect(ui->pushButtonExport,
            &QPushButton::clicked,
            this,
            &BarCodeAssistant::onExportButtonClicked);
    connect(ui->pushButtonRefresh,
            &QPushButton::clicked,
            this,
            &BarCodeAssistant::onRefreshButtonClicked);
#if defined(X_ENABLE_COMPLETER)
    connect(ui->comboBoxType,
            &QComboBox::currentTextChanged,
            this,
            &BarCodeAssistant::onTypeComboBoxTextChanged);
#endif
    connect(ui->comboBoxType, xComboBoxActivated, this, &BarCodeAssistant::onRefreshButtonClicked);
    connect(ui->plainTextEdit,
            &QPlainTextEdit::textChanged,
            this,
            &BarCodeAssistant::onRefreshButtonClicked);
}

BarCodeAssistant::~BarCodeAssistant()
{
    if (m_completer) {
        delete m_completer;
    }
    if (m_filterModel) {
        delete m_filterModel;
    }
    delete ui;
}

void BarCodeAssistant::onExportButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save Image"), "", tr("PNG Image (*.png);;JPEG Image (*.jpg);;BMP Image (*.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QPixmap pixmap = *ui->labelImage->pixmap();
#else
    QPixmap pixmap = ui->labelImage->pixmap(Qt::ReturnByValue);
#endif
    if (!pixmap.save(fileName)) {
        QMessageBox::warning(this, tr("Save Image"), tr("Failed to save image."));
    }
}

void BarCodeAssistant::onRefreshButtonClicked()
{
    Zint::QZint code;
    Zint::QZint::AspectRatioMode mode = Zint::QZint::KeepAspectRatio;

    // Get the barcode type from current selection
    int barcodeType = BARCODE_MICROQR; // Default fallback

    // If there's a valid selection, use the selected type
    if (ui->comboBoxType->currentIndex() >= 0) {
        barcodeType = ui->comboBoxType->currentData().toInt();
    } else {
        // If user typed something, try to find matching type
        QString currentText = ui->comboBoxType->currentText();
        for (const auto &item : std::as_const(m_allItems)) {
            if (item.second.compare(currentText, Qt::CaseInsensitive) == 0) {
                barcodeType = item.first;
                break;
            }
        }
    }

    QString text = ui->plainTextEdit->toPlainText();
    if (text.isEmpty()) {
        text = ui->plainTextEdit->placeholderText();
    }

    code.setSymbol(barcodeType);
    code.setSecurityLevel(0);
    code.setText(text);
    QImage image(ui->labelImage->size(), QImage::Format_ARGB32);
    QPainter painter(&image);
    code.render(painter, image.rect(), mode);
    ui->labelImage->setPixmap(QPixmap::fromImage(image));
}

void BarCodeAssistant::onTypeComboBoxTextChanged(const QString &text)
{
    // Update the ComboBox items based on the search text
    updateComboBoxItems(text);
}

void BarCodeAssistant::setupTypeComboBox()
{
    // Store all items for filtering
    m_allItems.append({BARCODE_CODE11, QString("Code 11")});
    m_allItems.append({BARCODE_C25STANDARD, QString("2 of 5 Standard (Matrix)")});
    m_allItems.append({BARCODE_C25MATRIX, QString("2 of 5 Matrix")});
    m_allItems.append({BARCODE_C25INTER, QString("2 of 5 Interleaved")});
    m_allItems.append({BARCODE_C25IATA, QString("2 of 5 IATA")});
    m_allItems.append({BARCODE_C25LOGIC, QString("2 of 5 Data Logic")});
    m_allItems.append({BARCODE_C25IND, QString("2 of 5 Industrial")});
    m_allItems.append({BARCODE_CODE39, QString("Code 39")});
    m_allItems.append({BARCODE_EXCODE39, QString("Extended Code 39")});
    m_allItems.append({BARCODE_EANX, QString("EAN (European Article Number)")});
    m_allItems.append({BARCODE_EANX_CHK, QString("EAN + Check Digit")});
    m_allItems.append({BARCODE_GS1_128, QString("GS1-128")});
    m_allItems.append({BARCODE_CODABAR, QString("Codabar")});
    m_allItems.append({BARCODE_CODE128, QString("Code 128")});
    m_allItems.append({BARCODE_DPLEIT, QString("Deutsche Post Leitcode")});
    m_allItems.append({BARCODE_DPIDENT, QString("Deutsche Post Identcode")});
    m_allItems.append({BARCODE_CODE16K, QString("Code 16k")});
    m_allItems.append({BARCODE_CODE49, QString("Code 49")});
    m_allItems.append({BARCODE_CODE93, QString("Code 93")});
    m_allItems.append({BARCODE_FLAT, QString("Flattermarken")});
    m_allItems.append({BARCODE_DBAR_OMN, QString("GS1 DataBar Omnidirectional")});
    m_allItems.append({BARCODE_DBAR_LTD, QString("GS1 DataBar Limited")});
    m_allItems.append({BARCODE_DBAR_EXP, QString("GS1 DataBar Expanded")});
    m_allItems.append({BARCODE_TELEPEN, QString("Telepen Alpha")});
    m_allItems.append({BARCODE_UPCA, QString("UPC-A")});
    m_allItems.append({BARCODE_UPCA_CHK, QString("UPC-A + Check Digit")});
    m_allItems.append({BARCODE_UPCE, QString("UPC-E")});
    m_allItems.append({BARCODE_UPCE_CHK, QString("UPC-E + Check Digit")});
    m_allItems.append({BARCODE_POSTNET, QString("USPS POSTNET")});
    m_allItems.append({BARCODE_MSI_PLESSEY, QString("MSI Plessey")});
    m_allItems.append({BARCODE_FIM, QString("Facing Identification Mark")});
    m_allItems.append({BARCODE_LOGMARS, QString("LOGMARS")});
    m_allItems.append({BARCODE_PHARMA, QString("Pharmacode One-Track")});
    m_allItems.append({BARCODE_PZN, QString("Pharmazentralnummer")});
    m_allItems.append({BARCODE_PHARMA_TWO, QString("Pharmacode Two-Track")});
    m_allItems.append({BARCODE_CEPNET, QString("Brazilian CEPNet Postal Code")});
    m_allItems.append({BARCODE_PDF417, QString("PDF417")});
    m_allItems.append({BARCODE_PDF417COMP, QString("Compact PDF417 (Truncated PDF417)")});
    m_allItems.append({BARCODE_MAXICODE, QString("MaxiCode")});
    m_allItems.append({BARCODE_QRCODE, QString("QR Code")});
    m_allItems.append({BARCODE_CODE128AB, QString("Code 128 (Suppress Code Set C)")});
    m_allItems.append({BARCODE_AUSPOST, QString("Australia Post Standard Customer")});
    m_allItems.append({BARCODE_AUSREPLY, QString("Australia Post Reply Paid")});
    m_allItems.append({BARCODE_AUSROUTE, QString("Australia Post Routing")});
    m_allItems.append({BARCODE_AUSREDIRECT, QString("Australia Post Redirection")});
    m_allItems.append({BARCODE_ISBNX, QString("ISBN")});
    m_allItems.append({BARCODE_RM4SCC, QString("Royal Mail 4-State Customer Code")});
    m_allItems.append({BARCODE_DATAMATRIX, QString("Data Matrix (ECC200)")});
    m_allItems.append({BARCODE_EAN14, QString("EAN-14")});
    m_allItems.append({BARCODE_VIN, QString("Vehicle Identification Number")});
    m_allItems.append({BARCODE_CODABLOCKF, QString("Codablock-F")});
    m_allItems.append({BARCODE_NVE18, QString("NVE-18 (SSCC-18)")});
    m_allItems.append({BARCODE_JAPANPOST, QString("Japanese Postal Code")});
    m_allItems.append({BARCODE_KOREAPOST, QString("Korea Post")});
    m_allItems.append({BARCODE_DBAR_STK, QString("GS1 DataBar Stacked")});
    m_allItems.append({BARCODE_DBAR_OMNSTK, QString("GS1 DataBar Stacked Omnidirectional")});
    m_allItems.append({BARCODE_DBAR_EXPSTK, QString("GS1 DataBar Expanded Stacked")});
    m_allItems.append({BARCODE_PLANET, QString("USPS PLANET")});
    m_allItems.append({BARCODE_MICROPDF417, QString("MicroPDF417")});
    m_allItems.append({BARCODE_USPS_IMAIL, QString("USPS Intelligent Mail (OneCode)")});
    m_allItems.append({BARCODE_PLESSEY, QString("UK Plessey")});

    // Tbarcode 8 codes
    m_allItems.append({BARCODE_TELEPEN_NUM, QString("Telepen Numeric")});
    m_allItems.append({BARCODE_ITF14, QString("ITF-14")});
    m_allItems.append({BARCODE_KIX, QString("Dutch Post KIX Code")});
    m_allItems.append({BARCODE_AZTEC, QString("Aztec Code")});
    m_allItems.append({BARCODE_DAFT, QString("DAFT Code")});
    m_allItems.append({BARCODE_DPD, QString("DPD Code")});
    m_allItems.append({BARCODE_MICROQR, QString("Micro QR Code")});

    // Tbarcode 9 codes
    m_allItems.append({BARCODE_HIBC_128, QString("HIBC Code 128")});
    m_allItems.append({BARCODE_HIBC_39, QString("HIBC Code 39")});
    m_allItems.append({BARCODE_HIBC_DM, QString("HIBC Data Matrix")});
    m_allItems.append({BARCODE_HIBC_QR, QString("HIBC QR Code")});
    m_allItems.append({BARCODE_HIBC_PDF, QString("HIBC PDF417")});
    m_allItems.append({BARCODE_HIBC_MICPDF, QString("HIBC MicroPDF417")});
    m_allItems.append({BARCODE_HIBC_BLOCKF, QString("HIBC Codablock-F")});
    m_allItems.append({BARCODE_HIBC_AZTEC, QString("HIBC Aztec Code")});

    // Tbarcode 10 codes
    m_allItems.append({BARCODE_DOTCODE, QString("DotCode")});
    m_allItems.append({BARCODE_HANXIN, QString("Han Xin (Chinese Sensible) Code")});

    // Tbarcode 11 codes
    m_allItems.append({BARCODE_MAILMARK_2D, QString("Royal Mail 2D Mailmark (CMDM)")});
    m_allItems.append({BARCODE_UPU_S10, QString("Universal Postal Union S10")});
    m_allItems.append({BARCODE_MAILMARK_4S, QString("Royal Mail 4-State Mailmark")});

    // Zint specific
    // clang-format off
    m_allItems.append({BARCODE_AZRUNE, QString("Aztec Runes")});
    m_allItems.append({BARCODE_CODE32, QString("Code 32")});
    m_allItems.append({BARCODE_EANX_CC, QString("EAN Composite")});
    m_allItems.append({BARCODE_GS1_128_CC, QString("GS1-128 Composite")});
    m_allItems.append({BARCODE_DBAR_OMN_CC, QString("GS1 DataBar Omnidirectional Composite")});
    m_allItems.append({BARCODE_DBAR_LTD_CC, QString("GS1 DataBar Limited Composite")});
    m_allItems.append({BARCODE_DBAR_EXP_CC, QString("GS1 DataBar Expanded Composite")});
    m_allItems.append({BARCODE_UPCA_CC, QString("UPC-A Composite")});
    m_allItems.append({BARCODE_UPCE_CC, QString("UPC-E Composite")});
    m_allItems.append({BARCODE_DBAR_STK_CC, QString("GS1 DataBar Stacked Composite")});
    m_allItems.append({BARCODE_DBAR_OMNSTK_CC, QString("GS1 DataBar Stacked Omnidirectional Composite")});
    m_allItems.append({BARCODE_DBAR_EXPSTK_CC, QString("GS1 DataBar Expanded Stacked Composite")});
    m_allItems.append({BARCODE_CHANNEL, QString("Channel Code")});
    m_allItems.append({BARCODE_CODEONE, QString("Code One")});
    m_allItems.append({BARCODE_GRIDMATRIX, QString("Grid Matrix")});
    m_allItems.append({BARCODE_UPNQR, QString("UPNQR")});
    m_allItems.append({BARCODE_ULTRA, QString("Ultracode")});
    m_allItems.append({BARCODE_RMQR, QString("Rectangular Micro QR Code (rMQR)")});
    m_allItems.append({BARCODE_BC412, QString("IBM BC412 (SEMI T1-95)")});
    m_allItems.append({BARCODE_DXFILMEDGE, QString("DX Film Edge Barcode")});
    // clang-format on

    // Set ComboBox to be editable for search functionality
#if defined(X_ENABLE_COMPLETER)
    ui->comboBoxType->setEditable(true);
    ui->comboBoxType->setInsertPolicy(QComboBox::NoInsert);
#endif

    // Create completer for auto-completion
    QStringList itemNames;
    for (const auto &item : std::as_const(m_allItems)) {
        itemNames << item.second;
    }

    m_filterModel = new QStringListModel(itemNames, this);
    m_completer = new QCompleter(m_filterModel, this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setFilterMode(Qt::MatchContains);
    if (ui->comboBoxType->isEditable()) {
        ui->comboBoxType->setCompleter(m_completer);
    }

    // Fill the ComboBox with all barcode types initially
    updateComboBoxItems();
}

void BarCodeAssistant::updateComboBoxItems(const QString &filter)
{
    // Clear current items
    ui->comboBoxType->blockSignals(true);
    ui->comboBoxType->clear();

    // Filter items based on the search text
    for (const auto &item : std::as_const(m_allItems)) {
        if (filter.isEmpty() || item.second.contains(filter, Qt::CaseInsensitive)) {
            ui->comboBoxType->addItem(item.second, item.first);
        }
    }

    // If there are filtered results and the current text doesn't match any item exactly,
    // show the dropdown to display filtered options
    if (!filter.isEmpty() && ui->comboBoxType->count() > 0) {
        // Find exact match
        bool exactMatch = false;
        for (int i = 0; i < ui->comboBoxType->count(); ++i) {
            if (ui->comboBoxType->itemText(i).compare(filter, Qt::CaseInsensitive) == 0) {
                ui->comboBoxType->setCurrentIndex(i);
                exactMatch = true;
                break;
            }
        }

        // If no exact match found, set the current text but don't change selection
        if (!exactMatch) {
            ui->comboBoxType->setEditText(filter);
        }
    }
    ui->comboBoxType->blockSignals(false);
}
