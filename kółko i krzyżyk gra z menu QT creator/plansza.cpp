#include "plansza.h"
#include "ui_plansza.h"
#include "gra.h"
#include "QPixmap"
#include "QDebug"
#include "QProcess"
plansza::plansza(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::plansza)

{

    ui->setupUi(this);

gra k1;
k1.losuj(ui,olubx);
}

plansza::~plansza()
{
    delete ui;

}
/*klasa plansza jest jakby szkieletem calej struktury, to planszą klasa gra manipuluje zmienia wszysciutko
  w niej,plansza to taka jakby kartka do obliczeń na która wpisujemy dane i z niej je odczytujemy a tym który pisze jest gra i odczytuje to tez gra
już nie mam zielonego pojęcia co dodać.*/
void plansza::on_p1_clicked()
{
gra k1;
k1.ustawk1(ui,1,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
k1.sprawdz(ui,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
qDebug()<<k12<<k22<<k23<<k24<<k25<<k26<<k27<<k28<<k29;
}


void plansza::on_p2_clicked()
{
    gra k2;
k2.ustawk1(ui,2,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
k2.sprawdz(ui,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);

}


void plansza::on_p3_clicked()
{
    gra k3;
k3.ustawk1(ui,3,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
k3.sprawdz(ui,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
}


void plansza::on_p4_clicked()
{
    gra k4;
k4.ustawk1(ui,4,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
k4.sprawdz(ui,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
}



void plansza::on_p5_clicked()
{
    gra k5;
k5.ustawk1(ui,5,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
k5.sprawdz(ui,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
}


void plansza::on_p6_clicked()
{
    gra k6;
k6.ustawk1(ui,6,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
k6.sprawdz(ui,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
}


void plansza::on_p7_clicked()
{
    gra k7;
k7.ustawk1(ui,7,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
k7.sprawdz(ui,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
}


void plansza::on_p8_clicked()
{
    gra k8;
k8.ustawk1(ui,8,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
k8.sprawdz(ui,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
}



void plansza::on_p9_clicked()
{
    gra k9;
k9.ustawk1(ui,9,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);
k9.sprawdz(ui,k12,k22,k23,k24,k25,k26,k27,k28,k29,olubx,rem);

}


void plansza::on_powtorka_clicked()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());

}


void plansza::on_wyjscie_clicked()
{
    exit(0);
}


void plansza::on_wyjscie2_clicked()
{
     exit(0);
}


void plansza::on_pushButton_clicked()
{
    plansza::~plansza();// destruktor zeby zniszczyc obiekt i aktualny stan gry
    mMyNewWindow = new plansza;
    mMyNewWindow -> show();

}

