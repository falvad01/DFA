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
}

mainwindow::~mainwindow() {
    delete ui;
}

void mainwindow::on_button_clicked() {

    string re = "(a|b)*.(c|d*).(a|b)*.d.#";
    //string re = "(a|b)*.a.b.#";
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

    recursive(mainTree, splits, 0);



    cout << "sale" << endl;
    //QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);


}

BT mainwindow::recursive(BT mainTree, vector<string> splits, int i) {

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
                    mainTree.root->right->right = new Node(chars[headPos + 1]);
                    mainTree.insertRoot(new Node('*'));

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
                    // * at right
                    if (asteriskPosition > headPos) {

                        aste->right = new Node(chars[headPos + 1]);
                        mainTree.root->right->right = aste;
                        mainTree.root->right->left = new Node(chars[headPos - 1]);
                        // * at right
                    } else if (asteriskPosition < headPos) {
                        aste->left = new Node(chars[headPos - 2]);
                        mainTree.root->right->left = aste;
                        mainTree.root->right->right = new Node(chars[headPos + 1]);
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
                mainTree.root->right->right = new Node(chars[headPos + 1]);


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

                if (chars[asteriskPosition - 1] == ')') {

                    int headPos;
                    for (int i = 0; i < chars.size(); i++) {

                        if (chars[i] == '^' || chars[i] == '$' || chars[i] == '\\' || chars[i] == '?' ||
                            chars[i] == '+' ||
                            chars[i] == '|') {
                            mainTree.root = new Node(chars[i]);
                            headPos = i;
                        }
                    }

                    mainTree.root->left = new Node(chars[headPos - 1]);
                    mainTree.root->right = new Node(chars[headPos + 1]);
                    mainTree.insertRoot(new Node('*'));

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
                    // * at right
                    if (asteriskPosition > headPos) {

                        aste->right = new Node(chars[headPos + 1]);
                        mainTree.root->right = aste;
                        mainTree.root->left = new Node(chars[headPos - 1]);
                        // * at right
                    } else if (asteriskPosition < headPos) {
                        aste->left = new Node(chars[headPos - 2]);
                        mainTree.root->left = aste;
                        mainTree.root->right = new Node(chars[headPos + 1]);
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
                mainTree.root->right = new Node(chars[headPos + 1]);


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
        return mainTree;
    } else {
        i = i + 1;
        return recursive(mainTree, splits, i);
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

