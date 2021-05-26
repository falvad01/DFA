//
// Created by javi on 21/5/21.
//

#ifndef UNTITLED_BT_H
#define UNTITLED_BT_H

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>


using std::vector;
using std::string;
using std::cout;


struct cell_display {
    string valstr;
    bool present;

    cell_display() : present(false) {}

    cell_display(std::string valstr) : valstr(valstr), present(true) {}
};


using display_rows = vector<vector<cell_display> >;


class BT {

    struct Node {
        char value;
        Node *left, *right;

        Node() : left(nullptr), right(nullptr) {}

        Node(const char &value) : value(value), left(nullptr), right(nullptr) {}


        int max_depth() const {
            const int left_depth = left ? left->max_depth() : 0;
            const int right_depth = right ? right->max_depth() : 0;
            return (left_depth > right_depth ? left_depth : right_depth) + 1;
        }
    };


public:
    Node *root;


    BT();

    ~BT();

    void insertSimple(char root, char leaf1, char leaf2);

    void addLeft(const BT& bt);

    void addRight(const BT& bt);

    template<typename ...Args>
    void insert(const char &value, Args...more);

    void clear();

    display_rows get_row_display() const;

    void Dump() const;

    const int get_max_depth() const;


private:

    vector<string> row_formatter(const vector<vector<cell_display>> &rows_disp) const;

    static void trim_rows_left(vector<string> &rows);
};


#endif //UNTITLED_BT_H
