#include "memory_game.h"
#include "ui_memory_game.h"
#include "card.h"
#include <QTimer>
#include <QMessageBox>
#include <QSpacerItem>
#include <cmath>

Memory_game::Memory_game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Memory_game)
{
    p = parent;
    ui->setupUi(this);
}

Memory_game::~Memory_game()
{
    delete ui;
}

void Memory_game::generate(int num)
{
    this->field = new int[2*num];
    for(int i = 0; i < num; i++)
    {
        field[i] = i+1;
        field[i+num] = i+1;
    }
    srand(time(NULL));
    for(int j = 2*num-1; j>0; j--)
    {
        int k = rand() % (j + 1);
        int temp = field[j];
        field[j] = field[k];
        field[k] = temp;
    }
}

void Memory_game::start_of_game(int number, bool mr)
{
    int num = number;
    int x = 1;
    int y = 0;
    number_active = num;
    b = mr;
    generate(num);
    for(int i = 0; i < 2*num; i++)
    {
        y ++;
        Card* temp = new Card(x,y,field[i]);
        temp->setFixedSize(75,75);
        set_marker(temp);
        connect(temp, SIGNAL(clicked()),this, SLOT(flip_card()));
        ui->field->addWidget(temp, x, y);
        if(y >= sqrt(2*num))
        {
            x++;
            y = 0;
        }
        temp = nullptr;
    }
}

void Memory_game::flip_card()
{
    if(wait == false)
    {
        auto temp = qobject_cast<Card*>(sender());
        picture(temp);
        flipped_add(temp);
        temp->setDisabled(true);
        if(check_same())
        {
            wait = true;
            QTimer::singleShot(500, this, SLOT(same_cards()));
        }
        else if(flipped[0] != nullptr && flipped[1]!= nullptr)
        {
            wait = true;
            QTimer::singleShot(1500, this, SLOT(unflip()));
        }
    }
}

void Memory_game::same_cards()
{
    if(ui->player_turn_lbl->text() == "Hráč 1")
        ui->player_1_points->setText(QString::number(ui->player_1_points->text().toInt() + 1));
    else
        ui->player_2_points->setText(QString::number(ui->player_2_points->text().toInt() + 1));
    int c[2] = {flipped[0]->get_x(), flipped[0]->get_y()};
    int d[2] = {flipped[1]->get_x(), flipped[1]->get_y()};
    delete flipped[0];
    delete flipped[1];
    ui->field->addItem(new QSpacerItem(75,75),c[0],c[1]);
    ui->field->addItem(new QSpacerItem(75,75),d[0],d[1]);
    flipped[0] = nullptr;
    flipped[1] = nullptr;
    number_active--;

    if(number_active == 0)
    {
        end_of_game();
        return;
    }
    else if(multi == false)
    {
        wait = false;
    }
    else if(b)
    {
        change_player();
    }
}

void Memory_game::unflip()
{
    set_marker(flipped[0]);
    flipped[0]->setDisabled(false);
    set_marker(flipped[1]);
    flipped[1]->setDisabled(false);
    flipped[0] = nullptr;
    flipped[1] = nullptr;
    if(multi == false)
        wait = false;
    change_player();
}

void Memory_game::picture(Card* t)
{
    QString path = ":/icons/" + QString::number(t->get_type()) + ".png";
    t->setIcon(QIcon(path));
    t->setIconSize(t->size());
}

void Memory_game::flipped_add(Card* t)
{
    if(this->flipped[0] == nullptr)
        flipped[0] = t;
    else
        flipped[1] = t;
}

bool Memory_game::check_same()
{
    if(flipped[0] == nullptr || flipped[1] == nullptr)
        return false;
    else if (flipped[0]->get_type() == flipped[1]->get_type())
        return true;
    else
        return false;
}

void Memory_game::set_marker(Card* t)
{
    t->setIcon(QIcon(":/icons/q_mark.png"));
    t->setIconSize(t->size());
}

void Memory_game::change_player()
{
    if(ui->player_turn_lbl->text() == "Hráč 1")
        ui->player_turn_lbl->setText("Hráč 2");
    else
        ui->player_turn_lbl->setText("Hráč 1");
}

void Memory_game::end_of_game()
{
    QMessageBox* msgBox = new QMessageBox();
    msgBox->setText("Výsledek hry");
    if(ui->player_1_points->text().toInt() == ui->player_2_points->text().toInt())
        msgBox->setInformativeText("Remíza");
    else if(ui->player_1_points->text().toInt() > ui->player_2_points->text().toInt())
        msgBox->setInformativeText("Vítězem je hráč 1");
    else
        msgBox->setInformativeText("Vítězem je hráč 2");
    connect(msgBox, SIGNAL(buttonClicked(QAbstractButton *)), p, SLOT(menu()));
    msgBox->exec();
    clean();
}

void Memory_game::clean()
{
    ui->player_1_points->setText("0");
    ui->player_2_points->setText("0");
    ui->player_turn_lbl->setText("Hráč 1");
    wait = false;
    multi = false;
    QLayoutItem *child;
    while ((child = ui->field->takeAt(0)) != 0)
    {
        delete child;
    }
}

void Memory_game::add_card(Card* temp, int x, int y)
{
    ui->field->addWidget(temp, x, y);
}

Card* Memory_game::card_at(int x, int y)
{
    return qobject_cast<Card*>(ui->field->itemAtPosition(x,y)->widget());
}
