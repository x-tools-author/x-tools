/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKPORTNAMECOMBOBOX_H
#define SAKPORTNAMECOMBOBOX_H

#include "sakcombobox.h"

class SAKPortNameComboBox : public SAKComboBox
{
    Q_OBJECT
public:
    SAKPortNameComboBox(QWidget *parent = nullptr);

private:
    void refresh();
};

#endif // SAKPORTNAMECOMBOBOX_H
