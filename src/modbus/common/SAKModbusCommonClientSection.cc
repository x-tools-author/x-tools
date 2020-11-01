#include "SAKModbusCommonClientSection.hh"
#include "ui_SAKModbusCommonClientSection.h"

SAKModbusCommonClientSection::SAKModbusCommonClientSection(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonClientSection)
{
    ui->setupUi(this);
}

SAKModbusCommonClientSection::~SAKModbusCommonClientSection()
{
    delete ui;
}
