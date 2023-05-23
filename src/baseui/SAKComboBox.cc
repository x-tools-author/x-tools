/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKComboBox.hh"

SAKComboBox::SAKComboBox(QWidget *parent)
    : QComboBox(parent)
{

}

void SAKComboBox::setCurrentIndexFromData(const QVariant &data)
{
    int ret = findData(data);
    if (ret != -1) {
        setCurrentIndex(ret);
    }
}
