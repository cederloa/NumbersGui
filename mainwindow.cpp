/*
#############################################################################
# COMP.CS.110 Ohjelmointi 2: Rakenteet / Programming 2: Structures          #
# Project 4: Game called 2048                                               #
# File: mainwindow                                                          #
# Description: Defines the graphical user interface for the game.           #
#############################################################################
*/

#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <math.h>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("QMainWindow {background-color: white}");

    board = new GameBoard();

    // Create the scene for GUI gameboard
    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setGeometry(LEFT_MARGIN, TOP_MARGIN,
                                   BOARD_SIZE + 2, BOARD_SIZE + 2);
    ui->graphicsView->setScene(scene_);

    scene_->setSceneRect(0, 0, BOARD_SIZE - 1, BOARD_SIZE - 1);

    // Initialize timer
    timer = new QTimer(parent);
    ui->lcdNumberSec->setStyleSheet("background-color: blue");
    ui->lcdNumberMin->setStyleSheet("background-color: blue");

    // Signal-slot connections
    connect(ui->startButton, &QPushButton::clicked,
            this, &MainWindow::startGame);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::reset);
    connect(ui->quitButton, &QPushButton::clicked, this, &MainWindow::quit);

    connect(ui->pushButtonDown, &QPushButton::clicked,
            this,&MainWindow::moveDown);
    connect(ui->pushButtonUp, &QPushButton::clicked,
            this, &MainWindow::moveUp);
    connect(ui->pushButtonLeft, &QPushButton::clicked,
            this, &MainWindow::moveLeft);
    connect(ui->pushButtonRight, &QPushButton::clicked,
            this, &MainWindow::moveRight);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateLCD);
    connect(ui->pauseButton, &QPushButton::clicked, this, &MainWindow::pause);


    // Widget initialization
    ui->spinBoxGoal->setRange(2, 15);
    ui->spinBoxGoal->setValue(11);
    ui->radioButton4->setChecked(true);

    ui->pushButtonLeft->setDisabled(true);
    ui->pushButtonRight->setDisabled(true);
    ui->pushButtonUp->setDisabled(true);
    ui->pushButtonDown->setDisabled(true);

    ui->resetButton->setDisabled(true);
    ui->pauseButton->setDisabled(true);

    ui->labelGameOver->setStyleSheet("font: 18pt");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
    delete board;
}

void MainWindow::initializeGuiBoard(GameBoard* board)
{
    // Initialize the tiles and labels with the right number of elements
    std::vector<QGraphicsRectItem*> rect_row;
    std::vector<QLabel*> lbl_row;
    for( int i = 0; i < board->get_size(); ++i)
    {
        rect_row.push_back(nullptr);
        lbl_row.push_back(nullptr);
    }
    for( int i = 0; i < board->get_size(); ++i)
    {
        rects_.push_back(rect_row);
        labels_.push_back(lbl_row);
    }

    // Create the grid
    for( auto y = 0; y < board->get_size(); ++y )
    {
        for( auto x = 0; x < board->get_size(); ++x )
        {
            rects_.at(y).at(x) = scene_->addRect(x*rectSize_, y*rectSize_,
                                                 rectSize_, rectSize_);
        }
    }

    // Create labels
    for( auto y = 0; y < board->get_size(); ++y )
    {
        for( auto x = 0; x < board->get_size(); ++x )
        {
            labels_.at(y).at(x) = new QLabel("", this);
            labels_.at(y).at(x)->setGeometry(LEFT_MARGIN + (x + 0.3)*rectSize_,
                                             TOP_MARGIN + y*rectSize_,
                                             rectSize_, rectSize_);
        }
    }
}

void MainWindow::updateGuiBoard()
{
    // Update labels
    for( auto y = 0; y < board->get_size(); ++y )
    {
        for( auto x = 0; x < board->get_size(); ++x )
        {
            int value = board->get_item(std::make_pair(y,x))->get_value();
            if (value == 0)
            {
                labels_.at(y).at(x)->setText("");
                rects_.at(y).at(x)->setBrush(QBrush());
            }
            else
            {
                labels_.at(y).at(x)->setText(QString::number(value));

                if (brushes.find(value) == brushes.end())
                {
                    // Higher values than 2048 get the same color as 2048
                    rects_.at(y).at(x)->setBrush(brushes.rbegin()->second);
                }
                else
                {
                    rects_.at(y).at(x)->setBrush(brushes.at(value));
                }

            }

            labels_.at(y).at(x)->show();
        }
    }
}

void MainWindow::move(Coords dir)
{
    if(board->is_full())
    {
        ui->labelGameOver->setText("Can't add new tile, you lost!");
        this->setStyleSheet("QMainWindow {background-color: red}");
        timer->stop();
        ui->pauseButton->setDisabled(true);

    }
    else if(board->move(dir, goal_))
    {
        updateGuiBoard();
        ui->labelGameOver->setText("You reached the goal value of " +
                                   QString::number(goal_) + "!");
        this->setStyleSheet("QMainWindow {background-color: green}");

        ui->pushButtonLeft->setDisabled(true);
        ui->pushButtonRight->setDisabled(true);
        ui->pushButtonUp->setDisabled(true);
        ui->pushButtonDown->setDisabled(true);

        ui->pauseButton->setDisabled(true);
        timer->stop();
    }
    else
    {
        board->new_value(false);
    }

    updateGuiBoard();
}


// MainWindow slots:

void MainWindow::startGame()
{
    // Use right sized board according to the user's choice
    board->set_size(ui->radioButton4->isChecked(),
                    ui->radioButton6->isChecked(),
                    ui->radioButton8->isChecked());
    rectSize_ = (BOARD_SIZE - 1) / board->get_size();

    goal_ = pow(2, ui->spinBoxGoal->value());
    ui->labelGoal->setText(QString::number(goal_));

    board->init_empty();
    board->fill(ui->spinBoxRng->value());

    initializeGuiBoard(board);
    updateGuiBoard();

    ui->startButton->setDisabled(true);
    ui->resetButton->setDisabled(false);
    ui->pauseButton->setDisabled(false);

    ui->pushButtonLeft->setDisabled(false);
    ui->pushButtonRight->setDisabled(false);
    ui->pushButtonUp->setDisabled(false);
    ui->pushButtonDown->setDisabled(false);

    ui->radioButton4->setDisabled(true);
    ui->radioButton6->setDisabled(true);
    ui->radioButton8->setDisabled(true);

    timer->start(1000);
}

void MainWindow::moveDown()
{
    move(DOWN);
}

void MainWindow::moveUp()
{
    move(UP);
}

void MainWindow::moveLeft()
{
    move(LEFT);
}

void MainWindow::moveRight()
{
    move(RIGHT);
}

void MainWindow::reset()
{
    goal_ = pow(2, ui->spinBoxGoal->value());
    ui->labelGoal->setText(QString::number(goal_));

    board->fill(ui->spinBoxRng->value());
    updateGuiBoard();

    ui->labelGameOver->setText("");
    this->setStyleSheet("QMainWindow {background-color: white}");

    ui->pushButtonLeft->setDisabled(false);
    ui->pushButtonRight->setDisabled(false);
    ui->pushButtonUp->setDisabled(false);
    ui->pushButtonDown->setDisabled(false);

    ui->pauseButton->setDisabled(false);

    time = 0;
    ui->lcdNumberMin->display(0);
    ui->lcdNumberSec->display(0);

    ui->pauseButton->setText("Pause");
    timer->start(1000);
}

void MainWindow::pause()
{
    if(timer->isActive())
    {
        timer->stop();
        ui->labelGameOver->setText("Paused");
        ui->pauseButton->setText("Continue");
        ui->pushButtonLeft->setDisabled(true);
        ui->pushButtonRight->setDisabled(true);
        ui->pushButtonUp->setDisabled(true);
        ui->pushButtonDown->setDisabled(true);
    }
    else
    {
        timer->start(1000);
        ui->labelGameOver->setText("");
        ui->pauseButton->setText("Pause");
        ui->pushButtonLeft->setDisabled(false);
        ui->pushButtonRight->setDisabled(false);
        ui->pushButtonUp->setDisabled(false);
        ui->pushButtonDown->setDisabled(false);
    }
}

void MainWindow::quit()
{
    this->close();
}

void MainWindow::updateLCD()
{
    time++;
    ui->lcdNumberMin->display(time/60);
    ui->lcdNumberSec->display(time%60);
}


