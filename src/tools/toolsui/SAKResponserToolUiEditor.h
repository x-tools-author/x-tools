/*********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKRESPONSERTOOLUIEDITOR_HH
#define SAKRESPONSERTOOLUIEDITOR_HH

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

#endif // SAKRESPONSERTOOLUIEDITOR_HH
