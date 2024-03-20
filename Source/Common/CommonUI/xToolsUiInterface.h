/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKUIINTERFACE_H
#define SAKUIINTERFACE_H

#include <QLineEdit>
#include <QMainWindow>
#include <QObject>

class SAKUiInterface : public QObject
{
    Q_OBJECT
public:
    explicit SAKUiInterface(QObject *parent = nullptr);

    static void setValidator(QLineEdit *le, int textFormat);
    static QIcon cookedIcon(const QIcon &icon);
    static QMainWindow *mainWindow();
};

#endif // SAKUIINTERFACE_H
