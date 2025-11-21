/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QWidget>

namespace Ui {
class BaseNodeUi;
}

class BaseNode;
class BaseNodeUi : public QWidget
{
    Q_OBJECT
public:
    BaseNodeUi(BaseNode *node, QWidget *parent = nullptr);
    ~BaseNodeUi() override;
    void setEmbeddedWidget(QWidget *widget);
    void setMessage(const QString &message);

    virtual QJsonObject save() const;
    virtual void load(const QJsonObject &parameters);

protected:
    BaseNode *m_node;

private:
    void onExpandButtonClicked();

private:
    Ui::BaseNodeUi *ui;
    QWidget *m_innerWidget;
};
