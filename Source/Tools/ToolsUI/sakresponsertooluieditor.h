/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKRESPONSERTOOLUIEDITOR_H
#define SAKRESPONSERTOOLUIEDITOR_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class SAKResponserToolUiEditor;
}

class SAKResponserToolUiEditor : public QDialog
{
    Q_OBJECT
public:
    explicit SAKResponserToolUiEditor(QWidget *parent = nullptr);
    ~SAKResponserToolUiEditor();

    Q_INVOKABLE QJsonObject parameters();
    Q_INVOKABLE void setParameters(const QJsonObject &params);

private:
    Ui::SAKResponserToolUiEditor *ui{nullptr};
};

#endif // SAKRESPONSERTOOLUIEDITOR_H
