/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QDockWidget>
#include <QTextBrowser>

namespace Ui {
class NodesDockWidgetContent;
}

namespace xFlow {

class NodeEditorView;
class NodesDockWidgetContent : public QWidget
{
    Q_OBJECT
public:
    NodesDockWidgetContent(NodeEditorView *view, QWidget *parent = nullptr);
    ~NodesDockWidgetContent() override;

private:
    QIcon categoryIcon(const QString &category);
    void onFilterTextChanged(const QString &text);

private:
    Ui::NodesDockWidgetContent *ui;
};

} // namespace xFlow