/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTOOLSTRINGASSISTANT_HH
#define SAKTOOLSTRINGASSISTANT_HH

#include <QDialog>

namespace Ui {
    class SAKToolStringAssistant;
}
class SAKToolStringAssistant : public QDialog
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKToolStringAssistant(QWidget *parent = Q_NULLPTR);
    ~SAKToolStringAssistant();
private:
    Ui::SAKToolStringAssistant *ui;
private slots:
    void on_textEdit_textChanged();
    void on_inputFormatComboBox_currentIndexChanged(int index);
    void on_createPushButton_clicked();
    void on_outputFormatComboBox_currentTextChanged(const QString &arg1);
};

#endif // SAKTOOLSTRINGASSISTANT_HH
