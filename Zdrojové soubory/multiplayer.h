#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>

class Window;
class Memory_game;

namespace Ui {
class Multiplayer;
}

class Multiplayer : public QWidget
{
    Q_OBJECT
    QTcpSocket* socket = nullptr;
    QTcpServer* server = nullptr;
    Window* parent;
    Memory_game* game;
    int role;

    void hide_objects(bool t);
private slots:
    void ready_read();
    void display_error();
    void send(QString data);
    void start_info();
    void client_m();
    void server_m();
    void ip_le_visible();
    void start_multiplayer(QString data);
    void flip_card_multiplayer();
    void disconnected();
    void back();
    void flip_at(QString data);
public:
    explicit Multiplayer(Window* p, Memory_game* g);
    ~Multiplayer();

private:
    Ui::Multiplayer *ui;
};

#endif // MULTIPLAYER_H
