/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKFONTAWESOMEASSISTANT_H
#define SAKFONTAWESOMEASSISTANT_H

#include <QWidget>

namespace Ui {
class SAKFontAwesomeAssistant;
}

class SAKFontAwesomeAssistant : public QWidget
{
    Q_OBJECT
public:
    explicit SAKFontAwesomeAssistant(QWidget *parent = nullptr);

private:
    Ui::SAKFontAwesomeAssistant *ui;
};

#endif // SAKFONTAWESOMEASSISTANT_H
