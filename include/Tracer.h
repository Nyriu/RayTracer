#ifndef TRACER_H
#define TRACER_H

#include <iostream>
#include <ostream>
#include <string.h>
#include "Color.h"
#include "Ray.h"
#include "Scene.h"
#include "Octree.h"


class Tracer {
  protected:
    // Tracer Options
    int   max_distance_  = 100; // max n of steps along a ray // draw distance?
    float hit_threshold_ = 10e-6; // min distance to signal a ray-sruface hit
    //float anti_selfhit_shadow_threshold_ = 10e-15;
  public:
    //virtual bool hasScene() const = 0;
    virtual bool hasScene() const {
      std::cout << "ERROR: Tracer.hasScene() not implemented!" << std::endl;
      exit(1);
    }
    //virtual Tracer setScene(Scene* scene) = 0;
    virtual Tracer setScene(Scene* scene) {
      std::cout << "ERROR: Tracer.setScene() not implemented!" << std::endl;
      exit(1);
    }

    //virtual Color trace(const Ray& r) = 0;
    virtual Color trace(const Ray& r) { // better with pointer?
      std::cout << "ERROR: Tracer.trace(ray) not implemented!" << std::endl;
      exit(1);
    }
};


class SphereTracer : public Tracer {
  private:
    const Scene* scene_ = nullptr;
  public:
    SphereTracer() {}

    bool hasScene() const { return scene_ != nullptr; }
    Tracer setScene(Scene* scene) {
      scene_ = scene;
      return *this;
    }

    Color trace(const Ray& r); // better with pointer?

  private:
    Color sphereTrace(const Ray& r); // better with pointer?
    Color shade(const Point3& p, const Vec3& viewDir, const ImplicitShape *shape);
    bool sphereTraceShadow(const Ray& r, const ImplicitShape *shapeToShadow);
};


class OctreeTracer : public Tracer {
  private:
    const Octree* oct_scene_ = nullptr;
    struct Span {
      public:
        float min = 0.f;
        float max = 1.f;
        //Span() = default;
        Span(float t_min, float t_max) : min(t_min), max(t_max) {};

        friend std::ostream& operator<<(std::ostream& out, const Span& v) {
          return out << "[" << v.min << "," << v.max << "]";
        }
    };


    struct RayInfo {
      private:
        float epsilon = 10e-7; // TODO adjust
      public:
        Point3 origin;
        Vec3 direction;

        float tx_coef=0.f, ty_coef=0.f, tz_coef=0.f;
        float tx_bias=0.f, ty_bias=0.f, tz_bias=0.f;

        RayInfo(const Ray *ray) {
          Vec3 d = ray->direction();

          // handle division by zero
          if (std::abs(d.x()) < epsilon) d.set_x(epsilon);
          if (std::abs(d.y()) < epsilon) d.set_y(epsilon);
          if (std::abs(d.z()) < epsilon) d.set_z(epsilon);

          // TODO mirroring + octant_mask here?

          tx_coef = 1.f / d.x();
          ty_coef = 1.f / d.y();
          tz_coef = 1.f / d.z();

          Point3 o = ray->origin();
          tx_bias = -tx_coef * o.x();
          ty_bias = -ty_coef * o.y();
          tz_bias = -tz_coef * o.z();

          origin = o;
          direction = d;
        }

        friend std::ostream& operator<<(std::ostream& out, const RayInfo& ri) {
          return out << "{" <<
            "\n\torigin = " << ri.origin <<
            "\n\tdirection = " << ri.direction <<
            "\n\ttx_coef = [" <<
            ri.tx_coef << ", " <<
            ri.ty_coef << ", " <<
            ri.tz_coef << "]" <<
            "\n\ttx_bias = [" <<
            ri.tx_bias << ", " <<
            ri.ty_bias << ", " <<
            ri.tz_bias << "]" <<
            "\n}";
        }
    };

    struct NodeInfo {
      private:
        const Node *node_ptr = nullptr;
        const RayInfo *rayinfo_ptr = nullptr;
      public:
        float size  = 0.f;
        int depth = 0;

        float x0=0.f, y0=0.f, z0=0.f;
        float x1=0.f, y1=0.f, z1=0.f;

        float tx0=0.f, ty0=0.f, tz0=0.f;
        float tx1=0.f, ty1=0.f, tz1=0.f;

        NodeInfo(Point3 default_p0, Point3 default_p1, const RayInfo *ri, float node_size, int node_depth, Node *node) : size(node_size), depth(node_depth), node_ptr(node), rayinfo_ptr(ri) {
          Point3 orig = ri->origin;

          // sort vertices' planes using distance
          if ( std::fabs(orig.x() - default_p0.x()) > std::fabs(orig.x() - default_p1.x()) ) {
            x0 = default_p1.x();
            x1 = default_p0.x();
          } else {
            x0 = default_p0.x();
            x1 = default_p1.x();
          }
          if ( std::fabs(orig.y() - default_p0.y()) > std::fabs(orig.y() - default_p1.y()) ) {
            y0 = default_p1.y();
            y1 = default_p0.y();
          } else {
            y0 = default_p0.y();
            y1 = default_p1.y();
          }
          if ( std::fabs(orig.z() - default_p0.z()) > std::fabs(orig.z() - default_p1.z()) ) {
            z0 = default_p1.z();
            z1 = default_p0.z();
          } else {
            z0 = default_p0.z();
            z1 = default_p1.z();
          }


          //x0 = default_p0.x(); y0 = default_p0.y(); z0 = default_p0.z();
          //x1 = default_p1.x(); y1 = default_p1.y(); z1 = default_p1.z();

          tx0 = x0*ri->tx_coef + ri->tx_bias;
          ty0 = y0*ri->ty_coef + ri->ty_bias;
          tz0 = z0*ri->tz_coef + ri->tz_bias;

          tx1 = x1*ri->tx_coef + ri->tx_bias;
          ty1 = y1*ri->ty_coef + ri->ty_bias;
          tz1 = z1*ri->tz_coef + ri->tz_bias;

          if (tx1 < 0) tx1 = utilities::infinity; // TODO here a fixed tmax?
          if (ty1 < 0) ty1 = utilities::infinity; // TODO here a fixed tmax?
          if (tz1 < 0) tz1 = utilities::infinity; // TODO here a fixed tmax?


          //if (tx0 > tx1) {
          //  float tmp_x = x0; float tmp_t = tx0;
          //  x0 = x1; tx0 = tx1;
          //  x1 = tmp_x; tx1 = tmp_t;
          //}
          //if (ty0 > ty1) {
          //  float tmp_y = y0; float tmp_t = ty0;
          //  y0 = y1; ty0 = ty1;
          //  y1 = tmp_y; ty1 = tmp_t;
          //}
          //if (tz0 > tz1) {
          //  float tmp_z = z0; float tmp_t = tz0;
          //  z0 = z1; tz0 = tz1;
          //  z1 = tmp_z; tz1 = tmp_t;
          //}

          if (!(tx0 <= tx1 && ty0 <= ty1 && tz0 <= tz1)) {
            std::cout << "ERROR on NodeInfo" <<
              "\nx0 = " << x0 <<
              "\ny0 = " << y0 <<
              "\nz0 = " << z0 <<
              "\nx1 = " << x1 <<
              "\ny1 = " << y1 <<
              "\nz1 = " << z1 <<

              "\ntx0 = " << tx0 <<
              "\nty0 = " << ty0 <<
              "\ntz0 = " << tz0 <<
              "\ntx1 = " << tx1 <<
              "\nty1 = " << ty1 <<
              "\ntz1 = " << tz1 <<
              std::endl;;
            exit(2);
          }
        }

        unsigned char getChildMask() const { return node_ptr->getChildMask(); }
        Node *getFirstChild()   const { return node_ptr->getFirstChild(); }
        Node *getNextSibling()  const { return node_ptr->getNextSibling(); }
        ImplicitShape *getShape() const { return node_ptr->getShape(); }
        bool isEmpty() const { return node_ptr->isEmpty(); }

        bool doesNodeExist() const { return node_ptr != nullptr; }
        bool doesChildExist(int child_idx) const {
          return getChildMask() & 1<<(7-child_idx);
        }

        const RayInfo* getRayInfo() const { return rayinfo_ptr; }

        friend std::ostream& operator<<(std::ostream& out, const NodeInfo& ni) {
          return out << "{" <<
            "\n\tsize = " << ni.size <<
            "\n\tdepth = " << ni.depth <<

            "\n\tp0 = [" <<
            ni.x0 << ", " <<
            ni.y0 << ", " <<
            ni.z0 << "]" <<

            "\n\tp1 = [" <<
            ni.x1 << ", " <<
            ni.y1 << ", " <<
            ni.z1 << "]" <<

            "\n\tt_p0 = [" <<
            ni.tx0 << ", " <<
            ni.ty0 << ", " <<
            ni.tz0 << "]" <<

            "\n\tt_p1 = [" <<
            ni.tx1 << ", " <<
            ni.ty1 << ", " <<
            ni.tz1 << "]" <<

            "\n}";
        }
    };

    struct Pos {
      private:
        int* A_ = nullptr;
        int max_depth_ = -1;
        int depth_ = 0;
        int null_value_ = 9;
        int row_size_ = -1;
      public:
        Pos(int max_depth) : max_depth_(max_depth) , row_size_(max_depth+1) {
          size_t size = row_size_*4* sizeof(int);
          A_ = (int *)malloc(size);

          for (int i=0; i<=max_depth_; i++) {
            A_[0*row_size_ + i] = null_value_;
            A_[1*row_size_ + i] = null_value_;
            A_[2*row_size_ + i] = null_value_;
            A_[3*row_size_ + i] = null_value_;
          }
          depth_ = 0; // root is 0, leaf is octree.height
          // set root info
          // note that (0,0,0) in the root's column doesn't mean zero-th child
          // but that we are using the right root
          // if one of the zeros becomes 1 then something is wrong
          A_[0*row_size_ + 0] = 0;
          A_[1*row_size_ + 0] = 0;
          A_[2*row_size_ + 0] = 0;
          A_[3*row_size_ + 0] = 0;
        }

        Pos(const Pos& pos) : max_depth_(pos.max_depth_), depth_(pos.depth_), row_size_(pos.row_size_) {
          size_t size = row_size_*4* sizeof(int);
          A_ = (int *)malloc(size);
          for (int i=0; i<=depth_; i++) {
            A_[0*row_size_ + i] = pos.A_[0*row_size_ + i];
            A_[1*row_size_ + i] = pos.A_[1*row_size_ + i];
            A_[2*row_size_ + i] = pos.A_[2*row_size_ + i];
            A_[3*row_size_ + i] = pos.A_[3*row_size_ + i];
          }
          for (int i=depth_+1; i<=max_depth_; i++) {
            A_[0*row_size_ + i] = null_value_;
            A_[1*row_size_ + i] = null_value_;
            A_[2*row_size_ + i] = null_value_;
            A_[3*row_size_ + i] = null_value_;
          }
        }

        bool reached_max_depth() const { return depth_ >= max_depth_; }

        bool add(int mask, int sign) {
          // returns true if it's all ok
          std::cout << "\ndepth_ = " << depth_ << std::endl;
          int x=0, y=0, z=0;
          for (int i=0; i<=depth_; i++) {
            x ^= A_[1*row_size_+ i]<<(depth_-i);
            y ^= A_[2*row_size_+ i]<<(depth_-i);
            z ^= A_[3*row_size_+ i]<<(depth_-i);
          }
          std::cout << "\nx = " << x << "\ny = " << y << "\nz = " << z << "\n" << std::endl;
          x += (mask & 4)? sign : 0;
          y += (mask & 2)? sign : 0;
          z += (mask & 1)? sign : 0;
          std::cout << "\nmask = " << mask << "\nsign = " << sign << std::endl;
          std::cout << "\nx = " << x << "\ny = " << y << "\nz = " << z << "\n" << std::endl;

          int idx = 0;
          for (int i=0; i<=depth_; i++) {
            idx=0;
            idx += ((x & (1<<(depth_-i)))? 1:0)<<2;
            idx += ((y & (1<<(depth_-i)))? 1:0)<<1;
            idx += ((z & (1<<(depth_-i)))? 1:0)<<0;
            A_[i] = idx;
            A_[1*row_size_+ i] = (idx & 4)? 1 : 0; // x
            A_[2*row_size_+ i] = (idx & 2)? 1 : 0; // y
            A_[3*row_size_+ i] = (idx & 1)? 1 : 0; // z
          }

          if (A_[0] != 0) { // changed root idx
            std::cout << "WARNING : changing root idx : " << idx << std::endl;
            //exit(1);
          }
          return A_[0] == 0;
        }

        void step_in(int idx) {
          if (depth_ == max_depth_) {
            std::cout <<
              "ERROR: step_in : depth_ " << depth_ << " already at max_depth_ " << max_depth_ << std::endl;
            exit(1);
          }
          depth_++;
          A_[0*row_size_ + depth_] = idx;              // idx
          A_[1*row_size_ + depth_] = (idx & 4)? 1 : 0; // x
          A_[2*row_size_ + depth_] = (idx & 2)? 1 : 0; // y
          A_[3*row_size_ + depth_] = (idx & 1)? 1 : 0; // z
        }

        //int highest_differing_bit(Pos *pos_ptr)
        int highest_ancestor_depth(Pos *pos_ptr) {
          int i=0;
          while (
              i<=depth_ &&
              A_[0*row_size_ + i] == pos_ptr->A_[0*row_size_ + i]
              ) {
            i++;
          }
          i--;
          if (i<0) {
            std::cout << "ERROR: highest_ancestor_depth : i = " << i << std::endl;
            exit(1);
          }
          return i;
        }

        int round_position(int ancestor_depth) {
          // return ancestor idx
          if (ancestor_depth < 0 || ancestor_depth > max_depth_) {
            std::cout << "ERROR: round_position : invalid depth=" << ancestor_depth << std::endl;
            exit(1);
          }
          for (int i=ancestor_depth+1; i<=depth_; i++) {
            A_[0*row_size_ + i] = null_value_;
            A_[1*row_size_ + i] = null_value_;
            A_[2*row_size_ + i] = null_value_;
            A_[3*row_size_ + i] = null_value_;
          }
          depth_ = ancestor_depth;
          return A_[0*row_size_ + ancestor_depth];
        }

        int get_idx_at(int depth) {
          if (depth < 0 || depth > max_depth_) {
            std::cout << "ERROR: get_idx_at : invalid depth=" << depth << std::endl;
            exit(1);
          }
          return A_[0*row_size_ + depth];
        }

        friend std::ostream& operator<<(std::ostream& out, const Pos& pos) {
          out << "dpt";
          for (int i=0; i<=pos.max_depth_; i++) {
            out << " " << i;
          }
          out << "\n";
          std::string line_init[4] = {"idx", " x ", " y ", " z "};
          for (int j=0; j<4; j++) {
            out << line_init[j];
            for (int i=0; i<=pos.max_depth_; i++) {
              int tmp_val = pos.A_[j*pos.row_size_ + i];
              if (tmp_val == pos.null_value_) {
                out << " N";
                continue;
              }
              out << " " << tmp_val;
            }
            out << "\n";
          }
          return out;
        }
    };

  public:
    bool hasScene() const { return oct_scene_ != nullptr; }
    virtual Tracer setScene(Scene* scene) {
      oct_scene_ = new Octree(scene);
      return *this;
    }

    Color trace(const Ray& r); // better with pointer?

  private:
    Color octTrace(const Ray& r); // better with pointer?
    float octTrace(const Ray *r);
    float sphereTrace(const Ray *r, const ImplicitShape *shape, const Span& tv);

    Color shade(const Point3& p, const Vec3& viewDir, const ImplicitShape *shape);
    //bool sphereTraceShadow(const Ray& r, const ImplicitShape *shapeToShadow);

    //inline Span project_cube(const Point3& p0, const Point3& p1, const Ray *r);
    Span project_cube(const Point3& p0, const Point3& p1, const Ray *r);
    Span project_cube(const NodeInfo *p_info, const int child_idx, const RayInfo *ri);

    //inline ?
    Span project_cube(const NodeInfo *n_info, const RayInfo *ri);
    int select_child(const NodeInfo  *p_info, const RayInfo *ri, float t_min);
    NodeInfo* get_child_info(const NodeInfo *p_info, int child_idx);
    int gen_step_mask(const NodeInfo *ni, const RayInfo *ri, float t_max);

// END /// OctreeTracer --------------------------------------------------- ///

};


#endif
