#include "game.h"
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

Game::Game(QObject *parent) : QObject(parent)
{
  playerpos = new QPoint();
}

Game::~Game()
{
  delete playerpos;
}

void Game::loadMap(QString mapname)
{
  map.clear();

  QFile mapfile(QCoreApplication::applicationDirPath()+"/map/"+mapname);
  mapfile.open(QIODevice::ReadOnly);
  QTextStream str(&mapfile);

  QString ba = str.readLine();
  mapsize = ba.toInt();

  for(int y=0; y<mapsize; y++) {
      ba = str.readLine();
      QVector<bool> line;
      for(int x=0; x<mapsize;x++) {
          if(ba.at(x)=='1') {
              line.append(true);
            } else {
              line.append(false);
            }
        }
      map.append(line);
    }
  playerpos->setX(1);
  playerpos->setY(mapsize-2);
}

bool Game::isFloor(int line, int column)
{
  return(line>0 && line<(mapsize-1) && column>0 && column<mapsize && (!map[line][column]));
}

void Game::NewGame(QString map)
{
  loadMap(map);
}

bool Game::Up()
{
  bool ok = isFloor(playerpos->y()-1, playerpos->x());
  if( ok ) {
    playerpos->ry()--;
    Check();
  }
  else
    emit InvalidMove();
  return(ok);
}

bool Game::Right()
{
  bool ok = isFloor(playerpos->y(), playerpos->x()+1);
  if( ok ) {
    playerpos->rx()++;
    Check();
    }
  else
    emit InvalidMove();
  return(ok);
}

bool Game::Down()
{
  bool ok = isFloor(playerpos->y()+1, playerpos->x());
  if( ok ) {
    playerpos->ry()++;
    Check();
    }
  else
    emit InvalidMove();
  return(ok);
}

bool Game::Left()
{
  bool ok = isFloor(playerpos->y(), playerpos->x()-1);
  if( ok ) {
    playerpos->rx()--;
    Check();
    }
  else
    emit InvalidMove();
  return(ok);
}

void Game::Check()
{
  if( playerpos->y()==1 && playerpos->x()==mapsize-1 )
    emit GameOver();
}


