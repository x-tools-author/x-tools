/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "framelistitemeditor.h"
#include "ui_framelistitemeditor.h"

#include <QGridLayout>
#include <QLineEdit>

#include "common/xtools.h"
#include "utilities/compatibility.h"

#include "canbuscommon.h"

namespace Ui {
class FrameListItemEditor;
} // namespace Ui

namespace xCanBus {

class FrameListItemEditorPrivate : public QObject
{
public:
    FrameListItemEditorPrivate(FrameListItemEditor *parent)
        : QObject(parent)
        , q(parent)
    { }

public:
    Ui::FrameListItemEditor *ui{nullptr};

public:
    void initHexEditor(QWidget *hexEditor)
    {
        QGridLayout *l = new QGridLayout(hexEditor);
        l->setContentsMargins(0, 0, 0, 0);
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                QLabel *label = new QLabel(hexEditor);
                label->setText(QString("%1").arg(row * 8 + col + 1, 2, 10, QChar('0')).toUpper());
                label->setAlignment(Qt::AlignCenter);
                l->addWidget(label, row * 2, col);
                m_hexLabels.append(label);

                QLineEdit *le = new QLineEdit(hexEditor);
                le->setMaximumWidth(48);
                le->setText(QStringLiteral("00"));
                le->setAlignment(Qt::AlignCenter);
                l->addWidget(le, row * 2 + 1, col);
                xSetupTextFormatValidator(le, int(TextFormat::Hex), 2);
                m_hexEditors.append(le);
            }
        }
    }
    QByteArray payload() const
    {
        QByteArray payload;
        for (int i = 0; i < m_hexEditors.count(); ++i) {
            if (m_hexEditors[i]->isEnabled()) {
                bool ok;
                int byte = m_hexEditors[i]->text().toInt(&ok, 16);
                if (ok) {
                    payload.append(static_cast<char>(byte));
                } else {
                    payload.append(static_cast<char>(0));
                }
            }
        }
        return payload;
    }
    void setPayload(const QByteArray &payload)
    {
        for (int i = 0; i < payload.size() && i < m_hexEditors.count(); ++i) {
            m_hexEditors[i]->setText(
                QString("%1")
                    .arg(static_cast<unsigned char>(payload.at(i)), 2, 16, QChar('0'))
                    .toUpper());
        }
    }

    void onPayloadLengthChanged(int length)
    {
        for (int i = 0; i < m_hexEditors.count(); ++i) {
            m_hexEditors[i]->setEnabled(i < length);
            m_hexLabels[i]->setEnabled(i < length);
        }
    }

private:
    FrameListItemEditor *q{nullptr};
    QList<QLineEdit *> m_hexEditors;
    QList<QLabel *> m_hexLabels;
};

FrameListItemEditor::FrameListItemEditor(QWidget *parent)
    : QWidget(parent)
{
    d = new FrameListItemEditorPrivate(this);
    d->ui = new Ui::FrameListItemEditor;
    d->ui->setupUi(this);
    d->initHexEditor(d->ui->widgetHexEditor);

    xSetupFrameTypes(d->ui->comboBoxFrameType);

    // clang-format off
    connect(d->ui->buttonBox, &QDialogButtonBox::accepted, this, &FrameListItemEditor::accepted);
    connect(d->ui->buttonBox, &QDialogButtonBox::rejected, this, &FrameListItemEditor::rejected);
    connect(d->ui->spinBoxPayloadLength, xSpinBoxValueChanged, d, &FrameListItemEditorPrivate::onPayloadLengthChanged);
    // clang-format on

    d->onPayloadLengthChanged(d->ui->spinBoxPayloadLength->value());
}

FrameListItemEditor::~FrameListItemEditor()
{
    delete d->ui;
}

FrameItem FrameListItemEditor::frameItem() const
{
    FrameItem item;
    item.name = d->ui->lineEditFrameName->text();
    item.cycle = d->ui->checkBoxTimedSending->isChecked();
    item.cycleInterval = d->ui->spinBoxTimedSendingInterval->value();
    item.response = d->ui->checkBoxResponseEnable->isChecked();

    item.frame.setFrameId(d->ui->spinBoxFrameId->value());
    item.frame.setExtendedFrameFormat(d->ui->checkBoxExtendedFormat->isChecked());
    item.frame.setFlexibleDataRateFormat(d->ui->checkBoxFlexibleDataRate->isChecked());
    item.frame.setBitrateSwitch(d->ui->checkBoxBitRateSwitch->isChecked());

    QByteArray payload = d->payload();
    payload.resize(d->ui->spinBoxPayloadLength->value());
    item.frame.setPayload(payload);

    return item;
}

void FrameListItemEditor::setFrameItem(const FrameItem &frameItem)
{
    d->ui->lineEditFrameName->setText(frameItem.name);
    d->ui->checkBoxTimedSending->setChecked(frameItem.cycle);
    d->ui->spinBoxTimedSendingInterval->setValue(frameItem.cycleInterval);
    d->ui->checkBoxResponseEnable->setChecked(frameItem.response);

    int payloadLength = frameItem.frame.payload().size();
    d->ui->spinBoxPayloadLength->setValue(payloadLength);
    d->setPayload(frameItem.frame.payload());

    d->ui->spinBoxFrameId->setValue(frameItem.frame.frameId());
    d->ui->checkBoxExtendedFormat->setChecked(frameItem.frame.hasExtendedFrameFormat());
    d->ui->checkBoxFlexibleDataRate->setChecked(frameItem.frame.hasFlexibleDataRateFormat());
    d->ui->checkBoxBitRateSwitch->setChecked(frameItem.frame.hasBitrateSwitch());

    d->onPayloadLengthChanged(payloadLength);
}

} // namespace xCanBus