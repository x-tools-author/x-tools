/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKEMITTERTOOLUI_HH
#define SAKEMITTERTOOLUI_HH

#include "SAKEmitterTool.hh"
#include "SAKEmitterToolUiEditor.hh"
#include "SAKTableViewWithController.hh"

class SAKEmitterToolUi : public SAKTableViewWithController
{
    Q_OBJECT
public:
    explicit SAKEmitterToolUi(QWidget *parent = nullptr);
    ~SAKEmitterToolUi();
    void setupEmitterTool(SAKEmitterTool *tool);

protected:
    virtual void edit(const QModelIndex &index) override;
    virtual void clear() override;
    virtual void remove(const QModelIndex &index) override;
    virtual void importFromFile(const QString &fileName) override;
    virtual void exportToFile(const QString &fileName) override;
    virtual void append() override;

private:
    SAKEmitterTool *mTool{nullptr};
    SAKEmitterToolUiEditor *mEditor{nullptr};
};

#endif // SAKEMITTERTOOLUI_HH
