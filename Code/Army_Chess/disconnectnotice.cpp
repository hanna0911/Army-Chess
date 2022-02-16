#include "disconnectnotice.h"
#include "ui_disconnectnotice.h"

DisconnectNotice::DisconnectNotice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisconnectNotice)
{
    ui->setupUi(this);
}

DisconnectNotice::~DisconnectNotice()
{
    delete ui;
}
