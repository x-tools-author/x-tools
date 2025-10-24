/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <atomic>

#include <QRunnable>
#include <QString>

class Translator : public QRunnable
{
public:
    explicit Translator(const QString &from,
                        const QString &to,
                        const QString &fileName,
                        const QString &sourceText,
                        int sourceLineNumber);
    ~Translator();
    static void setRequestInterrupted(bool interrupted);

    void run() override;

private:
    static std::atomic_bool s_requestInterrupted;

    QString m_from;
    QString m_to;
    QString m_fileName;
    QString m_sourceText;
    int m_sourceLineNumber;
};
