/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <private/qobject_p.h>

#include <QActionGroup>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QLocale>
#include <QMenu>
#include <QSettings>
#include <QTranslator>

namespace xTools {

class I18nPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(I18n)
public:
    I18nPrivate() {}
    ~I18nPrivate() {}

    void setupLanguage()
    {
        if (!settings) {
            return;
        }

        QString defaultLanguage = QLocale::system().name();
        QString language = settings->value(keyLanguage, defaultLanguage).toString();

        const QString appPath = QCoreApplication::applicationDirPath();
        const QString qmFilesPath = appPath + "/translations";

        qInfo() << "The qm files path is:" << qmFilesPath;

        QStringList qmFiles;
        findQmFile(qmFilesPath, language, qmFiles);
        for (const QString& qmFile : qmFiles) {
            setupLanguage(qmFile);
        }
    }

    void initLanguageMenu()
    {
        Q_Q(I18n);
        QString path = QCoreApplication::applicationDirPath();
        path += "/translations/";
        QDir dir(path);
        QFileInfoList fileInfoList = dir.entryInfoList(QStringList("*.qm"),
                                                       QDir::Files | QDir::NoDotAndDotDot);
        QStringList languages;
        for (const QFileInfo& fileInfo : fileInfoList) {
            QString baseName = fileInfo.baseName(); // e.g., xTools_zh_CN

            if (baseName.startsWith("qt_")) {
                continue;
            }

            int index = baseName.indexOf("_");
            if (index == -1) {
                continue;
            }

            QString prefix = baseName.left(index + 1); // e.g., xTools_
            QString locale = baseName.remove(prefix);  // e.g., zh_CN
            if (!languages.contains(locale)) {
                languages.append(locale);
            }
        }

        languageMenu = new QMenu();
        static auto languageActionGroup = new QActionGroup(languageMenu);
        for (const QString& language : std::as_const(languages)) {
            QString name = language;
            QString nativeName = QLocale(language).nativeLanguageName();
            if (nativeName.isEmpty()) {
                continue;
            }

            QLocale locale(language);
            auto* action = new QAction(nativeName, languageMenu);
            action->setCheckable(true);
            languageMenu->addAction(action);
            languageActionGroup->addAction(action);

            QObject::connect(action, &QAction::triggered, action, [=]() {
                if (!this->settings) {
                    return;
                }

                settings->setValue(keyLanguage, language);
                emit q->languageChanged();
            });

            QString defaultLanguage = QLocale::system().name();
            if (!settings) {
                continue;
            }

            QString tmp = settings->value(keyLanguage, defaultLanguage).toString();
            if (tmp == name) {
                action->setChecked(true);
            }
        }
    }

public:
    QMenu* languageMenu{nullptr};
    QSettings* settings{nullptr};

private:
    const QString keyLanguage{"Application/language"};

private:
    void findQmFile(const QString& path, const QString& language, QStringList& qmFiles)
    {
        QDir dir(path);
        QFileInfoList infos = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        for (const QFileInfo& info : infos) {
            if (info.isDir()) {
                findQmFile(info.absoluteFilePath(), language, qmFiles);
            } else if (info.isFile() && info.suffix().toLower() == "qm") {
                QString fileName = info.fileName();
                if (fileName.endsWith("_" + language + ".qm")) {
                    qmFiles.append(info.absoluteFilePath());
                }
            }
        }
    }

    void setupLanguage(const QString& qmFile)
    {
        QString appPath = QCoreApplication::applicationDirPath();
        QString tmp = qmFile;
        tmp = tmp.remove(appPath + "/");

        QTranslator* translator = new QTranslator();
        if (!translator->load(qmFile)) {
            qWarning() << qPrintable(QString("The qm file(%1) can not be loaded.").arg(tmp));
            return;
        }

        if (!qApp->installTranslator(translator)) {
            qWarning() << qPrintable(QString("The qm file(%1) can not be installed.").arg(tmp));
        }

        qInfo() << qPrintable(QString("The qm file(%1) is installed.").arg(tmp));
    }
};

} // namespace xTools
