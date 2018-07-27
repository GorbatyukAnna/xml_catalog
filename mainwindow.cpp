#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QtCore>
#include <QtXml>
#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    item = new QTableWidgetItem[2];
    item2 = new QTableWidgetItem[2];
    item3 = new QTableWidgetItem[2];
    item4 = new QTableWidgetItem[2];

/*
    QStringList headers = {trUtf8("Наименование"),trUtf8("Количество")};
    ui->in->setColumnCount(2);
    ui->in->setShowGrid(true);
   // ui->in->setRowCount(1);
    ui->in->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->in->setHorizontalHeaderLabels(headers);
    QTableWidgetItem* item = new QTableWidgetItem;
    QTableWidgetItem* item2 = new QTableWidgetItem;
    item->setText("Name1");
    ui->in->setItem(0,0, item);
    item2->setText("5");
    ui->in->setItem(0,1, item2);

*/

    connect(ui->text_in,SIGNAL(returnPressed()),this,SLOT(Pressin()));
    connect(ui->text_out,SIGNAL(returnPressed()),this,SLOT(Pressout()));
    table_settings();
}

QStringList ListElements(QDomElement root, QString tagname, QString attribute);

QStringList MainWindow::read_from_xml(QString attribute)
{
    QDomDocument document;
    //QFile file(QDir::currentPath() + "/catalog.xml");
    QFile file(path+"/catalog.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Failed to open file";
        //break;

    }else
    {
        if(!document.setContent(&file))
        {
            qDebug()<<"Failed to open file";
            //break;
        }
        file.close();
    }

    QDomElement r = document.firstChildElement();
    QStringList l = ListElements(r,"Elements",attribute);
    return l;
}

void MainWindow::write_in_xml(QString& str, QString& str2)
{
    if(str == " " || str2 == " ")
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Был введен неподобающий символ или одно из полей осталось пустым."));
        return;
    }
    //QFile file("D:/qtprojects/catalog/catalog.xml");
    QDomDocument document;
    QDomElement root = document.createElement("catalog");
    document.appendChild(root);
    QStringList ind = read_from_xml("index");
    QStringList name = read_from_xml("name");

    if(ind.empty() || name.empty())
    {
        if(str == " " || str2 == " ") return;
        QDomElement node = document.createElement("Elements");
        qDebug()<<"xml is empty";
        node.setAttribute("index",str);
        node.setAttribute("name", str2);
        root.appendChild(node);

    }
    else
    {

        if(str == " " || str2 == " ") return;

        ind.push_back(str);
        name.push_back(str2);
        qDebug()<<"Name"<< str<<str2;
        //root.appendChild(node);
        for(int i =0; i < ind.length(); i++)
        {

            QDomElement node = document.createElement("Elements");
            qDebug()<< "List"<<ind[i]<<name[i];
            node.setAttribute("index",ind[i]);
            node.setAttribute("name",name[i]);
            root.appendChild(node);
        }

    }


    QFile file(path + "/catalog.xml");

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<< "Can't open file";
    }else
    {
        QTextStream stream(&file);
        //document.save(stream,0);
        stream << document.toString();
        file.close();
        qDebug()<<"Done";
    }

}
void MainWindow::write_in_xml(QStringList& list, QStringList& list2)
{
    if(list.empty() || list2.empty())
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Заполните все поля. Новый элемент номенклатуры должен содержать идентификатор и имя."));
        return;
    }
    if(list.length() != list2.length())
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Количество идентификаторов не равно количеству имен."));
        return;
    }else
{

    for(int k =0; k< list.length(); k++)
    {
        if(list[k] ==" " || list[k] == "")
        {
            QMessageBox::information(NULL,QObject::tr("Информация"),tr("Количество идентификаторов не равно количеству имен."));
            return;
        }
        if(list2[k] =="" || list2[k] ==" ")
        {
            QMessageBox::information(NULL,QObject::tr("Информация"),tr("Количество идентификаторов не равно количеству имен."));
            return;
        }
    }
    QDomDocument document;
    QDomElement root = document.createElement("catalog");
    document.appendChild(root);
    QStringList ind = read_from_xml("index");
    QStringList name = read_from_xml("name");

    if(ind.empty() || name.empty())
    {
        for(int i =0; i<list.length(); i++)
        {/*
            if(list[i] == " " || list2[i] == " ") return;
            if(list[i] == "" || list2[i] == "")
            {
               QMessageBox::information(NULL,QObject::tr("Информация"),tr("Количество идентификаторов не равно количеству имен."));
               return;
            }*/
            QDomElement node = document.createElement("Elements");
            qDebug()<<"xml is empty";
            node.setAttribute("index",list[i]);
            node.setAttribute("name", list2[i]);
            root.appendChild(node);
        }

    }
    else
    {
        for( int i=0; i< list.length(); i++)
        {
            if(list[i] == " " || list2[i] == " ") return;
            QString pt = converting_index(list[i]);

            if(pt == " ")
            {

                return;
            }

            ind.push_back(pt);
            name.push_back(list2[i]);
        }
        //qDebug()<<"Name"<< str<<str2;
        //root.appendChild(node);
        for(int i =0; i < ind.length(); i++)
        {


            QDomElement node = document.createElement("Elements");
            qDebug()<< "List"<<ind[i]<<name[i];
            node.setAttribute("index",ind[i]);
            node.setAttribute("name",name[i]);
            root.appendChild(node);
        }

    }
    QFile file(path + "/catalog.xml");

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<< "Can't open file";
    }else
    {
        QTextStream stream(&file);
        //document.save(stream,0);
        stream << document.toString();
        file.close();
        qDebug()<<"Done";
    }
    }
}
QString& MainWindow::converting_index(QString& list)
{
    list = list.toUpper();
    QRegExp reg("^[A-Z0-9]+$");


    if(reg.indexIn(list) != -1)
    {
        return list;
    }else
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Не подходящий идентификатор. Он будет игнорироваться"));
        list =" ";
        return list;
    }



}
MainWindow::~MainWindow()
{
    delete ui;
    delete[] item;
    delete[] item2;
    delete[] item3;
    delete[] item4;
}
QStringList ListElements(QDomElement root, QString tagname, QString attribute)
{
    QStringList list;
    QDomNodeList items = root.elementsByTagName(tagname);
    for( int i = 0; i< items.count(); i++)
    {
        QDomNode itemnode = items.at(i);
        if(itemnode.isElement())
        {
            QDomElement itemele = itemnode.toElement();
            list.push_back(itemele.attribute(attribute));
            //qDebug()<<itemele.attribute(attribute);
        }
    }
    return list;

}
bool MainWindow::incatalog(const QString& str)
{
    QDomDocument document;

    //QFile file(QDir::currentPath() + "/catalog.xml");
    QFile file(path+"/catalog.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Failed to open file1";
        //break;

    }else
    {
        if(!document.setContent(&file))
        {
            qDebug()<<"Failed to open file";
            //break;
        }
        file.close();
    }

    QDomElement r = document.firstChildElement();
    QStringList l = ListElements(r,"Elements","index");
    for(int i = 0; i < l.size(); i++)
    {
        if(l[i] == str) return true;

    }
    return false;
}
void MainWindow::delete_from_xml(QString& str, QString& str2)
{
    qDebug()<<"name1"<<str<<"name2"<<str2;
    if(str == "" || str2 == "")
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Был введен неподобающий символ или одно из полей осталось пустым."));
        return;
    }

    QDomDocument document;
    QDomElement root = document.createElement("catalog");
    document.appendChild(root);
    QStringList ind = read_from_xml("index");
    QStringList name = read_from_xml("name");

    if(ind.empty() || name.empty())
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Xml файл пуст.Невозможно выполнить операцию удаления."));
        return;
    }
    else
    {

        if(str == " " || str2 == " ") return;

        for(int i =0; i < ind.length(); i++)
        {
            if(ind[i] == str && name[i] == str2)
            {
                qDebug()<<"I deleted this element";
            }else
            {
                QDomElement node = document.createElement("Elements");
                qDebug()<< "List"<<ind[i]<<name[i];
                node.setAttribute("index",ind[i]);
                node.setAttribute("name",name[i]);
                root.appendChild(node);
            }


        }

    }


    QFile file(path + "/catalog.xml");

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<< "Can't open file";
    }else
    {
        QTextStream stream(&file);
        //document.save(stream,0);
        stream << document.toString();
        file.close();
        qDebug()<<"Done";
    }

}
void MainWindow::delete_from_xml(QStringList& list, QStringList& list2)
{
    if(list.empty() || list2.empty())
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Заполните все поля. Элемент номенклатуры должен содержать идентификатор и имя."));
        return;
    }
    if(list.length() != list2.length())
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Количество идентификаторов не равно количеству имен."));
        return;
    }else
{
        for(int k =0; k< list.length(); k++)
        {
            if(list[k] ==" " || list[k] == "")
            {
                QMessageBox::information(NULL,QObject::tr("Информация"),tr("Количество идентификаторов не равно количеству имен."));
                return;
            }
            if(list2[k] =="" || list2[k] ==" ")
            {
                QMessageBox::information(NULL,QObject::tr("Информация"),tr("Количество идентификаторов не равно количеству имен."));
                return;
            }
        }
    bool noelement = 1;
    QDomDocument document;
    QDomElement root = document.createElement("catalog");
    document.appendChild(root);
    QStringList ind = read_from_xml("index");
    QStringList name = read_from_xml("name");

    if(ind.empty() || name.empty())
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Xml файл пуст.Невозможно выполнить операцию удаления."));
        return;
    }
    else
    {
        for( int i=0; i< ind.length(); i++)
        {


            for(int j = 0; j<list.length(); j++)
            {/*
                if(list[j] =="" || list2[j]=="")
                {
                    QMessageBox::information(NULL,QObject::tr("Информация"),tr("Был введен неподобающий символ или одно из полей осталось пустым."));
                    return;
                }
                if(list[j] == " " || list2[j] == " ") return;*/
                QString pt = converting_index(list[j]);

                if(pt == " ")
                {
                    return;
                }
                if(ind[i] == pt && name[i] == list2[j])
                {
                    noelement =0;
                    qDebug()<<" I Deleted element from List"<<name[i];
                    continue;
                }

            }
            if(noelement == 1)
            {
                QDomElement node = document.createElement("Elements");
                qDebug()<< "List"<<ind[i]<<name[i];
                node.setAttribute("index",ind[i]);
                node.setAttribute("name",name[i]);
                root.appendChild(node);
            }
            noelement = 1;


        }


    }
    QFile file(path + "/catalog.xml");

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<< "Can't open file";
    }else
    {
        QTextStream stream(&file);
        //document.save(stream,0);
        stream << document.toString();
        file.close();
        qDebug()<<"Done";
    }
    }
}
void MainWindow::on_enter_clicked()
{

   QString str =  ui->id->toPlainText();
   QString str2 = ui->name->toPlainText();


   QStringList strList = str.split(' ');
   QStringList str2List = str2.split(' ');

   for(int i =0; i<strList.length(); i++)
   {
       if(incatalog(strList[i]))
       {
           QMessageBox::information(NULL,QObject::tr("Информация"),tr("Такой индекс уже есть в списке номенклатуры."));
           return;
       }
   }

   //write_in_xml("45646","petr");
   if(strList.length() == 1)
   {
       if(str2List[0] == "" || strList[0] == "")
       {
           QMessageBox::information(NULL,QObject::tr("Информация"),tr("Одно из полей пустое"));
           return;
       }
       if(str2List.length() != strList.length())
       {
           QMessageBox::information(NULL,QObject::tr("Информация"),tr("Количество индексов не равно количеству имен."));
           return;
       }
       qDebug()<<"Write ver1";
       QString pt = converting_index(strList[0]);
       if(pt == " ")
       {
           return;
       }

       write_in_xml(pt,str2List[0]);

   }else
   {
       qDebug()<<"Write ver2";
       write_in_xml(strList,str2List);
   }




}
QString MainWindow::find_name(QString& str)
{
    QStringList index = read_from_xml("index");
    QStringList name = read_from_xml("name");

    for(int i = 0; i< index.length(); i++)
    {
        if(index[i] == str)
        {
            return name[i];
        }
    }
    QString tmp = "";
    return tmp;
}
void MainWindow::in_temple_catalog(QStringList& index, QStringList& name, QList<int>& count,QString& str)
{

    if( incatalog(str) == true)
    {
        bool b = 1;
       // QStringList temple = read_from_xml("name");
        for(int i =0; i< index.length(); i++)
        {
            if(index[i] == str)
            {
                b=0;
                count[i] += 1;

            }

        }
        if(b == 1)
        {

            QString n = find_name(str);
            if(n == "")
            {
                QMessageBox::information(NULL,QObject::tr("Информация"),tr("Имя не найдено."));
                return;
            }
            index.push_back(str);
            name.push_back(n);
            count.push_back(1);

        }


    }

}
void MainWindow::in_temple_catalog(QStringList& index, QStringList& name, QList<int>& count,QStringList& list)
{

        bool b = 1;
        QString pt;
        int it;
        for(int i = 0; i< list.length(); i++)
        {

            if(list[i] == "") return;
            pt = converting_index(list[i]);
            if(pt == " ")
            {
                return;
            }
            if(incatalog(pt) == false){return;}
            for(int j = 0; j< index.length(); j++)
            {
                if(pt == index[j])
                {
                    b = 0;
                    count[j] += 1;
                }
            }
            if(b == 1)
            {
                QString n = find_name(pt);
                if(n == "")
                {
                    QMessageBox::information(NULL,QObject::tr("Информация"),tr("Имя не найдено."));
                    return;
                }
                index.push_back(pt);
                name.push_back(n);
                count.push_back(1);

            }
            b = 1;
        }



}
void MainWindow::out_of_temple_catalog(QStringList& index3, QStringList& name, QList<int>& count, QString& str)
{
    if(index3.length() == 0)
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Временное хранилище не содержит ни одного объекта."));
        return;
    }
    bool b = 1;
    int it;
    for(int i = 0; i<index3.length(); i++)
    {

        if(index3[i] == str && count[i] > 1)
        {

            b =0;
            count[i] -=1;

        }else if(index3[i] == str)
        {

            it = i;
        }

    }
    if(b == 1)
    {

        index3.removeAt(it);
        name.removeAt(it);
        count.removeAt(it);
        if(index.length() == 0)
        {
            ui->in->clear();
            table_settings();
            ui->in->setRowCount(0);
        }else if(index2.length() == 0)
        {
            ui->out->clear();
            table_settings();
            ui->out->setRowCount(0);

        }



    }



}
void MainWindow::out_of_temple_catalog(QStringList& index, QStringList& name, QList<int>& count,QStringList& list)
{
    if(index.length() == 0)
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Временное хранилище не содержит ни одного объекта."));
        return;
    }
    int it;
    QString pt;
    bool b = 1;
    for(int i = 0; i<list.length(); i++)
    {
        if(list[i] == "")
        {
            return;
        }
        pt = converting_index(list[i]);
        if(pt == " ")
        {
            return;
        }
        if(incatalog(pt) == false){return;}
        for(int j = 0 ; j< index.length(); j++)
        {
            if(pt == index[j] && count[j] > 1)
            {
                b = 0;
                count[j] -= 1;
            }
            else if(pt == index[j])
            {
                it = j;
            }

        }
        if(b == 1)
        {
            index.removeAt(it);
            name.removeAt(it);
            count.removeAt(it);
        }
        b = 1;

    }

}
void MainWindow::showListin()
{
    if(index.length() == 0)
    {
        ui->in->clear();
        table_settings();
        ui->in->setRowCount(0);
        return;
    }
    delete[] item;
    delete[] item2;
    //}
    ui->in->clear();
    table_settings();
    if(index.length() == 0)
    {
        return;
    }


    ui->in->setRowCount(index.length());
    item = new QTableWidgetItem[index.length()];
    item2 = new QTableWidgetItem[index.length()];
    for(int i=0; i <index.length(); i++)
    {

     (item+i)->setText(name[i]);
     ui->in->setItem(i,0, item+i);
     (item2+i)->setText(QString::number(count[i]));
     ui->in->setItem(i,1, item2+i);

    }

}
void MainWindow::showListout()
{
    if(index2.length() == 0)
    {
      ui->out->clear();
      table_settings();
      ui->out->setRowCount(0);
      return;
    }
    delete[] item3;
    delete[] item4;

    ui->out->clear();
    table_settings();
    if(index2.length() == 0)
    {
        return;
    }

    ui->out->setRowCount(index2.length());
    item3 = new QTableWidgetItem[index2.length()];
    item4 = new QTableWidgetItem[index2.length()];
    for(int i=0; i <index2.length(); i++)
    {
     (item3+i)->setText(name2[i]);
     ui->out->setItem(i,0, item3 + i);
     //item2->set
     (item4+i)->setText(QString::number(count2[i]));
     ui->out->setItem(i,1, item4 + i);
    }
}
bool MainWindow::inList(QStringList list, QString str)
{
    for(int i =0; i< list.length(); i++)
    {
        if(list[i] == str) return true;
    }
    return false;
}
void MainWindow::Pressin()
{
  qDebug()<<"Enter1";
  //QString str =  ui->text_in->toPlainText();
  //QString str2 = ui->text_out->toPlainText();
  QString str = ui->text_in->text();
  QStringList in = str.split(' ');
  bool b =0;

  if(in.length()== 0)
  {
      QMessageBox::information(NULL,QObject::tr("Информация"),tr("Поле пусто."));
      return;

  }
  if(in.length() == 1)
  {
      if(in[0] == "" || in[0] == " ") return;
      QString pt = converting_index(in[0]);
      if(pt == " ")
      {
          return;
      }
      if(incatalog(pt) == false)
      {
          QMessageBox::information(NULL,QObject::tr("Информация"),tr("Индекс не найден в каталоге."));
      }else{
      in_temple_catalog(index,name,count,pt);
      showListin();
      if(inList(index2,pt) == true)
      {
          out_of_temple_catalog(index2,name2,count2,pt);
          if(index2.length() == 0)
          {
              ui->out->clear();
              table_settings();
              ui->out->setRowCount(0);
          }else{
          showListout();
          }


      }
      }
      showListin();

  }

  if(in.length() > 1)
  {


      for(int k = 0; k<in.length(); k++)
      {
          QString tmp = converting_index(in[k]);
          if(tmp == " ") return;
          if(incatalog(tmp) == false)
          {
              QMessageBox::information(NULL,QObject::tr("Информация"),tr("Индекс не найден в каталоге."));
          }else{
            in_temple_catalog(index,name,count, tmp);
          if(inList(index2,tmp))
          {
             out_of_temple_catalog(index2,name2,count2,tmp);
             b = 1;

          }
          }
      }
      showListin();
      if(b == 1)
      {
          if(index2.length() == 0)
          {
              ui->out->clear();
              table_settings();
              ui->out->setRowCount(0);
          }else{
          showListout();
          }
      }

  }



}
void MainWindow::Pressout()
{

    QString str2 = ui->text_out->text();
    QStringList out = str2.split(' ');
    bool b = 0;

    if(out.length() == 0)
    {
        QMessageBox::information(NULL,QObject::tr("Информация"),tr("Оба поля пусты. Заполните хотя бы одно"));
        return;

    }

    if(out.length() == 1)
    {
        if(out[0] == "" || out[0] == " ") return;
        QString pt = converting_index(out[0]);
        if(pt == " ")
        {
            return;
        }
        if(incatalog(pt) == false)
        {
            QMessageBox::information(NULL,QObject::tr("Информация"),tr("Индекс не найден в каталоге."));
        }else{
        in_temple_catalog(index2,name2,count2,pt);
        showListout();
        if(inList(index,pt) == true)
        {
            out_of_temple_catalog(index,name,count,pt);
            if(index.length() == 0)
            {
                ui->in->clear();
                table_settings();
                ui->in->setRowCount(0);
            }else{
                    showListin();
            }
        }
        }
    }

    if(out.length() > 1)
    {

       for(int k = 0; k< out.length(); k++)
       {
           QString tmp = converting_index(out[k]);
           if(tmp == " ") return;
           if(incatalog(tmp) == false)
           {
                QMessageBox::information(NULL,QObject::tr("Информация"),tr("Индекс не найден в каталоге."));
           }else{
            in_temple_catalog(index2,name2,count2,tmp);
           if(inList(index,tmp))
           {
               out_of_temple_catalog(index,name,count,tmp);
               b = 1;
           }
           }
       }
       showListout();
       if(b == 1)
       {
           if(index.length() == 0)
           {
               ui->in->clear();
               table_settings();
               ui->in->setRowCount(0);
           }else{
           showListin();
           }
       }

    }

}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if( event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {



         //qDebug()<<"Enter";
        /*
        QString str =  ui->text_in->toPlainText();
        QString str2 = ui->text_out->toPlainText();

        QStringList in = str.split(' ');
        QStringList out = str2.split(' ');

        if(in.length() == out.length() == 0)
        {
            QMessageBox::information(NULL,QObject::tr("Информация"),tr("Оба поля пусты. Заполните хотя бы одно"));
            return;

        }
        if(in.length() == 1)
        {
            if(in[0] == "" || in[0] == " ") return;
            QString pt = converting_index(in[0]);
            if(pt == " ")
            {
                return;
            }
            in_temple_catalog(pt);
            showLists();
        }
        if(out.length() == 1)
        {
            if(out[0] == "" || out[0] == " ") return;
            QString pt = converting_index(in[0]);
            if(pt == " ")
            {
                return;
            }
            out_of_temple_catalog(pt);
        }
        if(in.length() > 1)
        {
            in_temple_catalog(in);
        }
        if(out.length() > 1)
        {
           out_of_temple_catalog(out);
        }*/


    }



}

void MainWindow::on_delet_clicked()
{
    QString str =  ui->id->toPlainText();
    QString str2 = ui->name->toPlainText();

    QStringList strList = str.split(' ');
    QStringList str2List = str2.split(' ');


    if(strList.length() == 1)
    {
        if(str2List.length() != strList.length())
        {
            QMessageBox::information(NULL,QObject::tr("Информация"),tr("Количество индексов не равно количеству имен."));
            return;
        }
        if(str2List[0] == "" || strList[0] == "")
        {
            QMessageBox::information(NULL,QObject::tr("Информация"),tr("Одно из полей пустое"));
            return;
        }
        qDebug()<<"Delete ver1";
        QString pt = converting_index(strList[0]);
        if(pt == " ")
        {
            return;
        }

        delete_from_xml(pt,str2List[0]);

    }else
    {
        qDebug()<<"Write ver2";
        delete_from_xml(strList,str2List);
    }
}

void MainWindow::on_clean_clicked()
{
    ui->in->clear();
    ui->out->clear();
    ui->in->setRowCount(0);
    ui->out->setRowCount(0);
    table_settings();
}

void MainWindow::on_clean_enter_clicked()
{
    ui->text_in->clear();
    ui->text_out->clear();
}
void MainWindow::table_settings()
{
    QStringList headers = {trUtf8("Наименование"),trUtf8("Количество")};

    ui->in->setColumnCount(2);
    ui->in->setShowGrid(true);
    ui->in->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->in->setHorizontalHeaderLabels(headers);
    ui->in->horizontalHeader()->setStretchLastSection(true);

    ui->out->setColumnCount(2);
    ui->out->setShowGrid(true);
    ui->out->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->out->setHorizontalHeaderLabels(headers);
    ui->out->horizontalHeader()->setStretchLastSection(true);

}
