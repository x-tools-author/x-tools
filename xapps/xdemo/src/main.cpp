#/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QStyleHints>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.styleHints()->setColorScheme(Qt::ColorScheme::Dark);
    QMainWindow w;
    w.setMenuBar(new QMenuBar(&w));
    w.menuBar()->addMenu("File");
    w.setWindowTitle("xDemo Application");
    w.resize(800, 600);
    w.setWindowFlags(w.windowFlags() | Qt::ExpandedClientAreaHint);
    w.show();
    return a.exec();
}