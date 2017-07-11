#ifndef SELECTMAP_H
#define SELECTMAP_H
#include <QDialog>

namespace Ui {
  class selectmap;
}

class selectmap : public QDialog
{
  Q_OBJECT
public:
  explicit selectmap(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
  ~selectmap();
  void setItems(QStringList &list);
  QString getSelected();

private:
  Ui::selectmap *ui;

signals:

public slots:
private slots:
};

#endif // SELECTMAP_H
