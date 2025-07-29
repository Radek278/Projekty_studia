#include "menu.h"
#include "plansza.h"
#include "ui_menu.h"
#include "ui_plansza.h"
#include <QApplication>
#include <gra.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    menu w;

    w.show();


    return a.exec();
}
