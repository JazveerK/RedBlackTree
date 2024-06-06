// Jazveer Kaler
// 05/02/24
// RedBlacktree
// Credit to Taaha for debugging and Jayden Huang for a lot of help
/* Used some random sources:
 https://www.geeksforgeeks.org/insertion-in-red-black-tree/
 https://www.codesdope.com/course/data-structures-red-black-trees-insertion/
 https://stackoverflow.com/questions/74105624/is-it-possible-to-avoid-duplicate-values-from-being-inserted-in-a-red-black-tree (SJ HOWE)
 https://www.youtube.com/watch?v=IuG3WSaDumc (watched a lot of his videos)
 
 TROUBLESHOOTING
 https://www.geeksforgeeks.org/deletion-in-red-black-tree/
 https://stackoverflow.com/questions/70249620/segmentation-fault-red-black-trees
 */

#include <iostream>
#include <cstring>
#include "node.h"

using namespace std;

// Function Prototypes
void rotateLeft(Node*& node);
void rotateRight(Node*& node);
void add(int value);
void remove(int value);
void fixTree(Node*& node);
void read(const char* filename);
void printFormat(Node* head, int space);
bool search(int value);
Node* findMin(Node* node);
void replaceNode(Node*& oldNode, Node*& newNode);
void fixDoubleBlack(Node*& node);
Node* getSibling(Node* node);
bool isLeftChild(Node* node);
bool hasRedChild(Node* node);
void deleteNode(Node* node);

Node* root = nullptr;

int main() {
    bool running = true;
    char command[50];

    cout << "Welcome to RedBlackTree" << endl; // This is hell

    while (running) {
        cout << endl;
        cout << "Please enter a command: ADD, READ, PRINT, DELETE, SEARCH, QUIT" << endl;
        cin.get(command, 50);
        cin.get();

        if (strcmp(command, "ADD") == 0) {
            int value;
            cout << "Enter the value to add: ";
            cin >> value;
            cin.get();
            add(value);
        }
        else if (strcmp(command, "READ") == 0) {
            char filename[50];
            cout << "Enter the filename: ";
            cin.get(filename, 50);
            cin.get();
            read(filename);
        }
        else if (strcmp(command, "PRINT") == 0) {
            printFormat(root, 0);
        }
        else if (strcmp(command, "DELETE") == 0) {
            int value;
            cout << "Enter the value to delete: ";
            cin >> value;
            cin.get();
            remove(value);
        }
        else if (strcmp(command, "SEARCH") == 0) {
            int value;
            cout << "Enter the value to search for: ";
            cin >> value;
            cin.get();
            bool found = search(value);
            if (found) {
                cout << "Value " << value << " found in the tree." << endl;
            } else {
                cout << "Value " << value << " not found in the tree." << endl;
            }
        }
        else if (strcmp(command, "QUIT") == 0) {
            cout << "Exiting the program..." << endl;
            running = false;
        }
    }

    deleteNode(root); //clears up some storage

    return 0;
}

void rotateLeft(Node*& node) {
    Node* rightChild = node->right;
    node->right = rightChild->left;

    if (node->right)
        node->right->parent = node;

    rightChild->parent = node->parent;

    if (!node->parent)
        root = rightChild;
    else if (node == node->parent->left)
        node->parent->left = rightChild;
    else
        node->parent->right = rightChild;

    rightChild->left = node;
    node->parent = rightChild;
}

void rotateRight(Node*& node) {
    Node* leftChild = node->left;
    node->left = leftChild->right;

    if (node->left)
        node->left->parent = node;

    leftChild->parent = node->parent;

    if (!node->parent)
        root = leftChild;
    else if (node == node->parent->left)
        node->parent->left = leftChild;
    else
        node->parent->right = leftChild;

    leftChild->right = node;
    node->parent = leftChild;
}

//Fix the RBT
void fixTree(Node*& node) {
    Node* parent = nullptr;
    Node* grandparent = nullptr;

    while (node != root && node->color == true && node->parent && node->parent->color == true) {
        parent = node->parent;
        grandparent = parent->parent;

        // Case 1: Parent is the left child of the grandparent
        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;
            if (uncle && uncle->color == true) {
                grandparent->color = true;
                parent->color = false;
                uncle->color = false;
                node = grandparent;
            } else {
                if (node == parent->right) {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }
                rotateRight(grandparent);
                bool temp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = temp;
                node = parent;
            }
        }
        // Case 2: Parent is the right child of the grandparent
        else {
            Node* uncle = grandparent->left;
            if (uncle && uncle->color == true) {
                grandparent->color = true;
                parent->color = false;
                uncle->color = false;
                node = grandparent;
            } else {
                if (node == parent->left) {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }
                rotateLeft(grandparent);
                bool temp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = temp;
                node = parent;
            }
        }
    }

    // Root of the tree is always black
    if (root)
        root->color = false;
}

void add(int value) {
    Node* newNode = createNode(value);

    if (!root) {
        root = newNode;
        root->color = false;
        return;
    }

    Node* node = root;
    Node* parent = nullptr;

    while (node) {
        parent = node;
        if (value < node->data)
            node = node->left;
        else if (value > node->data)
            node = node->right;
        // Handles duplicate values
        else {
            free(newNode);
            return;
        }
    }

    newNode->parent = parent;

    if (value < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;

    fixTree(newNode);
}

// Fixes deletion issues

// deletion & rebalancing not working
// double black
void remove(int value) {
    Node* node = root;
    Node* nodeToBeDeleted = nullptr;
    Node* child = nullptr;

    // Find the node
    while (node != nullptr) {
        if (value == node->data) {
            nodeToBeDeleted = node;
            break;
        } else if (value < node->data) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    if (nodeToBeDeleted == nullptr) {
        return;
    }

    Node* replacement = nullptr;

    if (nodeToBeDeleted->left == nullptr || nodeToBeDeleted->right == nullptr) {
        replacement = nodeToBeDeleted;
    } else {
        replacement = findMin(nodeToBeDeleted->right);
    }

    if (replacement->left != nullptr) {
        child = replacement->left;
    } else {
        child = replacement->right;
    }

    if (child != nullptr) {
        child->parent = replacement->parent;
    }

    if (replacement->parent == nullptr) {
        root = child;
    } else if (replacement == replacement->parent->left) {
        replacement->parent->left = child;
    } else {
        replacement->parent->right = child;
    }

    if (replacement != nodeToBeDeleted) {
        nodeToBeDeleted->data = replacement->data;
    }

    if (replacement->color == false && child != nullptr) {
        fixDoubleBlack(child);
    }

    free(replacement);
}

Node* findMin(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

void replaceNode(Node*& oldNode, Node*& newNode) {
    if (oldNode->parent == nullptr) {
        root = newNode;
    } else if (oldNode == oldNode->parent->left) {
        oldNode->parent->left = newNode;
    } else {
        oldNode->parent->right = newNode;
    }

    if (newNode != nullptr) {
        newNode->parent = oldNode->parent;
    }
}

void fixDoubleBlack(Node*& node) {
    if (node == root) {
        return;
    }

    Node* sibling = getSibling(node);
    Node* parent = node->parent;

    if (sibling == nullptr) {
        fixDoubleBlack(parent);
    } else {
        if (sibling->color == true) {
            parent->color = true;
            sibling->color = false;
            if (isLeftChild(sibling)) {
                rotateRight(parent);
            } else {
                rotateLeft(parent);
            }
            fixDoubleBlack(node);
        } else {
            if (hasRedChild(sibling)) {
                if (sibling->left != nullptr && sibling->left->color == true) {
                    if (isLeftChild(sibling)) {
                        sibling->left->color = sibling->color;
                        sibling->color = parent->color;
                        rotateRight(parent);
                    } else {
                        sibling->left->color = parent->color;
                        rotateRight(sibling);
                        rotateLeft(parent);
                    }
                } else {
                    if (isLeftChild(sibling)) {
                        sibling->right->color = parent->color;
                        rotateLeft(sibling);
                        rotateRight(parent);
                    } else {
                        sibling->right->color = sibling->color;
                        sibling->color = parent->color;
                        rotateLeft(parent);
                    }
                }
                parent->color = false;
            } else {
                sibling->color = true;
                if (parent->color == false) {
                    fixDoubleBlack(parent);
                } else {
                    parent->color = false;
                }
            }
        }
    }
}

Node* getSibling(Node* node) {
    if (node->parent == nullptr) {
        return nullptr;
    }

    if (isLeftChild(node)) {
        return node->parent->right;
    } else {
        return node->parent->left;
    }
}

bool isLeftChild(Node* node) {
    return node == node->parent->left;
}

bool hasRedChild(Node* node) {
    return (node->left != nullptr && node->left->color == true) ||
           (node->right != nullptr && node->right->color == true);
}

bool search(int value) {
    Node* node = root;

    while (node != nullptr) {
        if (value == node->data) {
            return true;
        } else if (value < node->data) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    return false;
}

// Randomly doesn't work
void read(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        cout << "Failed" << endl;
        return;
    }

    int num;
    while (fscanf(file, "%d", &num) != EOF) { //EOF checks if file has been completely read
        add(num);
    }

    fclose(file);
}

// Credit: Jayden Huang
void printFormat(Node* head, int space) {
    if (head == nullptr) {
        return;
    }

    space += 5;

    if (head->right != nullptr) {
        printFormat(head->right, space);
    }

    cout << endl;

    for (int i = 0; i < space; i++) {
        cout << " ";
    }

    cout << head->data << " (" << (head->color == true ? "Red" : "Black") << ")";
    if (head->parent)
        cout << " - Parent: " << head->parent->data;

    if (head->left != nullptr) {
        printFormat(head->left, space);
    }
}
