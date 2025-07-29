#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include "plansza.h"
QT_BEGIN_NAMESPACE
namespace Ui { class menu; }
QT_END_NAMESPACE

class menu : public QMainWindow
{
    Q_OBJECT

public:
    menu(QWidget *parent = nullptr);
    ~menu();
public slots:
    void openNewWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    plansza *mMyNewWindow;
private:
    Ui::menu *ui;
};
#endif // MENU_H
