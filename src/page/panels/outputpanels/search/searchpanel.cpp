/***************************************************************************************************
 * Copyright 2025-2025 le-cheng(https://github.com/le-cheng). All rights reserved.
 **************************************************************************************************/
#include "searchpanel.h"
#include "ui_searchpanel.h"

SearchPanel::SearchPanel(QWidget *parent)
    : Panel(parent)
    , ui(new Ui::SearchPanel)
{
    ui->setupUi(this);

    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &SearchPanel::onSearchButtonClicked);
    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &SearchPanel::onSearchTextChanged);
    connect(ui->checkBoxRegex, &QCheckBox::clicked, this, [this](int) { performSearch(); });
    connect(ui->checkBoxMatchCase, &QCheckBox::clicked, this, [this](int) { performSearch(); });
    connect(ui->checkBoxWholeWord, &QCheckBox::clicked, this, [this](int) { performSearch(); });
}

SearchPanel::~SearchPanel()
{
    delete ui;
}

void SearchPanel::setOriginalTextBrowser(QTextBrowser *textBrowser)
{
    m_originalTextBrowser = textBrowser;
}

void SearchPanel::performSearch(QString &line)
{
    QString searchText = ui->lineEditSearch->text();
    if (searchText.isEmpty()) {
        return;
    }

    // 获取搜索选项
    bool useRegex = ui->checkBoxRegex->isChecked();
    bool matchCase = ui->checkBoxMatchCase->isChecked();
    bool wholeWord = ui->checkBoxWholeWord->isChecked();

    // 构建正则表达式
    QString pattern = searchText;
    if (!useRegex) {
        // 如果不使用正则表达式，则转义特殊字符
        pattern = QRegularExpression::escape(pattern);
    }

    // 如果需要全词匹配，则添加单词边界
    if (wholeWord) {
        pattern = QString("\\b%1\\b").arg(pattern);
    }

    // 创建正则表达式对象
    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    if (!matchCase) {
        options |= QRegularExpression::CaseInsensitiveOption;
    }
    QRegularExpression regex(pattern, options);

    // 只在当前行中搜索匹配项
    highlightSearchResultsForLine(line, regex);
}

void SearchPanel::setWholeWordCheckBoxEnabled(bool checked)
{
    ui->checkBoxWholeWord->setChecked(checked);
}

void SearchPanel::onSearchButtonClicked()
{
    performSearch();
}

void SearchPanel::onSearchTextChanged()
{ // 当搜索文本变化时，如果文本不为空，则执行搜索
    if (!ui->lineEditSearch->text().isEmpty()) {
        performSearch();
    } else {
        // 如果搜索文本为空，则清空搜索结果
        ui->textBrowserSearchResult->clear();
    }
}

void SearchPanel::performSearch()
{
    QString searchText = ui->lineEditSearch->text();
    if (searchText.isEmpty()) {
        return;
    }

    if (m_originalTextBrowser == Q_NULLPTR) {
        return;
    }

    // 获取搜索选项
    bool useRegex = ui->checkBoxRegex->isChecked();
    bool matchCase = ui->checkBoxMatchCase->isChecked();
    bool wholeWord = ui->checkBoxWholeWord->isChecked();

    // 构建正则表达式
    QString pattern = searchText;
    if (!useRegex) {
        // 如果不使用正则表达式，则转义特殊字符
        pattern = QRegularExpression::escape(pattern);
    }

    // 如果需要全词匹配，则添加单词边界
    if (wholeWord) {
        pattern = QString("\\b%1\\b").arg(pattern);
    }

    // 创建正则表达式对象
    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    if (!matchCase) {
        options |= QRegularExpression::CaseInsensitiveOption;
    }
    QRegularExpression regex(pattern, options);

    // 获取textBrowserOutput的文本内容
    QString content = m_originalTextBrowser->toPlainText();

    // 执行搜索并高亮结果
    highlightSearchResults(content, regex);
}

void SearchPanel::highlightSearchResults(const QString &text, const QRegularExpression &regex)
{
    ui->textBrowserSearchResult->clear();

    // 查找所有匹配项
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(text);
    int matchCount = 0;

    // 存储所有匹配结果
    QList<QPair<int, QRegularExpressionMatch>> matches;
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        matches.append(qMakePair(match.capturedStart(), match));
        matchCount++;
    }

    // 如果没有匹配项，显示提示信息
    if (matchCount == 0) {
        ui->textBrowserSearchResult->setHtml(QString("<p style='color:gray'>未找到匹配项</p>"));
        return;
    }

    // 显示匹配数量
    ui->textBrowserSearchResult->setHtml(
        QString("<p style='color:gray'>找到 %1 个匹配项</p>").arg(matchCount));

    // 获取文本的行
    QStringList lines = text.split('\n');

    // 对于每个匹配项，显示上下文
    for (const auto &matchPair : matches) {
        int matchPos = matchPair.first;
        QRegularExpressionMatch match = matchPair.second;

        // 计算匹配项所在的行号
        int lineNumber = 0;
        int posInLine = matchPos;
        for (int i = 0; i < lines.size(); i++) {
            if (posInLine <= lines[i].length()) {
                lineNumber = i;
                break;
            }
            posInLine -= lines[i].length() + 1; // +1 是为了换行符
        }

        // 获取匹配行及其上下文（前后各一行）
        QString contextBefore = lineNumber > 0 ? lines[lineNumber - 1] : QString();
        QString matchLine = lines[lineNumber];
        QString contextAfter = lineNumber < lines.size() - 1 ? lines[lineNumber + 1] : QString();

        // 在匹配行中高亮匹配文本
        QString matchedText = match.captured();
        int matchStartInLine = posInLine;
        int matchEndInLine = matchStartInLine + matchedText.length();

        QString highlightedLine = matchLine.left(matchStartInLine)
                                  + QString("<span style='background-color:yellow;'>%1</span>")
                                        .arg(matchedText)
                                  + matchLine.mid(matchEndInLine);

        // 构建HTML显示上下文
        QString html = "<div style='margin-bottom:10px;'>";
        html += QString("<p style='color:gray;margin:0;'>行 %1:</p>").arg(lineNumber + 1);

        if (!contextBefore.isEmpty()) {
            html += QString("<pre style='margin:0;color:gray;'>%1</pre>")
                        .arg(contextBefore.toHtmlEscaped());
        }

        html += QString("<pre style='margin:0;'>%1</pre>").arg(highlightedLine);

        if (!contextAfter.isEmpty()) {
            html += QString("<pre style='margin:0;color:gray;'>%1</pre>")
                        .arg(contextAfter.toHtmlEscaped());
        }

        html += "</div>";

        // 添加到搜索结果
        ui->textBrowserSearchResult->append(html);
    }
}

void SearchPanel::highlightSearchResultsForLine(const QString &line, const QRegularExpression &regex)
{
    // 查找所有匹配项
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(line);

    // 如果没有匹配项，直接返回
    if (!matchIterator.hasNext()) {
        return;
    }

    // 如果这是第一个匹配项，初始化搜索结果区域
    if (ui->textBrowserSearchResult->document()->isEmpty()) {
        ui->textBrowserSearchResult->setHtml(QString("<p style='color:gray'>实时搜索结果：</p>"));
    }

    // 使用原始文本进行匹配和高亮，与highlightSearchResults保持一致
    QString originalText = line;

    // 对于每个匹配项，高亮并添加到结果中
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        QString matchedText = match.captured();
        int matchStart = match.capturedStart();
        int matchEnd = matchStart + matchedText.length();

        // 在匹配行中高亮匹配文本，使用与highlightSearchResults相同的逻辑
        QString highlightedLine = originalText.left(matchStart)
                                  + QString("<span style='background-color:yellow;'>%1</span>")
                                        .arg(matchedText)
                                  + originalText.mid(matchEnd);

        // 构建HTML显示
        QString html = "<div style='margin-bottom:5px;'>";
        html += QString("<pre style='margin:0;'>%1</pre>").arg(highlightedLine);
        html += "</div>";

        // 添加到搜索结果
        ui->textBrowserSearchResult->append(html);

        // 更新匹配计数（可选）
        static int matchCount = 0;
        matchCount++;
        // 更新标题显示匹配数量
        QTextCursor cursor = ui->textBrowserSearchResult->textCursor();
        cursor.setPosition(0);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        cursor.insertHtml(
            QString("<p style='color:gray'>实时搜索结果：找到 %1 个匹配项</p>").arg(matchCount));
    }
}
