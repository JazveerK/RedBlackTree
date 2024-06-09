#ifndef NODE_H
#define NODE_H

#include <cstddef>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node* parent;
    bool color; // true = Red, false = Black

    Node(int data) : data(data), left(NULL), right(NULL), parent(NULL), color(true) {}
};

Node* createNode(int data);
//void deleteNode(Node* node);

#endif
