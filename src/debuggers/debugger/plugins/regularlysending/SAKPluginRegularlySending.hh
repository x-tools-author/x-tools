/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKPLUGINREGULARLYSENDING_HH
#define SAKPLUGINREGULARLYSENDING_HH

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QListWidget>
#include <QPushButton>

namespace Ui {
    class SAKPluginRegularlySending;
}


/// @brief Timing sent item manager
class SAKPluginRegularlySending:public QWidget
{
    Q_OBJECT
public:
    SAKPluginRegularlySending(QWidget *parent = Q_NULLPTR);
    ~SAKPluginRegularlySending();

    struct TimingSendingItemKey {
        const QString id = QString("id");
        const QString interval = QString("interval");
        const QString format = QString("format");
        const QString description = QString("description");
        const QString text = QString("text");
    };
private:
    QString mTableName;
    QTimer mClearMessageTimer;
private:
    void readinRecord();
    bool contains(quint64 paraID);
    void outputMessage(QString msg, bool isError = false);
    // update record
    void changeInterval(int interval);
    void changeFormat(int format);
    void changeDescription(QString description);
    void changeInputText(QString text);
private:
    Ui::SAKPluginRegularlySending *mUi;
    QListWidget *mItemListWidget;
    QPushButton *mOutportPushButton;
    QPushButton *mImportPushButton;
    QPushButton *mDeletePushButton;
    QPushButton *mAddPushButton;
    QLabel *mMessageLabel;
private slots:
    void on_outportPushButton_clicked();
    void on_importPushButton_clicked();
    void on_deletePushButton_clicked();
    void on_addPushButton_clicked();
};

#endif
