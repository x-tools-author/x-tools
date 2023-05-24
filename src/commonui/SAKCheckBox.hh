/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKCHECKBOX_HH
#define SAKCHECKBOX_HH

#include <QCheckBox>

class SAKCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    SAKCheckBox(QWidget *parent = nullptr);
    void setGroupKey(const QString &group,
                     const QString &key);

private:
    QString mKey;

private:
    void readFromSettingsFile();
    void writeToSettingsFile();
};

#endif // SAKCHECKBOX_HH
