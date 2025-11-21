/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QWidget>

namespace Ui {
class xFlow;
}

namespace xFlow {

class NodesDockWidgetContent;
class NavigatorDockWidgetContent;
class xFlow : public QWidget
{
    Q_OBJECT
public:
    explicit xFlow(QWidget *parent = nullptr);
    ~xFlow() override;

    QJsonObject save();
    void load(const QJsonObject &obj);

private:
    Ui::xFlow *ui;
    NodesDockWidgetContent *m_nodes{nullptr};
    NavigatorDockWidgetContent *m_navigator{nullptr};

private:
    void onThemeChanged();

    QJsonObject cookedGraphicsViewStyle(const QJsonObject &style);
    QJsonObject cookedNodeStyle(const QJsonObject &style);
    QJsonObject cookedConnectionStyle(const QJsonObject &style);
};

} // namespace xFlow