#include "abou_dialog.h"
#include "ui_abou_dialog.h"

abou_Dialog::abou_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::abou_Dialog)
{
    ui->setupUi(this);
}

abou_Dialog::~abou_Dialog()
{
    delete ui;
}
