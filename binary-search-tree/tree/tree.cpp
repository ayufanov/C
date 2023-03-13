#include "tree.hpp"
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

auto tree_find(tree &tree, int val) -> tree_node * {

    if (tree.root == nullptr) {
        return nullptr;
    }

    tree_node *temp = tree.root.get();
    while (temp != nullptr) {

        if (temp->value == val) {
            return temp;
        }
        if (temp->value < val) {
            temp = temp->right.get();
        } else {
            temp = temp->left.get();
        }
    }
    return nullptr;
};

auto tree::insert(int val) -> tree_node * {




    auto node = std::make_unique<tree_node>(val);

    if (root == nullptr) {
        root = std::move(node);
        root->up = nullptr;
        return root.get();
    } else {
        tree_node *temp = root.get();
        tree_node *prev = root.get();
        while (temp != nullptr) {
            prev = temp;
            if (temp->value < val) {
                temp = temp->right.get();
            } else if (temp->value > val){
                temp = temp->left.get();
            }else {
            return temp;
            }
        }
        if (prev->value == val)
            return (prev);
        if (prev->value < val) {
            prev->right = std::move(node);
            prev->right->up = prev;
            return prev->right.get();
        } else {
            prev->left = std::move(node);
            prev->left->up = prev;
            return prev->left.get();
        }
    }

    return nullptr;
}

/*void obhod(tree_node*& temp){
    tree_node *buffer = root.get();

    if (buffer == nullptr) return;
    array1[i] = buffer->value;
    i++;


    obhod(buffer->left);
    obhod(buffer->right);


}
*/



/*auto tree::oobhod(int value) -> tree_node * {

    tree_node *temp = tree_find(*this, value);

    if (temp == nullptr) {
        return nullptr;
    }
    wasd.push_back(temp->value);
    // temp = nullptr;
    if (temp->left == nullptr && temp->right == nullptr) {
        temp = nullptr;
        return nullptr;
    }
    oobhod(temp->left->value);
    oobhod(temp->right->value);
    return nullptr;
}
*/
void qwerty(tree_node *t, std::vector<int> &qw) {
    if (t->left) {
        auto *ty = t->left.get();
        qw.push_back(ty->value);
        qwerty(ty, qw);
    }
    if (t->right) {
        auto *ty = t->right.get();
        qw.push_back(ty->value);
        qwerty(ty, qw);
    }
}

auto tree::remove(int val) -> bool {

    auto *node = tree_find(*this, val);

    if (node == nullptr) {
        return false;
    }

    //  oobhod(node->value);
    // node = nullptr;
    std::vector<int> wasd;
    qwerty(node, wasd);

    if (node->up != nullptr) {
        if (node->up->value > node->value) {
            node->up->left = nullptr;

        } else {
            node->up->right = nullptr;
        }

    } else {
        root = nullptr;
    }

    /* for (int j = 0; j < wasd.size(); j++) {
         t.insert(wasd[j]);
     }
 */
    for (int j = wasd.size() - 1; j >= 0; j--) {
        this->insert(wasd[j]);
    }
    wasd.clear();
    return true;
}
