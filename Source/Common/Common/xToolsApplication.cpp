/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsApplication.h"

#include <QAbstractTableModel>
#include <QClipboard>
#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QList>
#include <QLocale>
#include <QPainter>
#include <QRegularExpressionValidator>
#include <QSettings>
#include <QStandardItemModel>
#include <QTextDocument>
#include <QTranslator>

#include "xToolsSettings.h"

xToolsApplication::xToolsApplication(int argc, char *argv[])
    : QApplication(argc, argv)
{
    m_languageFlagNameMap.insert("zh_CN", "简体中文");
    m_languageFlagNameMap.insert("en", "English");
#if 0
  m_languageFlagNameMap.insert("zh_TW", "繁體中文");
  m_languageFlagNameMap.insert("ar", "العربية");
  m_languageFlagNameMap.insert("cs", "Čeština");
  m_languageFlagNameMap.insert("da", "Dansk");
  m_languageFlagNameMap.insert("de", "Deutsch");
  m_languageFlagNameMap.insert("es", "Español");
  m_languageFlagNameMap.insert("fa", "فارسی");
  m_languageFlagNameMap.insert("fi", "Suomi");
  m_languageFlagNameMap.insert("fr", "Français");
  m_languageFlagNameMap.insert("he", "עִבְרִית");
  m_languageFlagNameMap.insert("uk", "українська мова");
  m_languageFlagNameMap.insert("it", "Italiano");
  m_languageFlagNameMap.insert("ja", "日本语");
  m_languageFlagNameMap.insert("ko", "한글");
  m_languageFlagNameMap.insert("lt", "Lietuvių kalba");
  m_languageFlagNameMap.insert("pl", "Polski");
  m_languageFlagNameMap.insert("pt", "Português");
  m_languageFlagNameMap.insert("ru", "русский язык");
  m_languageFlagNameMap.insert("sk", "Slovenčina");
  m_languageFlagNameMap.insert("sl", "Slovenščina");
  m_languageFlagNameMap.insert("sv", "Svenska");
#endif
    QString language = xToolsSettings::instance()->language();
    xToolsApplication::setupLanguage(language);

    // Palette
    showSplashScreenMessage(tr("Load palette..."));
    QString palette = xToolsSettings::instance()->palette();
    setupPalette(palette);

    // Splash screen
    m_splashScreen.setPixmap(splashScreenPixmap());
    m_splashScreen.show();
    processEvents();
    showSplashScreenMessage(tr("Initialize application..."));
}

void xToolsApplication::showSplashScreenMessage(const QString &msg)
{
    m_splashScreen.showMessage(msg, Qt::AlignBottom, QColor(255, 255, 255));
}

void xToolsApplication::setupPalette(const QString &fileName)
{
    if (fileName.isEmpty()) {
        qWarning() << "The palette file name is empty, use default palette.";
        return;
    }

    if (!QFile::exists(fileName)) {
        QString info = QString("The palette file not found: %1, use default palette.").arg(fileName);
        qInfo() << qPrintable(info);
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QDataStream out(&file);
        QPalette palette;
        out >> palette;
        file.close();
        setPalette(palette);
        qInfo() << "The palette of application is:" << fileName;
    } else {
        qWarning() << "Open palette file error:" << file.errorString();
    }
}

QSplashScreen &xToolsApplication::splashScreen()
{
    return m_splashScreen;
}

QStringList xToolsApplication::supportedLanguages()
{
    return m_languageFlagNameMap.values();
}

QString xToolsApplication::language()
{
    QString language = xToolsSettings::instance()->language();
    if (language.isEmpty()) {
        language = QLocale::system().name();
    }

    if (!m_languageFlagNameMap.contains(language)) {
        language = "en";
    }

    return m_languageFlagNameMap.value(language);
}

void xToolsApplication::setupLanguage(const QString &language)
{
    setupLanguageWithPrefix(language, m_translatorPrefix);
}

void xToolsApplication::setValidator(QLineEdit *target, int validatorType, int maxLength)
{
    static QMap<int, QRegularExpressionValidator *> regularExpressionMap;
    if (regularExpressionMap.isEmpty()) {
        typedef QRegularExpressionValidator REV;
        auto noneRE = nullptr;
        auto binRE = new REV(QRegularExpression("([01][01][01][01][01][01][01][01][ ])*"));
        auto otcRE = new REV(QRegularExpression("([0-7][0-7][ ])*"));
        auto decRE = new REV(QRegularExpression("([-+.e0-9])*"));
        auto hexRE = new REV(QRegularExpression("([0-9a-fA-F][0-9a-fA-F][ ])*"));
        auto asciiRE = new REV(QRegularExpression("([ -~])*"));
        auto floatRE = new REV(QRegularExpression("^[-+]?[0-9]*\\.?[0-9]+$"));

        regularExpressionMap.insert(int(ValidatorType::None), noneRE);
        regularExpressionMap.insert(int(ValidatorType::Bin), binRE);
        regularExpressionMap.insert(int(ValidatorType::Otc), otcRE);
        regularExpressionMap.insert(int(ValidatorType::Dec), decRE);
        regularExpressionMap.insert(int(ValidatorType::Hex), hexRE);
        regularExpressionMap.insert(int(ValidatorType::Ascii), asciiRE);
        regularExpressionMap.insert(int(ValidatorType::Float), floatRE);
    }

    if (!target || !regularExpressionMap.contains(validatorType) || maxLength < 0) {
        qWarning() << "Invalid parameter, the operation will be ignored!";
        return;
    }

    target->setValidator(regularExpressionMap.value(validatorType));
    target->setMaxLength(maxLength);
}

QIcon xToolsApplication::cookedIcon(const QIcon &icon)
{
    QPixmap pixmap = icon.pixmap(QSize(128, 128));
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), qApp->palette().windowText().color());
    QIcon colorIcon = QIcon(pixmap);
    return colorIcon;
}

QMainWindow *xToolsApplication::mainWindow()
{
    for (const auto& it : qobject_cast<QApplication*>(qApp)->topLevelWidgets()) {
        auto mainWindow = qobject_cast<QMainWindow*>(it);
        if (mainWindow) {
            return mainWindow;
        }
    }

    return nullptr;
}

QString xToolsApplication::clipboardText()
{
    return QGuiApplication::clipboard()->text();
}

void xToolsApplication::setClipboardText(const QString &text)
{
    QGuiApplication::clipboard()->setText(text);
}

QString xToolsApplication::dateTimeString(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
}

QString xToolsApplication::stringToHexString(const QString &str)
{
    return QString::fromLatin1(str.toUtf8().toHex());
}

QString xToolsApplication::hexStringToString(const QString &str)
{
    QByteArray arr = QByteArray::fromHex(str.toUtf8());
    return QString::fromUtf8(arr);
}

QString xToolsApplication::buildDateTime(const QString &format)
{
    QString str = QString(__DATE__);
    QDate date = QDate::fromString(str, "MMM d yyyy");
    if (!date.isValid()) {
        date = QDate::fromString(str, "MMM  d yyyy");
    }
    QTime time = QTime::fromString(__TIME__, "hh:mm:ss");
    return QDateTime(date, time).toString(format);
}

QString xToolsApplication::systemDateFormat()
{
    return QLocale::system().dateFormat();
}

QString xToolsApplication::systemTimeFormat()
{
    return QLocale::system().timeFormat();
}

void xToolsApplication::setupLanguageWithPrefix(const QString &language, const QString &prefix)
{
    QString key = m_languageFlagNameMap.key(language);
    if (language.isEmpty()) {
        key = QLocale::system().name();

        qWarning() << "The language is not specified, system language will be used:" << key;
    }

    if (!m_languageFlagNameMap.contains(key)) {
        qWarning() << "Unsupported language, english will be used";

        key = "en";
    }

    static QMap<QString, QTranslator *> fileTranslatorMap;
    if (!fileTranslatorMap.contains(prefix)) {
        QTranslator *translator = new QTranslator();
        fileTranslatorMap.insert(prefix, translator);
    } else {
        if (!QCoreApplication::removeTranslator(fileTranslatorMap.value(prefix))) {
            qWarning() << "Remove translator failed:" << prefix;
        }
    }

    QString fileName = QString(":/Resources/Translations/%1_%2.qm").arg(prefix, key);
    QTranslator *translator = fileTranslatorMap.value(prefix);
    if (translator->load(fileName)) {
        if (QCoreApplication::installTranslator(translator)) {
            QString info = QString("The translator(%1) has been installed,").arg(fileName);
            qInfo() << qPrintable(info)
                    << "current language is:" << m_languageFlagNameMap.value(key);
            emit languageChanged();
        } else {
            QString info = QString("Install translator failed, the file is: \"%1\".").arg(fileName);
            qInfo() << qPrintable(info) << "Maybe the qm file if empty!";
            translator->deleteLater();
            translator = Q_NULLPTR;
        }
    } else {
        qWarning() << "Load file failed: " << fileName;
    }
}

QPixmap xToolsApplication::splashScreenPixmap()
{
    QPixmap pixmap(600, 260);
    pixmap.fill(QColor(0x2d2d30));

    QPainter painter(&pixmap);
    painter.setPen(QColor(Qt::white));
    auto font = painter.font();
    font.setPixelSize(52);
    painter.setFont(font);
    painter.drawText(pixmap.rect(),
                     Qt::AlignHCenter | Qt::AlignVCenter,
                     xToolsApplication::applicationName());
    painter.drawRect(pixmap.rect() - QMargins(1, 1, 1, 1));

    return pixmap;
}
