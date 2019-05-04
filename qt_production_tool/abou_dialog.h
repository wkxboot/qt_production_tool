#ifndef ABOU_DIALOG_H
#define ABOU_DIALOG_H

#include <QDialog>

namespace Ui {
class abou_Dialog;
}

class abou_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit abou_Dialog(QWidget *parent = nullptr);
    ~abou_Dialog();

private:
    Ui::abou_Dialog *ui;
};

#endif // ABOU_DIALOG_H
