#include "tree.hpp"

auto tree::iterator::operator++() -> iterator & {
    auto *qwer = p;
    if (qwer->right) {
        qwer = qwer->right.get();
        while (qwer->left)
            qwer = qwer->left.get();

        p = qwer;

    } else {
        if (p->up) {
            qwer = p->up;
            while (qwer) {
                if (p != qwer->left.get()) {
                    p = qwer;
                    qwer = qwer->up;
                } else {
                    break;
                }
            }
        }
        p = qwer;
    }
    return *this;
}

auto tree::iterator::operator--() -> iterator & {
   auto *qwer = p;
    if (qwer){
        if (qwer->left){
            qwer= qwer->left.get();
            while (qwer->right)
                qwer = qwer->right.get();

            p=qwer;    
            

        }else {
            auto *i = p->up;
            while(i){
                if (p != i->right.get())
                {
                    p=i;
                    i=i->up;
                }else{break;}
                
            }
            p=i;

        }

  
    }else{
        p=k;
    }

    return *this;
}

auto tree::begin() const -> iterator {
    auto *qwer = root.get();
    while (qwer->left)
        qwer = qwer->left.get();

    auto *k = root.get();
    while (k->right)
        k = k->right.get();

    return {qwer, k};
}

auto tree::end() const -> iterator {
    auto *k = root.get();
    while (k->right)
        k = k->right.get();

    return {NULL, k};
}

// Можно не менять
auto tree::iterator::operator++(int) -> iterator {
    auto it = *this;
    ++*this;
    return it;
}

auto tree::iterator::operator--(int) -> iterator {
    auto it = *this;
    --*this;
    return it;
}
