#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent, int num, int b) :
    QWidget(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    ui->main_rule_chb->setChecked(b);
    ui->number_of_cards_spb->setValue(num);
    connect(ui->back_btn, SIGNAL(clicked()), parent, SLOT(change()));
    connect(ui->save_btn, SIGNAL(clicked()), parent, SLOT(save_settings()));
}

int* Options::save()
{
    static int temp[2] = {ui->main_rule_chb->isChecked(), ui->number_of_cards_spb->value()};
    return temp;
}

Options::~Options()
{
    delete ui;
}
