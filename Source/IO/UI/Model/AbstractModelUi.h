/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>
#include <QActionGroup>
#include <QJsonObject>
#include <QMenu>
#include <QModelIndex>
#include <QWidget>

#include "../AbstractIOUi.h"

namespace Ui {
class AbstractModelUi;
}

namespace xTools {

class AbstractModel;
class AbstractModelUi : public AbstractIOUi
{
    Q_OBJECT
public:
    explicit AbstractModelUi(QWidget *parent = nullptr);
    ~AbstractModelUi();
    void setStretchSections(QList<int> columns);
    void setSectionResizeModeToStretch();
    void setColumnVisible(int column, bool visible);

protected:
    virtual void onBaseToolUiInitialized(AbstractIO *tool, const QString &settingGroup);
    virtual QList<int> defaultHideColumns();
    virtual void afterRowEdited(int row);
    virtual QDialog *itemEditor() = 0;

protected:
    QAbstractTableModel *mTableModel{nullptr};
    AbstractModel *m_TableModelTool{nullptr};

private:
    void clear();
    void remove(const QModelIndex &index);
    void importFromJson(const QByteArray &json);
    QByteArray exportAsJson();
    void edit(const QModelIndex &index);
    bool append();

private:
    QMenu *m_menu{nullptr};
    QString mItemsKey;

private:
    QModelIndex currentIndex();
    void writeToSettingsFile();
    bool isInitialized();

private:
    Ui::AbstractModelUi *ui{nullptr};

    void onPushButtonEditClicked();
    void onPushButtonClearClicked();
    void onPushButtonDeleteClicked();
    void onPushButtonImportClicked();
    void onPushButtonExportClicked();
    void onPushButtonAppendClicked();
};

} // namespace xTools
