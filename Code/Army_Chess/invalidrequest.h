#ifndef INVALIDREQUEST_H
#define INVALIDREQUEST_H

#include <QDialog>

namespace Ui {
class InvalidRequest;
}

class InvalidRequest : public QDialog
{
    Q_OBJECT

public:
    explicit InvalidRequest(QWidget *parent = nullptr);
    ~InvalidRequest();

private:
    Ui::InvalidRequest *ui;
};

#endif // INVALIDREQUEST_H
