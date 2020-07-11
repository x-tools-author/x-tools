/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKINPUTDATAITEM_HH
#define SAKINPUTDATAITEM_HH

#include <QTimer>
#include <QWidget>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

#include "SAKDebugPageInputManager.hh"

namespace Ui {
    class SAKInputDataItem;
}

class SAKDebugPage;
class SAKCRCInterface;
class SAKInputDataFactory;
class SAKDebugPageInputManager;
class SAKInputDataItem:public QWidget
{
    Q_OBJECT
public:
    SAKInputDataItem(SAKDebugPage *debugPage, SAKDebugPageInputManager *inputManager, QWidget *parent = Q_NULLPTR);
    SAKInputDataItem(quint64 id,
                     quint32 format,
                     QString comment,
                     quint32 classify,
                     QString data,
                     SAKDebugPage *debugPage,
                     SAKDebugPageInputManager *inputManager,
                     QWidget *parent = Q_NULLPTR);
    ~SAKInputDataItem();

    quint64 parameterID();
    quint32 parameterFormat();
    QString parameterComment();
    quint32 parameterClassify();
    QString parameterData();
private:
    QPushButton *menuPushButton;
    QAction *action;
    SAKDebugPage *debugPage;
    SAKDebugPageInputManager *inputManager;
    SAKDebugPageInputManager::InputParameters inputParameters;
    quint64 id;
private:
    void addDataAction(QPushButton *menuPushButton);
    void removeDataAction(QPushButton *menuPushButton);
    void updateActionTitle(const QString &title);
    void updateTextFormat();
    void sendRawData();
    void initUi();
signals:
    void rawDataChanged(QString rawData, SAKDebugPageInputManager::InputParameters parameters);
private:
    Ui::SAKInputDataItem *ui;
    QComboBox *textFormatComboBox;
    QComboBox *classifyComboBox;
    QLineEdit *descriptionLineEdit;
    QTextEdit *inputDataTextEdit;
    QPushButton *updatePushButton;
private slots:
    void on_updatePushButton_clicked();
};

#endif
