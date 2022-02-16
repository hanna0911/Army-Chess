#include "invalidinput.h"
#include "ui_invalidinput.h"

InvalidInput::InvalidInput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InvalidInput)
{
    ui->setupUi(this);
}

InvalidInput::~InvalidInput()
{
    delete ui;
}
