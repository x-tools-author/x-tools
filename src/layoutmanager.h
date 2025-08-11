/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QActionGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QObject>
#include <QStackedLayout>
#include <QToolButton>

namespace xFlow {
class NodeEditor;
}

class LayoutManager : public QObject
{
    Q_OBJECT
public:
    explicit LayoutManager(QStackedLayout* layout, QMenuBar* menuBar, QObject* parent = nullptr);
    ~LayoutManager() override;

    QAction* addLayoutPage(const QString& name, QWidget* page);
    void setupPages();

private:
    QStackedLayout* m_layout{nullptr};
    QActionGroup* m_group{nullptr};

    QMenuBar* m_mainMenuBar{nullptr};
    xFlow::NodeEditor* m_nodeEditor{nullptr};
};
