#include "window.h"
#include "start_menu.h"
#include "options.h"
#include "memory_game.h"
#include<QStackedWidget>
#include<QGridLayout>
#include<QWidget>
#include<QPushButton>

Window::Window(QWidget *parent)
    : QMainWindow(parent), opt_settings("Organization", "Memory Game")
{
    if(opt_settings.contains("main_rule") == false)
    {
        opt_settings.setValue("main_rule", QString::number(1));
        opt_settings.setValue("number_of_cards", QString::number(5));
    }

    forms = new QStackedWidget();
    form_1 = new Start_menu(this);
    form_2 = new Options(this, opt_settings.value("number_of_cards").toInt(), opt_settings.value("main_rule").toInt());
    form_3 = new Memory_game(this);
    form_4 = new Multiplayer(this, form_3);

    forms->addWidget(form_1);
    forms->addWidget(form_2);
    forms->addWidget(form_3);
    forms->addWidget(form_4);
    forms->setCurrentIndex(0);
    this->setCentralWidget(forms);
}

void Window::change()
{
    auto temp = static_cast<QPushButton*>(sender());
    QString temp_name = temp->objectName();
    if(temp_name == "option_btn")
    {
        forms->setCurrentIndex(1);
    }
    else if(temp_name == "back_btn")
    {
        forms->setCurrentIndex(0);
    }
    else if(temp_name == "online_btn")
    {
        forms->setCurrentIndex(3);
    }
    else if(temp_name == "hotseat_btn")
    {
        forms->setCurrentIndex(2);
        form_3->start_of_game(opt_settings.value("number_of_cards").toInt(), opt_settings.value("main_rule").toInt());
    }
}

void Window::menu()
{
    forms->setCurrentIndex(0);
}

void Window::save_settings()
{
    int* temp = form_2->save();
    opt_settings.setValue("main_rule", temp[0]);
    opt_settings.setValue("number_of_cards", temp[1]);
}

void Window::game_form()
{
    forms->setCurrentIndex(2);
}

int Window::get_mr()
{
    return opt_settings.value("main_rule").toInt();
}

int Window::get_num()
{
    return opt_settings.value("number_of_cards").toInt();
}

Window::~Window()
{
}

