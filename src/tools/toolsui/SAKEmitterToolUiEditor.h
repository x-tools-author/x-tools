/*********************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#ifndef SAKEMITTERTOOLUIEDITOR_HH
#define SAKEMITTERTOOLUIEDITOR_HH

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class SAKEmitterToolUiEditor;
}

class SAKEmitterToolUiEditor : public QDialog
{
    Q_OBJECT
public:
    explicit SAKEmitterToolUiEditor(QWidget *parent = nullptr);
    ~SAKEmitterToolUiEditor();

    Q_INVOKABLE QJsonObject parameters();
    Q_INVOKABLE void setParameters(const QJsonObject &params);

private:
    Ui::SAKEmitterToolUiEditor *ui{nullptr};
};

#endif // SAKEMITTERTOOLUIEDITOR_HH
