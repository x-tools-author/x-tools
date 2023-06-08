#ifndef SAKLOGUI_HH
#define SAKLOGUI_HH

#include <QWidget>

#include "SAKLog.hh"

namespace Ui {
class SAKLogUi;
}

class SAKLogUi : public QWidget
{
    Q_OBJECT

public:
    explicit SAKLogUi(QWidget *parent = nullptr);
    ~SAKLogUi();

private:
    Ui::SAKLogUi *ui;
};

#endif // SAKLOGUI_HH
