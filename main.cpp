#include <iostream>
#include <string>
#include <vector>
#include "BT.h"
#include <cstring>

using namespace std;
void recursive(Node *node, vector<string> splits);


vector<string> analiceRe(string re) {


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

char stringToChar(string st) {

    char p[st.length()];

    for (int i = 0; i < sizeof(p); i++) {
        p[i] = st[i];

    }
    char ret = p[0];

    return ret;

}

vector<char>stringToCharVector(string st) {

    vector <char> p;

    for (int i = 0; i < st.size(); i++) {
        p.push_back(st[i]);
    }

    return p;

}


int main() {
    const char *c_str();


    string re = "(a|b)*.(c|d*).a.d.#";
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

    for (int i = 0; i < splits.size(); ++i) {
        cout << splits[i] << endl;
    }



   // mainTree.Dump();

   // recursive(mainTree->root,splits);













    BT bt;

    bt.root= new Node('z');

    Node *b= new Node('b');
    Node *c= new Node('c');
    bt.root->left=b;
    bt.root->right=c;
    bt.root->right->value='x';
    Node *d= new Node('d');
    bt.insertRoot(d);






    cout << "Tree 1 from OP:\n\n";
    bt.Dump();
    cout << "\n\n";





}
void recursive(Node *node, vector<string> splits) {

    for (int i = splits.size() - 1; i >= 0; i--) {

        bool haveAste = false;

        vector<char> charArray = stringToCharVector(splits[i]);
        for (char j : charArray) {

            if (j == '*') {
                haveAste = true;
            }
        }
        // cout<<haveAste<<endl;

        if (haveAste) {


        } else {


            Node * aux;

            aux->value;
            if (node->left== nullptr){
                node->left = aux;
            }else{
                recursive(node->left,splits);
            }




        }


    }
}



