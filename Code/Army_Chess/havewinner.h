#ifndef HAVEWINNER_H
#define HAVEWINNER_H

#include <QDialog>

namespace Ui {
class HaveWinner;
}

class HaveWinner : public QDialog
{
    Q_OBJECT

public:
    explicit HaveWinner(QWidget *parent = nullptr);
    Ui::HaveWinner *ui;
    void show_text(QString);
    ~HaveWinner();
};

#endif // HAVEWINNER_H
