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
 
 Restarted from almost scratch
 https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Operations
 */

#include <iostream>
#include <fstream>
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
void deleteTree(Node* node);

Node* root = NULL;

int main() {
    bool running = true;
    char command[50];

    cout << "Welcome to RedBlackTree" << endl;

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
            if (found)
                cout << "Value " << value << " found in the tree." << endl;
            else
                cout << "Value " << value << " not found in the tree." << endl;
            
        } 
        
        else if (strcmp(command, "QUIT") == 0) {
            cout << "Exiting the program..." << endl;
            running = false;
        }
    }

    deleteTree(root);
    return 0;
}

// Crashing --> Resolved
void read(const char* filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Failed to open file" << endl;
        return;
    }

    int num;
    while (file >> num) {
        add(num);
    }

    file.close();
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

// Fix the RBT
void fixTree(Node*& node) {
    Node* parent = NULL;
    Node* grandparent = NULL;

    while (node != root && node->color == true && node->parent && node->parent->color == true) {
        parent = node->parent;
        grandparent = parent->parent;

        // Case 1 - Parent is the left child of the gp
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
        // Case 2: Parent is the right child of the gp
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
    Node* parent = NULL;

    while (node) {
        parent = node;
        if (value < node->data)
            node = node->left;
        else if (value > node->data)
            node = node->right;
        // dupes
        else {
            delete newNode;
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

void remove(int value) {
    Node* node = root;
    Node* nodeToBeDeleted = nullptr;
    Node* child = nullptr;

    // look for node
    while (node != nullptr) {
        if (value == node->data) {
            nodeToBeDeleted = node;
            break;
        }
        else if (value < node->data)
            node = node->left;
        else
            node = node->right;
    
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

    delete replacement;
}


Node* findMin(Node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

Node* getSibling(Node* node) {
    if (node->parent == NULL) {
        return NULL;
    }

    if (node == node->parent->left) {
        return node->parent->right;
    } else {
        return node->parent->left;
    }
}

bool isLeftChild(Node* node) {
    return node == node->parent->left;
}

bool hasRedChild(Node* node) {
    return (node->left && node->left->color == true) || (node->right && node->right->color == true);
}

void fixDoubleBlack(Node*& node) {
    if (node == root)
        return;
    

    Node* sibling = getSibling(node);
    Node* parent = node->parent;

    if (sibling == NULL) {
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
                if (sibling->left != NULL && sibling->left->color == true) {
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

bool search(int value) {
    Node* node = root;

    while (node != NULL) {
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

//Credit: Jayden Huang
void printFormat(Node* head, int space) {
    if (head == nullptr) 
        return;
    

    space += 10;

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
    
    // cout << head->data << (head->color ? "R" : "B") << endl;
}


void deleteTree(Node* node) {
    if (node) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}


