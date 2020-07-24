/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKINPUTDATAPRESETITEM_HH
#define SAKINPUTDATAPRESETITEM_HH

#include <QTimer>
#include <QWidget>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QSqlQuery>
#include <QPushButton>
#include <QSqlDatabase>

namespace Ui {
    class SAKInputDataPresetItem;
}

class SAKDebugPage;
// Preset data parameters editting widget
class SAKInputDataPresetItem:public QWidget
{
    Q_OBJECT
public:
    SAKInputDataPresetItem(int pageType, QSqlDatabase *sqlDatabase = Q_NULLPTR, QWidget *parent = Q_NULLPTR);
    SAKInputDataPresetItem(quint64 itemID,
                           quint32 format,
                           QString comment,
                           QString text,
                           int pageType,
                           QSqlDatabase *sqlDatabase = Q_NULLPTR,
                           QWidget *parent = Q_NULLPTR);
    ~SAKInputDataPresetItem();

    struct DatabaseColumns {
        const QString id = QString("ID");
        const QString format = QString("Format");
        const QString description = QString("Description");
        const QString text = QString("text");
    };

    /**
     * @brief itemID: Get the id of the item
     * @return The id of the item
     */
    quint64 itemID();

    /**
     * @brief itemDescription: Get the description of the item
     * @return The description of the item
     */
    QString itemDescription();

    /**
     * @brief itemText: Get the text of the item
     * @return The text of the item
     */
    QString itemText();

    /**
     * @brief itemTextFromat: Get the text format of the item
     * @return The text format of the item
     */
    int itemTextFromat();
private:
    quint64 mItemID;
    int mPageType;
    QSqlDatabase *mSqlDatabase;
    QSqlQuery *mSqlQuery;
    QString mTableName;
private:
    void initializeVariable();
private:
    Ui::SAKInputDataPresetItem *mUi;
    QComboBox *mTextFormatComboBox;
    QLineEdit *mDescriptionLineEdit;
    QTextEdit *mInputTextEdit;
private slots:
    void on_textFormatComboBox_currentTextChanged(const QString &text);
    void on_descriptionLineEdit_currentTextChanged(const QString &text);
    void on_inputTextEdit_currentTextChanged(const QString &text);
signals:
    void descriptionChanged(const QString &text);
};

#endif
