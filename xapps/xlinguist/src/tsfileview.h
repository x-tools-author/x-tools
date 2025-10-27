/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace Ui {
class TsFileView;
};

class TsFile;
class TsFileFilter;
class TsFileView : public QWidget
{
    Q_OBJECT
public:
    explicit TsFileView(const QString &filePath, QWidget *parent = nullptr);
    ~TsFileView() override;

    TsFile *tsFile() const;

private:
    Ui::TsFileView *ui;
    TsFile *m_tsFile{nullptr};
    TsFileFilter *m_filterModel{nullptr};

private:
    void onAllItemsChecked(bool checked);
    void onSourceItemsChecked(bool checked);
    void onTranslationItemsChecked(bool checked);
};