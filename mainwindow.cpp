//
// Created by javi on 28/5/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>
#include <vector>
#include "BT.h"
#include <unistd.h>


using namespace std;


mainwindow::mainwindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);

    br = ui->textEdit;
    br2 = ui->textEdit_2;

}

mainwindow::~mainwindow() {
    delete ui;
}

void mainwindow::on_button_clicked() {

    QString re = ui->input->text();
    string resrt = re.toStdString();






    //string re = "(a|b)*.(c+d).a.d.#";
    // string re = "(a|b)*.a.b.b.#";

   // (a|b)*.(c+d*).(c*^d).a.d.#

    /**
     * invert
     *
     * #
     * d
     * a
     * (c|d*)
     * (a|b)*
     */

    vector<string> splits = analiceRe(resrt);

    vector<char> chars = this->stringToCharVector(resrt);

    int parethesisIn = 0;
    int parethesisOut = 0;
    bool symbolsToguether = false;
    for (int i = 0; i < chars.size(); i++) {

        if (chars[i] == '(') {
            parethesisIn++;
        }

        if (chars[i] == ')') {
            parethesisOut++;
        }

        if (i == 0) {
            if ((chars[0] == '(' || chars[0] == ')' || chars[0] == '^' || chars[0] == '$' || chars[0] == '\\' ||
                 chars[0] == '?' || chars[0] == '+' || chars[0] == '|' || chars[0] == '.')
                &&
                (chars[1] == '.' || chars[1] == '(' || chars[1] == ')' || chars[1] == '^' || chars[1] == '$' ||
                 chars[1] == '\\' ||
                 chars[1] == '?' ||
                 chars[1] == '+' ||
                 chars[1] == '|')) {

                symbolsToguether = true;
            }
        } else if (i == chars.size() - 1) {
            if ((chars[i] == '(' || chars[i] == ')' || chars[i] == '^' || chars[i] == '$' || chars[i] == '\\' ||
                 chars[i] == '?' || chars[i] == '+' || chars[i] == '|' || chars[i] == '.')
                &&
                (chars[i + 1] == '.' || chars[i + 1] == '(' || chars[i + 1] == ')' || chars[i + 1] == '^' ||
                 chars[i + 1] == '$' ||
                 chars[i + 1] == '\\' ||
                 chars[i + 1] == '?' ||
                 chars[i + 1] == '+' ||
                 chars[i + 1] == '|')) {

                symbolsToguether = true;
            }
        } else {

            if ((chars[chars.size() - 1] == '(' || chars[chars.size() - 1] == ')' || chars[chars.size() - 1] == '^' ||
                 chars[chars.size() - 1] == '$' || chars[chars.size() - 1] == '\\' ||
                 chars[chars.size() - 1] == '?' || chars[chars.size() - 1] == '+' || chars[chars.size() - 1] == '|' ||
                 chars[chars.size() - 1] == '.')
                &&
                (chars[chars.size() - 2] == '.' || chars[chars.size() - 2] == '(' || chars[chars.size() - 2] == ')' ||
                 chars[chars.size() - 2] == '^' ||
                 chars[chars.size() - 2] == '$' ||
                 chars[chars.size() - 2] == '\\' ||
                 chars[chars.size() - 2] == '?' ||
                 chars[chars.size() - 2] == '+' ||
                 chars[chars.size() - 2] == '|')) {

                symbolsToguether = true;
            }

        }

    }


    if (splits[splits.size() - 1] != "#") {

        ui->errorLabel->setText("<font color='red'>NO # at the end</font>");


    } else if (parethesisIn != parethesisOut) {
        ui->errorLabel->setText("<font color='red'>Missing ( or )</font>");

    } else if (symbolsToguether) {
        ui->errorLabel->setText("<font color='red'>Bad using of the symbols</font>");
    } else {


        for (auto &split : splits) {
            cout << split << endl;
        }


        recursive(splits, 0, 1, 0);

        cout << "sale" << endl;


        QTableWidget *table = ui->tableWidget;

        table->setRowCount(followPos.size());

        for (int i = 0; i < followPos.size(); i++) {

            stringstream ss;
            for (int j = 0; j < followPos[i].size(); j++) {

                ss << followPos[i][j] << ", ";
            }
            cout << ss.str() << endl;
            QTableWidgetItem *newItem = new QTableWidgetItem(tr("%3").arg(QString::fromStdString(ss.str())));
            table->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
            table->setItem(i, 1, newItem);

        }

        //  table->setEditTriggers(QAbstractItemView::NoEditTriggers);

        table->show();

    }
}

bool mainwindow::recursive(vector<string> splits, int i, int pos, int followPosPos) {

    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << splits[i] << "-->" << endl;

    //rest of iteractions
    if (i > 0) {
        mainTree.insertRoot(new Node('.'));


        vector<char> chars = stringToCharVector(splits[i]);

        bool haveParentesis = false;
        int parentesisPosition;
        for (int i = 0; i < chars.size(); i++) {

            if (chars[i] == '(') {
                haveParentesis = true;
                parentesisPosition = i;
            }
        }


        //if the sentence have no parentesis the node are without leafs
        if (!haveParentesis) {

            mainTree.root->right = new Node(chars[0]);
            mainTree.root->right->firstPos.push_back(pos);
            mainTree.root->right->lastPost.push_back(pos);
            firstAndLast << chars[0] << " --> " << pos << endl;
            pos++;

        } else {




            //Check if the string have an *
            bool haveasterisk = false;
            int asteriskPosition;
            for (int i = 0; i < chars.size(); i++) {

                if (chars[i] == '*') {
                    haveasterisk = true;
                    asteriskPosition = i;
                }
            }


            //the part of the RE have an *
            if (haveasterisk) {

                if (chars[asteriskPosition - 1] == ')') {

                    int headPos;
                    for (int i = 0; i < chars.size(); i++) {

                        if (chars[i] == '^' || chars[i] == '$' || chars[i] == '\\' || chars[i] == '?' ||
                            chars[i] == '+' ||
                            chars[i] == '|') {
                            mainTree.root->right = new Node(chars[i]);
                            mainTree.root->right->firstPos.push_back(pos);
                            mainTree.root->right->firstPos.push_back(pos + 1);
                            mainTree.root->right->lastPost.push_back(pos);
                            mainTree.root->right->lastPost.push_back(pos + 1);
                            firstAndLast << chars[i] << " --> " << pos << " " << pos + 1 << endl;
                            headPos = i;
                        }
                    }

                    mainTree.root->right->left = new Node(chars[headPos - 1]);
                    mainTree.root->right->left->firstPos.push_back(pos);
                    mainTree.root->right->left->lastPost.push_back(pos);
                    firstAndLast << chars[headPos - 1] << " --> " << pos << endl;
                    pos++;
                    mainTree.root->right->right = new Node(chars[headPos + 1]);
                    mainTree.root->right->right->firstPos.push_back(pos);
                    mainTree.root->right->right->lastPost.push_back(pos);
                    firstAndLast << chars[headPos + 1] << " --> " << pos << endl;
                    pos++;
                    mainTree.insertRoot(new Node('*')); //TODO revisar
                    mainTree.root->firstPos.push_back(pos - 2);
                    mainTree.root->firstPos.push_back(pos - 1);

                    mainTree.root->lastPost.push_back(pos - 2);
                    mainTree.root->lastPost.push_back(pos - 1);


                    //asterisk is not with a )
                } else {


                    int headPos;
                    for (int i = 0; i < chars.size(); i++) {

                        if (chars[i] == '^' || chars[i] == '$' || chars[i] == '\\' || chars[i] == '?' ||
                            chars[i] == '+' ||
                            chars[i] == '|') {
                            mainTree.root->right = new Node(chars[i]);
                            mainTree.root->right->firstPos.push_back(pos);
                            mainTree.root->right->firstPos.push_back(pos + 1);
                            mainTree.root->right->lastPost.push_back(pos);
                            mainTree.root->right->lastPost.push_back(pos + 1);
                            firstAndLast << chars[i] << " --> " << pos << " " << pos + 1 << endl;
                            headPos = i;
                        }
                    }

                    Node *aste = new Node('*');
                    aste->firstPos.push_back(pos + 1); //TODO posible revison, no se aun
                    aste->lastPost.push_back(pos + 1);
                    firstAndLast << "*" << " --> " << pos + 1 << endl;
                    // * at right
                    if (asteriskPosition > headPos) {

                        aste->right = new Node(chars[headPos + 1]);
                        aste->right->firstPos.push_back(pos + 1);
                        aste->right->lastPost.push_back(pos + 1);
                        firstAndLast << chars[headPos + 1] << " --> " << pos + 1 << endl;
                        mainTree.root->right->right = aste;
                        mainTree.root->right->left = new Node(chars[headPos - 1]);
                        mainTree.root->right->left->firstPos.push_back(pos);
                        mainTree.root->right->left->lastPost.push_back(pos);
                        firstAndLast << chars[headPos - 1] << " --> " << pos << endl;
                        pos += 2;
                        // * at right
                    } else if (asteriskPosition < headPos) {
                        aste->left = new Node(chars[headPos - 2]);
                        aste->left->firstPos.push_back(pos);
                        aste->left->lastPost.push_back(pos);
                        firstAndLast << chars[headPos - 2] << " --> " << pos << endl;
                        pos++;
                        mainTree.root->right->left = aste;
                        mainTree.root->right->right = new Node(chars[headPos + 1]);
                        mainTree.root->right->right->firstPos.push_back(pos);
                        mainTree.root->right->right->lastPost.push_back(pos);
                        firstAndLast << chars[headPos + 1] << " --> " << pos << endl;
                        pos++;
                    }

                }
                //the part of the RE not have an *
            } else {


                int headPos;
                for (int i = 0; i < chars.size(); i++) {

                    if (chars[i] == '^' || chars[i] == '$' || chars[i] == '\\' || chars[i] == '?' || chars[i] == '+' ||
                        chars[i] == '|') {
                        mainTree.root->right = new Node(chars[i]);
                        mainTree.root->right->lastPost.push_back(pos);
                        mainTree.root->right->lastPost.push_back(pos + 1);
                        mainTree.root->right->firstPos.push_back(pos);
                        mainTree.root->right->firstPos.push_back(pos + 1);
                        firstAndLast << chars[i] << " --> " << pos << " " << pos + 1 << endl;
                        headPos = i;
                    }
                }

                mainTree.root->right->left = new Node(chars[headPos - 1]);
                mainTree.root->right->left->firstPos.push_back(pos);
                mainTree.root->right->left->lastPost.push_back(pos);
                firstAndLast << chars[headPos - 1] << " --> " << pos << endl;
                pos++;
                mainTree.root->right->right = new Node(chars[headPos + 1]);
                mainTree.root->right->right->firstPos.push_back(pos);
                mainTree.root->right->right->lastPost.push_back(pos);
                firstAndLast << chars[headPos + 1] << " --> " << pos << endl;
                pos++;

            }

        }











        //Only first iteractoin (i == 0)
    } else {


        vector<char> chars = stringToCharVector(splits[i]);

        bool haveParentesis = false;
        int parentesisPosition;
        for (int i = 0; i < chars.size(); i++) {

            if (chars[i] == '(') {
                haveParentesis = true;
                parentesisPosition = i;
            }
        }


        //if the sentence have no parentesis the node are without leafs
        if (!haveParentesis) {

            mainTree.root = new Node(chars[0]);
            mainTree.root->firstPos.push_back(pos);
            mainTree.root->lastPost.push_back(pos);
            firstAndLast << chars[0] << " --> " << pos << endl;
            pos++;
        } else {




            //Cech if the string have an *
            bool haveasterisk = false;
            int asteriskPosition;
            for (int i = 0; i < chars.size(); i++) {

                if (chars[i] == '*') {
                    haveasterisk = true;
                    asteriskPosition = i;
                }
            }


            //the part of the RE have an *
            if (haveasterisk) {

                //asterisk englove all paprentesis
                if (chars[asteriskPosition - 1] == ')') {

                    int headPos;
                    for (int i = 0; i < chars.size(); i++) {

                        if (chars[i] == '^' || chars[i] == '$' || chars[i] == '\\' || chars[i] == '?' ||
                            chars[i] == '+' ||
                            chars[i] == '|') {
                            mainTree.root = new Node(chars[i]); // add the parent
                            mainTree.root->firstPos.push_back(pos);
                            mainTree.root->firstPos.push_back(pos + 1);
                            mainTree.root->lastPost.push_back(pos);
                            mainTree.root->lastPost.push_back(pos + 1);
                            firstAndLast << chars[i] << " --> " << pos << " " << pos + 1 << endl;
                            headPos = i;
                        }
                    }

                    mainTree.root->left = new Node(chars[headPos - 1]);
                    mainTree.root->left->firstPos.push_back(pos);
                    mainTree.root->left->lastPost.push_back(pos);
                    firstAndLast << chars[headPos - 1] << " --> " << pos << endl;
                    pos++;
                    mainTree.root->right = new Node(chars[headPos + 1]);
                    mainTree.root->right->firstPos.push_back(pos);
                    mainTree.root->right->lastPost.push_back(pos);
                    firstAndLast << chars[headPos + 1] << " --> " << pos << endl;
                    pos++;
                    mainTree.insertRoot(new Node('*'));//TODO revisar
                    mainTree.root->firstPos.push_back(pos - 2);
                    mainTree.root->firstPos.push_back(pos - 1);

                    mainTree.root->lastPost.push_back(pos - 2);
                    mainTree.root->lastPost.push_back(pos - 1);


                    //asterisk not englove all parenthesis
                } else {


                    int headPos;
                    for (int i = 0; i < chars.size(); i++) {

                        if (chars[i] == '^' || chars[i] == '$' || chars[i] == '\\' || chars[i] == '?' ||
                            chars[i] == '+' ||
                            chars[i] == '|') {
                            mainTree.root = new Node(chars[i]);
                            mainTree.root->firstPos.push_back(pos);
                            mainTree.root->firstPos.push_back(pos + 1);
                            mainTree.root->lastPost.push_back(pos);
                            mainTree.root->lastPost.push_back(pos + 1);
                            firstAndLast << chars[i] << " --> " << pos << " " << pos + 1 << endl;
                            headPos = i;
                        }
                    }

                    Node *aste = new Node('*');
                    aste->firstPos.push_back(pos + 1);
                    aste->lastPost.push_back(pos + 1);
                    firstAndLast << "*" << " --> " << pos + 1 << endl;

                    // * at right
                    if (asteriskPosition > headPos) {

                        aste->right = new Node(chars[headPos + 1]);
                        aste->right->firstPos.push_back(pos + 1);
                        aste->right->lastPost.push_back(pos + 1);
                        firstAndLast << chars[headPos + 1] << " --> " << pos + 1 << endl;
                        mainTree.root->right = aste;
                        mainTree.root->left = new Node(chars[headPos - 1]);
                        mainTree.root->left->firstPos.push_back(pos);
                        mainTree.root->left->lastPost.push_back(pos);
                        firstAndLast << chars[headPos - 1] << " --> " << pos << endl;
                        pos += 2;
                        // * at right
                    } else if (asteriskPosition < headPos) {
                        aste->left = new Node(chars[headPos - 2]);
                        aste->left->firstPos.push_back(pos);
                        aste->left->lastPost.push_back(pos);
                        firstAndLast << chars[headPos - 2] << " --> " << pos << endl;
                        pos++;
                        mainTree.root->left = aste;
                        mainTree.root->right = new Node(chars[headPos + 1]);
                        mainTree.root->right->firstPos.push_back(pos);
                        mainTree.root->right->lastPost.push_back(pos);
                        firstAndLast << chars[headPos + 1] << " --> " << pos << endl;
                        pos++;

                    }


                }
                //the part of the RE not have an *
            } else {


                int headPos;
                for (int i = 0; i < chars.size(); i++) {

                    if (chars[i] == '^' || chars[i] == '$' || chars[i] == '\\' || chars[i] == '?' || chars[i] == '+' ||
                        chars[i] == '|') {
                        mainTree.root = new Node(chars[i]);
                        mainTree.root->firstPos.push_back(pos);
                        mainTree.root->firstPos.push_back(pos + 1);
                        mainTree.root->lastPost.push_back(pos);
                        mainTree.root->lastPost.push_back(pos + 1);
                        firstAndLast << chars[i] << " --> " << pos << " " << pos + 1 << endl;
                        headPos = i;
                    }
                }

                mainTree.root->left = new Node(chars[headPos - 1]);
                mainTree.root->left->firstPos.push_back(pos);
                mainTree.root->left->lastPost.push_back(pos);
                firstAndLast << chars[headPos - 1] << " --> " << pos << endl;
                pos++;
                mainTree.root->right = new Node(chars[headPos + 1]);
                mainTree.root->right->firstPos.push_back(pos);
                mainTree.root->right->lastPost.push_back(pos);
                firstAndLast << chars[headPos + 1] << " --> " << pos << endl;
                pos++;

            }


        }

    }

    cout << "esto: " << mainTree.root->left->value << endl;

    //////////////////first and last position to * and .
    if (mainTree.root->left->value == '*') {

        vector<int> a = mainTree.root->left->firstPos;

        stringstream first;
        stringstream last;

        for (int &j : a) {
            mainTree.root->firstPos.push_back(j);
            first << j << " ";
        }
        vector<int> b = mainTree.root->right->firstPos;
        for (int &j : b) {
            mainTree.root->firstPos.push_back(j);
            mainTree.root->lastPost.push_back(j);
            first << j << " ";
            last << j << " ";
        }

        firstAndLast << mainTree.root->value << " --> " << "first: " << first.str() << " last: " << last.str() << endl;


    } else if (mainTree.root->left->value == '.') {

        stringstream first;
        stringstream last;
        vector<int> a = mainTree.root->left->firstPos;

        for (int &j : a) {
            mainTree.root->firstPos.push_back(j);
            first << j << " ";
        }
        vector<int> b = mainTree.root->right->firstPos;
        for (int &j : b) {
            mainTree.root->lastPost.push_back(j);
            last << j << " ";
        }

        firstAndLast << mainTree.root->value << " --> " << "first: " << first.str() << " last: " << last.str() << endl;

    }

    cout << "EL VALOR ES: " << mainTree.root->value <<"y "<<followPosPos << endl;

    /////////////////////////follow positions

    if (mainTree.root->value == '*') {

        followPos.push_back(mainTree.root->firstPos);
        followPosPos++;
        followPos.push_back(mainTree.root->firstPos);
        followPosPos++;
    } else if (mainTree.root->value == '.') {


        if (mainTree.root->left->value == '*') {
            for (int &firstPo : mainTree.root->right->firstPos) {

                followPos[followPosPos - 2].push_back(firstPo);
                followPos[followPosPos - 1].push_back(firstPo);
            }





        } else if (mainTree.root->left->value == '.') {

            if (followPos.size() <= followPosPos - 1) {
                followPos.push_back(mainTree.root->right->firstPos);

                if(mainTree.root->left->left->right != nullptr) {
                    if (mainTree.root->left->left->right->right != nullptr) {
                        if (mainTree.root->left->left->right->right->value == '*') {

                            for (int &firstPo : mainTree.root->left->left->right->right->firstPos) {

                                followPos[followPosPos - 1].push_back(firstPo);

                            }


                        } else if (mainTree.root->left->left->right->left->value == '*') {

                            for (int &firstPo : mainTree.root->left->left->right->left->firstPos) {

                                followPos[followPosPos - 1].push_back(firstPo);

                            }
                        }
                    }
                }

            } else {

                for (int &firstPo : mainTree.root->right->firstPos) {

                    followPos[followPosPos - 1].push_back(firstPo);

                }
            }

        }
        cout << followPosPos << endl;
        followPosPos++;
    }


    string sa = mainTree.Dump();
    // cout << sa << endl;



    br->append(QString::fromStdString(sa));
    br->show();
    update();

    //sleep(4);

    if (i == splits.size() - 1) {
        // cout << firstAndLast.str() << endl;
        br2->append(QString::fromStdString(firstAndLast.str()));
        br2->show();
        return true;
    } else {
        i++;
        return recursive(splits, i, pos, followPosPos);
    }
}

vector<string> mainwindow::analiceRe(string re) {


    vector<string> splits;
    string delimiter = ".";

    size_t pos = 0;
    string token;
    while ((pos = re.find(delimiter)) != string::npos) {
        token = re.substr(0, pos);
        splits.push_back(token);
        re.erase(0, pos + delimiter.length());
    }
    splits.push_back(re);
/*
    for (int i = 0; i < splits.size(); ++i) {
        cout<<splits[i]<< endl;
    }
*/
    return splits;
}

char mainwindow::stringToChar(string st) {

    char p[st.length()];

    for (int i = 0; i < sizeof(p); i++) {
        p[i] = st[i];

    }
    char ret = p[0];

    return ret;

}

vector<char> mainwindow::stringToCharVector(string st) {

    vector<char> p;

    for (int i = 0; i < st.size(); i++) {
        p.push_back(st[i]);
    }

    return p;

}


void mainwindow::on_pushButton_clicked() {

    br->clear();
    br2->clear();
    while (ui->tableWidget->rowCount() > 0) {
        ui->tableWidget->removeRow(0);
    }

}
