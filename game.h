#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QPoint>
#include <QVector>

const int PPT = 16;

class Game : public QObject
{
  Q_OBJECT
public:
  explicit Game(QObject *parent = 0);
  ~Game();
  void loadMap(QString mapname);
  int MapSize() {return mapsize;}
  QPoint PlayerPos() {return *playerpos;}
  bool isFloor(int line, int column);
  void NewGame(QString map);

  bool Up();
  bool Right();
  bool Down();
  bool Left();

private:
  QPoint *playerpos;
  int mapsize;
  QVector<QVector<bool> > map;

  void Check();

signals:
  void InvalidMove();
  void GameOver();

};

#endif // GAME_H
