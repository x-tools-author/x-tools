/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKHighlightSettings.hh"
#include "SAKHighlightSettingsWidget.hh"

#include "ui_SAKHighlightSettingsWidget.h"

SAKHighlightSettingsWidget::SAKHighlightSettingsWidget(QTextDocument *doc, QWidget* parent)
    :QWidget (parent)
    ,mUi (new Ui::SAKHighlightSettingsWidget)
{
    mUi->setupUi(this);
    mLabelLayout = new QGridLayout(mUi->frame);
    mUi->frame->setLayout(mLabelLayout);

    mHighlighter = new SAKHighlightSettings(doc);
    mInputLineEdit = mUi->lineEdit;
    mClearLabelBt = mUi->pushButtonClear;
    mAddLabelBt = mUi->pushButtonAdd;

    mInputLineEdit->installEventFilter(this);

    connect(mClearLabelBt, &QPushButton::clicked, this, &SAKHighlightSettingsWidget::clearLabel);
    connect(mAddLabelBt, &QPushButton::clicked, this, &SAKHighlightSettingsWidget::addLabelFromInput);
}

SAKHighlightSettingsWidget::~SAKHighlightSettingsWidget()
{
    delete mUi;
}

void SAKHighlightSettingsWidget::addLabel(QString str)
{
    if (str.isEmpty()){
        return;
    }

    QStringList keyWords;
    for (int i = 0; i < mLabelList.length(); i++){
        QString temp = mLabelList.at(i)->text();
        /// 标签重复不处理
        if (temp.compare(str) == 0){
            return;
        }
        keyWords.append(temp);
    }


    QPushButton* tempLabel = new QPushButton(str);
    tempLabel->installEventFilter(this);
    mLabelList.append(tempLabel);

    resetLabelViewer();

    keyWords.append(str);
    resetHighlightKeyword(keyWords);
}

void SAKHighlightSettingsWidget::addLabelFromInput()
{
    QString str = mInputLineEdit->text();
    addLabel(str);
}

void SAKHighlightSettingsWidget::deleteLabel(QPushButton *bt)
{
    for (int i= 0; i < mLabelList.length(); i++){
        if (mLabelList.at(i) == bt){
            QPushButton *BtTemp = mLabelList.takeAt(i);
            BtTemp->deleteLater();
            break;
        }
    }

    resetLabelViewer();

    QStringList keyWords;
    for (int i = 0; i < mLabelList.length(); i++){
        keyWords.append(mLabelList.at(i)->text());
    }    

    resetHighlightKeyword(keyWords);
}

bool SAKHighlightSettingsWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick){
        for (int i = 0; i < mLabelList.length(); i++){
            if (mLabelList.at(i) == watched){
                deleteLabel(mLabelList.at(i));
                return true;
            }
        }
    }

    if (event->type() == QEvent::KeyPress){
        QKeyEvent* e = dynamic_cast<QKeyEvent*>(event);
        if ((e->key() == Qt::Key_Enter) || e->key() == Qt::Key_Return){
            addLabel(mInputLineEdit->text());
            mInputLineEdit->clear();
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void SAKHighlightSettingsWidget::clearLabel()
{
    QPushButton *bt = Q_NULLPTR;
    while (!mLabelList.isEmpty()) {
        bt = mLabelList.takeFirst();
        bt->deleteLater();
    }

    resetHighlightKeyword(QStringList());
}

void SAKHighlightSettingsWidget::resetLabelViewer()
{
    for (int index = 0; index < mLabelList.count(); index++){
        mLabelLayout->addWidget(mLabelList.at(index), index/5, index%5);
    }
}

void SAKHighlightSettingsWidget::resetHighlightKeyword(QStringList keyWords)
{
    mHighlighter->setHighlighterKeyWord(keyWords);
    mHighlighter->rehighlight();
}
