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

  QFile mapfile(mapname);
  mapfile.open(QIODevice::ReadOnly);
  QTextStream str(&mapfile);

  QString ba = str.readLine();
  palyameret = ba.toInt();

  for(int y=0; y<palyameret; y++) {
      ba = str.readLine();
      QVector<bool> sor;
      for(int x=0; x<palyameret;x++) {
          if(ba.at(x)=='1') {
              sor.append(true);
            } else {
              sor.append(false);
            }
        }
      map.append(sor);
    }
  playerpos->setX(1);
  playerpos->setY(palyameret-2);
}

bool Game::isFloor(int sor, int oszlop)
{
  return(sor>0 && sor<(palyameret-1) && oszlop>0 && oszlop<palyameret && (!map[sor][oszlop]));
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
  if( playerpos->y()==1 && playerpos->x()==palyameret-1 )
    emit GameOver();
}


