/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERTIMINGSENTITEM_HH
#define SAKOTHERTIMINGSENTITEM_HH

#include <QTimer>
#include <QRegExp>
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QRegExpValidator>

namespace Ui {
    class SAKOtherTimingSentItem;
}
class SAKDebugPage;
class SAKDebugPageCommonDatabaseInterface;
/// @brief Timing sent item
class SAKOtherTimingSentItem:public QWidget
{
    Q_OBJECT
public:
    SAKOtherTimingSentItem(SAKDebugPage *mDebugPage, QWidget *parent = Q_NULLPTR);
    SAKOtherTimingSentItem(SAKDebugPage *mDebugPage,
                           quint64 mID,
                           quint32 interval,
                           quint32 format,
                           QString comment,
                           QString data,
                           QWidget *parent = Q_NULLPTR);
    ~SAKOtherTimingSentItem();

    quint64 itemID();
    quint32 itemInterval();
    quint32 itemFormat();
    QString itemDescription();
    QString itemText();
private:
    SAKDebugPage *mDebugPage;
    quint64 mID;
    QTimer mWriteTimer;
    SAKDebugPageCommonDatabaseInterface *mDatabaseInterface;
private:
    void write();
    void commonInitializing();
private:
    Ui::SAKOtherTimingSentItem *mUi;
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
};

#endif
