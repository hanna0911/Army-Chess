#include "clientpanel.h"
#include "ui_clientpanel.h"
#include <QDebug>
#include <QKeyEvent>

ClientPanel::ClientPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientPanel)
{
    ui->setupUi(this);
    QRegularExpression regx("[0-9][.0-9]{0,62}"); // numbers and . only //QRegExp regx("[0-9][.0-9]{0,62}"); // Qt 5
    QValidator *val = new QRegularExpressionValidator(regx, ui->lineEdit); //QValidator *val = new QRegExpValidator(regx, ui->lineEdit); // Qt 5
    ui->lineEdit->setValidator(val);
}

QString ClientPanel::returnIP(){
    return ui->lineEdit->text();
}

void ClientPanel::on_pushButton_1_clicked(){
    ui->lineEdit->insert("1");
}

void ClientPanel::on_pushButton_2_clicked(){
    ui->lineEdit->insert("2");
}

void ClientPanel::on_pushButton_3_clicked(){
    ui->lineEdit->insert("3");
}

void ClientPanel::on_pushButton_4_clicked(){
    ui->lineEdit->insert("4");
}

void ClientPanel::on_pushButton_5_clicked(){
    ui->lineEdit->insert("5");
}

void ClientPanel::on_pushButton_6_clicked(){
    ui->lineEdit->insert("6");
}

void ClientPanel::on_pushButton_7_clicked(){
    ui->lineEdit->insert("7");
}

void ClientPanel::on_pushButton_8_clicked(){
    ui->lineEdit->insert("8");
}

void ClientPanel::on_pushButton_9_clicked(){
    ui->lineEdit->insert("9");
}

void ClientPanel::on_pushButton_0_clicked(){
    ui->lineEdit->insert("0");
}

void ClientPanel::on_pushButton_dot_clicked(){
    ui->lineEdit->insert(".");
}

void ClientPanel::on_pushButton_delete_clicked(){
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
    ui->lineEdit->setFocus();
    QApplication::sendEvent(focusWidget(), event);
}

ClientPanel::~ClientPanel()
{
    delete ui;
}
