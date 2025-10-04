#include "start_menu.h"
#include "ui_start_menu.h"

Start_menu::Start_menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Start_menu)
{
    ui->setupUi(this);
    connect(ui->hotseat_btn, SIGNAL(clicked()), parent, SLOT(change()));
    connect(this->ui->option_btn, SIGNAL(clicked()), parent, SLOT(change()));
    connect(this->ui->online_btn, SIGNAL(clicked()), parent, SLOT(change()));
    connect(ui->end_btn, SIGNAL(clicked()), parent, SLOT(close()));
}

QPushButton* Start_menu::get_button()
{
    return ui->hotseat_btn;
}

Start_menu::~Start_menu()
{
    delete ui;
}
