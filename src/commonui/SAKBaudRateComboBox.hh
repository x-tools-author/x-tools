/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBAUDRATECOMBOBOX_HH
#define SAKBAUDRATECOMBOBOX_HH

#include <QComboBox>

class SAKBaudRateComboBox : public QComboBox
{
    Q_OBJECT
public:
    SAKBaudRateComboBox(QWidget *parent = nullptr);
};

#endif // SAKBAUDRATECOMBOBOX_H
