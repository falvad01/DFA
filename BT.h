//
// Created by javi on 21/5/21.
//

#ifndef UNTITLED_BT_H
#define UNTITLED_BT_H

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "BT.h"

using std::vector;
using std::string;
using std::cout;


struct cell_display {
    string valstr;
    bool present;

    cell_display() : present(false) {}

    cell_display(std::string valstr) : valstr(valstr), present(true) {}
};


using display_rows = vector< vector< cell_display > >;


template<typename T>

class BT {



    struct Node {
        T value;
        Node *left, *right;

        Node() : left(nullptr), right(nullptr) {}

        Node(const T &value) : value(value), left(nullptr), right(nullptr) {}

        // stack-abusing recursion everywhere, for small code
        ~Node() {
            delete left;
            delete right;
        }

        int max_depth() const {
            const int left_depth = left ? left->max_depth() : 0;
            const int right_depth = right ? right->max_depth() : 0;
            return (left_depth > right_depth ? left_depth : right_depth) + 1;
        }
    };

    Node *root;

public:


    BT();
    ~BT();
    void insert() {};
    template<typename ...Args>
    void insert(const T &value, Args...more);
    void clear();
    display_rows get_row_display() const;
    void Dump() const;
    const int get_max_depth() const;



private:


    vector<string> row_formatter(const vector<vector<cell_display>> &rows_disp) const;
    static void trim_rows_left(vector<string>& rows);
};


#endif //UNTITLED_BT_H
