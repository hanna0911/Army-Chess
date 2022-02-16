#ifndef DISCONNECTNOTICE_H
#define DISCONNECTNOTICE_H

#include <QDialog>

namespace Ui {
class DisconnectNotice;
}

class DisconnectNotice : public QDialog
{
    Q_OBJECT

public:
    explicit DisconnectNotice(QWidget *parent = nullptr);
    ~DisconnectNotice();

private:
    Ui::DisconnectNotice *ui;
};

#endif // DISCONNECTNOTICE_H
