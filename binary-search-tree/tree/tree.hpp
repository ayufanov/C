#include <memory>

struct tree_node {
  int value;
  tree_node *up;
  std::unique_ptr<tree_node> left, right;
    tree_node(const int &value1)

    :value(value1),left(nullptr), right(nullptr){} ;
  
    
    
};

struct tree {
  std::unique_ptr<tree_node> root;
//auto oobhod(int val) -> tree_node *;
  auto insert(int val) -> tree_node *;
  auto remove(int val) -> bool;
  

};