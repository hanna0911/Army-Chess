#include "serverpanel.h"
#include "ui_serverpanel.h"

ServerPanel::ServerPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerPanel)
{
    ui->setupUi(this);
    ui->label_2->clear();
    ui->label_2->setText(getIP());
}

QString ServerPanel::getIP(){
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address, info.addresses()){
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            if(!address.isLoopback()) return address.toString();
        }
    }
    return 0;
}

ServerPanel::~ServerPanel()
{
    delete ui;
}
