#include <QFile>

#include "SAKMainWindow.hh"
#include "ui_SAKMainWindow.h"

#include "UdpSAKIODeviceWidget.hh"
#include "TcpSAKIODeviceWidget.hh"
#include "TcpServerSAKIODeviceWidget.hh"
#include "nslookup.h"
#include "GetPublicIPWidget.h"
#include "SAKVersion.hh"
#include "UpdateManager.h"
#include "CRCCalculator.hh"
#include "SAKApplication.hh"
#include "MoreInformation.hh"
#include "SAKConsole.hh"
#include "SAKGlobal.hh"
#include "QtStyleSheetApi.hh"

#ifndef SAK_NO_SERIALPORT_ASSISTANT
#include "SerialportSAKIODeviceWidget.hh"
#endif

#include <QStyleFactory>
#include <QSettings>
#include <QMessageBox>
#include <QTabBar>
#include <QSpacerItem>
#include <QMetaEnum>
#include <QAction>

const static char* configureFile = "http://wuhai.pro/software/QtSwissArmyKnife/update.json";
const char* SAKMainWindow::appStyleKey = "Universal/appStyle";
SAKMainWindow::SAKMainWindow(QWidget *parent)
    :QMainWindow (parent)
    ,mpTabWidget (new QTabWidget)
    ,ui (new Ui::SAKMainWindow)
    ,versionDialog (new SAKVersion)
    ,moreInformation (new MoreInformation)
{
    ui->setupUi(this);
    updateManager = new UpdateManager(QUrl(configureFile), SAKVersion::instance()->getVersion());

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(mpTabWidget);
    ui->centralWidget->setLayout(layout);
    setWindowTitle(tr("瑞士军刀--开发调试工具集")
                   + " v" + versionDialog->getVersion()
                   + QString(" (QQ交流群%1 )").arg(versionDialog->getQQNum())
                   + QString(" 作者邮箱 %1").arg(versionDialog->getEmail()));

    this->resize(800, 600);
    this->setMinimumWidth(1024);

    AddTab();
    InitMenu();
    AddTool();
}

SAKMainWindow::~SAKMainWindow()
{
    delete ui;
}

void SAKMainWindow::AddTab()
{
#ifndef SAK_NO_SERIALPORT_ASSISTANT
    /// 串口页(不支持winrt、树莓派...)
    SerialportSAKIODevice *serialportDevice = new SerialportSAKIODevice;
    this->mpTabWidget->addTab(new SerialportSAKIODeviceWidget(serialportDevice, new SerialportSAKIODeviceControler), tr("串口助手"));
    serialportDevice->start();
#endif

    /// tcp客户端
    TcpSAKIODevice *tcpDevice = new TcpSAKIODevice;
    this->mpTabWidget->addTab(new TcpSAKIODeviceWidget(tcpDevice, new TcpSAKIODeviceControler), tr("TCP客户端"));
    tcpDevice->start();

    /// TCP 服务器
    TcpServerSAKIODevice *tcpServerDevice = new TcpServerSAKIODevice;
    this->mpTabWidget->addTab(new TcpServerSAKIODeviceWidget(tcpServerDevice, new TcpServerSAKIODeviceControler), tr("Tcp服务器"));
    tcpServerDevice->start();

    /// udp客户端
    UdpSAKIODevice *udpDevice = new UdpSAKIODevice;
    this->mpTabWidget->addTab(new UdpSAKIODeviceWidget(udpDevice, new UdpSAKIODeviceControler), tr("UDP客户端"));
    udpDevice->start();

    /// 终端输出
    this->mpTabWidget->addTab(new SAKConsole, tr("终端"));
}

void SAKMainWindow::AddTool()
{
    addTool(tr("域名转IP"),        new ToolsNsLookup);
    addTool(tr("公网IP获取工具"),   new GetPublicIPWidget);

    QAction *action = nullptr;
    action = new QAction("CRC计算器", this);
    toolsMenu->addAction(action);
    connect(action, &QAction::triggered, this, &SAKMainWindow::createCRCCalculator);
}

void SAKMainWindow::InitMenu()
{
    /// 文件菜单
    QMenu *pFileMenu = new QMenu(tr("文件"));
    menuBar()->addMenu(pFileMenu);

    QMenu *tabMenu = new QMenu(tr("新建页面"), this);
    pFileMenu->addMenu(tabMenu);
    QMetaEnum enums = QMetaEnum::fromType<SAKGlobal::SAKEnumIODeviceType>();
    for (int i = 0; i < enums.keyCount(); i++){
        QAction *a = new QAction(SAKGlobal::getIODeviceTypeName(i), this);
        a->setObjectName(SAKGlobal::getIODeviceTypeName(i));
        QVariant var = QVariant::fromValue<int>(enums.value(i));
        connect(a, &QAction::triggered, this, &SAKMainWindow::addRemovablePage);
        tabMenu->addAction(a);
    }

    QMenu *windowMenu = new QMenu(tr("新建窗口"), this);
    pFileMenu->addMenu(windowMenu);
    for (int i = 0; i < enums.keyCount(); i++){
        QAction *a = new QAction(SAKGlobal::getIODeviceTypeName(i), this);
        a->setObjectName(SAKGlobal::getIODeviceTypeName(i));
        QVariant var = QVariant::fromValue<int>(enums.value(i));
        connect(a, &QAction::triggered, this, &SAKMainWindow::openIODeviceWindow);
        windowMenu->addAction(a);
    }

    pFileMenu->addSeparator();

    QAction *pExitAction = new QAction(tr("退出"));
    pFileMenu->addAction(pExitAction);
    connect(pExitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    /// 工具菜单
    QMenu *pToolMenu = new QMenu(tr("工具"));
    menuBar()->addMenu(pToolMenu);
    toolsMenu = pToolMenu;

    /// 选项
    QMenu *optionMenu = new QMenu(tr("选项"));
    menuBar()->addMenu(optionMenu);

    /// 选项菜单--皮肤切换菜单
    initSkinMenu(optionMenu);

#if 1
    QSettings settings;
    QString value = settings.value(appStyleKey).toString();
    /// 默认使用Qt支持的软件风格的第一种软件风格
    if (value.isEmpty()){
        foreach (QString style,  QStyleFactory::keys()){
            value = style;
            break;
        }
    }

    QMenu *appStyleMenu = new QMenu(tr("软件风格"));
    optionMenu->addMenu(appStyleMenu);
    QStringList styleKeys = QStyleFactory::keys();
    QAction *styleAction;
    QActionGroup *styleActionGroup = new QActionGroup(this);
    foreach (QString style, styleKeys) {
        styleAction = new QAction(style);
        styleAction->setCheckable(true);
        styleActionGroup->addAction(styleAction);
        styleAction->setObjectName(style);
        appStyleMenu->addAction(styleAction);
        connect(styleAction, SIGNAL(triggered(bool)), this, SLOT(styleActionTriggered()));

        if (style.compare(value) == 0){
            styleAction->setChecked(true);
        }
    }
#endif

    // 帮助菜单
    QMenu *pHelpMenu = new QMenu(tr("帮助"));
    menuBar()->addMenu(pHelpMenu);

    QAction *pAboutQtAction = new QAction(tr("关于Qt"));
    pHelpMenu->addAction(pAboutQtAction);
    connect(pAboutQtAction, SIGNAL(triggered(bool)), this, SLOT(AboutQt()));

    QAction *pAboutAction = new QAction(tr("关于软件"));
    pHelpMenu->addAction(pAboutAction);
    connect(pAboutAction, SIGNAL(triggered(bool)), this, SLOT(About()));

    QAction *pUpdateAction = new QAction(tr("检查更新"));
    pHelpMenu->addAction(pUpdateAction);
    connect(pUpdateAction, SIGNAL(triggered(bool)), updateManager, SLOT(checkForUpdate()));

    QAction *pMoreInformation = new QAction(tr("更多信息"));
    pHelpMenu->addAction(pMoreInformation);
    connect(pMoreInformation, SIGNAL(triggered(bool)), moreInformation, SLOT(show()));
}

void SAKMainWindow::About()
{
    versionDialog->show();
    versionDialog->activateWindow();
}

void SAKMainWindow::AboutQt()
{
    QMessageBox::aboutQt(this, tr("关于Qt"));
}

void SAKMainWindow::styleActionTriggered()
{
    QSettings setting;
    setting.setValue(QString(appStyleKey), sender()->objectName());

#if 1
    int ret = QMessageBox::information(nullptr, tr("请手动重启软件"),
                                       tr("更改应用样式完成，重启生效，是否手动重启软件！(点击“是”将关闭软件。)"),
                                       QMessageBox::Yes|QMessageBox::No);
    if (ret == QMessageBox::Yes){
        close();
    }
#else
    /**
     *  频繁切换导致程序crash，原因不详...........(Fusion样式切换有问题)
     *  由其它风格切换至function风格或者有function风格切换至其它风格造成程序崩溃
     *  tmd
     */
    qDebug() << sender()->objectName();
    QString style = sender()->objectName();
    SAKApplication::setStyle(QStyleFactory::create(style));
#endif
}

void SAKMainWindow::addTool(QString toolName, QWidget *toolWidget)
{
    QAction *action = new QAction(toolName);
    toolsMenu->addAction(action);
    connect(action, SIGNAL(triggered(bool)), toolWidget, SLOT(show()));
}

void SAKMainWindow::changeStylesheet()
{
    /*
     * 样式文件来自互联网，并非本软件作者编写，如有侵权，请联系软件作者删除。
     * https://blog.csdn.net/feiyangqingyun/article/details/80211690
     */

    QFile file;
    QString skin = sender()->objectName();

    QSettings setting;
    setting.setValue(QString(appStylesheetKey), skin);

    if (skins.keyToValue(skin.toLatin1().data()) == QtDefault){
        qApp->setStyleSheet("");
        return;
    }else if (skins.keyToValue(skin.toLatin1().data()) == FlatWhite){
        file.setFileName(":/qss/flatwhite.css");
    }else if (skins.keyToValue(skin.toLatin1().data()) == LightBlue){
        file.setFileName(":/qss/lightblue.css");
    }else if (skins.keyToValue(skin.toLatin1().data()) == PSBlack){
        file.setFileName(":/qss/psblack.css");
    }else {

    }

    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}

void SAKMainWindow::initSkinMenu(QMenu *optionMenu)
{
    skins = QMetaEnum::fromType<SAKStyleSheet>();
    QSettings settings;
    QString value = settings.value(appStylesheetKey).toString();
    if (value.isEmpty()){
        value = QString(skins.valueToKey(LightBlue));
    }

    QMenu *stylesheetMenu = new QMenu("皮肤");
    optionMenu->addMenu(stylesheetMenu);
    stylesheetMenu->addActions(QtStyleSheetApi::instance()->actions());
//    QActionGroup *stylesheetActionGroup = new QActionGroup(this);
//    QAction *action = nullptr;
//    for (int i = 0; i < skins.keyCount(); i++){
//        action = new QAction(QString(skins.valueToKey(i)));
//        action->setCheckable(true);
//        action->setObjectName(QString(skins.valueToKey(i)));
//        stylesheetMenu->addAction(action);
//        stylesheetActionGroup->addAction(action);

//        connect(action, &QAction::triggered, this, &SAKMainWindow::changeStylesheet);
//        if (QString(skins.valueToKey(i)).compare(value) == 0){
//            action->setChecked(true);
//            emit action->triggered();
//        }
//    }
}

void SAKMainWindow::addRemovablePage()
{
    qDebug() << sender()->objectName();
}

void SAKMainWindow::openIODeviceWindow()
{
#if 0
    int type = qobject_cast<QAction*>(sender())->data().value<int>();

    switch (type) {
    case SAKGlobal::SAKEnumIODeviceTypeUDP:
        break;
    case SAKGlobal::SAKEnumIODeviceTypeTCPClient:
        break;
    case SAKGlobal::SAKEnumIODeviceTypeTCPServer:
        break;
#ifndef SAK_NO_SERIALPORT_ASSISTANT
    case SAKGlobal::SAKEnumIODeviceTypeSerialport:
        SerialportSAKIODevice *serialportDevice = new SerialportSAKIODevice;
        SerialportSAKIODeviceWidget *widget = new SerialportSAKIODeviceWidget(serialportDevice, new SerialportSAKIODeviceControler);
        widget->setWindowTitle(sender()->objectName());
        widget->show();
        break;
#endif
    }
#endif
}

void SAKMainWindow::createCRCCalculator()
{
    CRCCalculator *cal = new CRCCalculator;
    cal->show();
}
