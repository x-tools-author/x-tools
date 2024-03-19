/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKSOCKETCLIENTTRANSMITTERTOOLUIEDITOR_H
#define SAKSOCKETCLIENTTRANSMITTERTOOLUIEDITOR_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class SAKSocketClientTransmitterToolUiEditor;
}

class SAKSocketClientTransmitterToolUiEditor : public QDialog
{
    Q_OBJECT
public:
    SAKSocketClientTransmitterToolUiEditor(QWidget *parent = nullptr);
    ~SAKSocketClientTransmitterToolUiEditor();

    void setWebSocketContextVisible(bool v);

    Q_INVOKABLE QJsonObject parameters();
    Q_INVOKABLE void setParameters(const QJsonObject &params);

private:
    Ui::SAKSocketClientTransmitterToolUiEditor *ui;
};

#endif // SAKSOCKETCLIENTTRANSMITTERTOOLUIEDITOR_H
