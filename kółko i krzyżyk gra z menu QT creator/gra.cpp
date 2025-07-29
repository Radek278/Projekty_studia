#include "gra.h"
#include "plansza.h"
#include "QLabel"
#include <ui_plansza.h>
#include "QPixmap"
#include "QMessageBox"
#include "QWidget"
#include "QDebug"
/////////////////////Proszę czytać opisy//////////
gra::gra()
{

}

/*idea:ustawia poszczególne pola jako wyłączone "nie klikalne" i zmienia
 znak o lub x  na przeciwny, zalezy kto mial wczesniej ruch, przypisując każdemu polu (1 z 9) znak o lub x (domyślnie ma p-pusty),
dzięki czemu w metodzie z klasy gra::sprawdz mamy możliwość sprawdzić czy wygrał krzyżyk
czy kółeczko, znając poszczególne kombinacje w których wygra kółeczko a w którym krzyżyk, jeżeli natomiast
nie spelnia warunków wygranej i kółko i krzyzyk to wyswietla sie remis */

/*
 metoda sprawdz -sprawdza czy wygrał krzyżyk lub kółeczko oraz czy jest remis. remis polega na tym ze mamy w incie przechowane ile przycisków jest wyłączonych
 jeżeli zmienna remis będzie = 9 i warunek wygranej któregoś z graczy nie zostanei spełniony to zostanei wyświetlony napisa "remis"

*/

void gra::sprawdz(Ui::plansza *ui,char &k1,char &k2,char &k3,char &k4,char &k5,char &k6,char &k7,char &k8,char &k9,char &olubx,int &remis)
{    if(olubx=='o')
    {
         ui->kolej->setPixmap(QPixmap(":/zdjecia/zdjecia/osmall.bmp"));
    }
    else
    {
         ui->kolej->setPixmap(QPixmap(":/zdjecia/zdjecia/xsmall.bmp"));
    }


    if((k1==k2 && k2==k3 && k1!='p') ||
           (k4==k5 && k5==k6 && k4!='p') ||
           (k7==k8 && k8==k9 && k7!='p') ||
           (k1==k4 && k4==k7 && k7!='p') ||
           (k2==k5 && k5==k8 && k2!='p') ||
           (k3==k6 && k6==k9 && k3!='p') ||
           (k1==k5 && k5==k9 && k1!='p') ||
           (k3==k5 && k5==k7 && k3!='p'))
    {

        if(olubx=='x'){

         ui->label->setText("ZWYCIĘSTWO KÓŁECZKA");
ui->p1->setEnabled(false);
ui->p2->setEnabled(false);
ui->p3->setEnabled(false);
ui->p4->setEnabled(false);
ui->p5->setEnabled(false);
ui->p6->setEnabled(false);
ui->p7->setEnabled(false);
ui->p8->setEnabled(false);
ui->p9->setEnabled(false);
        }
        else{

         ui->label->setText("ZWYCIĘSTWO XKSIKI");
         ui->p1->setEnabled(false);
         ui->p2->setEnabled(false);
         ui->p3->setEnabled(false);
         ui->p4->setEnabled(false);
         ui->p5->setEnabled(false);
         ui->p6->setEnabled(false);
         ui->p7->setEnabled(false);
         ui->p8->setEnabled(false);
         ui->p9->setEnabled(false);

    }

    }
else {
        if (remis==9 )

        {
            ui->label->setText("Remis");
        }
    }
        }
//metoda losuj-losuje na poczatku gry czy zaczyna kolko czy krzyżyk
char gra::losuj(Ui::plansza *ui,char &olubx)
{

    int x;
        srand(time(NULL));
    x = rand()%2;
    if (x==0)
    {
        ui->kolej->setPixmap(QPixmap(":/zdjecia/zdjecia/osmall.bmp"));
       olubx='o';
 ui->label_13->setPixmap(QPixmap(":/zdjecia/zdjecia/exit.jpg"));
return olubx;
    }
    else
    {
         ui->kolej->setPixmap(QPixmap(":/zdjecia/zdjecia/xsmall.bmp"));
         olubx='x';
 ui->label_13->setPixmap(QPixmap(":/zdjecia/zdjecia/exit.jpg"));
         return olubx;

}

}
/* ustawia poszczególne pola jako wyłączone "nie klikalne" i zmienia znak o lub x na przeciwny zależy kto miał wcześniej kolej oraz do zmiennej nazwanej
"remis" dodaje wartość jeden i wyłącza pole w diabełki  */
void gra::ustawk1(Ui::plansza *ui,int który_przycisk,char &k1,char &k2,char &k3,char &k4,char &k5,char &k6,char &k7,char &k8,char &k9,char &olubx,int &remis)
{
    switch(który_przycisk)
      {
      case 1:
      if (olubx=='o')
      {
      ui->l1->setPixmap(QPixmap(":/zdjecia/zdjecia/o1.bmp"));
     k1='o';
    olubx='x';
  ui->p1->setEnabled(false);
remis+=1;
  break;
      }

      else
      {
          ui->l1->setPixmap(QPixmap(":/zdjecia/zdjecia/x.bmp"));
          k1='x';
         olubx='o';
        ui->p1->setEnabled(false);
    remis+=1;
     break;
      }

      case 2:
          if (olubx=='o'){
          ui->l2->setPixmap(QPixmap(":/zdjecia/zdjecia/o1.bmp"));
         k2='o';
          olubx='x';
      ui->p2->setEnabled(false);
remis+=1;
  break;
          }
          else
          {
              ui->l2->setPixmap(QPixmap(":/zdjecia/zdjecia/x.bmp"));
              k2='x';
              olubx='o';
            ui->p2->setEnabled(false);
remis+=1;
      }
          break;
      case 3:
          if (olubx=='o'){
          ui->l3->setPixmap(QPixmap(":/zdjecia/zdjecia/o1.bmp"));
          k3='o';
          olubx='x';
      ui->p3->setEnabled(false);
remis+=1;
      break;
          }
          else
          {
              ui->l3->setPixmap(QPixmap(":/zdjecia/zdjecia/x.bmp"));
              k3='x';
              olubx='o';
            ui->p3->setEnabled(false);
remis+=1;
      }
          break;
      case 4:
          if (olubx=='o'){
          ui->l4->setPixmap(QPixmap(":/zdjecia/zdjecia/o1.bmp"));
          k4='o';
          olubx='x';
      ui->p4->setEnabled(false);
remis+=1;
      break;

          }
          else
          {
              ui->l4->setPixmap(QPixmap(":/zdjecia/zdjecia/x.bmp"));
              k4='x';
              olubx='o';
            ui->p4->setEnabled(false);
remis+=1;
      }
          break;
      case 5:
          if (olubx=='o'){
          ui->l5->setPixmap(QPixmap(":/zdjecia/zdjecia/o1.bmp"));
          k5='o';
         olubx='x';
      ui->p5->setEnabled(false);
remis+=1;
          }
          else
          {
              ui->l5->setPixmap(QPixmap(":/zdjecia/zdjecia/x.bmp"));
             k5='x';
             olubx='o';

            ui->p5->setEnabled(false);
            remis+=1;
      }
          break;
      case 6:
          if (olubx=='o'){
          ui->l6->setPixmap(QPixmap(":/zdjecia/zdjecia/o1.bmp"));
          k6='o';
          olubx='x';
      ui->p6->setEnabled(false);
remis+=1;
          }
          else
          {
              ui->l6->setPixmap(QPixmap(":/zdjecia/zdjecia/x.bmp"));
              k6='x';
            olubx='o';
            ui->p6->setEnabled(false);
            remis+=1;
      }
          break;
      case 7:
          if (olubx=='o'){
          ui->l7->setPixmap(QPixmap(":/zdjecia/zdjecia/o1.bmp"));
          k7='o';
        olubx='x';
      ui->p7->setEnabled(false);
      remis+=1;

          }
          else
          {
              ui->l7->setPixmap(QPixmap(":/zdjecia/zdjecia/x.bmp"));
            k7='x';
            olubx='o';
            ui->p7->setEnabled(false);
            remis+=1;
      }
          break;
      case 8:
          if (olubx=='o'){
          ui->l8->setPixmap(QPixmap(":/zdjecia/zdjecia/o1.bmp"));
          k8='o';
         olubx='x';
      ui->p8->setEnabled(false);
      remis+=1;

          }
          else
          {
              ui->l8->setPixmap(QPixmap(":/zdjecia/zdjecia/x.bmp"));
             k8='x';
             olubx='o';
            ui->p8->setEnabled(false);
            remis+=1;
      }
          break;
      case 9:
          if (olubx=='o'){
          ui->l9->setPixmap(QPixmap(":/zdjecia/zdjecia/o1.bmp"));
     k9='o';
         olubx='x';
      ui->p9->setEnabled(false);
      remis+=1;
          }
          else
          {
              ui->l9->setPixmap(QPixmap(":/zdjecia/zdjecia/x.bmp"));
            k9='x';
          olubx='o';
            ui->p9->setEnabled(false);
            remis+=1;
      }
  }

    }



