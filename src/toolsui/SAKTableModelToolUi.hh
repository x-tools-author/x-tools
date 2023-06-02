/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTABLEVIEWWITHCONTROLLER_HH
#define SAKTABLEVIEWWITHCONTROLLER_HH

#include <QMenu>
#include <QWidget>
#include <QJsonObject>
#include <QModelIndex>
#include <QActionGroup>
#include <QLoggingCategory>
#include <QAbstractTableModel>

namespace Ui {
class SAKTableViewWithController;
}

class SAKTableViewWithController : public QWidget
{
    Q_OBJECT
public:
    explicit SAKTableViewWithController(const char *lg,
                                        QWidget *parent = nullptr);
    ~SAKTableViewWithController();

    void initialize(QAbstractTableModel *tableModel,
                    const QString &settingGroup);

    void setStretchSections(QList<int>columns);

protected:
    const QLoggingCategory mLoggingCategory;
    QAbstractTableModel *mTableModel{nullptr};

    virtual void edit(const QModelIndex &index) = 0;
    virtual void clear() = 0;
    virtual void remove(const QModelIndex &index) = 0;
    virtual void importFromJson(const QByteArray &json) = 0;
    virtual QByteArray exportAsJson() = 0;
    virtual bool append() = 0;

private:
    QMenu *mMenu{nullptr};
    QString mItemsKey;

private:
    QModelIndex currentIndex();
    void writeToSettingsFile();

private:
    Ui::SAKTableViewWithController *ui{nullptr};

    void onPushButtonEditClicked();
    void onPushButtonClearClicked();
    void onPushButtonDeleteClicked();
    void onPushButtonImportClicked();
    void onPushButtonExportClicked();
    void onPushButtonAppendClicked();
};

#endif // SAKTABLEVIEWWITHCONTROLLER_H
