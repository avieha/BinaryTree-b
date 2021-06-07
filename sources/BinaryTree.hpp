#include <iostream>
#include <stack>
using namespace std;

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
            Node *father;
            Node(const T &v, Node *l = nullptr, Node *r = nullptr, Node *f = nullptr) : val(v), left_son(l), right_son(r), father(f){};
            bool operator==(const Node &other)
            {
                return this->father == other.father && this->left_son == other.left_son && this->right_son == other.right_son;
            };
            bool operator!=(const Node &other)
            {
                return this->father == other.father || this->left_son == other.left_son || this->right_son == other.right_son;
            };
            ~Node(){};
        };

        // Class parameter
        Node *root;

    public:
        BinaryTree() : root(nullptr){};

        BinaryTree<T> &add_root(T r)
        {
            if (this->root == nullptr)
            {
                this->root = new Node(r);
            }
            else
            {
                root->val = r;
            }
            return *this;
        };

        BinaryTree<T> &add_left(T father, T child)
        {
            Node *temp = find_exist(this->root, father);
            if (temp == nullptr)
            {
                throw invalid_argument("father doesn't exists");
            }
            if (temp->left_son == nullptr)
            {
                temp->left_son = new Node(child);
                temp->left_son->father = temp;
            }
            else
            {
                temp->left_son->val = child;
            }
            return *this;
        };

        BinaryTree<T> &add_right(T father, T child)
        {
            Node *temp = find_exist(this->root, father);
            if (temp == nullptr)
            {
                throw invalid_argument("father doesn't exists");
            }
            if (temp->right_son == nullptr)
            {
                temp->right_son = new Node(child);
                temp->right_son->father = temp;
            }
            else
            {
                temp->right_son->val = child;
            }
            return *this;
        };

        // move constructor - shallow copy
        BinaryTree(BinaryTree &&other) noexcept
        {
            this->root = other.root;
            other.root = nullptr;
        }

        // move assignment - shallow copy
        BinaryTree &operator=(BinaryTree<T> &&other) noexcept
        {
            if (this != &other)
            {
                delete (root);
                this->root = other.root;
                other.root = nullptr;
            }
            return *this;
        }

        // copy constructor - deep copy
        BinaryTree(const BinaryTree &other)
        {
            copy_node(root, other.root);
        }

        // copy assignment - deep copy
        BinaryTree &operator=(const BinaryTree<T> &other) noexcept
        {
            if (this != &other)
            {
                delete (root);
                if (other.root != nullptr)
                {
                    root = new Node(other.root->val);
                    copy_node(root, other.root);
                }
            }
            return *this;
        }

        void copy_node(Node *curr, const Node *other)
        {
            if (other->right_son != nullptr)
            {
                curr->right_son = new Node(other->right_son->val);
                copy_node(curr->right_son, other->right_son);
            }
            if (other->left_son != nullptr)
            {
                curr->left_son = new Node(other->left_son->val);
                copy_node(curr->left_son, other->left_son);
            }
        }

        Node *find_exist(Node *temp_root, T val)
        {
            if (temp_root == nullptr)
            {
                return nullptr;
            }
            if (temp_root->val == val)
            {
                return temp_root;
            }
            Node *a = nullptr;
            Node *b = nullptr;
            if (temp_root->left_son != nullptr)
            {
                a = find_exist(temp_root->left_son, val);
            }
            if (temp_root->right_son != nullptr)
            {
                b = find_exist(temp_root->right_son, val);
            }
            if (a != nullptr)
            {
                return a;
            }
            else
            {
                return b;
            }
        }

        ~BinaryTree(){};

        // inner class - pre_order_iterator
        class pre_order_iterator
        {
        private:
            Node *pointer_to_current_node;
            std::stack<Node *> stack;

        public:
            pre_order_iterator(Node *ptr = nullptr)
                : pointer_to_current_node(ptr) {}

            // Note that the method is const as this operator does not
            // allow changing of the iterator.
            // Note that it returns T& as it allows to change what it points to.
            // A const_iterator class will return const T&
            // and the method will still be const
            T &operator*() const
            {
                //return *pointer_to_current_node;
                return pointer_to_current_node->val;
            }

            T *operator->() const
            {
                return &(pointer_to_current_node->val);
            }

            // ++i;
            pre_order_iterator &operator++()
            {
                //++pointer_to_current_node;
                if (!stack.empty())
                {
                    stack.pop();
                }
                if (!stack.empty())
                {
                    pointer_to_current_node = stack.top();
                }
                else
                {
                    pointer_to_current_node = nullptr;
                }
                return *this;
            }

            // i++;
            // Usually iterators are passed by value and not by const& as they are small.
            const pre_order_iterator operator++(int)
            {
                if (*this==nullptr)
                {
                    return nullptr;
                }
                pre_order_iterator tmp = *this;
                ++*this;
                return tmp;
            }

            // Iterative function to perform preorder traversal on the tree
            void preorderIterative()
            {
                // return if the tree is empty
                if (pointer_to_current_node == nullptr)
                    return;

                stack.push(pointer_to_current_node);

                // loop till stack is empty
                while (!stack.empty())
                {
                    // pop a node from the stack and print it
                    Node *curr = stack.top();
                    stack.pop();

                    cout << curr->val << " ";

                    // push the right child of the popped node into the stack
                    if (curr->right_son)
                    {
                        stack.push(curr->right_son);
                    }

                    // push the left child of the popped node into the stack
                    if (curr->left_son)
                    {
                        stack.push(curr->left_son);
                    }

                    // the right child must be pushed first so that the left child
                    // is processed first (LIFO order)
                }
            }

            bool operator==(const pre_order_iterator &rhs) const
            {
                return pointer_to_current_node == rhs.pointer_to_current_node;
            }

            bool operator!=(const pre_order_iterator &rhs) const
            {
                return *pointer_to_current_node != *rhs.pointer_to_current_node;
            }
        }; // END OF CLASS ITERATOR

        // inner class - in_order_iterator
        class in_order_iterator
        {
        private:
            Node *pointer_to_current_node;
            std::stack<Node *> stack;

        public:
            in_order_iterator(Node *ptr = nullptr) : pointer_to_current_node(ptr){};

            // Note that the method is const as this operator does not
            // allow changing of the iterator.
            // Note that it returns T& as it allows to change what it points to.
            // A const_iterator class will return const T&
            // and the method will still be const
            T &operator*() const
            {
                //return *pointer_to_current_node;
                return pointer_to_current_node->val;
            }

            T *operator->() const
            {
                return &(pointer_to_current_node->val);
            }

            // ++i;
            in_order_iterator &operator++()
            {
                //++pointer_to_current_node;
                if (!stack.empty())
                {
                    stack.pop();
                }
                if (!stack.empty())
                {
                    pointer_to_current_node = stack.top();
                }
                else
                {
                    pointer_to_current_node = nullptr;
                }
                return *this;
            }

            // i++;
            // Usually iterators are passed by value and not by const& as they are small.
            const in_order_iterator operator++(int)
            {
                in_order_iterator tmp = *this;
                pointer_to_current_node = pointer_to_current_node->left_son;
                return tmp;
            }

            // /* Function to traverse the binary tree without recursion
            // and without stack */
            // void MorrisTraversal(struct tNode *root)
            // {
            //     struct tNode *current, *pre;

            //     if (root == NULL)
            //         return;

            //     current = root;
            //     while (current != NULL)
            //     {

            //         if (current->left == NULL)
            //         {
            //             printf("%d ", current->data);
            //             current = current->right;
            //         }
            //         else
            //         {

            //             /* Find the inorder predecessor of current */
            //             pre = current->left;
            //             while (pre->right != NULL && pre->right != current)
            //                 pre = pre->right;

            //             /* Make current as the right child of its
            //             inorder predecessor */
            //             if (pre->right == NULL)
            //             {
            //                 pre->right = current;
            //                 current = current->left;
            //             }

            //             /* Revert the changes made in the 'if' part to
            //             restore the original tree i.e., fix the right
            //             child of predecessor */
            //             else
            //             {
            //                 pre->right = NULL;
            //                 printf("%d ", current->data);
            //                 current = current->right;
            //             } /* End of if condition pre->right == NULL */
            //         }     /* End of if condition current->left == NULL*/
            //     }         /* End of while */
            // }

            // Iterative function to perform inorder traversal on the tree
            void inorderIterative()
            {
                // start from the root node (set current node to the root node)
                Node *curr = pointer_to_current_node;

                // if the current node is null and the stack is also empty, we are done
                while (!stack.empty() || curr != nullptr)
                {
                    // if the current node exists, push it into the stack (defer it)
                    // and move to its left child
                    if (curr != nullptr)
                    {
                        stack.push(curr);
                        curr = curr->left_son;
                    }
                    else
                    {
                        // otherwise, if the current node is null, pop an element from the stack,
                        // print it, and finally set the current node to its right child
                        curr = stack.top();
                        stack.pop();
                        cout << curr->val << " ";

                        curr = curr->right_son;
                    }
                }
            }

            bool operator==(const in_order_iterator &rhs) const
            {
                return pointer_to_current_node == rhs.pointer_to_current_node;
            }

            bool operator!=(const in_order_iterator &rhs) const
            {
                return pointer_to_current_node != rhs.pointer_to_current_node;
            }
        }; // END OF CLASS ITERATOR

        // inner class - post_order_iterator
        class post_order_iterator
        {
        private:
            Node *pointer_to_current_node;
            std::stack<Node *> stack;

        public:
            post_order_iterator(Node *ptr = nullptr) : pointer_to_current_node(ptr) {}

            // Note that the method is const as this operator does not
            // allow changing of the iterator.
            // Note that it returns T& as it allows to change what it points to.
            // A const_iterator class will return const T&
            // and the method will still be const
            T &operator*() const
            {
                //return *pointer_to_current_node;
                return pointer_to_current_node->val;
            }

            T *operator->() const
            {
                return &(pointer_to_current_node->val);
            }

            // ++i;
            post_order_iterator &operator++()
            {
                //++pointer_to_current_node;
                if (!stack.empty())
                {
                    stack.pop();
                }
                if (!stack.empty())
                {
                    pointer_to_current_node = stack.top();
                }
                else
                {
                    pointer_to_current_node = nullptr;
                }
                return *this;
            }

            // i++;
            // Usually iterators are passed by value and not by const& as they are small.
            const post_order_iterator operator++(int)
            {
                post_order_iterator tmp = *this;
                pointer_to_current_node = pointer_to_current_node->left_son;
                return tmp;
            }

            // Iterative function to perform postorder traversal on the tree
            void postorderIterative()
            {
                stack.push(pointer_to_current_node);

                // create another stack to store postorder traversal
                std::stack<int> out;

                // loop till stack is empty
                while (!stack.empty())
                {
                    // pop a node from the stack and push the data into the output stack
                    Node *curr = stack.top();
                    stack.pop();

                    out.push(curr->val);

                    // push the left and right child of the popped node into the stack
                    if (curr->left_son)
                    {
                        stack.push(curr->left_son);
                    }

                    if (curr->right_son)
                    {
                        stack.push(curr->right_son);
                    }
                }

                // print postorder traversal
                while (!out.empty())
                {
                    cout << out.top() << " ";
                    out.pop();
                }
            }

            bool operator==(const post_order_iterator &rhs) const
            {
                return pointer_to_current_node == rhs.pointer_to_current_node;
            }

            bool operator!=(const post_order_iterator &rhs) const
            {
                return pointer_to_current_node != rhs.pointer_to_current_node;
            }
        }; // END OF CLASS ITERATOR

        pre_order_iterator begin() { return pre_order_iterator(root); };
        pre_order_iterator end() { return nullptr; };
        pre_order_iterator begin_preorder() { return pre_order_iterator(root); };
        pre_order_iterator end_preorder() { return nullptr; };
        in_order_iterator begin_inorder() { return in_order_iterator(root); };
        in_order_iterator end_inorder() { return nullptr; };
        post_order_iterator begin_postorder() { return post_order_iterator(root); };
        post_order_iterator end_postorder() { return nullptr; };
        friend ostream &operator<<(ostream &os, const BinaryTree<T> &b) { return os; };
    };
}