#ifndef GRA_H
#define GRA_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "plansza.h"
#include "ui_plansza.h"

class gra
{

public:


    void sprawdz(Ui::plansza *ui,char &k1,char &k2,char &k3,char &k4,char &k5,char &k6,char &k7,char &k8,char &k9,char &olubx,int &remis);//nie mam pomyslu
    char losuj(Ui::plansza *ui,char &olubx);
    void  ustawk1(Ui::plansza *ui,int który_przycisk,char &k1,char &k2,char &k3,char &k4,char &k5,char &k6,char &k7,char &k8,char &k9,char &olubx,int &remis);

gra();

private:

};

#endif // GRA_H
