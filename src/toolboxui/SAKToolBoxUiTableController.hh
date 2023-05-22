/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLBOXUITABLECONTROLLER_HH
#define SAKTOOLBOXUITABLECONTROLLER_HH

#include <QWidget>

namespace Ui {
class SAKTableController;
}

class SAKToolBoxUiTableController : public QWidget
{
    Q_OBJECT
public:
    explicit SAKToolBoxUiTableController(QWidget *parent = nullptr);
    ~SAKToolBoxUiTableController();

private:
    Ui::SAKTableController *ui{nullptr} ;
};

#endif // SAKTOOLBOXUITABLECONTROLLER_HH
