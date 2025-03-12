/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QObject>
#include <QToolButton>

class Page;
class Pipe : public QObject
{
    Q_OBJECT
public:
    Pipe(Page *leftPage, Page *rightPage, QObject *parent = nullptr);
    ~Pipe();

private:
    Page *m_leftPage;
    Page *m_rightPage;
    QToolButton *m_leftButton;
    QToolButton *m_rightButton;

private:
    struct
    {
        const QString left2right{"MainWindow/left2right"};
        const QString right2left{"MainWindow/right2left"};
    } m_keys;

private:
    void onLeftButtonClicked();
    void onRightButtonClicked();

    void onLeftPageBytesRead(const QByteArray &bytes, const QString &from);
    void onRightPageBytesRead(const QByteArray &bytes, const QString &from);
};
