/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERHIGHLIGHTERMANAGER_HH
#define SAKOTHERHIGHLIGHTERMANAGER_HH

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QTextDocument>

namespace Ui {
    class SAKOtherHighlighterManager;
}

class SAKOtherHighlighter;
/// @brief Syntax highlighter manager
class SAKOtherHighlighterManager:public QWidget
{
    Q_OBJECT
public:
    SAKOtherHighlighterManager(QTextDocument *doc, QWidget* parent = Q_NULLPTR);
    ~SAKOtherHighlighterManager();
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
    Ui::SAKOtherHighlighterManager *mUi;
    SAKOtherHighlighter *mHighlighter;
    QLineEdit *mInputLineEdit;
    QPushButton *mClearLabelBt;
    QPushButton *mAddLabelBt;
};

#endif
