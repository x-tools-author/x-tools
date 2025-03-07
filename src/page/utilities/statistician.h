/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QLabel>
#include <QObject>

class Statistician : public QObject
{
    Q_OBJECT
public:
    explicit Statistician(QLabel *view, QObject *parent = nullptr);

    void inputBytes(const QByteArray &bytes);
    void reset();

private:
    void updateLabel();

private:
    int m_frames{0};
    int m_bytes{0};
    int m_speed{0};
    QByteArray m_tempBytes;
    QLabel *m_view;
};
