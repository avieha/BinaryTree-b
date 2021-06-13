#pragma once
#include <iostream>
#include <vector>
#include <string.h>

namespace ariel
{
    template <typename T>
    class BinaryTree
    {

    private:
        // inner class
        class Node
        {
        public:
            T val;
            Node *left_son;
            Node *right_son;
            Node(const T &v, Node *l = nullptr, Node *r = nullptr) : val(v), left_son(l), right_son(r){};
            Node(const Node *other) : val(other->val)
            {
                if (other->right_son != nullptr)
                {
                    right_son = new Node(other->right_son);
                }
                if (other->left_son != nullptr)
                {
                    left_son = new Node(other->left_son);
                }
            };
            bool operator==(const Node &other)
            {
                return val == other.val && left_son == other.left_son && right_son == other.right_son;
            };
            bool operator!=(const Node &other)
            {
                return val != other.val || left_son != other.left_son || right_son != other.right_son;
            };
        }; // END OF CLASS NODE

        // Class parameter
        Node *root;

    public:
        // inner class - iterator
        class iterator
        {
        private:
            unsigned long current_node;
            std::vector<Node *> nodes;

        public:
            iterator(Node *ptr, const std::string &type)
            {
                current_node = 0;
                fill_vector(ptr, type);
                nodes.push_back(nullptr);
            }

            T &operator*()
            {
                return nodes[current_node]->val;
            }

            T *operator->()
            {
                return &(nodes[current_node]->val);
            }

            // ++i;
            iterator &operator++()
            {
                ++current_node;
                return *this;
            }

            // i++;
            // Usually iterators are passed by value and not by const& as they are small.
            iterator operator++(int)
            {
                iterator tmp = *this;
                ++current_node;
                return tmp;
            }

            void fill_vector(Node *n, const std::string &type)
            {
                if (n == nullptr)
                {
                    return;
                }
                if (type == "preorder")
                {
                    nodes.push_back(n);
                }
                fill_vector(n->left_son, type);
                if (type == "inorder")
                {
                    nodes.push_back(n);
                }
                fill_vector(n->right_son, type);
                if (type == "postorder")
                {
                    nodes.push_back(n);
                }
            }

            bool operator==(const iterator &rhs) const
            {
                return nodes[current_node] == rhs.nodes[rhs.current_node];
            }

            bool operator!=(const iterator &rhs) const
            {
                return nodes[current_node] != rhs.nodes[rhs.current_node];
            }

            friend std::ostream &operator<<(std::ostream &os, const iterator &it)
            {
                std::cout << "[";
                for (auto n : it.nodes)
                {
                    std::cout << n->val << ",";
                }
                std::cout << "]";
                std::cout << std::endl;
                return os;
            }
        }; // END OF CLASS ITERATOR

        BinaryTree<T>() : root(nullptr){};

        // move constructor
        BinaryTree(BinaryTree &&other) noexcept
        {
            this->root = other.root;
            other.root = nullptr;
        }

        // move assignment
        BinaryTree<T> &operator=(BinaryTree<T> &&other) noexcept
        {
            root = other.root;
            other.root = nullptr;
            return *this;
        }

        // copy constructor
        BinaryTree(const BinaryTree &other)
        {
            if (other.root)
            {
                root = new Node(other.root);
            }
        }

        // copy assignment
        BinaryTree<T> &operator=(const BinaryTree<T> &other)
        {
            if (this == &other)
            {
                return *this;
            }
            delete (root);
            if (other.root != nullptr)
            {
                root = new Node{other.root};
            }
            return *this;
        }

        BinaryTree<T> &add_root(const T &r)
        {
            if (root == nullptr)
            {
                root = new Node(r);
            }
            else
            {
                root->val = r;
            }
            return *this;
        };

        BinaryTree<T> &add_left(const T &father, const T &child)
        {
            Node *temp = find_exist(root, father);
            if (temp == nullptr)
            {
                throw std::invalid_argument("father doesn't exists");
            }
            if (temp->left_son == nullptr)
            {
                temp->left_son = new Node(child);
            }
            else
            {
                temp->left_son->val = child;
            }
            return *this;
        };

        BinaryTree<T> &add_right(const T &father, const T &child)
        {
            Node *temp = find_exist(root, father);
            if (temp == nullptr)
            {
                throw std::invalid_argument("father doesn't exists");
            }
            if (temp->right_son == nullptr)
            {
                temp->right_son = new Node(child);
            }
            else
            {
                temp->right_son->val = child;
            }
            return *this;
        };

        Node *find_exist(Node *temp_root, T val)
        {
            if (!temp_root)
            {
                return temp_root;
            }
            if (temp_root->val == val)
            {
                return temp_root;
            }
            Node *b = nullptr;
            Node *a = find_exist(temp_root->left_son, val);
            if (a != nullptr)
            {
                return a;
            }
            b = find_exist(temp_root->right_son, val);
            return b;
        }

        iterator begin() { return begin_inorder(); };
        iterator end() { return end_inorder(); };
        iterator begin_preorder() { return iterator(root, "preorder"); };
        iterator end_preorder() { return iterator(nullptr, "preorder"); };
        iterator begin_inorder() { return iterator(root, "inorder"); };
        iterator end_inorder() { return iterator(nullptr, "inorder"); };
        iterator begin_postorder() { return iterator(root, "postorder"); };
        iterator end_postorder() { return iterator(nullptr, "postorder"); };

        friend std::ostream &operator<<(std::ostream &os, BinaryTree<T> &tree)
        {
            os << "check";
            return os;
        }

        ~BinaryTree()
        {
            if (root)
            {
                delete (root);
            }
        };
    };
};