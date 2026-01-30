/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coappayloadeditor.h"
#include "ui_coappayloadeditor.h"

#include <QAction>
#include <QActionGroup>
#include <QHeaderView>
#include <QJsonArray>
#include <QMenu>

#include "coapcommon.h"

namespace Ui {
class CoAPPayloadEditor;
};

namespace xCoAP {

class CoAPPayloadEditorPrivate : public QObject
{
public:
    CoAPPayloadEditorPrivate(CoAPPayloadEditor* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPPayloadEditor();
        ui->setupUi(q);
        CoAPCommon::setupContextFormat(ui->comboBoxContextFormat);

        connect(ui->buttonBox, &QDialogButtonBox::accepted, q, [=]() { emit q->accepted(); });
        connect(ui->buttonBox, &QDialogButtonBox::rejected, q, [=]() { q->close(); });
    }
    ~CoAPPayloadEditorPrivate() override {}

public:
    Ui::CoAPPayloadEditor* ui{nullptr};

private:
    CoAPPayloadEditor* q{nullptr};
};

struct CoAPPayloadEditorParameterKeys
{
    const QString columns{"columns"};
};

CoAPPayloadEditor::CoAPPayloadEditor(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPPayloadEditorPrivate(this);
    setAttribute(Qt::WA_ShowModal, true);
}

CoAPPayloadEditor::~CoAPPayloadEditor() {}

QJsonObject CoAPPayloadEditor::save()
{
    CoAPPayloadEditorParameterKeys keys;
    QJsonObject obj;
    return obj;
}

void CoAPPayloadEditor::load(const QJsonObject& obj)
{
    CoAPPayloadEditorParameterKeys keys;
}

} // namespace xCoAP
