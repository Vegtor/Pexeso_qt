#ifndef START_MENU_H
#define START_MENU_H

#include <QWidget>
class QPushButton;

namespace Ui {
class Start_menu;
}

class Start_menu : public QWidget
{
    Q_OBJECT
    Ui::Start_menu *ui;

public:
    QPushButton* get_button();
    explicit Start_menu(QWidget *parent);
    ~Start_menu();
};

#endif // START_MENU_H
