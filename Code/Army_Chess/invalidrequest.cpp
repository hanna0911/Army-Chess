#include "invalidrequest.h"
#include "ui_invalidrequest.h"

InvalidRequest::InvalidRequest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InvalidRequest)
{
    ui->setupUi(this);
}

InvalidRequest::~InvalidRequest()
{
    delete ui;
}
