/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "nodesdockwidgetcontent.h"
#include "ui_nodesdockwidgetcontent.h"

#include <QGraphicsScene>

#include <QContextMenuEvent>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QStandardPaths>

#include "utilities/iconengine.h"

#include "../../nodeeditor/nodeeditorregistry.h"
#include "../../nodeeditor/nodeeditorscene.h"
#include "../../nodeeditor/nodeeditorview.h"

namespace xFlow {

NodesDockWidgetContent::NodesDockWidgetContent(NodeEditorView *view, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NodesDockWidgetContent)
{
    ui->setupUi(this);
    setWindowTitle(tr("Nodes"));

    ui->treeWidget->clear();
    ui->treeWidget->setHeaderHidden(true);

    QGraphicsScene *scene = view->scene();
    NodeEditorScene *nodeScene = dynamic_cast<NodeEditorScene *>(scene);
    if (nodeScene == nullptr) {
        return;
    }

    QtNodes::DataFlowGraphModel &model = nodeScene->dataFlowGraphModel();
    auto registry = model.dataModelRegistry();
    auto xRegistry = std::static_pointer_cast<NodeEditorRegistry>(registry);

    auto categories = xRegistry->categoriesOrdered();
    for (QString &category : categories) {
        const QString categoryName = xRegistry->friendlyNodeCategory(category);
        auto categoryItem = new QTreeWidgetItem(ui->treeWidget);
        categoryItem->setText(0, categoryName);
        categoryItem->setIcon(0, categoryIcon(category));
        categoryItem->setFlags(categoryItem->flags() & ~Qt::ItemIsSelectable);

        QList<NodeEditorRegistry::NodeContext> nodes = xRegistry->categoryNodeContexts(category);
        for (NodeEditorRegistry::NodeContext &node : nodes) {
            const QString friendlyName = node.caption;
            const QString rawName = node.name;
            QTreeWidgetItem *item = new QTreeWidgetItem(categoryItem);
            item->setText(0, friendlyName);
            item->setIcon(0, xIcon(":/res/icons/commit.svg"));
            item->setData(0, Qt::UserRole, rawName);
        }
    }

    ui->treeWidget->setDragEnabled(true);
    ui->treeWidget->expandAll();

    connect(ui->lineEditFilter,
            &QLineEdit::textChanged,
            this,
            &NodesDockWidgetContent::onFilterTextChanged);
}

NodesDockWidgetContent::~NodesDockWidgetContent()
{
    delete ui;
}

QIcon NodesDockWidgetContent::categoryIcon(const QString &category)
{
    if (category == QString("Communication")) {
        return xIcon(":/res/icons/satellite_alt.svg");
    } else if (category == QString("Input/Output")) {
        return xIcon(":/res/icons/page_info.svg");
    } else if (category == QString("DataProcess")) {
        return xIcon(":/res/icons/function.svg");
    } else if (category == QString("Scripts")) {
        return xIcon(":/res/icons/script.svg");
    } else if (category == QString("Hub")) {
        return xIcon(":/res/icons/hub.svg");
    } else if (category == QString("Plugins")) {
        return xIcon(":/res/icons/extension.svg");
    } else {
        return QIcon();
    }
    return QIcon();
}

void NodesDockWidgetContent::onFilterTextChanged(const QString &text)
{
    auto treeView = ui->treeWidget;
    QTreeWidgetItemIterator categoryIt(treeView, QTreeWidgetItemIterator::HasChildren);
    while (*categoryIt) {
        (*categoryIt++)->setHidden(true);
    }

    QTreeWidgetItemIterator it(treeView, QTreeWidgetItemIterator::NoChildren);
    while (*it) {
        auto modelName = (*it)->text(0);
        const bool match = (modelName.contains(text, Qt::CaseInsensitive));
        (*it)->setHidden(!match);
        if (match) {
            QTreeWidgetItem *parent = (*it)->parent();
            while (parent) {
                parent->setHidden(false);
                parent = parent->parent();
            }
        }
        ++it;
    }
}

} // namespace xFlow