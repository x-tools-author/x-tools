/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QColor>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>

namespace xTools {

class SyntaxHighlighterJsonPrivate;
class SyntaxHighlighterJson : public QSyntaxHighlighter
{
    Q_OBJECT
    SyntaxHighlighterJsonPrivate *d{nullptr};

public:
    SyntaxHighlighterJson(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;
};

} // namespace xTools