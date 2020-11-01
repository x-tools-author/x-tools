#ifndef SAKMODBUSCOMMONSERIALPORTSECTION_HH
#define SAKMODBUSCOMMONSERIALPORTSECTION_HH

#include <QWidget>

namespace Ui {
    class SAKModbusCommonSerialPortSection;
}

class SAKModbusCommonSerialPortSection : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonSerialPortSection(QWidget *parent = nullptr);
    ~SAKModbusCommonSerialPortSection();
private:
    Ui::SAKModbusCommonSerialPortSection *ui;
};

#endif // SAKMODBUSCOMMONSERIALPORTSECTION_HH
