/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKBASETOOLUI_H
#define SAKBASETOOLUI_H

#include <QLoggingCategory>
#include <QWidget>

class SAKBaseTool;
class SAKBaseToolUi : public QWidget
{
    Q_OBJECT
public:
    explicit SAKBaseToolUi(QWidget *parent = nullptr);

    void initialize(SAKBaseTool *tool,
                    const QString &settingsGroup,
                    const char *loggingCategory = nullptr);

protected:
    virtual void onIsWorkingChanged(bool isWorking);
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool, const QString &settingsGroup);

protected:
    QLoggingCategory *mLoggingCategory{nullptr};
    SAKBaseTool *mTool{nullptr};
};

#endif // SAKBASETOOLUI_H
