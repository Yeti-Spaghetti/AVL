#include <bits/stdc++.h>
using namespace std;

class Node{
private:
    int data;
    Node *parent, *left, *right;
public:
    Node(){
        parent = NULL;
        left = NULL;
        right = NULL;
    }
    void set_data(int n){
        data = n;
    }
    void set_parent(Node *parentNode){
        parent = parentNode;
    }
    void set_left(Node *leftNode){
        left = leftNode;
    }
    void set_right(Node *rightNode){
        right = rightNode;
    }
    int get_data(){
        return data;
    }
    Node *get_parent(){
        return parent;
    }
    Node *get_left(){
        return left;
    }
    Node *get_right(){
        return right;
    }
};

class BinaryTree{
private:
    Node *header;
    int height, tempHeight;
public:
    BinaryTree(){
        header = NULL;
    }

    Node *get_header(){
        return header;
    }

    int check_balance(Node *parent){
        if (parent == NULL){
            return 0;
        }
        return get_height(parent->get_left()) - get_height(parent->get_right());
    }

    int get_height(Node *parent){
        if (parent == NULL){
            return -1;
        }
        return 1 + max(get_height(parent->get_left()), get_height(parent->get_right()));
    }

    void check_root_parent(Node *root, Node *newRoot){
        if (root->get_parent() == NULL){
            header = newRoot;
        } else {
            Node *rootParent = root->get_parent();
            if (root == rootParent->get_left()){
                rootParent->set_left(newRoot);
            } else {
                rootParent->set_right(newRoot);
            }
        }
    }

    void balance_l(Node *root){
        Node *newRoot = root->get_right();
        root->set_right(newRoot->get_left());
        if (newRoot->get_left() != NULL){
            newRoot->get_left()->set_parent(root);
        }
        newRoot->set_left(root);
        check_root_parent(root, newRoot);
        newRoot->set_parent(root->get_parent());
        root->set_parent(newRoot);
    }

    void balance_r(Node *root){
        Node *newRoot = root->get_left();
        root->set_left(newRoot->get_right());
        if (newRoot->get_right() != NULL){
            newRoot->get_right()->set_parent(root);
        }
        newRoot->set_right(root);
        check_root_parent(root, newRoot);
        newRoot->set_parent(root->get_parent());
        root->set_parent(newRoot);
    }

    void balance_rl(Node *root){
        Node *rootRight = root->get_right();
        balance_r(rootRight);
        balance_l(root);
    }

    void balance_lr(Node *root){
        Node *rootLeft = root->get_left();
        balance_l(rootLeft);
        balance_r(root);
    }

    void balance(Node *temp){
        int count = 0;
        while (true){
            if (temp == NULL){
                break;
            } else {
                
                int balance = check_balance(temp);
                // if right sub-tree is longer
                if (balance <= -2){
                    Node *tempChild = temp->get_right();
                    Node *tempChildLeft = tempChild->get_left();
                    Node *tempChildRight = tempChild->get_right();
                    // left rotation
                    if (get_height(tempChildLeft) <= get_height(tempChildRight)){
                        balance_l(temp);
                    // right left rotation
                    } else if (get_height(tempChildLeft) > get_height(tempChildRight)){
                        balance_rl(temp);
                    }
                }
                
                // if left sub-tree is longer
                if (balance >= 2){
                    Node *tempChild = temp->get_left();
                    Node *tempChildLeft = tempChild->get_left();
                    Node *tempChildRight = tempChild->get_right();
                    // right rotation
                    if (get_height(tempChildLeft) >= get_height(tempChildRight)){
                        balance_r(temp);
                    // left right rotation
                    } else if (get_height(tempChildLeft) < get_height(tempChildRight)){
                        balance_lr(temp);
                    }
                }
                
                temp = temp->get_parent();
            }
        }
    }

    void insert(int n){
        bool alreadyExists = false;
        Node *newNode = new Node;
        newNode->set_data(n);
        // if list is empty, set header to new Node
        if (header == NULL){
            header = newNode;

        // list not empty
        } else {
            Node *temp = header;
            tempHeight = 0;
            
            while (true){
                if (temp->get_data() == n){
                    alreadyExists = true;
                    break;

                // if new node value smaller
                } else if (n < temp->get_data()){
                    if (temp->get_left() == NULL){
                        temp->set_left(newNode);
                        newNode->set_parent(temp);
                        break;
                    // move down left
                    } else {
                        temp = temp->get_left();
                        tempHeight++;
                    }

                //f if new node value greater
                } else if (n > temp->get_data()){
                    if (temp->get_right() == NULL){
                        temp->set_right(newNode);
                        newNode->set_parent(temp);
                        break;
                    } else {
                        temp = temp->get_right();
                        tempHeight++;
                    }
                }
            }
        }

        // move to parent of newNode because leaf-node does not need balancing
        if (!alreadyExists){
            
            Node *temp = newNode->get_parent();
            balance(temp);
        }
    }

    void delete_node(int n){
        Node *deleteNode = header;
        // search for node
        while (deleteNode != NULL){
            if (deleteNode->get_data() == n){
                break;
            } else if (n < deleteNode->get_data()){
                deleteNode = deleteNode->get_left();
            } else {
                deleteNode = deleteNode->get_right();
            }
        }

        if (deleteNode == NULL || deleteNode->get_data() != n){
            // do nothing
        } else {
            Node *deleteNodeParent = deleteNode->get_parent();

            // deleteNode is header and no children
            if (deleteNode->get_parent() == NULL){
                if (deleteNode->get_left() == NULL && deleteNode->get_right() == NULL){
                    header = NULL;
                }

                // deleteNode is header and has only left child
                else if (deleteNode->get_left() != NULL && deleteNode->get_right() == NULL){
                    header = deleteNode->get_left();
                    header->set_parent(NULL);
                }

                // deleteNode is header and has only right child
                else if (deleteNode->get_left() == NULL && deleteNode->get_right() != NULL){
                    header = deleteNode->get_right();
                    header->set_parent(NULL);
                }

                // deleteNode is header and has two children
                else if (deleteNode->get_left() != NULL && deleteNode->get_right() != NULL){
                    header = deleteNode->get_left();

                    int count = 0;
                    while (header->get_right() != NULL){
                        header = header->get_right();
                        count++;
                    }
                    
                    header->set_right(deleteNode->get_right());
                    deleteNode->get_right()->set_parent(header);

                    if (count == 0){
                        header->set_parent(NULL);
                        balance(header);
                    } else {
                        Node *headerParent = header->get_parent();
                        Node *headerLeft = header->get_left();
                        if (header->get_left() != NULL){
                            headerParent->set_right(headerLeft);
                            headerLeft->set_parent(headerParent);
                            header->set_left(deleteNode->get_left());
                            deleteNode->get_left()->set_parent(header);
                            header->set_parent(NULL);
                        } else {
                            header->set_left(deleteNode->get_left());
                            deleteNode->get_left()->set_parent(header);
                            headerParent->set_right(NULL);
                            header->set_parent(NULL);
                        }
                        balance(headerParent);
                    }
                }
                delete deleteNode;
                
            }

            else if (deleteNode->get_parent() != NULL && (deleteNode->get_left() != NULL || deleteNode->get_right() != NULL)){
                Node *deleteNodeParent = deleteNode->get_parent();

                Node *replacementNode;
                // if deleteNode left node isn't NULL
                if (deleteNode->get_left() != NULL){
                    replacementNode = deleteNode->get_left();

                    int count = 0;
                    while (replacementNode->get_right() != NULL){
                        replacementNode = replacementNode->get_right();
                        count++;
                    }

                    Node *replacementNodeParent = replacementNode->get_parent();
                    if (count == 0){
                        replacementNode->set_parent(deleteNodeParent);
                        replacementNode->set_right(deleteNode->get_right());
                        if (deleteNode->get_right() != NULL){
                            deleteNode->get_right()->set_parent(replacementNode);
                        }

                        if (deleteNodeParent->get_left() == deleteNode){
                            deleteNodeParent->set_left(replacementNode);
                        } else {
                            deleteNodeParent->set_right(replacementNode);
                        }
                        balance(replacementNode);
                        
                    } else {
                        // if replacementNode has left child
                        if (replacementNode->get_left() != NULL){
                            replacementNodeParent->set_right(replacementNode->get_left());
                            replacementNode->get_left()->set_parent(replacementNodeParent);
                            replacementNode->set_right(deleteNode->get_right());
                            replacementNode->set_left(deleteNode->get_left());
                            deleteNode->get_right()->set_parent(replacementNode);
                            deleteNode->get_left()->set_parent(replacementNode);
                            replacementNode->set_parent(deleteNode->get_parent());
                        } else {
                            replacementNode->set_right(deleteNode->get_right());
                            replacementNode->set_left(deleteNode->get_left());
                            deleteNode->get_right()->set_parent(replacementNode);
                            deleteNode->get_left()->set_parent(replacementNode);
                            replacementNode->set_parent(deleteNode->get_parent());
                            replacementNodeParent->set_right(NULL);
                        }
                        if (deleteNodeParent->get_left() == deleteNode){
                        deleteNodeParent->set_left(replacementNode);
                        } else {
                            deleteNodeParent->set_right(replacementNode);
                        }
                        balance(replacementNodeParent);
                    }

                    
                    
                // if deleteNode left is NULL then replace with right node
                } else if (deleteNode->get_left() == NULL){
                    replacementNode = deleteNode->get_right();
                    replacementNode->set_parent(deleteNodeParent);
                    if (deleteNodeParent->get_left() == deleteNode){
                        deleteNodeParent->set_left(replacementNode);
                    } else {
                        deleteNodeParent->set_right(replacementNode);
                    }
                balance(replacementNode);
                }
                delete deleteNode;
                
            }


            // deleteNode is a leaf node
            else if (deleteNode->get_parent() != NULL && deleteNode->get_left() == NULL && deleteNode->get_right() == NULL){
                Node *leafParent = deleteNode->get_parent();
                if (leafParent->get_left() == NULL || leafParent->get_left()->get_data() != n){
                    leafParent->set_right(NULL);
                } else {
                    leafParent->set_left(NULL);
                }
                delete deleteNode;
                balance(leafParent);
            }
        }
    }
    
    void print_pre(Node *temp){
        if (temp != NULL){
            cout << temp->get_data() << " ";
            if (temp->get_left() != NULL){
                print_pre(temp->get_left());
            }
            if (temp->get_right() != NULL){
                print_pre(temp->get_right());
            }
        } else {
            cout << "EMPTY";
        }
    }
    void print_post(Node *temp){
        if (temp != NULL){
            if (temp->get_left() != NULL){
                print_post(temp->get_left());
            }
            if (temp->get_right() != NULL){
                print_post(temp->get_right());
            }
            cout << temp->get_data() << " ";
        } else {
            cout << "EMPTY";
        }
    }
    void print_in(Node *temp){
        if (temp != NULL){
            if (temp->get_left() != NULL){
                print_in(temp->get_left());
            }
            cout << temp->get_data() << " ";
            if (temp->get_right() != NULL){
                print_in(temp->get_right());
            }
        } else {
            cout << "EMPTY";
        }
    }
    void print_right(Node *temp){
        if (temp != NULL){
            cout << temp->get_data() << " ";
            temp = temp->get_right();
            print_right(temp);
        }
    }
    void print_left(Node *temp){
        if (temp != NULL){
            cout << temp->get_data() << " ";
            temp = temp->get_left();
            print_left(temp);
        }
    }
    void print_test1(Node *temp){
        cout << temp->get_data() << " ";

        temp = temp->get_left();
        cout << temp->get_data() << " ";
        temp = temp->get_left();
        cout << temp->get_data() << " ";
        temp = temp->get_right();
        cout << temp->get_data() << " ";
        temp = temp->get_right();
        cout << temp->get_data() << " ";
        temp = temp->get_right();
        cout << temp->get_data() << " ";
        temp = temp->get_parent();
        cout << temp->get_data() << " ";
    }
    void print_test2(Node *temp){
        cout << temp->get_data() << " ";

        temp = temp->get_left();
        cout << temp->get_data() << " ";
        temp = temp->get_left();
        cout << temp->get_data() << " ";
        temp = temp->get_right();
        cout << temp->get_data() << " ";
        temp = temp->get_left();
        cout << temp->get_data() << " ";
        temp = temp->get_parent();
        cout << temp->get_data() << " ";
    }
};

int main(){
    string input;
    getline(cin, input);

    vector<string> commands;
    int substringBegin = 0, substringEnd;

    while (true){
        substringEnd = input.find(' ', substringBegin);
        if (substringEnd >= input.length()){
            string temp = input.substr(substringBegin, input.length() - substringBegin);
            commands.push_back(temp);
            break;
        } else {
            string temp = input.substr(substringBegin, substringEnd - substringBegin);
            commands.push_back(temp);
            substringBegin = substringEnd+1;
        }
    }

    BinaryTree tree;
    for (int i = 0; i < commands.size()-1; i++){
        if (commands[i][0] == 'A'){
            int num = stoi(commands[i].substr(1, commands[i].length()));
            tree.insert(num);
        } else if (commands[i][0] == 'D'){
            int num = stoi(commands[i].substr(1, commands[i].length()));
            tree.delete_node(num);
        }
    }

    string printCommand = commands[commands.size()-1];
    Node *head = tree.get_header();
    if (printCommand == "PRE"){
        tree.print_pre(head);
    }
    if (printCommand == "POST"){
        tree.print_post(head);
    }
    if (printCommand == "IN"){
        tree.print_in(head);
    }
    
    return 0;
}