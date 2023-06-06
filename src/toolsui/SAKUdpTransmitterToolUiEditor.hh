/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUDPTRANSMITTERTOOLUIEDITOR_HH
#define SAKUDPTRANSMITTERTOOLUIEDITOR_HH

#include <QDialog>

namespace Ui {
class SAKUdpTransmitterToolUiEditor;
}

class SAKUdpTransmitterToolUiEditor : public QDialog
{
    Q_OBJECT

public:
    explicit SAKUdpTransmitterToolUiEditor(QWidget *parent = nullptr);
    ~SAKUdpTransmitterToolUiEditor();

private:
    Ui::SAKUdpTransmitterToolUiEditor *ui;
};

#endif // SAKUDPTRANSMITTERTOOLUIEDITOR_HH
