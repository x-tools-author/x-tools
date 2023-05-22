/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTABLECONTROLLER_HH
#define SAKTABLECONTROLLER_HH

#include <QWidget>

namespace Ui {
class SAKTableController;
}

class SAKTableController : public QWidget
{
    Q_OBJECT
public:
    explicit SAKTableController(QWidget *parent = nullptr);
    ~SAKTableController();

private:
    Ui::SAKTableController *ui{nullptr};
};

#endif // SAKTABLECONTROLLER_HH
