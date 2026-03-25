/***************************************************************************************************
 * Copyright 2024-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QSettings>
#include <QToolButton>
#include <QVariantMap>
#include <QWidget>

class PagePrivate;
class Page : public QWidget
{
    Q_OBJECT
    PagePrivate *d{nullptr};

public:
    enum ControllerDirection { Left, Right };

public:
    explicit Page(ControllerDirection direction, QSettings *settings, QWidget *parent = nullptr);
    ~Page() override;

    QVariantMap save() const;
    void load(const QVariantMap &parameters);
    void writeBytes(const QByteArray &bytes);
    void prependOutputControl(QWidget *widget);
    void appendOutputControl(QWidget *widget);
    void aboutToClose();
    void showLiteMode();

signals:
    void bytesWritten(const QByteArray &bytes, const QString &to);
    void bytesRead(const QByteArray &bytes, const QString &from);
};
