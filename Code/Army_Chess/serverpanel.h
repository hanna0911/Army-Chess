#ifndef SERVERPANEL_H
#define SERVERPANEL_H

#include <QDialog>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>
#include <QDebug>
#include "mainwindow.h"

namespace Ui {
class ServerPanel;
}

class ServerPanel : public QDialog
{
    Q_OBJECT

public:
    explicit ServerPanel(QWidget *parent = nullptr);
    ~ServerPanel();

private:
    Ui::ServerPanel *ui;
    QString getIP();
};

#endif // SERVERPANEL_H
