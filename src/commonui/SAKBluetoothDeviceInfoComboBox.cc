#include <QBluetoothDeviceInfo>
#include "SAKBluetoothDeviceInfoComboBox.hh"

SAKBluetoothDeviceInfoComboBox::SAKBluetoothDeviceInfoComboBox(QWidget *parent)
    : SAKComboBox(parent)
    , mScanner(Q_NULLPTR)
{
    mScanner = new SAKBleScanner(this);
    connect(mScanner, &SAKBleScanner::devicesInfoListChanged, this, [=](){
        if (!isEnabled()) {
            return;
        }

        clear();
        auto infos = mScanner->devicesInfoList();
        for (auto &info : infos) {
            QString name = mScanner->deviceName(info);
            addItem(name, info);
        }
    });
}

SAKBluetoothDeviceInfoComboBox::~SAKBluetoothDeviceInfoComboBox()
{
    mScanner->stopDiscover();
}

void SAKBluetoothDeviceInfoComboBox::startDiscover()
{
    mScanner->startDiscover();
}

void SAKBluetoothDeviceInfoComboBox::stopDiscover()
{
    mScanner->stopDiscover();
}

bool SAKBluetoothDeviceInfoComboBox::isActive()
{
    return mScanner->isActive();
}

void SAKBluetoothDeviceInfoComboBox::setTimeoutInterval(int interval)
{
    mScanner->setTimeoutInterval(interval);
}

void SAKBluetoothDeviceInfoComboBox::setNameFiltter(const QString &filtter)
{
    mScanner->setNameFiltter(filtter);
}
