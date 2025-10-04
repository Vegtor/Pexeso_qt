#ifndef CARD_H
#define CARD_H

#include <QPushButton>

class Card : public QPushButton
{
    Q_OBJECT
    int coordinates[2];
    int type;
public:
    int get_x() const;
    int get_y() const;
    void set_type(int x);
    int get_type() const;
    Card(int x, int y, int t, QWidget* parent = 0);

};

#endif // CARD_H
