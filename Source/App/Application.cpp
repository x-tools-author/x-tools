/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "Application.h"

#include <QAbstractTableModel>
#include <QClipboard>
#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QFontMetrics>
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

#include "App/Settings.h"
#include "IO/xIO.h"

namespace xTools {

bool Application::m_enableSplashScreen = true;
QString Application::m_friendlyAppName = QStringLiteral("xTools");

Application::Application(int argc, char *argv[])
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
    QString language = xTools::Settings::instance()->language();
    Application::setupLanguage(language);

    // Palette
    showSplashScreenMessage(tr("Load palette..."));
    QString palette = xTools::Settings::instance()->palette();
    setupPalette(palette);

    // Splash screen
    m_splashScreen.setPixmap(splashScreenPixMap());
#if 0
    m_splashScreen.setWindowFlags(m_splashScreen.windowFlags() | Qt::WindowStaysOnTopHint);
#endif
    if (enableSplashScreen()) {
        m_splashScreen.show();
        processEvents();
        showSplashScreenMessage(tr("Initialize application..."));
    }
}

bool Application::enableSplashScreen()
{
    return m_enableSplashScreen;
}

void Application::setEnableSplashScreen(bool enable)
{
    m_enableSplashScreen = enable;
}

QString Application::friendlyAppName()
{
    return m_friendlyAppName;
}

void Application::setFriendlyAppName(const QString &name)
{
    m_friendlyAppName = name;
}

void Application::showSplashScreenMessage(const QString &msg)
{
    m_splashScreen.showMessage(msg, Qt::AlignBottom, QColor(255, 255, 255));
}

void Application::setupPalette(const QString &fileName)
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

QSplashScreen &Application::splashScreen()
{
    return m_splashScreen;
}

QStringList Application::supportedLanguages()
{
    return m_languageFlagNameMap.values();
}

QString Application::language()
{
    QString language = xTools::Settings::instance()->language();
    if (language.isEmpty()) {
        language = QLocale::system().name();
    }

    if (!m_languageFlagNameMap.contains(language)) {
        qWarning() << "The language name is not supported:" << language << ". \"en\" will be used.";
        language = "en";
    }

    return m_languageFlagNameMap.value(language);
}

void Application::setupLanguage(const QString &language)
{
    setupLanguageWithPrefix(language, m_translatorPrefix);
}

void Application::setValidator(QLineEdit *target, int validatorType, int maxLength)
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

        regularExpressionMap.insert(int(xTools::xIO::TextFormat::Bin), binValidator);
        regularExpressionMap.insert(int(xTools::xIO::TextFormat::Oct), otcValidator);
        regularExpressionMap.insert(int(xTools::xIO::TextFormat::Dec), decValidator);
        regularExpressionMap.insert(int(xTools::xIO::TextFormat::Hex), hexValidator);
        regularExpressionMap.insert(int(xTools::xIO::TextFormat::Ascii), asciiValidator);
        regularExpressionMap.insert(int(xTools::xIO::TextFormat::Utf8), urf8Validator);
    }

    if (!target || !regularExpressionMap.contains(validatorType) || maxLength < 0) {
        qWarning() << "Invalid parameter, the operation will be ignored!";
        return;
    }

    target->setValidator(regularExpressionMap.value(validatorType));
    target->setMaxLength(maxLength);
}

QIcon Application::cookedIcon(const QIcon &icon)
{
    QPixmap pixmap = icon.pixmap(QSize(128, 128));
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), qApp->palette().windowText().color());
    QIcon colorIcon = QIcon(pixmap);
    return colorIcon;
}

QMainWindow *Application::mainWindow()
{
    for (const auto &it : qobject_cast<QApplication *>(qApp)->topLevelWidgets()) {
        auto mainWindow = qobject_cast<QMainWindow *>(it);
        if (mainWindow) {
            return mainWindow;
        }
    }

    return nullptr;
}

void Application::moveToScreenCenter(QWidget *widget)
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

QString Application::clipboardText()
{
    return QGuiApplication::clipboard()->text();
}

void Application::setClipboardText(const QString &text)
{
    QGuiApplication::clipboard()->setText(text);
}

QString Application::dateTimeString(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
}

QString Application::stringToHexString(const QString &str)
{
    return QString::fromLatin1(str.toUtf8().toHex());
}

QString Application::hexStringToString(const QString &str)
{
    QByteArray arr = QByteArray::fromHex(str.toUtf8());
    return QString::fromUtf8(arr);
}

QDateTime Application::buildDateTime()
{
    QString dateString = QString(__DATE__);
    QString timeString = QString(__TIME__);
    dateString = dateString.replace(QString("  "), " 0");
    QString dateTimeString = dateString + " " + timeString;
    QDateTime dateTime = QLocale(QLocale::English).toDateTime(dateTimeString, "MMM dd yyyy hh:mm:ss");
    return dateTime;
}

QString Application::buildDateTimeString(const QString &format)
{
    return buildDateTime().toString(format);
}

QString Application::systemDateFormat()
{
    return QLocale::system().dateFormat();
}

QString Application::systemTimeFormat()
{
    return QLocale::system().timeFormat();
}

QString Application::desktopPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void Application::setupLanguageWithPrefix(const QString &language, const QString &prefix)
{
    QString key = m_languageFlagNameMap.key(language);
    if (language.isEmpty()) {
        key = QLocale::system().name();
        qWarning() << "The language is not specified, system language will be used:" << key;
    }

    if (!m_languageFlagNameMap.contains(key)) {
        auto info = QString("The language(%1) isn't supported. English 'll be used.").arg(language);
        qWarning() << qPrintable(info);
        key = "en";
    }

    static QMap<QString, QTranslator *> fileTranslatorMap;
    QString fileName = QString(":/Resources/Translations/%1_%2.qm").arg(prefix, key);
    if (!fileTranslatorMap.contains(prefix)) {
        QTranslator *translator = new QTranslator();
        fileTranslatorMap.insert(prefix, translator);
    } else {
        bool ret = QCoreApplication::removeTranslator(fileTranslatorMap.value(prefix));
        const QString status = ret ? "success" : "failed";
        auto info = QString("The translator(%1) removing: %2.").arg(prefix, status);
        qInfo() << qPrintable(info);
    }

    QTranslator *translator = fileTranslatorMap.value(prefix);
    if (!translator->load(fileName)) {
        auto info = QString("The file(%1) can not be load to translator.").arg(fileName);
        qWarning() << qPrintable(info);
        return;
    }

    if (QCoreApplication::installTranslator(translator)) {
        const QString name = m_languageFlagNameMap.value(key);
        auto info = QString("The language(%1) has been setup, current language is:").arg(fileName);
        qInfo() << qPrintable(info) << name;
        emit languageChanged();
    } else {
        translator->deleteLater();
        translator = Q_NULLPTR;
        auto info = QString("The translator(%1) install failed(qm file is empty?).").arg(fileName);
        qInfo() << qPrintable(info);
        return;
    }
}

QPixmap Application::splashScreenPixMap()
{
    QFont font = qApp->font();
    font.setPixelSize(52);

    QFontMetrics fontMetrics(font);
    const QString displayName = Application::friendlyAppName();
    int width = fontMetrics.boundingRect(displayName).width() * 1.2;

    QPixmap pixmap(width < 600 ? 600 : width, 260);
    pixmap.fill(QColor(0x2d2d30));

    QPainter painter(&pixmap);
    painter.setPen(QColor(Qt::white));
    painter.setFont(font);
    painter.drawText(pixmap.rect(), Qt::AlignHCenter | Qt::AlignVCenter, displayName);
    painter.drawRect(pixmap.rect() - QMargins(1, 1, 1, 1));

    return pixmap;
}

} // namespace xTools
