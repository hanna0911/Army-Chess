#ifndef INVALIDINPUT_H
#define INVALIDINPUT_H

#include <QDialog>

namespace Ui {
class InvalidInput;
}

class InvalidInput : public QDialog
{
    Q_OBJECT

public:
    explicit InvalidInput(QWidget *parent = nullptr);
    ~InvalidInput();

private:
    Ui::InvalidInput *ui;
};

#endif // INVALIDINPUT_H
