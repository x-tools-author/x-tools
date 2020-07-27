/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKINPUTDATAITEMMANAGER_HH
#define SAKINPUTDATAITEMMANAGER_HH

#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>

#include "SAKDebugPageInputController.hh"

namespace Ui {
    class SAKInputDataPresetItemManager;
}

class SAKDebugPage;
/// @brief 预设数据管理窗口
class SAKInputDataPresetItemManager:public QWidget
{
    Q_OBJECT
public:
    SAKInputDataPresetItemManager(SAKDebugPage *debugPage, SAKDebugPageInputController *inputManager, QWidget *parent = Q_NULLPTR);
    ~SAKInputDataPresetItemManager();

    struct InputDataItemKey {
        const QString id = QString("id");
        const QString format = QString("format");
        const QString comment = QString("comment");
        const QString classify = QString("classify");
        const QString data = QString("data");
    };
private:
    SAKDebugPage *debugPage;
    SAKCRCInterface *crcInterface;
    SAKInputDataFactory *factory;
    SAKDebugPageInputController *inputManager;
    QListWidget *listWidget;
    SAKDebugPageInputController::InputParametersContext inputParameters;
    QString tableName;
    QTimer clearMessageInfoTimer;
private:
    void readinRecord();
    void outputMessage(QString msg, bool isError = false);
    bool contains(quint64 paraID);
private:
    Ui::SAKInputDataPresetItemManager *ui;
    QPushButton *deletePushButton;
    QPushButton *addPushButton;
    QPushButton *outportPushButton;
    QPushButton *importPushButton;
    QLabel *infoLabel;
private slots:
    void on_deletePushButton_clicked();
    void on_addPushButton_clicked();
    void on_outportPushButton_clicked();
    void on_importPushButton_clicked();
};

#endif
