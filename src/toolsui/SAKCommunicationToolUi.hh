/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKCOMMUNICATIONTOOLUI_HH
#define SAKCOMMUNICATIONTOOLUI_HH

#include <QWidget>

class SAKCommunicationTool;
class SAKCommunicationToolUi : public QWidget
{
    Q_OBJECT
public:
    explicit SAKCommunicationToolUi(QWidget *parent = nullptr);
    virtual void setupCommunicationTool(SAKCommunicationTool *tool) = 0;
    virtual void updateUiState(bool isWorking);

    void setSettingsGroup(const QString &group);

protected:
    QString mGroup;
};

#endif // SAKCOMMUNICATIONTOOLUI_HH
