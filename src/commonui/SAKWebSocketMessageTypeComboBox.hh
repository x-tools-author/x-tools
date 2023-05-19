/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKWEBSOCKETMESSAGETYPECOMBOBOX_HH
#define SAKWEBSOCKETMESSAGETYPECOMBOBOX_HH

#include <QComboBox>

class SAKWebSocketMessageTypeComboBox : public QComboBox
{
    Q_OBJECT
public:
    SAKWebSocketMessageTypeComboBox(QWidget *parent = nullptr);
};

#endif // SAKWEBSOCKETMESSAGETYPECOMBOBOX_HH
