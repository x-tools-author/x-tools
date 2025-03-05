/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QDialog>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui {
class TextItemEditor;
}
QT_END_NAMESPACE

class TextItemEditor : public QDialog
{
    Q_OBJECT
public:
    explicit TextItemEditor(QWidget *parent = nullptr);
    ~TextItemEditor();

    QJsonObject save() const;
    void load(const QJsonObject &parameters);

private:
    Ui::TextItemEditor *ui;

private:
    void onTextFormatChanged();
};
