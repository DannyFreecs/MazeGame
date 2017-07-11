#include <QString>
#include <QtTest>
#include <QPoint>
#include "game.h"

class MazeGameTest : public QObject
{
    Q_OBJECT

public:
    MazeGameTest();
    Game* game;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testloadMap();
    void testPalyaMeret();
    void testPlayerPos();
    void testIsFloor();
    void testUp();
    void testRight();
    void testDown();
    void testLeft();
};

MazeGameTest::MazeGameTest()
{
}

void MazeGameTest::initTestCase()
{
    game = new Game();
}

void MazeGameTest::cleanupTestCase()
{
    delete game;
}

void MazeGameTest::testloadMap()
{
    QVERIFY(game->map.length() == 0);

    game->loadMap("level1.map");
    for(int i=0; i<16; i++)
        QVERIFY(game->map[i].length() == 16);
}

void MazeGameTest::testPalyaMeret()
{
    QCOMPARE(game->PalyaMeret(), 16);

    game->loadMap("level2.map");
    QCOMPARE(game->PalyaMeret(), 24);

    game->loadMap("level3.map");
    QCOMPARE(game->PalyaMeret(), 32);
}

void MazeGameTest::testPlayerPos()
{
    game->loadMap("level1.map");
    QPoint ppos(1, 14);
    QVERIFY(game->PlayerPos() == ppos);

    game->loadMap("level2.map");
    ppos.setY(22);
    QVERIFY(game->PlayerPos() == ppos);

    game->loadMap("level3.map");
    ppos.setY(30);
    QVERIFY(game->PlayerPos() == ppos);
}

void MazeGameTest::testIsFloor()
{
    QCOMPARE(game->isFloor(game->PlayerPos().x(), game->PlayerPos().y()), true);
    QCOMPARE(game->isFloor(0,0), false);
}

void MazeGameTest::testUp()
{
    game->loadMap("level1.map");
    QPoint ppos(game->PlayerPos().x(), game->PlayerPos().y());
    game->Up();
    ppos.ry()--;
    QCOMPARE(game->PlayerPos(), ppos);
}

void MazeGameTest::testRight()
{
    game->loadMap("level3.map");
    QPoint ppos(game->PlayerPos().x(), game->PlayerPos().y());
    game->Right();
    ppos.rx()++;
    QCOMPARE(game->PlayerPos(), ppos);
}

void MazeGameTest::testDown()
{
    game->loadMap("level1.map");
    game->Up(); game->Up();
    QPoint ppos(game->PlayerPos().x(), game->PlayerPos().y());
    game->Down();
    ppos.ry()++;
    QCOMPARE(game->PlayerPos(), ppos);
}

void MazeGameTest::testLeft()
{
    game->loadMap("level3.map");
    game->Right();
    QPoint ppos(game->PlayerPos().x(), game->PlayerPos().y());
    game->Left();
    ppos.rx()--;
    QCOMPARE(game->PlayerPos(), ppos);
}

QTEST_APPLESS_MAIN(MazeGameTest)

#include "mazegametest.moc"
