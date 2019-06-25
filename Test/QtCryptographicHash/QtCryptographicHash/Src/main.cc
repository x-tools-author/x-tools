/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
 */
#include <QApplication>
#include "QtCryptographicHashController.hh"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QtCryptographicHashController controller;
    controller.show();

    return app.exec();
}
