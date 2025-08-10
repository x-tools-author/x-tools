/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QStackedLayout>
#include <QWidget>

namespace xFlow {
class NodeEditor;
}

class LayoutManager : public QObject
{
    Q_OBJECT
public:
    explicit LayoutManager(QStackedLayout* layout, QWidget* mw, QObject* parent = nullptr);
    ~LayoutManager() override;

    void addLayoutPage(const QString& name, QWidget* page);
    QWidget* controller();

    void setupPages();

private:
    void onGroupButtonClicked(QAbstractButton* button);

private:
    QStackedLayout* m_layout{nullptr};
    QWidget* m_controller{nullptr};
    QHBoxLayout* m_hLayout{nullptr};
    QButtonGroup* m_buttonGroup{nullptr};
    QLabel* m_leftLabel{nullptr};
    QLabel* m_rightLabel{nullptr};

private:
    QWidget* m_mainWindow{nullptr};
    xFlow::NodeEditor* m_nodeEditor{nullptr};
};
