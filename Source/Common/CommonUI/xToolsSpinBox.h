/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKSPINBOX_H
#define SAKSPINBOX_H

#include <QSpinBox>

class SAKSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    SAKSpinBox(QWidget *parent = nullptr);
    void setGroupKey(const QString &group, const QString &key);

private:
    QString mKey;

private:
    void readFromSettingsFile();
    void writeToSettingsFile();
};

#endif // SAKSPINBOX_H
