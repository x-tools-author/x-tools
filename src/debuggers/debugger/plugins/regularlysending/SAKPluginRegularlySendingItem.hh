/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKPLUGINREGULARLYSENDINGITEM_HH
#define SAKPLUGINREGULARLYSENDINGITEM_HH

#include <QTimer>
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

namespace Ui {
    class SAKPluginRegularlySendingItem;
}
class SAKDebugger;
/// @brief Timing sent item
class SAKPluginRegularlySendingItem:public QWidget
{
    Q_OBJECT
public:
    SAKPluginRegularlySendingItem(SAKDebugger *mDebugPage, QWidget *parent = Q_NULLPTR);
    SAKPluginRegularlySendingItem(SAKDebugger *mDebugPage,
                           quint64 mID,
                           quint32 interval,
                           quint32 format,
                           QString description,
                           QString text,
                           QWidget *parent = Q_NULLPTR);
    ~SAKPluginRegularlySendingItem();

    quint64 itemID();
    quint32 itemInterval();
    quint32 itemFormat();
    QString itemDescription();
    QString itemText();
private:
    SAKDebugger *mDebugPage;
    quint64 mID;
    QTimer mWriteTimer;
    bool isInitializing;
private:
    void write();
    void commonInitializing();
private:
    Ui::SAKPluginRegularlySendingItem *mUi;
    QCheckBox *mEnableCheckBox;
    QLineEdit *mIntervalLineEdit;
    QComboBox *mTextFormatComboBox;
    QLineEdit *mDescriptionLineEdit;
    QTextEdit *mInputDataTextEdit;
private slots:
    void on_enableCheckBox_clicked();
    void on_intervalLineEdit_textChanged(const QString &text);
    void on_textFormatComboBox_currentTextChanged(const QString &text);
    void on_descriptionLineEdit_textChanged(const QString &text);
    void on_inputDataTextEdit_textChanged();
signals:
    void intervalChanged(int interval);
    void formatChanged(int format);
    void descriptionChanged(QString description);
    void inputTextChanged(QString text);
};

#endif
