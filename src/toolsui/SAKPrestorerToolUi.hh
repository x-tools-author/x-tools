/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKPRESTORERTOOLUI_HH
#define SAKPRESTORERTOOLUI_HH

#include "SAKTableViewWithController.hh"

class SAKPrestorerTool;
class SAKPrestorerToolUiEditor;
class SAKPrestorerToolUi : public SAKTableViewWithController
{
    Q_OBJECT
public:
    explicit SAKPrestorerToolUi(QWidget *parent = nullptr);
    ~SAKPrestorerToolUi();
    void setupSAKPrestorerTool(SAKPrestorerTool *tool);

protected:
    virtual void edit(const QModelIndex &index) final;
    virtual void clear() final;
    virtual void remove(const QModelIndex &index) final;
    virtual void importFromFile(const QString &fileName) final;
    virtual void exportToFile(const QString &fineName) final;
    virtual void append() final;

private:
    SAKPrestorerTool *mTool{nullptr};
    SAKPrestorerToolUiEditor *mEditor{nullptr};
};

#endif // SAKPRESTORERTOOLUI_HH
