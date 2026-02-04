/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapresourceeditor.h"
#include "ui_coapresourceeditor.h"

#include "coapcommon.h"

namespace xCoAP {

class CoAPResourceEditorPrivate : public QObject
{
public:
    CoAPResourceEditorPrivate(CoAPResourceEditor* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPResourceEditor();
        ui->setupUi(q);

        CoAPCommon::setupContextFormat(ui->comboBoxContextFormat);
    }
    ~CoAPResourceEditorPrivate() override {}

public:
    Ui::CoAPResourceEditor* ui{nullptr};
    int m_editingRow{-1};

private:
    CoAPResourceEditor* q{nullptr};
};

CoAPResourceEditor::CoAPResourceEditor(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPResourceEditorPrivate(this);
    setAttribute(Qt::WA_ShowModal, true);
    connect(d->ui->buttonBox, &QDialogButtonBox::accepted, this, [=]() {
        close();
        emit accepted();
    });
    connect(d->ui->buttonBox, &QDialogButtonBox::rejected, this, [=]() {
        close();
        emit rejected();
    });
}

CoAPResourceEditor::~CoAPResourceEditor() {}

QJsonObject CoAPResourceEditor::save()
{
    CoAPCommon::ResourceItem item;
    item.description = d->ui->lineEditDescription->text();
    item.contextFormat = d->ui->comboBoxContextFormat->currentData().toInt();
    item.uriPath = d->ui->lineEditUriPath->text();
    item.payload = d->ui->textEdit->toPlainText().toUtf8();
    return CoAPCommon::resourceItem2JsonObject(item);
}

void CoAPResourceEditor::load(const QJsonObject& obj)
{
    CoAPCommon::ResourceItem item = CoAPCommon::jsonObject2ResourceItem(obj);
    d->ui->lineEditUriPath->setText(item.uriPath);
    d->ui->lineEditDescription->setText(item.description);
    int index = d->ui->comboBoxContextFormat->findData(item.contextFormat);
    d->ui->comboBoxContextFormat->setCurrentIndex(index >= 0 ? index : 0);
    d->ui->textEdit->setPlainText(QString::fromUtf8(item.payload));
}

int CoAPResourceEditor::editingRow() const
{
    return d->m_editingRow;
}

void CoAPResourceEditor::setEditingRow(int row)
{
    d->m_editingRow = row;
}

} // namespace xCoAP