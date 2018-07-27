#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QtCore>
#include <QtXml>
#include <QDebug>
#include<QTextEdit>
#include<QKeyEvent>
#include <QtGui>
#include <QList>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QStringList index;
    QStringList name;
    QList<int> count;

    QStringList index2;
    QStringList name2;
    QList<int> count2;
    QTableWidgetItem* item;
    QTableWidgetItem* item2;
    QTableWidgetItem* item3;
    QTableWidgetItem* item4;

public:
    explicit MainWindow(QWidget *parent = 0);
     QString path;
    ~MainWindow();


private slots:
    void on_enter_clicked();

    void on_delet_clicked();

    void on_clean_clicked();

    void on_clean_enter_clicked();

    void table_settings();
    void write_in_xml(QString& str, QString& str2);
    void write_in_xml(QStringList& list, QStringList& list2);
    QString& converting_index(QString& list);
    bool incatalog(const QString& str);
    QStringList read_from_xml(QString attribute);
    void delete_from_xml(QString& str, QString& str2);
    void delete_from_xml(QStringList& list, QStringList& list2);
    void in_temple_catalog(QStringList& index, QStringList& name, QList<int>& count,QString& str);
    void in_temple_catalog(QStringList& index, QStringList& name, QList<int>& count,QStringList& list);
    void out_of_temple_catalog(QStringList& index, QStringList& name, QList<int>& count,QString& str);
    void out_of_temple_catalog(QStringList& index, QStringList& name, QList<int>& count,QStringList& list);
    void showListin();
    void showListout();
    void Pressin();
    void Pressout();
    bool inList(QStringList list, QString str);
    QString find_name(QString & str);


protected:
    void keyPressEvent(QKeyEvent *);
private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
