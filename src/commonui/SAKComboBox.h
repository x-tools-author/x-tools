/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKCOMBOBOX_HH
#define SAKCOMBOBOX_HH

#include <QComboBox>

class SAKComboBox : public QComboBox
{
public:
    SAKComboBox(QWidget *parent = nullptr);

    void setCurrentIndexFromData(const QVariant &data);
    void setGroupKey(const QString &group,
                     const QString &key,
                     bool isIndex = true);

private:
    QString mKey;
    bool mIsIndex{true};

private:
    void readFromSettingsFile();
    void writeToSettingsFile();
};

#endif // SAKCOMBOBOX_HH
