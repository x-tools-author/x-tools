/***************************************************************************************************
 * Copyright 2025-2025 le-cheng(https://github.com/le-cheng). All rights reserved.
 **************************************************************************************************/
#pragma once

#include <QRegularExpression>
#include <QTextBrowser>

#include "page/panels/common/panel.h"

namespace Ui {
class SearchPanel;
}

class SearchPanel : public Panel
{
    Q_OBJECT
public:
    explicit SearchPanel(QWidget *parent = nullptr);
    ~SearchPanel();
    void setOriginalTextBrowser(QTextBrowser *textBrowser);
    void performSearch(QString &line);
    void setWholeWordCheckBoxEnabled(bool checked);

private:
    Ui::SearchPanel *ui;
    QTextBrowser *m_originalTextBrowser;

private:
    void onSearchButtonClicked();
    void onSearchTextChanged();

    void performSearch();
    void highlightSearchResults(const QString &text, const QRegularExpression &regex);
    void highlightSearchResultsForLine(const QString &line, const QRegularExpression &regex);
};
