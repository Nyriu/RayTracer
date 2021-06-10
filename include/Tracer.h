#ifndef TRACER_H
#define TRACER_H

#include <iostream>
#include <ostream>
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
            "\norigin = " << ri.origin <<
            "\ndirection = " << ri.direction <<
            "\ntx_coef = [" <<
            ri.tx_coef << ", " <<
            ri.ty_coef << ", " <<
            ri.tz_coef << "]" <<
            "\ntx_bias = [" <<
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
        float depth = 0.f;

        float x0=0.f, y0=0.f, z0=0.f;
        float x1=0.f, y1=0.f, z1=0.f;

        float tx0=0.f, ty0=0.f, tz0=0.f;
        float tx1=0.f, ty1=0.f, tz1=0.f;

        NodeInfo(Point3 default_p0, Point3 default_p1, const RayInfo *ri, float node_size, float node_depth, Node *node) :
          size(node_size), depth(node_depth), node_ptr(node), rayinfo_ptr(ri) {
          x0 = default_p0.x(); y0 = default_p0.y(); z0 = default_p0.z();
          x1 = default_p1.x(); y1 = default_p1.y(); z1 = default_p1.z();

          tx0 = x0*ri->tx_coef + ri->tx_bias;
          ty0 = y0*ri->ty_coef + ri->ty_bias;
          tz0 = z0*ri->tz_coef + ri->tz_bias;

          tx1 = x1*ri->tx_coef + ri->tx_bias;
          ty1 = y1*ri->ty_coef + ri->ty_bias;
          tz1 = z1*ri->tz_coef + ri->tz_bias;

          if (tx0 > tx1) {
            float tmp_x = x0; float tmp_t = tx0;
            x0 = x1; tx0 = tx1;
            x1 = tmp_x; tx1 = tmp_t;
          }
          if (ty0 > ty1) {
            float tmp_y = y0; float tmp_t = ty0;
            y0 = y1; ty0 = ty1;
            y1 = tmp_y; ty1 = tmp_t;
          }
          if (tz0 > tz1) {
            float tmp_z = z0; float tmp_t = tz0;
            z0 = z1; tz0 = tz1;
            z1 = tmp_z; tz1 = tmp_t;
          }

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
        
        bool doesNodeExist() const { return node_ptr != nullptr; }

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

  public:
    bool hasScene() const { return oct_scene_ != nullptr; }
    virtual Tracer setScene(Scene* scene) {
      oct_scene_ = new Octree(scene);
      return *this;
    }

    Color trace(const Ray& r); // better with pointer?

  private:
    Color octTrace(const Ray& r); // better with pointer?
    Color shade(const Point3& p, const Vec3& viewDir, const ImplicitShape *shape);
    //bool sphereTraceShadow(const Ray& r, const ImplicitShape *shapeToShadow);

    inline Span project_cube(const Point3& p0, const Point3& p1, const Ray *r);

    //inline ?
    Span project_cube(const NodeInfo *n_info, const RayInfo *ri);
    int select_child(const NodeInfo  *p_info, const RayInfo *ri, float t_min);
    NodeInfo* get_child_info(const NodeInfo *p_info, int child_idx);
    int gen_step_mask(const NodeInfo *ni, const RayInfo *ri, float t_max);

// END /// OctreeTracer --------------------------------------------------- ///

};


#endif
