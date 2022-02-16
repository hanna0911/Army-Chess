#ifndef OFFLINENOTICE_H
#define OFFLINENOTICE_H

#include <QDialog>

namespace Ui {
class OfflineNotice;
}

class OfflineNotice : public QDialog
{
    Q_OBJECT

public:
    explicit OfflineNotice(QWidget *parent = nullptr);
    ~OfflineNotice();

private:
    Ui::OfflineNotice *ui;
};

#endif // OFFLINENOTICE_H
