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
#include <QMessageBox>
#include <QMetaEnum>
#include <QPainter>
#include <QProcess>
#include <QRegularExpressionValidator>
#include <QScreen>
#include <QSettings>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QTextDocument>
#include <QTimer>
#include <QTranslator>

#include "App/Settings.h"

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

    // Splash screen
    if (enableSplashScreen()) {
        m_splashScreen.setAttribute(Qt::WA_TransparentForMouseEvents, true);
        m_splashScreen.setPixmap(splashScreenPixMap());
        m_splashScreen.show();
        showSplashScreenMessage(tr("Initialize application..."));
        processEvents();
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

void Application::showSplashScreenMessage(const QString &msg)
{
    m_splashScreen.showMessage(msg, Qt::AlignBottom, QColor(255, 255, 255));
}

QSplashScreen &Application::splashScreen()
{
    return m_splashScreen;
}

QString Application::friendlyAppName()
{
    return m_friendlyAppName;
}

void Application::setFriendlyAppName(const QString &name)
{
    m_friendlyAppName = name;
}

QString Application::defaultLanguage()
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

QStringList Application::supportedLanguages()
{
    return m_languageFlagNameMap.values();
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
QVariantList Application::supportedHighDpiPolicies()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::HighDpiScaleFactorRoundingPolicy>();
    QVariantList list;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        list.append(metaEnum.value(i));
    }
    return list;
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
QString Application::highDpiPolicyName(int policy)
{
    typedef Qt::HighDpiScaleFactorRoundingPolicy Policy;
    static QMap<Policy, QString> policyMap;
    if (policyMap.isEmpty()) {
        policyMap.insert(Policy::Unset, tr("System"));
        policyMap.insert(Policy::Round, tr("Round up for .5 and above"));
        policyMap.insert(Policy::Ceil, tr("Always round up"));
        policyMap.insert(Policy::Floor, tr("Always round down"));
        policyMap.insert(Policy::RoundPreferFloor, tr("Round up for .75 and above"));
        policyMap.insert(Policy::PassThrough, tr("Don't round"));
    }

    return policyMap.value(static_cast<Policy>(policy), "Unknown");
}
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
bool Application::isValidHighDpiPolicy(int policy)
{
    auto policies = supportedHighDpiPolicies();
    return policies.contains(QVariant(policy));
}
#endif

QStringList Application::supportedLanguagePrefixes()
{
    return QStringList() << "xToolsCore";
}

void Application::setupLanguage(const QString &language)
{
    QString tmp = language;
    if (tmp.isEmpty()) {
        tmp = xTools::Settings::instance()->language();
        if (tmp.isEmpty()) {
            tmp = defaultLanguage();
        }
    }

    auto prefixes = supportedLanguagePrefixes();
    for (const auto &prefix : prefixes) {
        setupLanguageWithPrefix(tmp, prefix);
    }
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
    for (const auto &widget : qApp->topLevelWidgets()) {
        auto mainWindow = qobject_cast<QMainWindow *>(widget);
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

QString Application::stringToHexString(const QString &str)
{
    return QString::fromLatin1(str.toUtf8().toHex());
}

QString Application::hexStringToString(const QString &str)
{
    QByteArray arr = QByteArray::fromHex(str.toUtf8());
    return QString::fromUtf8(arr);
}

QString Application::dateTimeString(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
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

QString Application::appVersioin()
{
    QString version = "0.0.0";
#if defined(X_TOOLS_GIT_TAG)
    auto tag = QString(X_TOOLS_GIT_TAG);
    if (tag == QString("continuous")) {
        version = "9.9.9";
    } else {
        tag = tag.remove("v");
        if (tag.split('.').length() == 3) {
            version = tag;
        }
    }
#endif
    return version;
}

bool Application::tryToReboot()
{
    int ret = QMessageBox::information(nullptr,
                                       tr("Reboot application to effective"),
                                       tr("Need to reboot, reboot to effective now?"),
                                       QMessageBox::Ok | QMessageBox::No,
                                       QMessageBox::No);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QCoreApplication::applicationFilePath(), QStringList());
        QTimer::singleShot(1000, qApp, [=]() {
            qApp->closeAllWindows();
            qApp->exit();
        });
        return true;
    }

    return false;
}

QByteArray Application::byteArray2Hex(const QByteArray &source, char separator)
{
    if (source.isEmpty()) {
        return source;
    }

    auto toHex = [](quint8 value) -> char { return "0123456789abcdef"[value & 0xF]; };

    const int length = separator ? (source.size() * 3 - 1) : (source.size() * 2);
    QByteArray hex(length, Qt::Uninitialized);
    char *hexData = hex.data();
    const uchar *data = reinterpret_cast<const uchar *>(source.constData());
    for (int i = 0, o = 0; i < source.size(); ++i) {
        hexData[o++] = toHex(data[i] >> 4);
        hexData[o++] = toHex(data[i] & 0xf);

        if ((separator) && (o < length))
            hexData[o++] = separator;
    }
    return hex;
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
