#ifndef PLANSZA_H
#define PLANSZA_H

#include <QMainWindow>
#include "QPixmap"



namespace Ui {
class plansza;
}

class plansza :public QMainWindow
{
    Q_OBJECT

public:
    // wywołałem wszystkie zmienne typu char oraz remis bo nie wiem jak zrobić to inaczej na ten moment :)
     char olubx1;
     char &olubx=olubx1;
        char k1='p';
     char &k12=k1;
    int remis=0;
    int &rem=remis;
         char k2='p';
         char &k22=k2;
         char *k11=&k2;
        char k3='p';
        char &k23=k3;
       char k4='p';
       char &k24=k4;
         char k5='p';
         char &k25=k5;
       char k6='p';
       char &k26=k6;
         char k7='p';
         char &k27=k7;
         char k8='p';
         char &k28=k8;
         char k9='p';
         char &k29=k9;
    explicit plansza(QWidget *parent = nullptr);
    ~plansza();

 friend class gra;

private slots:


    void on_p1_clicked();
    void on_p2_clicked();

    void on_p3_clicked();

    void on_p4_clicked();

    void on_p5_clicked();

    void on_p6_clicked();

    void on_p7_clicked();

    void on_p8_clicked();

    void on_p9_clicked();

    void on_powtorka_clicked();

    void on_wyjscie_clicked();

    void on_wyjscie2_clicked();

    void on_pushButton_clicked();

private:
     Ui::plansza *ui;
 plansza *mMyNewWindow;
};

#endif // PLANSZA_H
