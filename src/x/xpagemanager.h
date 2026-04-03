/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QLabel>
#include <QList>
#include <QMenuBar>
#include <QObject>
#include <QStackedLayout>
#include <QToolButton>
#include <QWidget>

namespace xTools {
class xTools;
}

class xPageManagerPrivate;
class xPageManager : public QObject
{
    Q_OBJECT
    xPageManagerPrivate* d{nullptr};

public:
    explicit xPageManager(QStackedLayout* layout, QMenuBar* menuBar, QObject* parent = nullptr);
    ~xPageManager() override;

    QJsonObject save();
    void load(const QJsonObject& obj);

    int currentIndex() const;
    void setCurrentIndex(int index);

    QList<QAction*> newWindowActions();
    xTools::xTools* xTools();
    void showLiteMode();
};
