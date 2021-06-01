//
// Created by javi on 28/5/21.
//

#ifndef DFAQT_MAINWINDOW_H
#define DFAQT_MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <sstream>
#include <QTextEdit>
#include "BT.h"

using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QMainWindow {
Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);

    ~mainwindow() override;

private slots:
    void on_button_clicked();

private:

    BT mainTree;

    QTextEdit *br;
    QTextEdit *br2;

    Ui::mainwindow *ui;
    bool recursive(vector<string> splits, int i,int pos,int followPosPos);
    vector<string> analiceRe(string re);
    vector<char> stringToCharVector(string st);
    char stringToChar(string st);

    stringstream firstAndLast;

    vector<vector<int>> followPos;

};


#endif //DFAQT_MAINWINDOW_H
