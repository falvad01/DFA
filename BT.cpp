//
// Created by javi on 21/5/21.
//

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include "BT.h"



using display_rows = vector<vector<cell_display>>;
using s_t = string::size_type;
using namespace std;

BT::BT() : root(nullptr) {}


BT::~BT() {
    DestroyRecursive(root);
}

template<typename ...Args>
void BT::insert(const char &value, Args...more) {
    if (!root) {
        root = new Node(value);
    } else {
        Node *p = root;
        for (;;) {
            if (value == p->value) return;
            Node *&pchild = value < p->value ? p->left : p->right;
            if (!pchild) {
                pchild = new Node(value);
                break;
            }
            p = pchild;
        }
    }
    insert(more...);
}


void BT::insertSimple(char rootArg, char leaf1, char leaf2) {

    root = new Node(rootArg);

    root->left = new Node(leaf1);
    root->right = new Node(leaf2);

}


void BT::addLeft(Node *bt) {

    this->root->left = bt;

}


void BT::addRight(const BT& bt) {
    this->root->right = bt.root;
}


display_rows BT::get_row_display() const {
    // start off by traversing the tree to
    // build a vector of vectors of Node pointers
    vector<Node *> traversal_stack;
    vector<  vector<Node *> > rows;
    if (!root) return display_rows();

    Node *p = root;
    const int max_depth = root->max_depth();
    rows.resize(max_depth);
    int depth = 0;
    for (;;) {
        // Max-depth Nodes are always a leaf or null
        // This special case blocks deeper traversal
        if (depth == max_depth - 1) {
            rows[depth].push_back(p);
            if (depth == 0) break;
            --depth;
            continue;
        }

        // First visit to node?  Go to left child.
        if (traversal_stack.size() == depth) {
            rows[depth].push_back(p);
            traversal_stack.push_back(p);
            if (p) p = p->left;
            ++depth;
            continue;
        }

        // Odd child count? Go to right child.
        if (rows[depth + 1].size() % 2) {
            p = traversal_stack.back();
            if (p) p = p->right;
            ++depth;
            continue;
        }

        // Time to leave if we get here

        // Exit loop if this is the root
        if (depth == 0) break;

        traversal_stack.pop_back();
        p = traversal_stack.back();
        --depth;
    }

    // Use rows of Node pointers to populate rows of cell_display structs.
    // All possible slots in the tree get a cell_display struct,
    // so if there is no actual Node at a struct's location,
    // its boolean "present" field is set to false.
    // The struct also contains a string representation of
    // its Node's value, created using a   stringstream object.
    display_rows rows_disp;
      stringstream ss;
    for (const auto &row : rows) {
        rows_disp.emplace_back();
        for (Node *pn : row) {
            if (pn) {
                ss << pn->value;
                rows_disp.back().push_back(cell_display(ss.str()));
                ss =   stringstream();
            } else {
                rows_disp.back().push_back(cell_display());
            }
        }
    }
    return rows_disp;

}


vector<string> BT::row_formatter(const display_rows &rows_disp) const {

    // First find the maximum value string length and put it in cell_width
    s_t cell_width = 0;
    for (const auto &row_disp : rows_disp) {
        for (const auto &cd : row_disp) {
            if (cd.present && cd.valstr.length() > cell_width) {
                cell_width = cd.valstr.length();
            }
        }
    }

    // make sure the cell_width is an odd number
    if (cell_width % 2 == 0) ++cell_width;

    // formatted_rows will hold the results
    vector<string> formatted_rows;

    // some of these counting variables are related,
    // so its should be possible to eliminate some of them.
    s_t row_count = rows_disp.size();

    // this row's element count, a power of two
    s_t row_elem_count = 1 << (row_count - 1);

    // left_pad holds the number of space charactes at the beginning of the bottom row
    s_t left_pad = 0;

    // Work from the level of maximum depth, up to the root
    // ("formatted_rows" will need to be reversed when done)
    for (s_t r = 0; r < row_count; ++r) {
        const auto &cd_row = rows_disp[row_count - r - 1]; // r reverse-indexes the row
        // "space" will be the number of rows of slashes needed to get
        // from this row to the next.  It is also used to determine other
        // text offsets.
        s_t space = (s_t(1) << r) * (cell_width + 1) / 2 - 1;
        // "row" holds the line of text currently being assembled
        string row;
        // iterate over each element in this row
        for (s_t c = 0; c < row_elem_count; ++c) {
            // add padding, more when this is not the leftmost element
            row += string(c ? left_pad * 2 + 1 : left_pad, ' ');
            if (cd_row[c].present) {
                // This position corresponds to an existing Node
                const string &valstr = cd_row[c].valstr;
                // Try to pad the left and right sides of the value string
                // with the same number of spaces.  If padding requires an
                // odd number of spaces, right-sided children get the longer
                // padding on the right side, while left-sided children
                // get it on the left side.
                s_t long_padding = cell_width - valstr.length();
                s_t short_padding = long_padding / 2;
                long_padding -= short_padding;
                row += string(c % 2 ? short_padding : long_padding, ' ');
                row += valstr;
                row += string(c % 2 ? long_padding : short_padding, ' ');
            } else {
                // This position is empty, Nodeless...
                row += string(cell_width, ' ');
            }
        }
        // A row of spaced-apart value strings is ready, add it to the result vector
        formatted_rows.push_back(row);

        // The root has been added, so this loop is finsished
        if (row_elem_count == 1) break;

        // Add rows of forward- and back- slash characters, spaced apart
        // to "connect" two rows' Node value strings.
        // The "space" variable counts the number of rows needed here.
        s_t left_space = space + 1;
        s_t right_space = space - 1;
        for (s_t sr = 0; sr < space; ++sr) {
            string row;
            for (s_t c = 0; c < row_elem_count; ++c) {
                if (c % 2 == 0) {
                    row += string(c ? left_space * 2 + 1 : left_space, ' ');
                    row += cd_row[c].present ? '/' : ' ';
                    row += string(right_space + 1, ' ');
                } else {
                    row += string(right_space, ' ');
                    row += cd_row[c].present ? '\\' : ' ';
                }
            }
            formatted_rows.push_back(row);
            ++left_space;
            --right_space;
        }
        left_pad += space + 1;
        row_elem_count /= 2;
    }

    // Reverse the result, placing the root node at the beginning (top)
      reverse(formatted_rows.begin(), formatted_rows.end());

    return formatted_rows;


}


void BT::trim_rows_left(vector<string> &rows) {
    if (!rows.size()) return;
    auto min_space = rows.front().length();
    for (const auto &row : rows) {
        auto i = row.find_first_not_of(' ');
        if (i == string::npos) i = row.length();
        if (i == 0) return;
        if (i < min_space) min_space = i;
    }
    for (auto &row : rows) {
        row.erase(0, min_space);
    }

}


string BT::Dump() const {
    const int d = get_max_depth();

    // If this tree is empty, tell someone
    if (d == 0) {
        cout << " <empty tree>\n";
        return "";
    }

    // This tree is not empty, so get a list of node values...
    const auto rows_disp = get_row_display();
    // then format these into a text representation...
    auto formatted_rows = row_formatter(rows_disp);
    // then trim excess space characters from the left sides of the text...
    trim_rows_left(formatted_rows);
    // then dump the text to cout.

    stringstream ret;
    for (const auto &row : formatted_rows) {
        ret << row<< '\n';
      //  cout << ' ' << row << '\n';
    }

    return ret.str();
}


void BT::clear() {
    delete root;
    root = nullptr;
}


const int BT::get_max_depth() const {
    return root ? root->max_depth() : 0;
}

void BT::insertRoot(Node *newroot) {

    newroot->left=root;
    root=newroot;

}

void BT::addNode(Node *node) {

    this->root = node;

}

void BT::DestroyRecursive(Node *node)
{
    if (node)
    {
        DestroyRecursive(node->left);
        DestroyRecursive(node->right);
        delete node;
    }
}





















