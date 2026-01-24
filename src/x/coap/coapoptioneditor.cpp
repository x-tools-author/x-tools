/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapoptioneditor.h"
#include "ui_coapoptioneditor.h"

namespace Ui {
class CoAPOptionEditor;
}

namespace xCoAP {

class CoAPOptionEditorPrivate : public QObject
{
public:
    explicit CoAPOptionEditorPrivate(CoAPOptionEditor *parent)
        : QObject(parent)
        , q(parent)
    {
        ui = new Ui::CoAPOptionEditor();
        ui->setupUi(q);
    }

public:
    Ui::CoAPOptionEditor *ui{nullptr};

private:
    CoAPOptionEditor *q{nullptr};
};

CoAPOptionEditor::CoAPOptionEditor(QWidget *parent)
    : QDialog(parent)
{
    d = new CoAPOptionEditorPrivate(this);
}

CoAPOptionEditor::~CoAPOptionEditor() {}

void CoAPOptionEditor::load(QComboBox *optionBox)
{
    if (!optionBox) {
        return;
    }
}

void CoAPOptionEditor::save(QComboBox *optionBox)
{
    if (!optionBox) {
        return;
    }
}

} // namespace xCoAP