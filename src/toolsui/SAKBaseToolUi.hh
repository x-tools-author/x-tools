/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBASETOOLUI_HH
#define SAKBASETOOLUI_HH

#include <QWidget>
#include <QLoggingCategory>

class SAKBaseTool;
class SAKBaseToolUi : public QWidget
{
    Q_OBJECT
public:
    explicit SAKBaseToolUi(QWidget *parent = nullptr);

    virtual void setupTool(SAKBaseTool *tool);
    virtual void setupSettingsGroup(const QString &group);

protected:
    virtual void onIsWorkingChanged(bool isWorking);

protected:
    SAKBaseTool *mTool{nullptr};
    QString mSettingsGroup;
};

#endif // SAKBASETOOLUI_HH
