#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>

namespace Ui{
class Console;
}

class Console : public QWidget
{
    Q_OBJECT
public:
    Console(QWidget *parent = Q_NULLPTR);
    ~Console();
    /// -----------------------------------------
    void OutputInfo(QString info);
    static Console *_console;
private:
    Ui::Console *ui;
};

Console *appConsole();

#endif  // CONSOLE_H
