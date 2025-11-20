/***************************************************************************************************
 * Copyright 2022-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "asciiassistant.h"
#include "ui_asciiassistant.h"

#include <QDebug>

AsciiAssistant::AsciiAssistant(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AsciiAssistant)
{
    ui->setupUi(this);
    initDescription();

    connect(ui->lineEdit, &QLineEdit::textChanged, this, &AsciiAssistant::setupFilter);
    connect(ui->checkBox, &QCheckBox::clicked, this, [=]() {
        this->setupFilter(ui->lineEdit->text());
    });

    QStringList headerLabels;
    headerLabels << tr("Bin") << tr("Oct") << tr("Dec") << tr("Hex") << tr("Description");
    ui->tableWidget->setColumnCount(headerLabels.length());
    ui->tableWidget->setRowCount(128);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    if (ui->tableWidget->verticalHeader()) {
        ui->tableWidget->verticalHeader()->hide();
    }

    int last = ui->tableWidget->columnCount() - 1;
    ui->tableWidget->model()->setHeaderData(last,
                                            Qt::Horizontal,
                                            QVariant(Qt::AlignLeft | Qt::AlignVCenter),
                                            Qt::TextAlignmentRole);

    for (int i = 0; i < 128; i++) {
        QString bin = QString("%1").arg(i, 8, 2, QChar('0'));
        QString oct = QString("%1").arg(i, 3, 8, QChar('0'));
        QString dec = QString("%1").arg(i, 3, 10, QChar('0'));
        QString hex = QString("%1").arg(i, 2, 16, QChar('0'));

        QString desc = m_descirption.value(i);
        if (desc.isEmpty()) {
            desc = QString("%1").arg(QChar(i));
        }

        QTableWidgetItem *binItem = new QTableWidgetItem(bin);
        binItem->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *octItem = new QTableWidgetItem(oct);
        octItem->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *hexItem = new QTableWidgetItem(hex);
        hexItem->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *decItem = new QTableWidgetItem(dec);
        decItem->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *descItem = new QTableWidgetItem(desc);
        descItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        ui->tableWidget->setItem(i, 0, binItem);
        ui->tableWidget->setItem(i, 1, octItem);
        ui->tableWidget->setItem(i, 2, decItem);
        ui->tableWidget->setItem(i, 3, hexItem);
        ui->tableWidget->setItem(i, 4, descItem);
    }
}

AsciiAssistant::~AsciiAssistant()
{
    delete ui;
}

void AsciiAssistant::initDescription()
{
    m_descirption.clear();
    m_descirption.insert(0, tr("NUL (NULL)"));
    m_descirption.insert(1, tr("SOH (Start Of Headling)"));
    m_descirption.insert(2, tr("STX (Start Of Text)"));
    m_descirption.insert(3, tr("ETX (End Of Text)"));
    m_descirption.insert(4, tr("EOT (End Of Transmission)"));
    m_descirption.insert(5, tr("ENQ (Enquiry)"));
    m_descirption.insert(6, tr("ACK (Acknowledge)"));
    m_descirption.insert(7, tr("BEL (Bell)"));
    m_descirption.insert(8, tr("BS (Backspace)"));
    m_descirption.insert(9, tr("HT (Horizontal Tab)"));
    m_descirption.insert(10, tr("LF/NL(Line Feed/New Line)"));
    m_descirption.insert(11, tr("VT (Vertical Tab)"));
    m_descirption.insert(12, tr("FF/NP (Form Feed/New Page)"));
    m_descirption.insert(13, tr("CR (Carriage Return)"));
    m_descirption.insert(14, tr("SO (Shift Out)"));
    m_descirption.insert(15, tr("SI (Shift In)"));
    m_descirption.insert(16, tr("DLE (Data Link Escape)"));
    m_descirption.insert(17, tr("DC1/XON(Device Control 1/Transmission On)"));
    m_descirption.insert(18, tr("DC2 (Device Control 2)"));
    m_descirption.insert(19, tr("DC3/XOFF(Device Control 3/Transmission Off)"));
    m_descirption.insert(20, tr("DC4 (Device Control 4)"));
    m_descirption.insert(21, tr("NAK (Negative Acknowledge)"));
    m_descirption.insert(22, tr("SYN (Synchronous Idle)"));
    m_descirption.insert(23, tr("ETB (End of Transmission Block)"));
    m_descirption.insert(24, tr("CAN (Cancel)"));
    m_descirption.insert(25, tr("EM (End of Medium)"));
    m_descirption.insert(26, tr("SUB (Substitute)"));
    m_descirption.insert(27, tr("ESC (Escape)"));
    m_descirption.insert(28, tr("FS (File Separator)"));
    m_descirption.insert(29, tr("GS (Group Separator)"));
    m_descirption.insert(30, tr("RS (Record Separator)"));
    m_descirption.insert(31, tr("US (Unit Separator)"));
    m_descirption.insert(32, tr("(Space)"));
    m_descirption.insert(127, tr("DEL (Delete)"));
}

void AsciiAssistant::setupFilter(const QString &text)
{
    bool preserveCase = ui->checkBox->isChecked();
    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        bool hasText = false;
        for (int column = 0; column < ui->tableWidget->columnCount(); column++) {
            auto item = ui->tableWidget->item(row, column);
            Qt::CaseSensitivity flag = preserveCase ? Qt::CaseSensitive : Qt::CaseInsensitive;
            if (item && item->text().contains(text, flag)) {
                hasText = true;
                break;
            }
        }

        if (hasText) {
            ui->tableWidget->showRow(row);
        } else {
            ui->tableWidget->hideRow(row);
        }
    }
}
