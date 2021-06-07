#include "Octree.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <string>


void Octree::fromScene(Scene *scene) {
  root_ = new Node;
  root_->depth_ = 0;

  int current_dim = root_dim_;

  std::vector<Node*> node_stack = std::vector<Node*>();
  node_stack.push_back(root_);
  int n_to_pop = 1;

  std::vector<Point3> x0_stack = std::vector<Point3>();
  // root centered in the origin so
  x0_stack.push_back(Point3(-current_dim/2.f)); // lower-left-front corner

  // Get scene's shape(s)
  auto shapes = scene->getShapes();
  if (shapes.size() > 1) {
    std::cout << "ERROR! Scene is not a singol shape" << std::endl;
    exit(1);
  }
  if (shapes.size() < 1) {
    std::cout << "ERROR! Scene is empty" << std::endl;
    exit(1);
  }
  ImplicitShape *shape = shapes.back();

  for (int h=0; h<height_; h++) {
    int future_n_to_pop = 0;
    for (int k=0; k<n_to_pop; k++) {
      if (node_stack.empty() || x0_stack.empty()) {
        break;
      }
      // popping node and corner from the stack head
      Node *node = node_stack[0];
      node_stack.erase(node_stack.begin());
      Point3 x0 = x0_stack[0];
      x0_stack.erase(x0_stack.begin());

      // sampling the 8 children centers
      Point3 offset = Point3(current_dim/4.f); // offset to center inside voxel
      for (unsigned char i=0; i<8; i++) {
        Point3 p(x0 + offset);
        if (i & 1) { // z
          p = p + Point3(0,0,current_dim/2.f);
        }
        if (i & 2) { // y
          p = p + Point3(0,current_dim/2.f,0);
        }
        if (i & 4) { // x
          p = p + Point3(current_dim/2.f,0,0);
        }

        auto d = shape->getDistance(p);
        if (d<=0 || d<=current_dim/4.f)  // internal sphere
          //if (d<=0 || d<current_dim/(2.f*1.414213f)) // external sphere
        {
          // child must contain something
          node->child_mask_ = node->child_mask_ | 1<<(7-i); // bitmask is left-to-right eg. 001 haas only child num 2
          Node *child = new Node;
          if (node->first_child_ == nullptr) {
            node->first_child_ = child;
          } else {
            node_stack.back()->next_sibling_ = child;
          }
          child->depth_ = h+1;
          child->shape_ = shape;
          node_stack.push_back(child);
          x0_stack.push_back((p-offset).as_Point());
          future_n_to_pop++;
        }
      }
    }
    n_to_pop = future_n_to_pop;
    current_dim /= 2;
  }
  DEBUG_print();
}

void Octree::DEBUG_print() {
  std::cout <<
    "\nroot_dim_ = " << root_dim_ <<
    "\nheight_ = "   << height_ <<
    "\n\nroot " <<
    std::endl;
    dfs_print(root_, 0);
}


void Octree::dfs_print(Node *node, int depth) {
  std::string space;
  for (int i=0; i<depth; i++) space.append("  ");

  if (depth == height_-1) {
    std::cout <<
      "leaf" <<
      std::endl;
    return;
  }

  std::cout <<
    std::bitset<8>(node->child_mask_) <<
    std::endl;
  bool is_first_child = true;
  Node *child = nullptr;
  for (unsigned char i=0; i<8; i++) {
    if (node->child_mask_ & 1<<(7-i)) {
      if (is_first_child) {
        child = node->first_child_;
        is_first_child = false;
      } else {
        child = child->next_sibling_;
      }
      std::cout <<
        space << (int)i << " " ;
      dfs_print(child, depth+1);
    }
  }
}
