/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKDebuggerOutputHighlighter.hh"
#include "ui_SAKDebuggerOutputHighlighter.h"

SAKDebuggerOutputHighlighter:: SAKDebuggerOutputHighlighter(QTextDocument *doc,
                                                            QWidget* parent)
    :QDialog(parent)
    ,mUi(new Ui::SAKDebuggerOutputHighlighter)
{
    mUi->setupUi(this);
    mLabelLayout = new QGridLayout(mUi->frame);
    mUi->frame->setLayout(mLabelLayout);

    mHighlighter = new Highlighter(doc);
    mInputLineEdit = mUi->lineEdit;
    mClearLabelBt = mUi->pushButtonClear;
    mAddLabelBt = mUi->pushButtonAdd;

    mInputLineEdit->installEventFilter(this);

    connect(mClearLabelBt, &QPushButton::clicked,
            this, & SAKDebuggerOutputHighlighter::clearLabel);
    connect(mAddLabelBt, &QPushButton::clicked,
            this, & SAKDebuggerOutputHighlighter::addLabelFromInput);
    setModal(true);
}

SAKDebuggerOutputHighlighter::~SAKDebuggerOutputHighlighter()
{
    delete mUi;
}

SAKDebuggerOutputHighlighter::Highlighter::Highlighter(QTextDocument* parent)
    :QSyntaxHighlighter (parent)
{
    setHighlighterKeyWord(QStringList());
}

void SAKDebuggerOutputHighlighter::Highlighter::setHighlighterKeyWord(
        QStringList keyWords
        )
{
    mHighlightingRules.clear();

    HighlightingRule rule;

    mKeywordFormat.setForeground(Qt::red);
    mKeywordFormat.setFontWeight(QFont::Normal);

    foreach (const QString &pattern, keyWords) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = mKeywordFormat;
        mHighlightingRules.append(rule);
    }

    mMultiLineCommentFormat.setForeground(Qt::red);

    mCommentStartExpression = QRegularExpression("/\\*");
    mCommentEndExpression = QRegularExpression("\\*/");
}

void SAKDebuggerOutputHighlighter::Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, mHighlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1){
        startIndex = text.indexOf(mCommentStartExpression);
    }

    while (startIndex >= 0) {
        QRegularExpressionMatch match = mCommentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, mMultiLineCommentFormat);
        startIndex = text.indexOf(mCommentStartExpression, startIndex + commentLength);
    }
}

void SAKDebuggerOutputHighlighter::addLabel(QString str)
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

void SAKDebuggerOutputHighlighter::addLabelFromInput()
{
    QString str = mInputLineEdit->text();
    addLabel(str);
}

void SAKDebuggerOutputHighlighter::deleteLabel(QPushButton *bt)
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

bool SAKDebuggerOutputHighlighter::eventFilter(QObject *watched, QEvent *event)
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

void SAKDebuggerOutputHighlighter::clearLabel()
{
    QPushButton *bt = Q_NULLPTR;
    while (!mLabelList.isEmpty()) {
        bt = mLabelList.takeFirst();
        bt->deleteLater();
    }

    resetHighlightKeyword(QStringList());
}

void SAKDebuggerOutputHighlighter::resetLabelViewer()
{
    for (int index = 0; index < mLabelList.count(); index++){
        mLabelLayout->addWidget(mLabelList.at(index), index/5, index%5);
    }
}

void SAKDebuggerOutputHighlighter::resetHighlightKeyword(QStringList keyWords)
{
    mHighlighter->setHighlighterKeyWord(keyWords);
    mHighlighter->rehighlight();
}
