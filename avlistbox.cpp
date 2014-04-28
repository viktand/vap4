#include "avlistbox.h"


QavListBox::QavListBox(QWidget *parent) : QListView(parent)
{
    lst=new QStringList();
    stringListModel=new QStringListModel(this);
    stringListModel->setStringList(*lst);
    this->setModel(stringListModel);
    connect(this->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(click_list(QModelIndex)));
}

void QavListBox::clearAll() // полная очистка списка
{
    lst->clear();
    stringListModel->setStringList(*lst);
}

QavListBox::~QavListBox()
{

}

void QavListBox::add(QString text) // добавить новый элемент в список
{
    lst->append(text);
    stringListModel->setStringList(*lst);
}

void QavListBox::kill(int index) // удалить указанный элемент, по индексу
{
    lst->removeAt(index);
    stringListModel->setStringList(*lst);
}

void QavListBox::click_list(QModelIndex i)
{
    emit newSelectLine(i.row());
}

QString QavListBox::textInLine(int index)
{
    if(index>lst->count()-1) return "";
    return lst->takeAt(index);
}
