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
class Pipeline : public QObject
{
    Q_OBJECT
public:
    Pipeline(Page *leftPage, Page *rightPage, QObject *parent = nullptr);
    ~Pipeline();

private:
    Page *m_leftPage;
    Page *m_rightPage;
    QToolButton *m_left2rightButton;
    QToolButton *m_right2leftButton;

private:
    void onLeft2RightButtonClicked();
    void onRight2LeftButtonClicked();

    void onLeftPageBytesRead(const QByteArray &bytes);
    void onRightPageBytesRead(const QByteArray &bytes);
};
