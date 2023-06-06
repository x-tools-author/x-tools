/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKWEBSOCKETTRANSMITTERTOOLUIEDITOR_HH
#define SAKWEBSOCKETTRANSMITTERTOOLUIEDITOR_HH

#include <QDialog>

namespace Ui {
class SAKWebSocketTransmitterToolUiEditor;
}

class SAKWebSocketTransmitterToolUiEditor : public QDialog
{
    Q_OBJECT

public:
    explicit SAKWebSocketTransmitterToolUiEditor(QWidget *parent = nullptr);
    ~SAKWebSocketTransmitterToolUiEditor();

private:
    Ui::SAKWebSocketTransmitterToolUiEditor *ui;
};

#endif // SAKWEBSOCKETTRANSMITTERTOOLUIEDITOR_HH
