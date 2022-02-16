#include "havewinner.h"
#include "ui_havewinner.h"

HaveWinner::HaveWinner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HaveWinner)
{
    ui->setupUi(this);
}

void HaveWinner::show_text(QString text){
    ui->winnerLb->setText(text);
}

HaveWinner::~HaveWinner()
{
    delete ui;
}
