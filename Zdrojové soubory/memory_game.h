#ifndef MEMORY_GAME_H
#define MEMORY_GAME_H

#include <QWidget>
#include "multiplayer.h"
class Card;

namespace Ui {
class Memory_game;
}

class Memory_game : public QWidget
{
    Q_OBJECT
    bool wait = false;
    int* field;
    int number_active;
    Card* flipped[2] = {nullptr, nullptr};
    bool b;
    QWidget* p;
    bool multi = false;

    void generate(int num);
    void flipped_add(Card* t);
    void picture(Card* t);
    bool check_same();
    void set_marker(Card* t);
    void change_player();
    void end_of_game();
    void clean();
    void add_card(Card* temp, int x, int y);
public slots:
    void start_of_game(int number, bool mr);
private slots:
    void flip_card();
    void same_cards();
    void unflip();
    Card* card_at(int x, int y);
public:
    explicit Memory_game(QWidget *parent);
    ~Memory_game();

private:
    Ui::Memory_game *ui;

    friend class Multiplayer;
};

#endif // MEMORY_GAME_H
