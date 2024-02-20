/*  COMP.CS.100 Project 4: GAME CALLED 2048
 *
 * The description of this project can be found in file "instructions.txt".
 *
 *
 * Writer of the program:
 * Name: Antti Cederlöf
 * Student-id: 283233
 * Username: cederloa ( Git-repo )
 * E-Mail: antti.cederlof@tuni.fi
 *
 * */

#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
