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
        Node *node_ptr = nullptr;
      public:
        float size  = 0.f;
        float depth = 0.f;

        float x0=0.f, y0=0.f, z0=0.f;
        float x1=0.f, y1=0.f, z1=0.f;

        NodeInfo(Point3 p0, Point3 p1, float node_size, float node_depth, Node *node) :
          size(node_size), depth(node_depth), node_ptr(node) {
          x0 = p0.x(); y0 = p0.y(); z0 = p0.z();
          x1 = p1.x(); y1 = p1.y(); z1 = p1.z();
        }

        unsigned char getChildMask() const { return node_ptr->getChildMask(); }
        Node *getFirstChild()   const { return node_ptr->getFirstChild(); }
        Node *getNextSibling()  const { return node_ptr->getNextSibling(); }
        
        bool doesNodeExist() const { return node_ptr != nullptr; }

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
