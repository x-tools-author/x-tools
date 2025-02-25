/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class InputSettings;
}
QT_END_NAMESPACE

class InputController;
class InputSettings : public QWidget
{
    Q_OBJECT
public:
    struct Parameters
    {
        bool showDataPreview;

        int prefix;
        int suffix;
        int escapeCharacter;

        int algorithm;
        int startIndex;
        int endIndex;
        bool appendCrc;
        bool bigEndian;
    };

public:
    InputSettings(QWidget *parent = nullptr);
    ~InputSettings();
    QVariantMap save();
    void load(const QVariantMap &parameters);

    Parameters parameters() const;

private:
    Ui::InputSettings *ui;
};
