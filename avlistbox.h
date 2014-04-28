// нормальный listView

#ifndef AVLISTBOX_H
#define AVLISTBOX_H

#include <QListView>
#include <QModelIndex>
#include <QStringListModel>

class QavListBox : public QListView
{
    Q_OBJECT
    QStringList *lst;
    QStringListModel *stringListModel;

public:
    QavListBox(QWidget *parent = 0);
    ~QavListBox();

private slots:
    void click_list(QModelIndex i);  // обработка щелчка по списку
    
signals:
    void newSelectLine(int index);  // выбор строки № index
    
public slots:
    void add(QString text);         // добавить строку text в конец списка
    void clearAll();                // очистить все
    void kill(int index);           // удалить строку index
    QString textInLine(int index);  // получить текст из строки index
    
};

#endif // AVLISTBOX_H
