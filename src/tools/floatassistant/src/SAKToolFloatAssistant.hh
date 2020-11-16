/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTOOLFLOATASSISTANT_HH
#define SAKTOOLFLOATASSISTANT_HH

#include <QDialog>

namespace Ui {
    class SAKToolFloatAssistant;
}
class SAKToolFloatAssistant : public QDialog
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKToolFloatAssistant(QWidget *parent = Q_NULLPTR);
    ~SAKToolFloatAssistant();
private:
    Ui::SAKToolFloatAssistant *ui;
};

#endif // SAKTOOLFLOATASSISTANT_HH
