// node.cpp
#include "node.h"
#include <cstdlib>

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node)); //fixes allocation errors
    newNode->data = value;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = nullptr;
    newNode->color = true; //initiallized as red
    return newNode;
}

void deleteNode(Node* node) {
    if (node) {
        deleteNode(node->left);
        deleteNode(node->right);
        free(node);
    }
}
