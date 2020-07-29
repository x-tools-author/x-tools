/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKOtherHighlighter.hh"
#include "SAKOtherHighlighterManager.hh"

#include "ui_SAKOtherHighlighterManager.h"

SAKOtherHighlighterManager::SAKOtherHighlighterManager(QTextDocument *doc, QWidget* parent)
    :QWidget (parent)
    ,mUi (new Ui::SAKOtherHighlighterManager)
{
    mUi->setupUi(this);
    mLabelLayout = new QGridLayout(mUi->frame);
    mUi->frame->setLayout(mLabelLayout);

    mHighlighter = new SAKOtherHighlighter(doc);
    mInputLineEdit = mUi->lineEdit;
    mClearLabelBt = mUi->pushButtonClear;
    mAddLabelBt = mUi->pushButtonAdd;

    mInputLineEdit->installEventFilter(this);

    connect(mClearLabelBt, &QPushButton::clicked, this, &SAKOtherHighlighterManager::clearLabel);
    connect(mAddLabelBt, &QPushButton::clicked, this, &SAKOtherHighlighterManager::addLabelFromInput);
}

SAKOtherHighlighterManager::~SAKOtherHighlighterManager()
{
    delete mUi;
}

void SAKOtherHighlighterManager::addLabel(QString str)
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

void SAKOtherHighlighterManager::addLabelFromInput()
{
    QString str = mInputLineEdit->text();
    addLabel(str);
}

void SAKOtherHighlighterManager::deleteLabel(QPushButton *bt)
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

bool SAKOtherHighlighterManager::eventFilter(QObject *watched, QEvent *event)
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

void SAKOtherHighlighterManager::clearLabel()
{
    QPushButton *bt = Q_NULLPTR;
    while (!mLabelList.isEmpty()) {
        bt = mLabelList.takeFirst();
        bt->deleteLater();
    }

    resetHighlightKeyword(QStringList());
}

void SAKOtherHighlighterManager::resetLabelViewer()
{
    for (int index = 0; index < mLabelList.count(); index++){
        mLabelLayout->addWidget(mLabelList.at(index), index/5, index%5);
    }
}

void SAKOtherHighlighterManager::resetHighlightKeyword(QStringList keyWords)
{
    mHighlighter->setHighlighterKeyWord(keyWords);
    mHighlighter->rehighlight();
}
