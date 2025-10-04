#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>

namespace Ui {
class Options;
}

class Options : public QWidget
{
    Q_OBJECT
    Ui::Options *ui;

public:
    explicit Options(QWidget *parent, int num, int b);
    int* save();
    ~Options();
};

#endif // OPTIONS_H
