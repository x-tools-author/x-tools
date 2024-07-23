/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
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
class OutputSettings;
}
QT_END_NAMESPACE

class OutputSettings : public QWidget
{
    Q_OBJECT
public:
    OutputSettings(QWidget *parent = nullptr);
    ~OutputSettings();
    bool isEnableFilter() const;
    bool isEnableHighlighter() const;
    QString filterText() const;
    QStringList highlighterKeywords() const;

    QVariantMap save();
    void load(const QVariantMap &data);

signals:
    void highlighterEnableChanged();
    void highlighterKeywordsChanged();

private:
    Ui::OutputSettings *ui;
};
