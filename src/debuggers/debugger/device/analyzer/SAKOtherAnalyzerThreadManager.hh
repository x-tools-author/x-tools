/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERANALYZERTHREADMANAGER_HH
#define SAKOTHERANALYZERTHREADMANAGER_HH

#include <QWidget>
#include <QSettings>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
    class SAKOtherAnalyzerThreadManager;
}

class SAKOtherAnalyzerThread;
/// @brief Parameters editing widget
class SAKOtherAnalyzerThreadManager:public QWidget
{
    Q_OBJECT
public:
    SAKOtherAnalyzerThreadManager(QSettings *settings, QWidget *parent = Q_NULLPTR);
    ~SAKOtherAnalyzerThreadManager();

    /**
     * @brief inputBytes: Input bytes to analyzed
     * @param bytes: Bytes taht need to be analyzed
     */
    void inputBytes(QByteArray bytes);
private:
    // Settings keys
    const QString mSettingKeyFixed;
    const QString mSettingKeyLenth;
    const QString mSettingKeyStartBytes;
    const QString mSettingKeyEndBytes;
    const QString mSettingKeyEnable;

    QSettings *mSettings;
    SAKOtherAnalyzerThread *mAnalyzer;
private:
    QByteArray string2bytes(QString hex);
private:
    Ui::SAKOtherAnalyzerThreadManager *mUi;
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
