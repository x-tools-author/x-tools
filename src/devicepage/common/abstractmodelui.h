/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>
#include <QActionGroup>
#include <QCheckBox>
#include <QJsonObject>
#include <QMenu>
#include <QModelIndex>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QWidget>

#include "devicepage/common/abstractioui.h"

namespace Ui {
class AbstractModelUi;
}

class TextItemEditor;
class AbstractModelIO;
class AbstractModelUi : public AbstractIOUi
{
    Q_OBJECT
public:
    explicit AbstractModelUi(QWidget *parent = nullptr);
    ~AbstractModelUi();

    QTableView *tableView() const;
    void addCustomWidget(QWidget *w);

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void setupIO(AbstractIO *io) override;

protected:
    QAbstractTableModel *m_model{nullptr};
    AbstractModelIO *m_io{nullptr};

protected:
    virtual QList<int> textItemColumns() const;
    virtual QStyledItemDelegate *createItemDelegate() const;

    void setDisableCheckBoxVisible(bool visible);
    QCheckBox *disableCheckBox() const;

private:
    Ui::AbstractModelUi *ui{nullptr};
    TextItemEditor *m_editor{nullptr};

private:
    void onPushButtonClearClicked();
    void onPushButtonDeleteClicked();
    void onPushButtonImportClicked();
    void onPushButtonExportClicked();
    void onPushButtonAddClicked();

    void onCellDoubleClicked(const QModelIndex &index);
};
