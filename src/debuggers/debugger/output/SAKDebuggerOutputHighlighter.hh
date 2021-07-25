/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGEROUTPUTHIGHLIGHTER_HH
#define SAKDEBUGGEROUTPUTHIGHLIGHTER_HH

#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QTextDocument>
#include <QTextDocument>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

namespace Ui {
    class SAKDebuggerOutputHighlighter;
}

/// @brief Syntax highlighter manager
class  SAKDebuggerOutputHighlighter:public QDialog
{
    Q_OBJECT
public:
     SAKDebuggerOutputHighlighter(QTextDocument *doc, QWidget* parent = Q_NULLPTR);
    ~ SAKDebuggerOutputHighlighter();

     class Highlighter : public QSyntaxHighlighter
     {
     public:
         Highlighter(QTextDocument* parent = Q_NULLPTR);

         /**
          * @brief setHighlighterKeyWord: Set highligh key word
          * @param keyWords: Key word
          */
         void setHighlighterKeyWord(QStringList keyWords);
     private:
         struct HighlightingRule{
             QRegularExpression pattern;
             QTextCharFormat format;
         };
         QVector<HighlightingRule> mHighlightingRules;

         QRegularExpression mCommentStartExpression;
         QRegularExpression mCommentEndExpression;

         QTextCharFormat mKeywordFormat;
         QTextCharFormat mClassFormat;
         QTextCharFormat mSingleLineCommentFormat;
         QTextCharFormat mMultiLineCommentFormat;
         QTextCharFormat mQuotationFormat;
         QTextCharFormat mFunctionFormat;

         void highlightBlock(const QString &text) override;
     };
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    void clearLabel();
    void resetLabelViewer();
    void addLabelFromInput();
    void addLabel(QString str);
    void deleteLabel(QPushButton *bt);
    void resetHighlightKeyword(QStringList keyWords);
private:
    QGridLayout *mLabelLayout;
    QList<QPushButton*> mLabelList;
private:
    Ui::SAKDebuggerOutputHighlighter *mUi;
    Highlighter *mHighlighter;
    QLineEdit *mInputLineEdit;
    QPushButton *mClearLabelBt;
    QPushButton *mAddLabelBt;
};

#endif
