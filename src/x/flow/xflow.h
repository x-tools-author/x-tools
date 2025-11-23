/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAction>
#include <QJsonObject>
#include <QLineEdit>
#include <QToolBar>
#include <QWidget>

namespace Ui {
class xFlow;
}

namespace xFlow {

class LogDockWidgetContent;
class NodesDockWidgetContent;
class OutputDockWidgetContext;
class NavigatorDockWidgetContent;
class xFlow : public QWidget
{
    Q_OBJECT
public:
    explicit xFlow(QWidget *parent = nullptr);
    ~xFlow() override;

    QJsonObject save();
    void load(const QJsonObject &obj);

    Q_INVOKABLE void outputBytes(const QString &txt, int channel);
    Q_INVOKABLE void clearOutput(int channel);

protected:
    bool event(QEvent *event) override;

private:
    Ui::xFlow *ui{nullptr};
    LogDockWidgetContent *m_log{nullptr};
    NodesDockWidgetContent *m_nodes{nullptr};
    OutputDockWidgetContext *m_output{nullptr};
    NavigatorDockWidgetContent *m_navigator{nullptr};
    int m_leftPanelWidth{180};
    int m_bottomPanelHeight{218};

private:
    struct ActionContext
    {
        QAction *leftPanel{nullptr};
        QAction *bottomPanel{nullptr};
        QAction *ruler{nullptr};

        QAction *newProject{nullptr};
        QAction *openProject{nullptr};
        QAction *saveProject{nullptr};
        QAction *saveAsProject{nullptr};

        QAction *fitScreen{nullptr};
        QAction *zoomIn{nullptr};
        QAction *zoomOut{nullptr};
        QAction *resetZoom{nullptr};
        QLineEdit *zoomFactor{nullptr};

        QAction *alignLeft{nullptr};
        QAction *alignRight{nullptr};
        QAction *alignTop{nullptr};
        QAction *alignBottom{nullptr};
        QAction *alignVCenter{nullptr};
        QAction *alignHCenter{nullptr};

        QAction *selectAll{nullptr};
        QAction *deleteSelected{nullptr};
        QAction *clearAllNodes{nullptr};

        QAction *undo{nullptr};
        QAction *redo{nullptr};
    } m_actions;
    QToolBar *m_toolBar{nullptr};
    QString m_projectFilePath;

    void initToolBar();

    void onNewProject();
    void onOpenProject();
    void onSaveProject();
    void onSaveAsProject();

    void onFitScreen();
    void onZoomIn();
    void onZoomOut();
    void onResetZoom();

    void onAlignLeft();
    void onAlignRight();
    void onAlignTop();
    void onAlignBottom();
    void onAlignVCenter();
    void onAlignHCenter();

    void onSelectAll();
    void onClearSelection();
    void onDeleteSelected();
    void onClearAllNodes();

    void onLeftPanel();
    void onBottomPanel();
    void onRuler();

    void onUndo();
    void onRedo();

private:
    void onThemeChanged();
    void onNodeCreated();
    void onNodeDeleted();

    QJsonObject cookedGraphicsViewStyle(const QJsonObject &style);
    QJsonObject cookedNodeStyle(const QJsonObject &style);
    QJsonObject cookedConnectionStyle(const QJsonObject &style);

    void updateScaleLineEdit();
};

} // namespace xFlow