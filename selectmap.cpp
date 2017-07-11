#include "selectmap.h"
#include "ui_selectmap.h"
#include <QListWidgetItem>

selectmap::selectmap(QWidget *parent, Qt::WindowFlags f) : QDialog(parent,f), ui(new Ui::selectmap)
{
  ui->setupUi(this);
}

selectmap::~selectmap()
{
  delete ui;
}

void selectmap::setItems(QStringList &list)
{
  if(list.count()==0) {
      ui->okButton->setEnabled(false);
  }
  else {
    ui->mapList->addItems(list);
  }
}

QString selectmap::getSelected()
{
  return(ui->mapList->currentItem()->text());
}

