#include "offlinenotice.h"
#include "ui_offlinenotice.h"

OfflineNotice::OfflineNotice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OfflineNotice)
{
    ui->setupUi(this);
}

OfflineNotice::~OfflineNotice()
{
    delete ui;
}
