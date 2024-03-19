/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKMENU_H
#define SAKMENU_H

#include <QMenu>

class SAKMenu : public QMenu
{
public:
    SAKMenu(const QString &title, QWidget *parent = nullptr);
    SAKMenu(QWidget *parent = nullptr);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
};

#endif // SAKMENU_H
