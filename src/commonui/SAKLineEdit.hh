/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKLINEEDIT_HH
#define SAKLINEEDIT_HH

#include <QLineEdit>

class SAKLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    SAKLineEdit(QWidget *parent = nullptr);
    void setGroupKey(const QString &group,
                     const QString &key);

private:
    QString mKey;

private:
    void readFromSettingsFile();
    void writeToSettingsFile();
};

#endif // SAKLINEEDIT_HH
