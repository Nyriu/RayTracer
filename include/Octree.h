#ifndef OCTREE_H
#define OCTREE_H

#include "geometry.h"
#include "utilities.h"
#include "Scene.h"

// DEBUG STUFF
#include <bitset>   // to print byte in binary
#include <iostream>
// END // DEBUG STUFF


class Octree {
  private:
    int root_dim_ = 1<<3;
    int height_ = 3;

    class Node {
      friend Octree;
      private:
        int depth_ = 0;
        unsigned char child_mask_ = 0x00;
        Node *first_child_ = nullptr;
        Node *next_sibling_ = nullptr;
        ImplicitShape *shape_ = nullptr;
    };
    Node *root_;

  public:
    //Octree() { }
    void fromScene(Scene *scene);
    void DEBUG_print();
    void dfs_print(Node *node, int depth);
};

#endif
