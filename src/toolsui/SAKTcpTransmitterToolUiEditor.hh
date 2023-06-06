/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTCPTRANSMITTERTOOLUIEDITOR_HH
#define SAKTCPTRANSMITTERTOOLUIEDITOR_HH

#include <QWidget>

namespace Ui {
class SAKTcpTransmitterToolUiEditor;
}

class SAKTcpTransmitterToolUiEditor : public QWidget
{
    Q_OBJECT

public:
    explicit SAKTcpTransmitterToolUiEditor(QWidget *parent = nullptr);
    ~SAKTcpTransmitterToolUiEditor();

private:
    Ui::SAKTcpTransmitterToolUiEditor *ui;
};

#endif // SAKTCPTRANSMITTERTOOLUIEDITOR_HH
