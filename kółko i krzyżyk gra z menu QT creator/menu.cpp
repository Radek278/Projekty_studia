#include "menu.h"
#include "ui_menu.h"
#include "plansza.h"
#include "QProcess"
menu::menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::menu)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(click()), this, SLOT(openNewWindow()));// wywolałem sygnał który po wcisnięciu przyciusku tworzy nowe okienko planszy
}



/*klasa menu ma w sumie jedną rzecz do zrobienia (włącz i wyłącz-włącz gre i wyłącz aplikacje, wykorzystałem do tego destruktory,
które niszczą obiekty (sprzątają) a tak się składa że okienko plansza i menu to obiekt wiec można sb fajnie z tym działać*/
menu::~menu()
{
    delete ui;
}

void menu::openNewWindow()
{

    mMyNewWindow=new plansza();
    mMyNewWindow->show();//wyświetla nowe okienko "plansza"


}


void menu::on_pushButton_clicked()
{

   openNewWindow();
menu::~menu();// destruktor który wykańcza okienko "menu" musiałem wykorzystać destruktor żeby wyłączyć tylko okienko a nie cały program
}


void menu::on_pushButton_2_clicked()
{
    exit (0);// wyłącza cała aplikacje
}

