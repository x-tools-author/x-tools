/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKRESPONSERTOOLUI_HH
#define SAKRESPONSERTOOLUI_HH

#include <QWidget>
#include "SAKTableViewWithController.hh"

class SAKResponserToolUi : public SAKTableViewWithController
{
    Q_OBJECT
public:
    explicit SAKResponserToolUi(QWidget *parent = nullptr);
    ~SAKResponserToolUi();

protected:
    virtual void edit(const QModelIndex &index) final;
    virtual void clear() final;
    virtual void remove(const QModelIndex &index) final;
    virtual void importFromFile(const QString &fileName) final;
    virtual void exportToFile(const QString &fineName) final;
    virtual void append() final;
};

#endif // SAKRESPONSERTOOL_HH
