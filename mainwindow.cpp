#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include "selectmap.h"
#include <QTime>
#include <QKeyEvent>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  sc = new QGraphicsScene(0,0,1,1,this);
  game = new Game(this);
  connect(ui->actionJatek,SIGNAL(triggered()),this,SLOT(NewGame()));
  connect(game,SIGNAL(InvalidMove()),this,SLOT(on_InvalidMove()));
  connect(game,SIGNAL(GameOver()),this,SLOT(Win()));
  gametimer = new QTimer(this);
  gametimer->setInterval(100);
  connect(gametimer,SIGNAL(timeout()),this,SLOT(UpdateTimer()));
  timelabel = new QLabel("0:00:00",this);
  ui->statusBar->addPermanentWidget(timelabel);
  QTimer::singleShot(0,this,SLOT(NewGame()));
}

MainWindow::~MainWindow()
{
  delete gametimer;
  delete timelabel;
  delete game;
  delete sc;
  delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
  QWidget::keyPressEvent(event);
  if(!paused && !gameover) {
      if((event->key()==Qt::Key_W)) {
          if(game->Up())
            MovePlayer();
      }
      if((event->key()==Qt::Key_D)) {
          if(game->Right())
            MovePlayer();
      }
      if((event->key()==Qt::Key_S)) {
          if(game->Down())
            MovePlayer();
      }
      if((event->key()==Qt::Key_A)) {
          if(game->Left())
            MovePlayer();
      }
  }
}

void MainWindow::NewGame()
{
  selectmap *dlg = new selectmap();
  QDir *mapdir = new QDir(QCoreApplication::applicationDirPath()+"/map");
  QStringList *sl = new QStringList("*.map");
  QStringList maps = mapdir->entryList(*sl,QDir::Files|QDir::Readable,QDir::Name);
  dlg->setItems(maps);
  delete mapdir;
  delete sl;
  if (dlg->exec()==QDialog::Accepted) {
      game->NewGame(dlg->getSelected());
      ShowMap();
      gametime_ms = 0;
      paused = false;
      gameover = false;
      gametimer->start();
    }
  delete dlg;
}

void MainWindow::ShowMap()
{
  sc->clear();
  mapitems.clear();
  sc->setSceneRect(0,0,game->MapSize()*PPT,game->MapSize()*PPT);
  QPixmap *fog = new QPixmap(QString("%1/res/fog.png").arg(QCoreApplication::applicationDirPath()));
  QGraphicsRectItem *rt = sc->addRect(sc->sceneRect(),QPen(),QBrush(*fog));
  delete fog;
  rt->setPos(0,0);
  rt->setZValue(1);

  QPixmap *pmWall = new  QPixmap(QString("%1/res/fal.png").arg(QCoreApplication::applicationDirPath()));
  QPixmap *pmFloor = new  QPixmap(QString("%1/res/padlo.png").arg(QCoreApplication::applicationDirPath()));

  for(int y=0; y<game->MapSize(); y++) {
      for(int x=0; x<game->MapSize();x++) {
          QGraphicsPixmapItem *it;
          if(!game->isFloor(y,x))
              it = sc->addPixmap(*pmWall);
           else
              it = sc->addPixmap(*pmFloor);
          it->setPos(x*PPT,y*PPT);
          mapitems.append(it);
        }
    }

  delete pmFloor;
  delete pmWall;
  QPixmap *pmPlayer = new QPixmap(QString("%1/res/jatekos.png").arg(QCoreApplication::applicationDirPath()));
  player = sc->addPixmap(*pmPlayer);
  player->setZValue(3);
  delete pmPlayer;
  MovePlayer();

  QRectF scr = sc->sceneRect();
  setFixedSize(scr.width()+48,scr.height()+72);
  ui->graphicsView->setScene(sc);
}

void MainWindow::MovePlayer()
{
    player->setPos(game->PlayerPos().x()*PPT+2,game->PlayerPos().y()*PPT+2);
    DeFog();
}

void MainWindow::DeFog()
{
  foreach (QGraphicsPixmapItem *item, mapitems) {
      if(item->zValue()) {
          item->setZValue(0);
        }
  }

  mapitems[game->PlayerPos().y()*game->MapSize()+game->PlayerPos().x()]->setZValue(2);
  //fenn
  if(!game->isFloor(game->PlayerPos().y()-1, game->PlayerPos().x()))
      mapitems[game->MapSize()*(game->PlayerPos().y()-1)+game->PlayerPos().x()]->setZValue(2);
  else
  {
      mapitems[game->MapSize()*(game->PlayerPos().y()-1)+game->PlayerPos().x()]->setZValue(2);
      mapitems[game->MapSize()*(game->PlayerPos().y()-2)+game->PlayerPos().x()]->setZValue(2);
      mapitems[game->MapSize()*(game->PlayerPos().y()-2)+game->PlayerPos().x()-1]->setZValue(2);
      if(game->PlayerPos().x()+2 < game->MapSize())
        mapitems[game->MapSize()*(game->PlayerPos().y()-2)+game->PlayerPos().x()+1]->setZValue(2);
  }

  //jobb fenn
  if(!game->isFloor(game->PlayerPos().y()-1, game->PlayerPos().x()+1) && game->PlayerPos().x()+1 < game->MapSize())
  {
      mapitems[game->MapSize()*(game->PlayerPos().y()-1)+game->PlayerPos().x()+1]->setZValue(2);
  }
  else
  {
      if(game->PlayerPos().x()+1 < game->MapSize())
      {
        mapitems[game->MapSize()*(game->PlayerPos().y()-1) + game->PlayerPos().x()+1]->setZValue(2);
        if(game->PlayerPos().x()+2 < game->MapSize())
        {
            mapitems[game->MapSize()*(game->PlayerPos().y()-1) + game->PlayerPos().x()+2]->setZValue(2);
            mapitems[game->MapSize()*(game->PlayerPos().y()-2) + game->PlayerPos().x()+2]->setZValue(2);
        }
      }
  }

  //jobb
  if(!game->isFloor(game->PlayerPos().y(), game->PlayerPos().x()+1) && game->PlayerPos().x()+1<game->MapSize())
      mapitems[game->MapSize()*game->PlayerPos().y()+game->PlayerPos().x()+1]->setZValue(2);
  else
  {   if(game->PlayerPos().x()+1 < game->MapSize())
        mapitems[game->MapSize()*game->PlayerPos().y()+game->PlayerPos().x()+1]->setZValue(2);
      if(game->PlayerPos().x()+2 < game->MapSize())
        mapitems[game->MapSize()*game->PlayerPos().y()+game->PlayerPos().x()+2]->setZValue(2);
  }

  //jobb lenn
  if(!game->isFloor(game->PlayerPos().y()+1, game->PlayerPos().x()+1) && game->PlayerPos().x()+1 < game->MapSize())
      mapitems[game->MapSize()*(game->PlayerPos().y()+1) + game->PlayerPos().x()+1]->setZValue(2);
  else
  {
      if(game->PlayerPos().x()+1 < game->MapSize())
      {
        mapitems[game->MapSize()*(game->PlayerPos().y()+1) + game->PlayerPos().x()+1]->setZValue(2);
        mapitems[game->MapSize()*(game->PlayerPos().y()+1) + game->PlayerPos().x()+2]->setZValue(2);
        mapitems[game->MapSize()*(game->PlayerPos().y()+2) + game->PlayerPos().x()+2]->setZValue(2);
      }
  }

  //lenn
  if(!game->isFloor(game->PlayerPos().y()+1, game->PlayerPos().x()))
      mapitems[game->MapSize()*(game->PlayerPos().y()+1) + game->PlayerPos().x()]->setZValue(2);
  else
  {
      mapitems[game->MapSize()*(game->PlayerPos().y()+1) + game->PlayerPos().x()]->setZValue(2);
      mapitems[game->MapSize()*(game->PlayerPos().y()+2) + game->PlayerPos().x()]->setZValue(2);
      mapitems[game->MapSize()*(game->PlayerPos().y()+2) + game->PlayerPos().x()-1]->setZValue(2);
      mapitems[game->MapSize()*(game->PlayerPos().y()+2) + game->PlayerPos().x()+1]->setZValue(2);
  }

  //bal lenn
  if(!game->isFloor(game->PlayerPos().y()+1, game->PlayerPos().x()-1))
      mapitems[game->MapSize()*(game->PlayerPos().y()+1) + game->PlayerPos().x()-1]->setZValue(2);
  else
  {
      mapitems[game->MapSize()*(game->PlayerPos().y()+1) + game->PlayerPos().x()-1]->setZValue(2);
      mapitems[game->MapSize()*(game->PlayerPos().y()+1) + game->PlayerPos().x()-2]->setZValue(2);
      mapitems[game->MapSize()*(game->PlayerPos().y()+2) + game->PlayerPos().x()-2]->setZValue(2);
  }

  //bal
  if(!game->isFloor(game->PlayerPos().y(), game->PlayerPos().x()-1))
      mapitems[game->MapSize()*game->PlayerPos().y()+game->PlayerPos().x()-1]->setZValue(2);
  else
  {
      mapitems[game->MapSize()*game->PlayerPos().y()+game->PlayerPos().x()-1]->setZValue(2);
      mapitems[game->MapSize()*game->PlayerPos().y()+game->PlayerPos().x()-2]->setZValue(2);
  }

  //bal fenn
  if(!game->isFloor(game->PlayerPos().y()-1, game->PlayerPos().x()-1))
      mapitems[game->MapSize()*(game->PlayerPos().y()-1)+game->PlayerPos().x()-1]->setZValue(2);
  else
  {
      mapitems[game->MapSize()*(game->PlayerPos().y()-1) + game->PlayerPos().x()-1]->setZValue(2);
      mapitems[game->MapSize()*(game->PlayerPos().y()-1) + game->PlayerPos().x()-2]->setZValue(2);
      mapitems[game->MapSize()*(game->PlayerPos().y()-2) + game->PlayerPos().x()-2]->setZValue(2);
  }

}

void MainWindow::on_actionKilepes_triggered()
{
  close();
}

void MainWindow::UpdateTimer()
{
  gametime_ms += gametimer->interval();
  timelabel->setText(QTime::fromMSecsSinceStartOfDay(gametime_ms).toString("H:mm:ss"));
}

void MainWindow::on_InvalidMove()
{
  ui->statusBar->showMessage("ÉRVÉNYTELEN LÉPÉS!",1000);
}

void MainWindow::Win()
{
  MovePlayer();
  gametimer->stop();
  gameover = true;
  QPixmap *pmPlayer = new QPixmap(QString("%1/res/winner.png").arg(QCoreApplication::applicationDirPath()));
  player->setPixmap(*pmPlayer);
  delete pmPlayer;
  QMessageBox::information(this,"Játék vége","Gratulálok!\n\nSikeresen kijutottál a labirintusból "+QTime::fromMSecsSinceStartOfDay(gametime_ms).toString("H:mm:ss")+" alatt.");
}

void MainWindow::on_actionSzunet_triggered()
{
  if(!gameover) {
      paused = !paused;
      if(paused) {
        gametimer->stop();
        timelabel->setText("A játék szünetel");
      }
      else {
        gametimer->start();
      }
  }
}
