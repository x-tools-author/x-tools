/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKDebuggerOutputHighlighter.hh"
#include "ui_SAKDebuggerOutputHighlighter.h"

SAKDebuggerOutputHighlighter:: SAKDebuggerOutputHighlighter(QTextDocument *doc,
                                                            QWidget* parent)
    :QWidget (parent)
    ,ui (new Ui::SAKDebuggerOutputHighlighter)
{
    ui->setupUi(this);
    m_labelLayout = new QGridLayout(ui->frame);
    ui->frame->setLayout(m_labelLayout);

    m_highlighter = new Highlighter(doc);
    m_inputLineEdit = ui->lineEdit;
    m_clearLabelBt = ui->pushButtonClear;
    m_addLabelBt = ui->pushButtonAdd;

    m_inputLineEdit->installEventFilter(this);

    connect(m_clearLabelBt, &QPushButton::clicked,
            this, & SAKDebuggerOutputHighlighter::clearLabel);
    connect(m_addLabelBt, &QPushButton::clicked,
            this, & SAKDebuggerOutputHighlighter::addLabelFromInput);
}

SAKDebuggerOutputHighlighter::~SAKDebuggerOutputHighlighter()
{
    delete ui;
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
    for (int i = 0; i < m_labelList.length(); i++){
        QString temp = m_labelList.at(i)->text();
        /// 标签重复不处理
        if (temp.compare(str) == 0){
            return;
        }
        keyWords.append(temp);
    }


    QPushButton* tempLabel = new QPushButton(str);
    tempLabel->installEventFilter(this);
    m_labelList.append(tempLabel);

    resetLabelViewer();

    keyWords.append(str);
    resetHighlightKeyword(keyWords);
}

void SAKDebuggerOutputHighlighter::addLabelFromInput()
{
    QString str = m_inputLineEdit->text();
    addLabel(str);
}

void SAKDebuggerOutputHighlighter::deleteLabel(QPushButton *bt)
{
    for (int i= 0; i < m_labelList.length(); i++){
        if (m_labelList.at(i) == bt){
            QPushButton *BtTemp = m_labelList.takeAt(i);
            BtTemp->deleteLater();
            break;
        }
    }

    resetLabelViewer();

    QStringList keyWords;
    for (int i = 0; i < m_labelList.length(); i++){
        keyWords.append(m_labelList.at(i)->text());
    }    

    resetHighlightKeyword(keyWords);
}

bool SAKDebuggerOutputHighlighter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick){
        for (int i = 0; i < m_labelList.length(); i++){
            if (m_labelList.at(i) == watched){
                deleteLabel(m_labelList.at(i));
                return true;
            }
        }
    }

    if (event->type() == QEvent::KeyPress){
        QKeyEvent* e = dynamic_cast<QKeyEvent*>(event);
        if ((e->key() == Qt::Key_Enter) || e->key() == Qt::Key_Return){
            addLabel(m_inputLineEdit->text());
            m_inputLineEdit->clear();
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void SAKDebuggerOutputHighlighter::clearLabel()
{
    QPushButton *bt = Q_NULLPTR;
    while (!m_labelList.isEmpty()) {
        bt = m_labelList.takeFirst();
        bt->deleteLater();
    }

    resetHighlightKeyword(QStringList());
}

void SAKDebuggerOutputHighlighter::resetLabelViewer()
{
    for (int index = 0; index < m_labelList.count(); index++){
        m_labelLayout->addWidget(m_labelList.at(index), index/5, index%5);
    }
}

void SAKDebuggerOutputHighlighter::resetHighlightKeyword(QStringList keyWords)
{
    m_highlighter->setHighlighterKeyWord(keyWords);
    m_highlighter->rehighlight();
}
