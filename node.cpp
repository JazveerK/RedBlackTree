#include "node.h"

//moved deleting nodes to main.cpp

Node* createNode(int data) {
    return new Node(data);
}

/*
 void deleteNode(Node* node) {
     if (node) {
         deleteNode(node->left);
         deleteNode(node->right);
         free(node);
     }
 }
 */
