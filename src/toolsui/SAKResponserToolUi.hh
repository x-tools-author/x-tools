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
#include "SAKResponserToolUi.hh"

class SAKResponserTool;
class SAKResponserToolUiEditor;
class SAKResponserToolUi : public SAKTableViewWithController
{
    Q_OBJECT
public:
    explicit SAKResponserToolUi(QWidget *parent = nullptr);
    ~SAKResponserToolUi();
    void setupResponserTool(SAKResponserTool *tool);

protected:
    virtual void edit(const QModelIndex &index) final;
    virtual void clear() final;
    virtual void remove(const QModelIndex &index) final;
    virtual void importFromFile(const QString &fileName) final;
    virtual void exportToFile(const QString &fineName) final;
    virtual void append() final;

private:
    SAKResponserTool *mTool{nullptr};
    SAKResponserToolUiEditor *mEditor{nullptr};
};

#endif // SAKRESPONSERTOOL_HH
