#ifndef SAKSERIALPORTTOOLBOXUI_H
#define SAKSERIALPORTTOOLBOXUI_H

#include "SAKToolBoxUi.hh"
#include "SAKSerialPortToolBoxUiController.hh"

class SAKSerialPortToolBoxUi : public SAKToolBoxUi
{
    Q_OBJECT
public:
    SAKSerialPortToolBoxUi(QWidget *parent = Q_NULLPTR);

protected:
    virtual QWidget *controller() final;

private:
    SAKSerialPortToolBoxUiController *mController{nullptr};
};

#endif // SAKSERIALPORTTOOLBOXUI_H
