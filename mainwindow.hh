/*
#############################################################################
# COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures          #
# Project 4: Game called 2048                                               #
# File: mainwindow                                                          #
# Description: Defines the graphical user interface for the game.           #
#############################################################################
*/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameboard.hh"
#include "QtWidgets"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QLabel>
#include <QColor>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor and deconstructor
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Set the rigt amount of rectangles on the board and labels on them
    void initializeGuiBoard(GameBoard* board);

    // Visualizes the board on the GUI
    void updateGuiBoard();

    // Function for moving the tiles on the board in the direction determined
    // by input dir. Also checks if the game is over and notifies when it is.
    void move(Coords dir);

private slots:
    // Starts the game with user chosen settings. Builds the board and allows
    // to start making moves.
    void startGame();

    // Slots that call the function move with the right input.
    void moveDown();
    void moveUp();
    void moveLeft();
    void moveRight();

    // Starts the game again with the same board size that was defined before
    // pressing the start button. RNG seed and goal are customizable before
    // reseting.
    void reset();

    // Pauses the game and the timer. Game can be continued when the pause
    // button is clicked again.
    void pause();

    // Closes MainWindow
    void quit();

    // Updates timer
    void updateLCD();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene_;
    QTimer* timer;
    GameBoard* board;

    // Size of a side of one tile in pixels
    int rectSize_;
    int goal_;
    int time = 0;

    // Number labels in the GUI
    std::vector<std::vector<QLabel*>> labels_;
    // GUI game board
    std::vector<std::vector<QGraphicsRectItem*>> rects_;

    const int BOARD_SIZE = 481;
    const int LEFT_MARGIN = 150;
    const int TOP_MARGIN = 80;

    const Coords LEFT = {0, -1};
    const Coords UP = {-1, 0};
    const Coords RIGHT = {0, 1};
    const Coords DOWN = {1, 0};

    const std::map<int, QBrush> brushes = {
        {2, QColor::fromRgb(0, 0, 0, 100)},
        {4, QColor::fromRgb(0, 0, 100, 100)},
        {8, QColor::fromRgb(0, 0, 200, 100)},
        {16, QColor::fromRgb(0, 100, 200, 100)},
        {32, QColor::fromRgb(0, 200, 200, 100)},
        {64, QColor::fromRgb(100, 200, 200, 100)},
        {128, QColor::fromRgb(200, 200, 100, 100)},
        {256, QColor::fromRgb(200, 200, 0, 100)},
        {512, QColor::fromRgb(200, 100, 0, 100)},
        {1024, QColor::fromRgb(200, 0, 0, 100)},
        {2048, QColor::fromRgb(100, 0, 0, 100)}
    };
};
#endif // MAINWINDOW_HH
