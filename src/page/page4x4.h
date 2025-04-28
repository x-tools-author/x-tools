/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAction>
#include <QActionGroup>
#include <QSettings>
#include <QVariantMap>
#include <QWidget>

class Page;
class Page4x4 : public QWidget
{
    Q_OBJECT
public:
    enum class WindowGrid { Grid1x1, Grid1x2, Grid2x1, Grid2x2 };

public:
    struct SettingsKeys
    {
        const QString windowGrid{"pages/windowGrid"};
        const QString page00{"pages/page00"};
        const QString page01{"pages/page01"};
        const QString page10{"pages/page10"};
        const QString page11{"pages/page11"};
    };

public:
    explicit Page4x4(QSettings* settings, QWidget* parent = nullptr);
    ~Page4x4() override;

    void updateGrid(WindowGrid grid);
    void load(const QVariantMap& parameters) const;
    QVariantMap save() const;
    int windowGrid() const;
    QList<QAction*> actions() const;
    void initActions();

private:
    Page* m_iopage00;
    Page* m_iopage01;
    Page* m_iopage10;
    Page* m_iopage11;
    WindowGrid m_windowGrid{WindowGrid::Grid1x1};
    QSettings* m_settings;
    SettingsKeys m_keys;
    QActionGroup* m_group{nullptr};
};