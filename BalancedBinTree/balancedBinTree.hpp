#pragma once

#include <iostream>
#include <iomanip>
#include <vector>


template<typename T>
struct Node {
    T data;
    Node* left;
    Node* right;
    int height;

    Node(const T& data) : data(data), left(nullptr), right(nullptr), height(1) {}

    Node(const T& data, const int height) : data(data), left(nullptr), right(nullptr), height(height) {}

    ~Node() {}
};


template<typename T>
class BalancedBinaryTree {
    Node<T>* root;

    public:
        BalancedBinaryTree() : root(nullptr) {}
        
        BalancedBinaryTree(BalancedBinaryTree<T>& other) : root(nullptr) {
            if (other.getRoot()) {
                root = new Node<T>(other.getRoot()->data, other.getRoot()->height);
                copyTree(root, other.getRoot());
            }
        }

        BalancedBinaryTree<T>& operator=(BalancedBinaryTree<T>& other) {
            if (this != &other) {
                deleteTree(root);
                if (other.getRoot()) {
                    root = new Node<T>(other.getRoot()->data, other.getRoot()->height);
                    copyTree(root, other.getRoot());
                } else {
                    root = nullptr;
                }
            }

            return *this;
        }

        ~BalancedBinaryTree() {
            deleteTree(root);
        }

        Node<T>* getRoot() {
            return root;
        }
        
        void add(const T& data) {
            root = addNode(root, data);
        }
        
        void addElements(std::vector<T>& elements) {
            for (const T& element : elements) {
                add(element);
            }
        }

        void clean() {
            deleteTree(root);
            root = nullptr;
        }
        
        void rebalance() {
            root = rebalanceTree(root);
        }
        
        void remove(const T& data) {
            root = removeNode(root, data);
        }

        bool getMin(T& value) {
            if (root) {
                value = findMin(root)->data;
                return true;
            }
            return false;
        }

        bool getMax(T& value) {
            if (root) {
                value = findMax(root)->data;
                return true;
            }
            return false;
        }

        void print() {
            printNode(root);
            std::cout << std::endl;
        }

        void show() {
            showNode(root, 0, 0);
        }

        void joinAnd(BalancedBinaryTree<T>& other) {
            BalancedBinaryTree<T> result;
            if (this != &other) {
                joinTreeAnd(root, other.getRoot(), result);
                *this = result;
            }
        }

        void joinOr(BalancedBinaryTree<T>& other) {
            BalancedBinaryTree<T> result;
            if (this != &other && other.getRoot()) {
                joinTreeOr(root, other.getRoot(), result);
                *this = result;
            }
        }

    private:
        int getHeight(Node<T>* node) {
            return node ? node->height : 0;
        }

        void updateHeight(Node<T>* node) {
            if (node) {
                node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
            }
        }
        
        Node<T>* addNode(Node<T>* node, const T& data) {
            if (!node) {
                return new Node<T>(data);
            }
            if (data < node->data) {
                node->left = addNode(node->left, data);
            } else if (data > node->data) {
                node->right = addNode(node->right, data);
            }
            updateHeight(node);
            return balance(node);
        }

        int getBalanceFactor(Node<T>* node) {
            return getHeight(node->right) - getHeight(node->left);
        }
        
        
        Node<T>* balanceRight(Node<T>* node) {
            Node<T>* left = node->left;
            Node<T>* leftRight = left->right;
            left->right = node;
            node->left = leftRight;
            updateHeight(node);
            updateHeight(left);
            return left;
        }
        
        Node<T>* balanceLeft(Node<T>* node) {
            Node<T>* right = node->right;
            Node<T>* rightLeft = right->left;
            right->left = node;
            node->right = rightLeft;
            updateHeight(node);
            updateHeight(right);
            return right;
        }

        Node<T>* balance(Node<T>* node) {
            int balanceFactor = getBalanceFactor(node);
            if (balanceFactor > 1) {
                if (getBalanceFactor(node->right) < 0) {
                    node->right = balanceRight(node->right);
                }
                return balanceLeft(node);
            }
            if (balanceFactor < -1) {
                if (getBalanceFactor(node->left) > 0) {
                    node->left = balanceLeft(node->left);
                }
                return balanceRight(node);
            }
            return node;
        }

        void copyTree(Node<T>* thisNode, const Node<T>* otherNode) {
            if (otherNode->left) {
                thisNode->left = new Node<T>(otherNode->left->data, otherNode->left->height);
                copyTree(thisNode->left, otherNode->left);
            }
            if (otherNode->right) {
                thisNode->right = new Node<T>(otherNode->right->data, otherNode->right->height);
                copyTree(thisNode->right, otherNode->right);
            }
        }
        
        void deleteTree(Node<T>* node) {
            if (node) {
                deleteTree(node->left);
                deleteTree(node->right);
                delete node;
            }
        }
        
        Node<T>* findMin(Node<T>* node) {
            while (node->left) {
                node = node->left;
            }
            return node;
        }
        
        Node<T>* findMax(Node<T>* node) {
            while (node->right) {
                node = node->right;
            }
            return node;
        }

        Node<T>* moveMin(Node<T>* node) {
            if (!node->left) {
                return node->right;
            }
            node->left = moveMin(node->left);
            return balance(node);
        }

        Node<T>* removeNode(Node<T>* node, const T& data) {
            if (!node) {
                return nullptr;
            }
            if (data < node->data) {
                node->left = removeNode(node->left, data);
            } else if (data > node->data) {
                node->right = removeNode(node->right, data);
            } else {
                Node<T>* left = node->left;
                Node<T>* right = node->right;
                delete node;
                if (!right) {
                    return left;
                }
                Node<T>* min = findMin(right);
                min->right = moveMin(right);
                min->left = left;
                return balance(min);
            }
            return balance(node);
        }

        Node<T>* rebalanceTree(Node<T>* node) {
            if (!node) {
                return nullptr;
            }
            node->right = rebalanceTree(node->right);
            node = balance(node);
            node->left = rebalanceTree(node->left);
            return node;
        }

        void printNode(Node<T>* node) {
            if (node) {
                printNode(node->left);
                std::cout << node->data << " ";
                printNode(node->right);
            }
        }

        void showNode(Node<T>* node, int level, int offset, bool missing = false) {
            if (node) {
                showNode(node->right, level + 1, offset + 8, (node->right || node->left));
                std::cout << std::setw(offset) << "";
                std::cout << node->data << "[" << level << "]" << std::endl;
                showNode(node->left, level + 1, offset + 8, (node->right || node->left));
            } else if (missing) {
                std::cout << std::setw(offset) << "";
                std::cout << std::endl;
            }
        }

        void joinTreeAnd(Node<T>* node1, Node<T>* node2, BalancedBinaryTree<T>& result) {
            if (node1 && node2) {
                if (node1->data < node2->data) {
                    joinTreeAnd(node1->right, node2, result);
                    joinTreeAnd(node1, node2->left, result);
                } else if (node1->data > node2->data) {
                    joinTreeAnd(node1, node2->right, result);
                    joinTreeAnd(node1->left, node2, result);
                } else {
                    result.add(node1->data);
                    joinTreeAnd(node1->left, node2->left, result);
                    joinTreeAnd(node1->right, node2->right, result);
                }
            }
        }

        void joinTreeOr(Node<T>* node1, Node<T>* node2, BalancedBinaryTree<T>& result) {
            if (node1) {
                joinTreeOr(node1->left, node2, result);
                result.add(node1->data);
                joinTreeOr(node1->right, node2, result);
            }
            if (node2) {
                joinTreeOr(node1, node2->left, result);
                result.add(node2->data);
                joinTreeOr(node1, node2->right, result);
            }
        }
};
