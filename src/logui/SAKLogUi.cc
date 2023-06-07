#include "SAKLogUi.hh"
#include "ui_SAKLogUi.h"

SAKLogUi::SAKLogUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SAKLogUi)
{
    ui->setupUi(this);
}

SAKLogUi::~SAKLogUi()
{
    delete ui;
}
