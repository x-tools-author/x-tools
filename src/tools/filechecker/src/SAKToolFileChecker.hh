/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTOOLFILECHECKER_HH
#define SAKTOOLFILECHECKER_HH

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QProgressBar>
#include <QCryptographicHash>

namespace Ui {
    class SAKToolFileChecker;
}

class QtCryptographicHashCalculator;
class SAKToolFileChecker : public QWidget
{
    Q_OBJECT
public:
    SAKToolFileChecker(QWidget *parent = Q_NULLPTR);
    ~SAKToolFileChecker();

    void setUiEnable(bool enable);
    QString fileName();
    QCryptographicHash::Algorithm algorithm();
    void updateResult(QByteArray result);
    void outputMessage(QString msg, bool isErrMsg = false);
    void updateProgressBar(int currentValue);
    void changeRemainTime(QString remainTime);
private:
    void finished();
    void clearMessage();
private:
    QString mFileName;
    QCryptographicHash::Algorithm mAlgorithm;
    QtCryptographicHashCalculator *mCalculator;
    QTimer mClearMessageTimer;
private:
    Ui::SAKToolFileChecker *mUi;
    QLineEdit *mFilePathlineEdit;
    QComboBox *mAlgorithmComboBox;
    QLineEdit *mResultLineEdit;
    QProgressBar *mCalculatorProgressBar;
    QPushButton *mOpenPushButton;
    QPushButton *mStartStopPushButton;
    QCheckBox *mUpperCheckBox;
    QLabel *mMessageLabel;
    QLabel *mRemainTimeLabel;
private slots:
    void on_openPushButton_clicked();
    void on_algorithmComboBox_currentIndexChanged(int index);
    void on_startStopPushButton_clicked();
    void on_upperCheckBox_clicked();
};

#endif
