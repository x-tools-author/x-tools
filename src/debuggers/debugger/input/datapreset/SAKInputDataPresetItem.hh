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
#include <QPushButton>

namespace Ui {
    class SAKInputDataPresetItem;
}

// Preset data parameters editting widget
class SAKInputDataPresetItem:public QWidget
{
    Q_OBJECT
public:
    SAKInputDataPresetItem(QWidget *parent = Q_NULLPTR);
    SAKInputDataPresetItem(quint64 id,
                           quint32 format,
                           QString description,
                           QString text,
                           QWidget *parent = Q_NULLPTR);
    ~SAKInputDataPresetItem();

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
    bool mInitializing;
private:
    void initializingItem();
private:
    Ui::SAKInputDataPresetItem *mUi;
    QComboBox *mTextFormatComboBox;
    QLineEdit *mDescriptionLineEdit;
    QTextEdit *mInputTextEdit;
private slots:
    void on_textFormatComboBox_currentTextChanged(const QString &text);
    void on_descriptionLineEdit_textChanged(const QString &text);
    void on_inputTextEdit_textChanged();
signals:
    void formatChanged(int format);
    void descriptionChanged(const QString &text);
    void textChanged(QString text);
};

#endif
