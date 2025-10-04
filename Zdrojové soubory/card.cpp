#include "card.h"

Card::Card(int x, int y, int t, QWidget* parent) : QPushButton(parent)
{
    coordinates[0] = x;
    coordinates[1] = y;
    type = t;
}

int Card::get_x() const
{
    return coordinates[0];
}

int Card::get_y() const
{
    return coordinates[1];
}

void Card::set_type(int x)
{
    type = x;
}

int Card::get_type() const
{
    return type;
}
