/*
 * Copyright 2022 Qter(qsaker@qq.com. All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */

#ifndef QSAKASCIIASSISTANT_HH
#define QSAKASCIIASSISTANT_HH

#include <QDialog>

namespace Ui {
    class QsakAsciiAssistant;
}

class QsakAsciiAssistant : public QDialog
{
    Q_OBJECT
public:
    Q_INVOKABLE QsakAsciiAssistant(QWidget *parent = Q_NULLPTR);
    ~QsakAsciiAssistant();

private:
    Ui::QsakAsciiAssistant *ui;
};

#endif // QSAKASCIIASSISTANT_HH
