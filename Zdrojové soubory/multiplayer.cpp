#include "multiplayer.h"
#include "ui_multiplayer.h"
#include "window.h"
#include "memory_game.h"
#include "card.h"
#include <QMessageBox>
#include <QNetworkInterface>
#include <QTimer>

Multiplayer::Multiplayer(Window* p, Memory_game* g) :
    QWidget(p),
    ui(new Ui::Multiplayer)
{
    ui->setupUi(this);
    hide_objects(false);
    connect(ui->server_btn, SIGNAL(clicked()), this, SLOT(server_m()));
    connect(ui->client_btn, SIGNAL(clicked()), this, SLOT(ip_le_visible()));
    connect(ui->ip_btn, SIGNAL(clicked()), this, SLOT(client_m()));
    connect(ui->back_btn, SIGNAL(clicked()), this, SLOT(back()));
    game = g;
    parent = p;
}

void Multiplayer::hide_objects(bool t)
{
    ui->ip_btn->setVisible(t);
    ui->ip_lbl->setVisible(t);
    ui->ip_le->setVisible(t);
    ui->waiting_lbl->setVisible(t);
}

void Multiplayer::client_m()
{
    if(ui->ip_le->text() == "")
        return;
    role = 1;
    socket = new QTcpSocket(this);
    socket->connectToHost(ui->ip_le->text(),1234);
    if(!socket->waitForConnected(500))
    {
        QMessageBox::critical(this, "Spojení", "Nelze navázat spojení se serverem.");
        delete socket;
        return;
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(ready_read()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(errorOccurred()), this, SLOT(disply_error()));
}

void Multiplayer::server_m()
{
    role = 0;
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, 1234))
    {
        QMessageBox::critical(this, "Spojení", "Nelze zapnout server");
    }
    QString ip_adress;
    QList<QHostAddress> ip_list = QNetworkInterface::allAddresses();
    for (int i = 0; i < ip_list.size(); ++i)
    {
         if (ip_list.at(i) != QHostAddress::LocalHost && ip_list.at(i).toIPv4Address())
         {
               ip_adress = ip_list.at(i).toString();
               break;
         }
    }
    if (ip_adress.isEmpty())
         ip_adress = QHostAddress(QHostAddress::LocalHost).toString();

    ui->waiting_lbl->setText("Server běží. IP adresa:  "+ ip_adress +" LAN: 127.0.0.1 (Port: 1234 nastaven automaticky)");
    ui->client_btn->setDisabled(true);
    ui->server_btn->setDisabled(true);
    ui->waiting_lbl->setVisible(true);

    connect(server, SIGNAL(newConnection()), this, SLOT(start_info()));
}

void Multiplayer::display_error()
{
    QMessageBox::information(this, "Chyba připojení", "Nastala chyba při připojení");
}

void Multiplayer::ready_read()
{
    QString data = socket->readLine();

    if(data.length() == 3)
    {
        flip_at(data);
    }
    else if(data.length() >= 9)
    {
        parent->game_form();
        start_multiplayer(data);
    }
}

void Multiplayer::send(QString data)
{
    socket->write(data.toUtf8());
}

void Multiplayer::start_info()
{
    game->generate(parent->get_num());
    game->multi = true;

    QString temp = "o";
    for(int i = 0; i < 2*parent->get_num()-1; i++)
    {
        temp = temp + QString::number(game->field[i]) + "x";
    }
    temp = temp + QString::number(game->field[2*parent->get_num()-1]);
    QString result = "startx" + QString::number(parent->get_mr()) + "x" + QString::number(parent->get_num()) + temp;

    socket =  server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(ready_read()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    send(result);

    start_multiplayer(result);
    game->wait = false;
    parent->game_form();
}

void Multiplayer::ip_le_visible()
{
    ui->ip_btn->setVisible(true);
    ui->ip_lbl->setVisible(true);
    ui->ip_le->setVisible(true);
    ui->client_btn->setDisabled(true);
    ui->server_btn->setDisabled(true);

}

Multiplayer::~Multiplayer()
{
    delete ui;
}

void Multiplayer::start_multiplayer(QString data)
{
    QStringList temp = data.split("o");
    QStringList sets = temp.at(0).split("x");
    QStringList numbers = temp.at(1).split("x");

    int num = sets.at(2).toInt();
    int x = 1;
    int y = 0;
    game->number_active = num;
    game->b = sets.at(1).toInt();
    game->multi = true;

    for(int i = 0; i < 2*num; i++)
    {
        y ++;

        Card* temp = new Card(x,y, numbers.at(i).toInt());
        temp->setFixedSize(75,75);
        game->set_marker(temp);
        connect(temp, SIGNAL(clicked()),this, SLOT(flip_card_multiplayer()));
        game->add_card(temp, x, y);
        if(y == 5)
        {
            x++;
            y = 0;
        }
        temp = nullptr;
    }
    game->wait = true;
}

void Multiplayer::flip_card_multiplayer()
{
    if(game->wait == false)
    {
        auto temp = qobject_cast<Card*>(sender());
        game->picture(temp);
        game->flipped_add(temp);
        temp->setDisabled(true);
        send(QString::number(temp->get_x())+ "x" + QString::number(temp->get_y()));
        if(game->check_same())
        {
            game->wait = true;
            if(game->number_active == 1)
            {
                hide_objects(false);
                ui->client_btn->setDisabled(false);
                ui->server_btn->setDisabled(false);
            }
            QTimer::singleShot(500, game, SLOT(same_cards()));
        }
        else if(game->flipped[0] != nullptr && game->flipped[1]!= nullptr)
        {
            game->wait = true;
            QTimer::singleShot(1500, game, SLOT(unflip()));
        }
    }
}

void Multiplayer::back()
{
    if(server != nullptr)
    {
        delete server;
        server = nullptr;
    }
    hide_objects(false);
    ui->client_btn->setDisabled(false);
    ui->server_btn->setDisabled(false);
    parent->menu();
}

void Multiplayer::disconnected()
{
    hide_objects(false);
    ui->client_btn->setDisabled(false);
    ui->server_btn->setDisabled(false);
    game->clean();
    parent->menu();
    QMessageBox::critical(this, "Spojení", "Spojení s druhým hráčem bylo ztraceno");
}

void Multiplayer::flip_at(QString data)
{
    QStringList coor = data.split("x");
    int x = coor.at(0).toInt();
    int y = coor.at(1).toInt();
    Card* temp = game->card_at(x,y);
    game->picture(temp);
    game->flipped_add(temp);
    if(game->check_same())
    {
        if(game->number_active == 1)
        {
            hide_objects(false);
            ui->client_btn->setDisabled(false);
            ui->server_btn->setDisabled(false);
        }
        QTimer::singleShot(500, game, SLOT(same_cards()));
        game->wait = false;
    }
    else if(game->flipped[0] != nullptr && game->flipped[1]!= nullptr)
    {
        QTimer::singleShot(1500, game, SLOT(unflip()));
        game->wait = false;
    }
}

