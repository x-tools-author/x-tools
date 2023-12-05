/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKFONTAWESOME_H
#define SAKFONTAWESOME_H

#include <QList>
#include <QObject>

#include "sakfontawesomedatastruct.h"

class SAKFontAwesome : public QObject
{
    Q_OBJECT
private:
    explicit SAKFontAwesome(QObject *parent = nullptr);

public:
    static SAKFontAwesome *instance();

    void addApplicationFonts();
    QList<SAKFontAwesomeIconContext> supportedIcons() const;
    QString familyToString(int family);

private:
    QList<SAKFontAwesomeIconContext> m_supportedIcons;

private:
    QString fontAwesomePath() const;
};

#endif // SAKFONTAWESOME_H
