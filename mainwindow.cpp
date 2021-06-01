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
#include <sstream>

using namespace std;


mainwindow::mainwindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);

    br = new QTextEdit(ui->textEdit);
    br2 = new QTextEdit(ui->textEdit_2);
    br->setMinimumHeight(300);
    br->setMinimumWidth(350);

    br2->setMinimumHeight(300);
    br2->setMinimumWidth(350);
}

mainwindow::~mainwindow() {
    delete ui;
}

void mainwindow::on_button_clicked() {

    string re = "(a|b)*.(c+d).d.#";
    // string re = "(a|b)*.a.b.b.#";
    /**
     * invert
     *
     * #
     * d
     * a
     * (c|d*)
     * (a|b)*
     */


    vector<string> splits = analiceRe(re);

    for (auto &split : splits) {
        cout << split << endl;
    }


    recursive(splits, 0, 1);


    cout << "sale" << endl;
    cout << "sale" << endl;
    cout << "sale" << endl;


}

bool mainwindow::recursive(vector<string> splits, int i, int pos) {

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

    if (mainTree.root->left->value == '*') {

        vector<int> a = mainTree.root->left->firstPos;

        stringstream first;
        stringstream last;

        for (int j = 0; j < a.size(); j++) {
            mainTree.root->firstPos.push_back(a[j]);
            first << a[j] << " ";
        }
        vector<int> b = mainTree.root->right->firstPos;
        for (int j = 0; j < b.size(); j++) {
            mainTree.root->firstPos.push_back(b[j]);
            mainTree.root->lastPost.push_back(b[j]);
            first << b[j] << " ";
            last << b[j] << " ";
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


    string sa = mainTree.Dump();
    cout << sa << endl;



    br->append(QString::fromStdString(sa));
    br->show();
    update();

    sleep(4);

    if (i == splits.size() - 1) {
        cout << firstAndLast.str() << endl;
        br2->append(QString::fromStdString(firstAndLast.str()));
        br2->show();
        return true;
    } else {
        i++;
        return recursive(splits, i, pos);
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

