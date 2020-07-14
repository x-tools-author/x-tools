/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKPROTOCOLANALYZERWIDGET_HH
#define SAKPROTOCOLANALYZERWIDGET_HH

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
    class SAKProtocolAnalyzerWidget;
}

class SAKProtocolAnalyzer;
/// @brief 协议分析面板
class SAKProtocolAnalyzerWidget:public QWidget
{
    Q_OBJECT
public:
    SAKProtocolAnalyzerWidget(QWidget *parent = Q_NULLPTR);
    ~SAKProtocolAnalyzerWidget();

    /**
     * @brief inputBytes 数据输入接口
     * @param bytes 待分析数据
     */
    void inputBytes(QByteArray bytes);
private:
    SAKProtocolAnalyzer *mAnalyzer;
private:
    void setLineEditFormat(QLineEdit *lineEdit);
private:
    Ui::SAKProtocolAnalyzerWidget *mUi;
    QCheckBox *mFixedLengthCheckBox;
    QLineEdit *mLengthLineEdit;
    QLineEdit *mStartLineEdit;
    QLineEdit *mEndLineEdit;
    QCheckBox *mDisableCheckBox;
    QPushButton *mClearPushButton;
private slots:
    void on_fixedLengthCheckBox_clicked();
    void on_lengthLineEdit_textChanged(const QString &text);
    void on_startLineEdit_textChanged(const QString &text);
    void on_endLineEdit_textChanged(const QString &text);
    void on_disableCheckBox_clicked();
    void on_clearPushButton_clicked();
signals:
    void bytesAnalysed(QByteArray bytes);
};

#endif
