/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSOCKETCLIENTTRANSMITTERTOOLUIEDITOR_HH
#define SAKSOCKETCLIENTTRANSMITTERTOOLUIEDITOR_HH

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

#endif // SAKSOCKETCLIENTTRANSMITTERTOOLUIEDITOR_HH
