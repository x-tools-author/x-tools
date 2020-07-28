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
/// @brief Timing sent item
class SAKOtherTimingSentItem:public QWidget
{
    Q_OBJECT
public:
    SAKOtherTimingSentItem(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    SAKOtherTimingSentItem(SAKDebugPage *debugPage,
                           quint64 id,
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
    SAKDebugPage *debugPage;
    quint64 id;
    QTimer writeTimer;
private:
    void write();
    void initUi();
private:
    Ui::SAKOtherTimingSentItem *ui;
    QCheckBox *timingCheckBox;
    QLineEdit *timingTimeLineEdit;
    QComboBox *textFormatComboBox;
    QLineEdit *descriptionLineEdit;
    QTextEdit *inputDataTextEdit;
    QPushButton *updatePushButton;
private slots:
    void on_timingCheckBox_clicked();
    void on_timingTimeLineEdit_textChanged(const QString &text);
    void on_updatePushButton_clicked();
};

#endif
