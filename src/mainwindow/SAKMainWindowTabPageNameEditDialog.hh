/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMAINWINDOWTABPAGENAMEEDITDIALOG_HH
#define SAKMAINWINDOWTABPAGENAMEEDITDIALOG_HH

#include <QDialog>
#include <QLineEdit>

namespace Ui {
    class SAKMainWindowTabPageNameEditDialog;
}
/// @brief Editing the name of tab page
class SAKMainWindowTabPageNameEditDialog : public QDialog
{
    Q_OBJECT
public:
    SAKMainWindowTabPageNameEditDialog(QWidget *parent = Q_NULLPTR);
    ~SAKMainWindowTabPageNameEditDialog();

    /**
     * @brief setName: Set the tab page name
     * @param name: Tab page name
     */
    void setName(const QString &name);

    /**
     * @brief name: Get the tab page name
     * @return The tab page name
     */
    QString name();
private:
    Ui::SAKMainWindowTabPageNameEditDialog *mUi;
    QLineEdit *mTabPageNameLineEdit;
};

#endif
