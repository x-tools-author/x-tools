/*
 * Copyright 2022 Qter(qsaker@qq.com. All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */

#ifndef SAKTOOLASCIIASSISTANT_H
#define SAKTOOLASCIIASSISTANT_H

#include <QDialog>

namespace Ui {
    class SAKToolAsciiAssistant;
}

class SAKToolAsciiAssistant : public QDialog
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKToolAsciiAssistant(QWidget *parent = Q_NULLPTR);
    ~SAKToolAsciiAssistant();

private:
    Ui::SAKToolAsciiAssistant *ui;
};

#endif // SAKTOOLASCIIASSISTANT_H
