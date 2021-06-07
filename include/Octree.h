#ifndef OCTREE_H
#define OCTREE_H

#include "geometry.h"
#include "utilities.h"
#include "Scene.h"

// DEBUG STUFF
#include <bitset>   // to print byte in binary
#include <iostream>
// END // DEBUG STUFF


class Node;

class Octree {
  private:
    int root_dim_ = 1<<3;
    int height_ = 3;
    Node *root_;

  public:
    Octree() = default;
    Octree(Scene *scene) {
      fromScene(scene);
    }

    void fromScene(Scene *scene);
    int getRootDimension() const { return root_dim_; }
    Node* getRoot() const { return root_; }
    int getHeight()        const { return height_; }


    void DEBUG_print();
    void dfs_print(Node *node, int depth);
};

class Node {
  public:
    friend Octree;

  private:
    int depth_ = 0;
    unsigned char child_mask_ = 0x00;
    Node *first_child_ = nullptr;  // TODO really needed?
    Node *next_sibling_ = nullptr; // TODO really needed?
    ImplicitShape *shape_ = nullptr;
  public:
    int getDepth() const { return depth_; }
    unsigned char getChildMask() const { return child_mask_; }
    Node *getFirstChild() const { return first_child_; }
    Node *getNextSibling()  const { return next_sibling_; }

};




#endif
