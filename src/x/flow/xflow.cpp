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
#include <QJsonArray>
#include <QJsonObject>
#include <QPalette>

#include "nodeeditor/nodeeditor.h"
#include "nodeeditor/nodeeditorview.h"
#include "utilities/thememanager.h"

namespace xFlow {

struct xFlowParameterKeys
{
    const QString scale{"xFlow/Scale"};
};

xFlow::xFlow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::xFlow)
{
    ui->setupUi(this);
    ui->widgetNodeEditor->setupRuler(ui->widgetHRuler, ui->widgetVRuler);
    ui->widgetHRuler->setup(ui->widgetNodeEditor, Qt::Orientation::Horizontal);
    ui->widgetVRuler->setup(ui->widgetNodeEditor, Qt::Orientation::Vertical);
    ui->widgetNodeEditor->setContentsMargins(0, 0, 0, 0);
    ui->widgetHRuler->setContentsMargins(0, 0, 0, 0);
    ui->widgetVRuler->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);

    connect(&xThemeMgr, &xTools::ThemeManager::colorSchemeChanged, this, &xFlow::onThemeChanged);
    onThemeChanged();
}

xFlow::~xFlow()
{
    delete ui;
}

QJsonObject xFlow::save()
{
    xFlowParameterKeys keys;
    QJsonObject obj;
    obj[keys.scale] = ui->widgetNodeEditor->scale();
    return obj;
}

void xFlow::load(const QJsonObject &obj)
{
    xFlowParameterKeys keys;
    qreal scale = obj.value(keys.scale).toDouble(1.0);
    ui->widgetNodeEditor->setScale(scale);
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
    QColor normalColor = palette.color(QPalette::ColorRole::ButtonText);
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

} // namespace xFlow