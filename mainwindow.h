#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QLabel>
#include "game.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  void keyPressEvent(QKeyEvent *event);

private:
  QGraphicsScene *sc;
  Game *game;
  QGraphicsPixmapItem *player;
  QVector<QGraphicsPixmapItem*> mapitems;
  QTimer *gametimer;
  int gametime_ms;
  QLabel *timelabel;
  bool paused;
  bool gameover;


  void ShowMap();
  void MovePlayer();
  void DeFog();

private slots:
  //Exit
  void on_actionKilepes_triggered();
  //Pause
  void on_actionSzunet_triggered();
  void UpdateTimer();
  void on_InvalidMove();
  void Win();


public slots:
  void NewGame();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
