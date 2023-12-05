/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKFONTAWESOMEICON_H
#define SAKFONTAWESOMEICON_H

#include <QWidget>

#include "sakfontawesomedatastruct.h"

namespace Ui {
class SAKFontAwesomeIcon;
}

class SAKFontAwesomeIcon : public QWidget
{
    Q_OBJECT
public:
    explicit SAKFontAwesomeIcon(const SAKFontAwesomeIconContext &ctx, QWidget *parent = nullptr);
    void setFilter(const QString &filter);

private:
    Ui::SAKFontAwesomeIcon *ui;
    SAKFontAwesomeIconContext m_ctx;
};

#endif // SAKFONTAWESOMEICON_H
