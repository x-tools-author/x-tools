/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "asciimodel.h"

AsciiModel::AsciiModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    initDescirption();
}

AsciiModel::~AsciiModel() {}

int AsciiModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 129;
}

int AsciiModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant AsciiModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    int col = index.column();
    if (role == Qt::DisplayRole) {
        if (col == 0) {
            return QString::number(row, 2).rightJustified(8, '0');
        } else if (col == 1) {
            return QString::number(row, 8).rightJustified(3, '0');
        } else if (col == 2) {
            return QString::number(row, 10);
        } else if (col == 3) {
            return QString::number(row, 16).rightJustified(2, '0').toUpper();
        } else if (col == 4) {
            if (m_descirption.contains(row)) {
                return m_descirption.value(row);
            } else {
                return QString(QChar(row));
            }
        }
    }

    return QVariant();
}

void AsciiModel::initDescirption()
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
