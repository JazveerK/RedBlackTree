#include "node.h"
#include <cstdlib>

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node)); // fixes allocation errors
    newNode->data = value;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = nullptr;
    newNode->color = true; // initialized as red
    return newNode;
}

//fixed --> made it so that it recursively deletes nodes
// BIG FIX
void deleteNode(Node* node) {
    if (node) {
        deleteNode(node->left);
        deleteNode(node->right);
        free(node);
    }
}
