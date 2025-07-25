/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xcode.h"

#include <QFileDialog>
#include <QMessageBox>

#include "tools/barcode/barcodeassistant.h"

xCode::xCode(QWidget *parent)
    : xUi(parent)
{
    m_barCodeAssistant = new BarCodeAssistant(this);
    setFixedSize(720, 480);
    setCentralWidget(m_barCodeAssistant);
    setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    setWindowFlag(Qt::WindowMinimizeButtonHint, false);
    setWindowTitle(tr("xCode - Barcode/QR code Generator"));
}

xCode::~xCode() {}

void xCode::generateBarcode()
{
    if (!m_barCodeAssistant) {
        QMessageBox::warning(this, tr("Error"), tr("Barcode Assistant is not initialized."));
        return;
    }

    m_barCodeAssistant->onRefreshButtonClicked();
}