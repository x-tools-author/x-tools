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
#include <QScreen>
#include <QSettings>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QTextDocument>
#include <QTranslator>

#include "xTools.h"
#include "xToolsDataStructure.h"
#include "xToolsSettings.h"

xToolsApplication::xToolsApplication(int argc, char *argv[])
    : QApplication(argc, argv)
{
    xToolsInitAppStyle();

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
        QRegularExpressionValidator *urf8Validator = nullptr;
        QRegularExpressionValidator *systemValidator = nullptr;

        QString binStr = "([01][01][01][01][01][01][01][01][ ])*";
        QString octStr = "^(0[0-7]{0,2}|[1-3][0-7]{2})( (0[0-7]{0,2}|[1-3][0-7]{2}))*$";
        QString decStr = "^(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9])( "
                         "(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9]))*";
        QString hexStr = "([0-9a-fA-F][0-9a-fA-F][ ])*";

        auto binValidator = new QRegularExpressionValidator(QRegularExpression(binStr));
        auto otcValidator = new QRegularExpressionValidator(QRegularExpression(octStr)); //0-377
        auto decValidator = new QRegularExpressionValidator(QRegularExpression(decStr)); // 0-255;
        auto hexValidator = new QRegularExpressionValidator(QRegularExpression(hexStr));
        auto asciiValidator = new QRegularExpressionValidator(QRegularExpression("([ -~])*"));

        regularExpressionMap.insert(int(xToolsDataStructure::TextFormatBin), binValidator);
        regularExpressionMap.insert(int(xToolsDataStructure::TextFormatOct), otcValidator);
        regularExpressionMap.insert(int(xToolsDataStructure::TextFormatDec), decValidator);
        regularExpressionMap.insert(int(xToolsDataStructure::TextFormatHex), hexValidator);
        regularExpressionMap.insert(int(xToolsDataStructure::TextFormatAscii), asciiValidator);
        regularExpressionMap.insert(int(xToolsDataStructure::TextFormatUtf8), urf8Validator);
        regularExpressionMap.insert(int(xToolsDataStructure::TextFormatSystem), systemValidator);
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

void xToolsApplication::moveToScreenCenter(QWidget *widget)
{
    QRect screenRect = QApplication::primaryScreen()->geometry();
    bool tooWidth = (widget->width() > screenRect.width());
    bool tooHeight = (widget->height() > screenRect.height());
    if (tooWidth || tooHeight) {
        widget->showMaximized();
        qInfo() << "The screen is too small.";
    } else {
        widget->move((screenRect.width() - widget->width()) / 2,
                     (screenRect.height() - widget->height()) / 2);
    }
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
    str = str.replace(QString("  "), " 0");
    return QLocale(QLocale::English).toDateTime(str, "MMM dd yyyy").toString(format);
}

QString xToolsApplication::systemDateFormat()
{
    return QLocale::system().dateFormat();
}

QString xToolsApplication::systemTimeFormat()
{
    return QLocale::system().timeFormat();
}

QString xToolsApplication::desktopPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
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
