/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKHIGHLIGHTERSETTINGSWIDGET_HH
#define SAKHIGHLIGHTERSETTINGSWIDGET_HH

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QTextDocument>

namespace Ui {
    class SAKHighlightSettingsWidget;
}

class SAKHighlightSettings;
/// @brief 高亮设置窗口
class SAKHighlightSettingsWidget:public QWidget
{
    Q_OBJECT
public:
    SAKHighlightSettingsWidget(QTextDocument *doc, QWidget* parent = Q_NULLPTR);
    ~SAKHighlightSettingsWidget();

    /**
     * @brief eventFilter 事件过滤器
     * @param watched 监控对象
     * @param event 实事件
     * @return 是否被过滤
     */
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
    Ui::SAKHighlightSettingsWidget *mUi;
    SAKHighlightSettings *mHighlighter;
    QLineEdit *mInputLineEdit;
    QPushButton *mClearLabelBt;
    QPushButton *mAddLabelBt;
};

#endif
