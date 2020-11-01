#ifndef SAKMODBUSCOMMONCLIENTSECTION_HH
#define SAKMODBUSCOMMONCLIENTSECTION_HH

#include <QWidget>

namespace Ui {
    class SAKModbusCommonClientSection;
}
class SAKModbusCommonClientSection : public QWidget
{
    Q_OBJECT
public:
    explicit SAKModbusCommonClientSection(QWidget *parent = nullptr);
    ~SAKModbusCommonClientSection();
private:
    Ui::SAKModbusCommonClientSection *ui;
};

#endif // SAKMODBUSCOMMONCLIENTSECTION_HH
