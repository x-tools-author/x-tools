/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QComboBox>
#include <QDialog>

namespace xCoAP {

class CoAPOptionEditorPrivate;
class CoAPOptionEditor : public QDialog
{
    Q_OBJECT
    CoAPOptionEditorPrivate *d{nullptr};

public:
    explicit CoAPOptionEditor(QWidget *parent = nullptr);
    ~CoAPOptionEditor();

    void load(QComboBox *optionBox);
    void save(QComboBox *optionBox);
};

} // namespace xCoAP