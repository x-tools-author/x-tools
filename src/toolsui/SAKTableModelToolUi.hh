/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTABLEMODELTOOLUI_HH
#define SAKTABLEMODELTOOLUI_HH

#include <QMenu>
#include <QWidget>
#include <QJsonObject>
#include <QModelIndex>
#include <QActionGroup>
#include <QLoggingCategory>
#include <QAbstractTableModel>

#include "SAKBaseToolUi.hh"

namespace Ui {
class SAKTableModelToolUi;
}

class SAKTableModelTool;
class SAKTableModelToolUi : public SAKBaseToolUi
{
    Q_OBJECT
public:
    explicit SAKTableModelToolUi(const char *lg, QWidget *parent = nullptr);
    ~SAKTableModelToolUi();
    void setStretchSections(QList<int>columns);
    void setSectionResizeModeToStretch();
    void setColumnVisible(int column, bool visible);

protected:
    virtual void onBaseToolUiInitialized(SAKBaseTool *tool,
                                         const QString &settingGroup) override;
    virtual QList<int> defaultHideColumns();
    virtual void afterRowEdited(int row);
    virtual QDialog *itemEditor() = 0;

protected:
    const QLoggingCategory mLoggingCategory;
    QAbstractTableModel *mTableModel{nullptr};
    SAKTableModelTool *mTableModelTool{nullptr};

private:
    void clear();
    void remove(const QModelIndex &index);
    void importFromJson(const QByteArray &json);
    QByteArray exportAsJson();
    void edit(const QModelIndex &index);
    bool append();

private:
    QMenu *mMenu{nullptr};
    QString mItemsKey;

private:
    QModelIndex currentIndex();
    void writeToSettingsFile();
    bool isInitialized();

private:
    Ui::SAKTableModelToolUi *ui{nullptr};

    void onPushButtonEditClicked();
    void onPushButtonClearClicked();
    void onPushButtonDeleteClicked();
    void onPushButtonImportClicked();
    void onPushButtonExportClicked();
    void onPushButtonAppendClicked();
};

#endif // SAKTABLEMODELTOOLUI_H
