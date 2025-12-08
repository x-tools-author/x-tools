/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xflow.h"
#include "ui_xflow.h"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QPalette>
#include <QUndoStack>

#include "nodeeditor/nodeeditor.h"
#include "nodeeditor/nodeeditorscene.h"
#include "nodeeditor/nodeeditorview.h"
#include "utilities/iconengine.h"
#include "utilities/thememanager.h"

#include "dockwidgets/log/logdockwidgetcontent.h"
#include "dockwidgets/navigator/navigatordockwidgetcontent.h"
#include "dockwidgets/nodes/nodesdockwidgetcontent.h"
#include "dockwidgets/output/outputdockwidgetcontent.h"

namespace xFlow {

struct xFlowParameterKeys
{
    const QString nodeEditor{"xFlow/nodeEditor"};
    const QString leftPanelWidth{"xFlow/leftPanelWidth"};
    const QString bottomPanelHeight{"xFlow/bottomPanelHeight"};
    const QString leftPanelVisible{"xFlow/leftPanelVisible"};
    const QString bottomPanelVisible{"xFlow/bottomPanelVisible"};
    const QString rulerVisible{"xFlow/rulerVisible"};
};

xFlow::xFlow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::xFlow)
{
    ui->setupUi(this);
    ui->widgetNodeEditor->setupRuler(ui->widgetHRuler, ui->widgetVRuler);
    ui->widgetNodeEditor->setStyleSheet("QWidget#widgetNodeEditor { border: none; }");
    ui->widgetHRuler->setup(ui->widgetNodeEditor, Qt::Orientation::Horizontal);
    ui->widgetVRuler->setup(ui->widgetNodeEditor, Qt::Orientation::Vertical);
    ui->widgetNodeEditor->setContentsMargins(0, 0, 0, 0);
    ui->widgetHRuler->setContentsMargins(0, 0, 0, 0);
    ui->widgetVRuler->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);

    connect(&xThemeMgr, &xTools::ThemeManager::colorSchemeChanged, this, &xFlow::onThemeChanged);
    onThemeChanged();

    QLabel *spacerLabel = new QLabel(tr("Navigator Window"), ui->frameLeftPanel);
    ui->frameLeftPanel->layout()->addWidget(spacerLabel);
    m_navigator = new NavigatorDockWidgetContent(ui->widgetNodeEditor);
    ui->frameLeftPanel->layout()->addWidget(m_navigator);
    ui->frameLeftPanel->layout()->addWidget(new QLabel(tr("Nodes List"), ui->frameLeftPanel));
    m_nodes = new NodesDockWidgetContent(ui->widgetNodeEditor->view());
    ui->frameLeftPanel->layout()->addWidget(m_nodes);
    ui->frameLeftPanel->setMaximumWidth(m_navigator->maximumWidth());
    ui->splitterLeftRight->setChildrenCollapsible(false);
    ui->splitterLeftRight->setSizes({m_leftPanelWidth, width() - m_leftPanelWidth});
#if 0
    ui->splitterLeftRight->handle(1)->setEnabled(false);
#endif
    ui->splitterTopBottom->setChildrenCollapsible(false);
    ui->splitterTopBottom->setSizes({height() - m_bottomPanelHeight, m_bottomPanelHeight});

    m_output = new OutputDockWidgetContext(ui->tabWidget);
    m_log = new LogDockWidgetContent(ui->tabWidget);

    ui->tabWidget->addTab(m_output, tr("Output"));
    ui->tabWidget->addTab(m_log, tr("Log"));
#if 1
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setStyleSheet("QTabWidget#tabWidget { border: none; }");
#endif

    NodeEditorView *view = ui->widgetNodeEditor->view();
    connect(view, &NodeEditorView::nodeCreated, this, &xFlow::onNodeCreated);
    connect(view, &NodeEditorView::nodeDeleted, this, &xFlow::onNodeDeleted);
    connect(view, &NodeEditorView::nodeUpdated, this, &xFlow::onNodeUpdated);
    connect(view, &NodeEditorView::nodePositionUpdated, this, &xFlow::onNodePositionUpdated);

    connect(ui->splitterLeftRight, &QSplitter::splitterMoved, this, [=](int pos, int index) {
        this->m_leftPanelWidth = ui->splitterLeftRight->sizes().at(0);
    });
    connect(ui->splitterTopBottom, &QSplitter::splitterMoved, this, [=](int pos, int index) {
        this->m_bottomPanelHeight = ui->splitterTopBottom->sizes().at(1);
    });

    initToolBar();
}

xFlow::~xFlow()
{
    delete ui;
}

QJsonObject xFlow::save()
{
    xFlowParameterKeys keys;
    QJsonObject obj;
    obj.insert(keys.nodeEditor, ui->widgetNodeEditor->save());
    obj.insert(keys.leftPanelWidth, m_leftPanelWidth);
    obj.insert(keys.bottomPanelHeight, m_bottomPanelHeight);
    obj.insert(keys.leftPanelVisible, m_actions.leftPanel->isChecked());
    obj.insert(keys.bottomPanelVisible, m_actions.bottomPanel->isChecked());
    obj.insert(keys.rulerVisible, m_actions.ruler->isChecked());
    return obj;
}

void xFlow::load(const QJsonObject &obj)
{
    xFlowParameterKeys keys;
    QJsonObject nodeEditorObject = obj.value(keys.nodeEditor).toObject();
    ui->widgetNodeEditor->load(nodeEditorObject);
    m_navigator->update();

    m_leftPanelWidth = obj.value(keys.leftPanelWidth).toInt(180);
    m_bottomPanelHeight = obj.value(keys.bottomPanelHeight).toInt(218);
    ui->splitterLeftRight->setSizes({m_leftPanelWidth, width() - m_leftPanelWidth});
    ui->splitterTopBottom->setSizes({height() - m_bottomPanelHeight, m_bottomPanelHeight});
    m_actions.leftPanel->setChecked(obj.value(keys.leftPanelVisible).toBool(true));
    m_actions.bottomPanel->setChecked(obj.value(keys.bottomPanelVisible).toBool(true));
    m_actions.ruler->setChecked(obj.value(keys.rulerVisible).toBool(true));
    ui->frameLeftPanel->setVisible(m_actions.leftPanel->isChecked());
    ui->tabWidget->setVisible(m_actions.bottomPanel->isChecked());
    ui->widgetNodeEditor->setRulerVisible(m_actions.ruler->isChecked());

    auto *view = ui->widgetNodeEditor->view();
    auto *scene = view->cookedScene();
    if (scene->undoStack().canUndo()) {
        scene->undoStack().clear();
    }

    updateScaleLineEdit();
    m_navigator->update();
}

NodeEditor *xFlow::nodeEditor() const
{
    return ui->widgetNodeEditor;
}

void xFlow::outputBytes(const QString &txt, int channel)
{
    m_output->outputBytes(txt, channel);
}

void xFlow::clearOutput(int channel)
{
    m_output->clearOutput(channel);
}

bool xFlow::event(QEvent *event)
{
    if (!ui) {
        return QWidget::event(event);
    }

    bool ret = QWidget::event(event);
    if (event->type() == QEvent::Resize) {
        ui->splitterLeftRight->setSizes({m_leftPanelWidth, width() - m_leftPanelWidth});
        ui->splitterTopBottom->setSizes({height() - m_bottomPanelHeight, m_bottomPanelHeight});
    } else if (event->type() == QEvent::Show) {
        qreal tmpScale = ui->widgetNodeEditor->scale();
        ui->widgetNodeEditor->setScale(tmpScale);
    }

    return ret;
}

void xFlow::initToolBar()
{
    if (m_toolBar) {
        return;
    }

    m_toolBar = new QToolBar(this);
    QLayout *layout = this->layout();
    QVBoxLayout *vBoxLayout = qobject_cast<QVBoxLayout *>(layout);
    if (vBoxLayout) {
        vBoxLayout->insertWidget(0, m_toolBar);
    } else {
        qWarning() << "The layout of xFlow is not QVBoxLayout.";
        return;
    }

    // clang-format off
    m_actions.leftPanel = m_toolBar->addAction(xIcon(":/res/icons/dock_to_right.svg"), tr("Toggle Left Panel"), this, &xFlow::onLeftPanel);
    m_actions.bottomPanel = m_toolBar->addAction(xIcon(":/res/icons/dock_to_bottom.svg"), tr("Toggle Bottom Panel"), this, &xFlow::onBottomPanel);
    m_actions.ruler = m_toolBar->addAction(xIcon(":/res/icons/design_services.svg"), tr("Show Ruler"), this, &xFlow::onRuler);
    m_actions.leftPanel->setCheckable(true);
    m_actions.leftPanel->setChecked(true);
    m_actions.bottomPanel->setCheckable(true);
    m_actions.bottomPanel->setChecked(true);
    m_actions.ruler->setCheckable(true);
    m_actions.ruler->setChecked(true);
    m_toolBar->addSeparator();
    m_actions.newProject = m_toolBar->addAction(xIcon(":/res/icons/note_add.svg"), tr("New Project"), this, &xFlow::onNewProject);
    m_actions.openProject = m_toolBar->addAction(xIcon(":/res/icons/file_open.svg"), tr("Open Project"), this, &xFlow::onOpenProject);
    m_actions.saveProject = m_toolBar->addAction(xIcon(":/res/icons/save.svg"), tr("Save Project"), this, &xFlow::onSaveProject);
    m_actions.saveAsProject = m_toolBar->addAction(xIcon(":/res/icons/save_as.svg"), tr("Save Project As..."), this, &xFlow::onSaveAsProject);
    m_toolBar->addSeparator();
    m_actions.fitScreen = m_toolBar->addAction(xIcon(":/res/icons/fit_screen.svg"), tr("Fit Screen"), this, &xFlow::onFitScreen);
    m_actions.fitScreen->setVisible(false);
    m_actions.zoomIn = m_toolBar->addAction(xIcon(":/res/icons/arrows_output.svg"), tr("Zoom In"), this, &xFlow::onZoomIn);
    m_actions.zoomFactor = new QLineEdit(m_toolBar);
    m_actions.zoomFactor->setFixedWidth(60);
    m_actions.zoomFactor->setReadOnly(true);
    m_toolBar->addWidget(m_actions.zoomFactor);
    m_actions.zoomOut = m_toolBar->addAction(xIcon(":/res/icons/arrows_input.svg"), tr("Zoom Out"), this, &xFlow::onZoomOut);
    m_actions.resetZoom = m_toolBar->addAction(xIcon(":/res/icons/restart_alt.svg"), tr("Reset Zoom"), this, &xFlow::onResetZoom);
    m_toolBar->addSeparator();
    m_actions.alignLeft = m_toolBar->addAction(xIcon(":/res/icons/align_horizontal_left.svg"), tr("Align Left"), this, &xFlow::onAlignLeft);
    m_actions.alignRight = m_toolBar->addAction(xIcon(":/res/icons/align_horizontal_right.svg"), tr("Align Right"), this, &xFlow::onAlignRight);
    m_actions.alignTop = m_toolBar->addAction(xIcon(":/res/icons/align_vertical_top.svg"), tr("Align Top"), this, &xFlow::onAlignTop);
    m_actions.alignBottom = m_toolBar->addAction(xIcon(":/res/icons/align_vertical_bottom.svg"), tr("Align Bottom"), this, &xFlow::onAlignBottom);
    m_actions.alignVCenter = m_toolBar->addAction(xIcon(":/res/icons/align_vertical_center.svg"), tr("Align Vertical Center"), this, &xFlow::onAlignVCenter);
    m_actions.alignHCenter = m_toolBar->addAction(xIcon(":/res/icons/align_horizontal_center.svg"), tr("Align Horizontal Center"), this, &xFlow::onAlignHCenter);
    m_toolBar->addSeparator();
    m_actions.selectAll = m_toolBar->addAction(xIcon(":/res/icons/select_all.svg"), tr("Select All"), this, &xFlow::onSelectAll);
    m_actions.deleteSelected = m_toolBar->addAction(xIcon(":/res/icons/delete.svg"), tr("Delete Selected"), this, &xFlow::onDeleteSelected);
    m_actions.clearAllNodes = m_toolBar->addAction(xIcon(":/res/icons/mop.svg"), tr("Clear All Nodes"), this, &xFlow::onClearAllNodes);
    m_toolBar->addSeparator();
    m_actions.undo = m_toolBar->addAction(xIcon(":/res/icons/undo.svg"), tr("Undo"), this, &xFlow::onUndo);
    m_actions.redo = m_toolBar->addAction(xIcon(":/res/icons/redo.svg"), tr("Redo"), this, &xFlow::onRedo);
    // clang-format on
}

void xFlow::onNewProject()
{
    ui->widgetNodeEditor->view()->clearAllNodes();
}

void xFlow::onOpenProject()
{
    QString fileName
        = QFileDialog::getOpenFileName(this,
                                       tr("Open Project"),
                                       QString(),
                                       tr("xFlow Project Files (*.xflow);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    ui->widgetNodeEditor->view()->loadProject(fileName);
    updateScaleLineEdit();
}

void xFlow::onSaveProject()
{
    if (m_projectFilePath.isEmpty()) {
        onSaveAsProject();
        return;
    }

    ui->widgetNodeEditor->view()->saveProject(m_projectFilePath);
}

void xFlow::onSaveAsProject()
{
    QString fileName
        = QFileDialog::getSaveFileName(this,
                                       tr("Save Project As..."),
                                       QString(),
                                       tr("xFlow Project Files (*.xflow);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    ui->widgetNodeEditor->view()->saveProject(fileName);
}

void xFlow::onFitScreen()
{
    ui->widgetNodeEditor->view()->fitInViewToAllItems();
    updateScaleLineEdit();
}

void xFlow::onZoomIn()
{
    ui->widgetNodeEditor->zoomIn();
    updateScaleLineEdit();
}

void xFlow::onZoomOut()
{
    ui->widgetNodeEditor->zoomOut();
    updateScaleLineEdit();
}

void xFlow::onResetZoom()
{
    ui->widgetNodeEditor->setScale(1.0);
    updateScaleLineEdit();
}

void xFlow::onAlignLeft()
{
    ui->widgetNodeEditor->view()->alignNodes(NodeEditorView::AlignTypeLeft);
}

void xFlow::onAlignRight()
{
    ui->widgetNodeEditor->view()->alignNodes(NodeEditorView::AlignTypeRight);
}

void xFlow::onAlignTop()
{
    ui->widgetNodeEditor->view()->alignNodes(NodeEditorView::AlignTypeTop);
}

void xFlow::onAlignBottom()
{
    ui->widgetNodeEditor->view()->alignNodes(NodeEditorView::AlignTypeBottom);
}

void xFlow::onAlignVCenter()
{
    ui->widgetNodeEditor->view()->alignNodes(NodeEditorView::AlignTypeVCenter);
}

void xFlow::onAlignHCenter()
{
    ui->widgetNodeEditor->view()->alignNodes(NodeEditorView::AlignTypeHCenter);
}

void xFlow::onSelectAll()
{
    ui->widgetNodeEditor->view()->selectAllNodes();
}

void xFlow::onClearSelection()
{
    ui->widgetNodeEditor->view()->deleteSelectedNodes();
}

void xFlow::onDeleteSelected()
{
    ui->widgetNodeEditor->view()->deleteSelectedNodes();
}

void xFlow::onClearAllNodes()
{
    int ret = QMessageBox::warning(this,
                                   tr("Clear All Nodes"),
                                   tr("Are you sure to clear all nodes?"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        ui->widgetNodeEditor->view()->clearAllNodes();
    }
}

void xFlow::onLeftPanel()
{
    bool visible = ui->frameLeftPanel->isVisible();
    ui->frameLeftPanel->setVisible(!visible);
}

void xFlow::onBottomPanel()
{
    bool visible = ui->tabWidget->isVisible();
    ui->tabWidget->setVisible(!visible);
}

void xFlow::onRuler()
{
    bool visible = m_actions.ruler->isChecked();
    ui->widgetHRuler->setVisible(visible);
    ui->widgetVRuler->setVisible(visible);
}

void xFlow::onUndo()
{
    auto *view = ui->widgetNodeEditor->view();
    auto *scene = view->cookedScene();
    if (scene->undoStack().canUndo()) {
        scene->undoStack().undo();
    }
}

void xFlow::onRedo()
{
    auto *view = ui->widgetNodeEditor->view();
    auto *scene = view->cookedScene();
    if (scene->undoStack().canRedo()) {
        scene->undoStack().redo();
    }
}

void xFlow::onThemeChanged()
{
    QPalette palette = qApp->palette();
    ui->widgetHRuler->setVernierColor(palette.color(QPalette::ColorRole::Accent));
    ui->widgetVRuler->setVernierColor(palette.color(QPalette::ColorRole::Accent));

    QFile file(":/res/x/flow/nodeeditorstyle.json");
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Failed to open nodeeditorstyle.json: " << file.errorString();
    }

    QByteArray json = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject style = doc.object();
    style["GraphicsViewStyle"] = cookedGraphicsViewStyle(style["GraphicsViewStyle"].toObject());
    style["NodeStyle"] = cookedNodeStyle(style["NodeStyle"].toObject());
    style["ConnectionStyle"] = cookedConnectionStyle(style["ConnectionStyle"].toObject());

    NodeEditorView *view = ui->widgetNodeEditor->view();
    view->setStylesheet(style);
}

void xFlow::onNodeCreated()
{
    m_navigator->update();
}

void xFlow::onNodeDeleted()
{
    m_navigator->update();
}

void xFlow::onNodeUpdated()
{
    m_navigator->update();
}

void xFlow::onNodePositionUpdated()
{
    m_navigator->update();
}

QJsonObject xFlow::cookedGraphicsViewStyle(const QJsonObject &style)
{
    QJsonObject graphicsViewStyle = style;
    QPalette palette = qApp->palette();
    QColor backgroundColor = palette.color(QPalette::ColorRole::Base);
    QJsonArray rgb;
    rgb.append(backgroundColor.red());
    rgb.append(backgroundColor.green());
    rgb.append(backgroundColor.blue());
    QJsonDocument rgbDoc;
    rgbDoc.setArray(rgb);
    graphicsViewStyle["BackgroundColor"] = rgb;

    QColor fineGridColor = palette.color(QPalette::ColorRole::ToolTipBase);
    rgb[0] = fineGridColor.red();
    rgb[1] = fineGridColor.green();
    rgb[2] = fineGridColor.blue();
    graphicsViewStyle["FineGridColor"] = rgb;

    QColor coarseGridColor = palette.color(QPalette::ColorRole::ToolTipBase);
    rgb[0] = coarseGridColor.red();
    rgb[1] = coarseGridColor.green();
    rgb[2] = coarseGridColor.blue();
    graphicsViewStyle["CoarseGridColor"] = rgb;

    return graphicsViewStyle;
}

QJsonObject xFlow::cookedNodeStyle(const QJsonObject &style)
{
    QPalette palette = qApp->palette();
    QJsonObject nodeStyle = style;
    QColor backgroundColor = palette.color(QPalette::ColorRole::Window);
    QJsonArray rgb;
    rgb.append(backgroundColor.red());
    rgb.append(backgroundColor.green());
    rgb.append(backgroundColor.blue());
    nodeStyle["GradientColor0"] = backgroundColor.name();
    nodeStyle["GradientColor1"] = rgb;
    nodeStyle["GradientColor2"] = rgb;
    nodeStyle["GradientColor3"] = rgb;

    QColor normalBoundaryColor = palette.color(QPalette::ColorRole::Button);
    rgb[0] = normalBoundaryColor.red();
    rgb[1] = normalBoundaryColor.green();
    rgb[2] = normalBoundaryColor.blue();
    nodeStyle["NormalBoundaryColor"] = rgb;

    QColor selectedBoundaryColor = palette.color(QPalette::Highlight);
    rgb[0] = selectedBoundaryColor.red();
    rgb[1] = selectedBoundaryColor.green();
    rgb[2] = selectedBoundaryColor.blue();
    nodeStyle["SelectedBoundaryColor"] = rgb;

    QColor shadowColor = palette.color(QPalette::ColorRole::Shadow);
    rgb[0] = shadowColor.red();
    rgb[1] = shadowColor.green();
    rgb[2] = shadowColor.blue();
    nodeStyle["ShadowColor"] = rgb;

    QColor filledConnectionPointColor = palette.color(QPalette::ColorRole::Highlight);
    rgb[0] = filledConnectionPointColor.red();
    rgb[1] = filledConnectionPointColor.green();
    rgb[2] = filledConnectionPointColor.blue();
    nodeStyle["FilledConnectionPointColor"] = rgb;

    QColor connectionPointColor = palette.color(QPalette::ColorRole::Mid);
    rgb[0] = connectionPointColor.red();
    rgb[1] = connectionPointColor.green();
    rgb[2] = connectionPointColor.blue();
    nodeStyle["ConnectionPointColor"] = rgb;

    QColor fontColor = palette.color(QPalette::ColorRole::Text);
    nodeStyle["FontColor"] = fontColor.name();

#if 0
    QColor fontColorFaded = currentTheme.secondaryColorDisabled;
    nodeStyle["FontColorFaded"] = fontColorFaded.name();
#endif

    return nodeStyle;
}

QJsonObject xFlow::cookedConnectionStyle(const QJsonObject &style)
{
    QJsonObject connectionStyle = style;
    QPalette palette = qApp->palette();
    QColor normalColor = palette.color(QPalette::ColorRole::Accent);
    QJsonArray rgb;
    rgb.append(normalColor.red());
    rgb.append(normalColor.green());
    rgb.append(normalColor.blue());
    connectionStyle["NormalColor"] = rgb;
    connectionStyle["ConstructionColor"] = rgb;

    QColor selectedColor = palette.color(QPalette::ColorRole::Accent);
    rgb[0] = selectedColor.red();
    rgb[1] = selectedColor.green();
    rgb[2] = selectedColor.blue();
    connectionStyle["SelectedColor"] = rgb;

    return connectionStyle;
}

void xFlow::updateScaleLineEdit()
{
    double scale = ui->widgetNodeEditor->scale();
    m_actions.zoomFactor->setText(QString::number(scale * 100.0, 'f', 1) + "%");
}

} // namespace xFlow
