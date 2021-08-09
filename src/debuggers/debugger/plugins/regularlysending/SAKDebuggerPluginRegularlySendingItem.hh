/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERPLUGINREGULARLYSENDINGITEM_HH
#define SAKDEBUGGERPLUGINREGULARLYSENDINGITEM_HH

#include <QTimer>
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

namespace Ui {
    class SAKDebuggerPluginRegularlySendingItem;
}

class SAKDebuggerPluginRegularlySendingItem:public QWidget
{
    Q_OBJECT
public:
    struct SAKStructItemContext {
        quint64 id;
        int interval;
        int format;
        QString description;
        QString data;
    };
public:
    SAKDebuggerPluginRegularlySendingItem(QWidget *parent = Q_NULLPTR);
    SAKDebuggerPluginRegularlySendingItem(SAKStructItemContext ctx,
                                          QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerPluginRegularlySendingItem();

    quint64 itemID();
    quint32 itemInterval();
    quint32 itemFormat();
    QString itemDescription();
    QString itemText();
    SAKStructItemContext context();
private:
    quint64 mID;
    QTimer mWriteTimer;
    bool isInitializing;
    SAKStructItemContext mContext;
private:
    void write();
    void commonInitializing();
private:
    Ui::SAKDebuggerPluginRegularlySendingItem *mUi;
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
    void intervalChanged(quint64 id, int interval);
    void formatChanged(quint64 id, int format);
    void descriptionChanged(quint64 id, QString description);
    void inputTextChanged(quint64 id, QString text);
    void invokeWriteBytes(QString bytes, int format);
};

#endif
