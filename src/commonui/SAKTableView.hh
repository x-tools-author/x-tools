/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTABLEVIEW_HH
#define SAKTABLEVIEW_HH

#include <QMenu>
#include <QWidget>
#include <QModelIndex>
#include <QActionGroup>
#include <QLoggingCategory>
#include <QAbstractTableModel>

namespace Ui {
class SAKTableView;
}

class SAKTableView : public QWidget
{
    Q_OBJECT
public:
    explicit SAKTableView(const char *lg, QWidget *parent = nullptr);
    ~SAKTableView();

    void setupTableModel(QAbstractTableModel *tableModel);

protected:
    const QLoggingCategory mLoggingCategory;
    QAbstractTableModel *mTableModel{nullptr};

    virtual void edit(const QModelIndex &index) = 0;
    virtual void clear() = 0;
    virtual void remove(const QModelIndex &index) = 0;
    virtual void importFromFile(const QString &fileName) = 0;
    virtual void exportToFile(const QString &fineName) = 0;
    virtual void append() = 0;

private:
    QMenu *mMenu{nullptr};

private:
    QModelIndex currentIndex();

private:
    Ui::SAKTableView *ui{nullptr};

    void onPushButtonEditClicked();
    void onPushButtonClearClicked();
    void onPushButtonDeleteClicked();
    void onPushButtonImportClicked();
    void onPushButtonExportClicked();
    void onPushButtonAppendClicked();
};

#endif // SAKTABLEVIEW_HH
