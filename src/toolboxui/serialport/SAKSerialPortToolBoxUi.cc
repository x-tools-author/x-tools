#include "SAKSerialPortToolBoxUi.hh"

SAKSerialPortToolBoxUi::SAKSerialPortToolBoxUi(QWidget *parent)
    :SAKToolBoxUi(parent)
{
    setWindowTitle(tr("SerialPort"));
    mController = new SAKSerialPortToolBoxUiController(this);

    init();
}

QWidget *SAKSerialPortToolBoxUi::controller()
{
    return mController;
}
