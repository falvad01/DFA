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
}

mainwindow::~mainwindow() {
    delete ui;
}

void mainwindow::on_button_clicked() {

    string re = "(a|b)*.(c|d*).d.#";
    // string re = "a.b.c.(a|b)*.a.b.#";
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

    BT mainTree;
    BT mainTree2;

    mainTree = recursive(mainTree, splits, 0, 1);


    cout << "sale" << endl;


}

BT mainwindow::recursive(BT mainTree, vector<string> splits, int i, int pos) {

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



                } else {


                    int headPos;
                    for (int i = 0; i < chars.size(); i++) {

                        if (chars[i] == '^' || chars[i] == '$' || chars[i] == '\\' || chars[i] == '?' ||
                            chars[i] == '+' ||
                            chars[i] == '|') {
                            mainTree.root->right = new Node(chars[i]);
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


                    //asterisk not englove all parenthesis
                } else {


                    int headPos;
                    for (int i = 0; i < chars.size(); i++) {

                        if (chars[i] == '^' || chars[i] == '$' || chars[i] == '\\' || chars[i] == '?' ||
                            chars[i] == '+' ||
                            chars[i] == '|') {
                            mainTree.root = new Node(chars[i]);
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
                        pos +=2;
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
    string sa = mainTree.Dump();
    cout << sa << endl;

    //  QTextEdit *br = new QTextEdit(ui->textEdit);

    //  br->setText(QString::fromStdString(sa));

    //   br->show();
    // sleep(2);

    if (i == splits.size() - 1) {
        cout << firstAndLast.str() << endl;
        return mainTree;
    } else {
        i ++;
        return recursive(mainTree, splits, i, pos);
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

