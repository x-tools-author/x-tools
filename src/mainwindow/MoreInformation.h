/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef MOREINFORMATION_H
#define MOREINFORMATION_H

#include <QDialog>

namespace Ui {
class MoreInformation;
};

class MoreInformation:public QDialog
{
    Q_OBJECT
public:
    MoreInformation(QWidget* parent = nullptr);
private:
    Ui::MoreInformation* ui = nullptr;
};

#endif
