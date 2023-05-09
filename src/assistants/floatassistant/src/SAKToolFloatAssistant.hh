/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTOOLFLOATASSISTANT_HH
#define SAKTOOLFLOATASSISTANT_HH

#include <QDialog>

namespace Ui {
    class SAKToolFloatAssistant;
}
class SAKCommonInterface;
class SAKToolFloatAssistant : public QDialog
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKToolFloatAssistant(QWidget *parent = Q_NULLPTR);
    ~SAKToolFloatAssistant();
private:
    SAKCommonInterface *mCommonInterface;
private:
    void fixedLength(QStringList &stringList);
private:
    Ui::SAKToolFloatAssistant *ui;
private slots:
    void on_hexRawDataCheckBox_clicked();
    void on_createPushButton_clicked();
    void on_rawDataLineEdit_textChanged(const QString &arg1);
    void on_bigEndianCheckBox_clicked();
    void on_floatRadioButton_clicked();
    void on_doubleRadioButton_clicked();
};

#endif // SAKTOOLFLOATASSISTANT_HH
