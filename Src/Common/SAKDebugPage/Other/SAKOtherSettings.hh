/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */
#ifndef TRANSMISSION_HH
#define TRANSMISSION_HH

#include <QWidget>

class Transmission:public QWidget
{
    Q_OBJECT
public:
    Transmission(QWidget *parent = nullptr);
};

#endif
