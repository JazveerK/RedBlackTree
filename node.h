// node.h
#ifndef NODE_H
#define NODE_H

struct Node {
    int data;
    Node* left;
    Node* right;
    Node* parent;
    bool color; //true = RED | false = BLACK
};

Node* createNode(int value);
void deleteNode(Node* node);

#endif
