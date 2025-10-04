#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QSettings>

class QStackedWidget;
class QGridLayout;
class Start_menu;
class Options;
class Memory_game;
class Multiplayer;

class Window : public QMainWindow
{
    Q_OBJECT
    QStackedWidget* forms;
    QSettings opt_settings;
    Start_menu* form_1;
    Options* form_2;
    Memory_game* form_3;
    Multiplayer* form_4;

public slots:
    void change();
    void save_settings();
    void menu();
    void game_form();
public:
    int get_num();
    int get_mr();
    Window(QWidget *parent = nullptr);
    ~Window();
};
#endif // WINDOW_H
