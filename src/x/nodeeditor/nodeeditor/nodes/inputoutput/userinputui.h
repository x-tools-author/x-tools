/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenodeui.h"

namespace Ui {
class UserInputUi;
};

class UserInput;
class UserInputUi : public BaseNodeUi
{
    Q_OBJECT
public:
    UserInputUi(BaseNode *node, QWidget *parent = nullptr);
    ~UserInputUi() override;

    QJsonObject save() const override;
    void load(const QJsonObject &parameters) override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void output();
    void updateFormat();
    void updateCycleTimer();

private:
    Ui::UserInputUi *ui;
    QTimer *m_cycleTimer{nullptr};
};
