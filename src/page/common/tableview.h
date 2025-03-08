/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QTableView>
#include <QWidget>

namespace Ui {
class TableView;
}

class TableModel;
class TextItemEditor;
class TableView;
class TableView : public QWidget
{
    Q_OBJECT
public:
    TableView(QWidget *parent = nullptr);
    ~TableView() override;

    bool isDisableAll() const;
    void setIsDisableAll(bool disable);
    void setIdDisableCheckBoxVisible(bool visible);

    TableModel *tableModel();
    void setTableModel(TableModel *model);
    QTableView *tableView() const;
    virtual QVariantMap save() const;
    virtual void load(const QVariantMap &parameters);

    virtual void inputBytes(const QByteArray &bytes);
signals:
    void outputBytes(const QByteArray &bytes);

protected:
    void onPushButtonClearClicked();
    void onPushButtonDeleteClicked();
    void onPushButtonImportClicked();
    void onPushButtonExportClicked();
    void onPushButtonAddClicked();
    void onCellDoubleClicked(const QModelIndex &index);

    virtual QList<int> textItemColumns() const;

private:
    Ui::TableView *ui;
    TableModel *m_model{nullptr};
    TextItemEditor *m_editor{nullptr};
};