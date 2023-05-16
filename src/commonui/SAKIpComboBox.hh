/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKIPCOMBOBOX_HH
#define SAKIPCOMBOBOX_HH

#include <QComboBox>

class SAKIpComboBox : public QComboBox
{
    Q_OBJECT
public:
    SAKIpComboBox(QWidget *parent = nullptr);
};

#endif // SAKIPCOMBOBOX_HH
