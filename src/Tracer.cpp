#include "Tracer.h"

// DEBUG STUFF
#include "utilities.h" // for DEBUG_message
#include <cmath>
#include <iostream>
#include <string>
#include <system_error>
bool DEBUG_sphereTraceShadow = false;
bool DEBUG_general = true;

// OctreeTracer
bool DEBUG_octTrace          = !true;
bool DEBUG_octTrace_return   = !true;
bool DEBUG_octTrace_ancestor = !true;
bool DEBUG_oct_sphereTrace   = !true;

using namespace utilities;
// END // DEBUG STUFF



/// SphereTracer ---------------------------------------------------------- ///
Color SphereTracer::trace(const Ray& r) {
  return sphereTrace(r);
}

Color SphereTracer::sphereTrace(const Ray& r) {
  float t=0;

  const ImplicitShape *intersectedShape;
  while (t < max_distance_) {
    float minDistance = infinity;

    for (auto shape : scene_->getShapes()) {
      float d = shape->getDistance(r.at(t));
      if (d < minDistance) {
        minDistance = d;
        intersectedShape = shape;
      }
    }

    // did we intersect the shape?
    if (minDistance <= hit_threshold_ * t) {
      //std::cout <<
      //  "\nsphereTrace hit at t = " << t <<
      //  "\nray orig = " << r.origin() <<
      //  "\nray dir  = " << r.direction() <<
      //  std::endl;
      return shade(r.at(t), r.direction(), intersectedShape);  // use lights
      //return intersectedShape->color_;        // use only surf color
      //return Color(float(n_steps)/tmax,0, 0); // color by pixel comput cost
      //return Color(1,0,0);                    // fixed color
    }
    t += minDistance;
  }
  //std::cout <<
  //  "sphereTrace miss at t = " << t <<
  //  "\nray orig = " << r.origin() <<
  //  "\nray dir  = " << r.direction() <<
  //  std::endl;
  return Color(0); // bg color // no hit color
  //return Color(1,0,0);
  //return Color(0.2,.2,.7);
}


Color SphereTracer::shade(const Point3& p, const Vec3& viewDir, const ImplicitShape *shape) {
  Vec3 n = shape->getNormalAt(p);

  Color outRadiance = Color(0);

  Vec3 lightDir, h;
  float nDotl, nDotv, nDoth, vDoth;
  Color brdf;

  bool shadow;
  float dist2 = 0;

  Color cdiff = shape->getAlbedo(p);
  float shininess_factor = shape->getShininess(p);
  Color cspec = shape->getSpecular(p);

  for (const auto& light : scene_->getLights()) {
    lightDir = (light->getPosition() - p);
    dist2 = lightDir.length2(); // squared dist
    lightDir.normalize();
    nDotl = n.dot(lightDir);

    if (nDotl > 0) {
      h = (viewDir+lightDir).normalize();
      vDoth = viewDir.dot(h);
      nDotv = viewDir.dot(h);
      nDoth = n.dot(h);

      Vec3 r = 2 * nDotl * n - lightDir;
      float vDotr = viewDir.dot(r);

      brdf =
        cdiff / M_PI +
        cspec * powf(vDotr, shininess_factor);

      // With shadows below
      shadow = sphereTraceShadow(Ray(p,lightDir), shape);
      outRadiance += (1-shadow) * brdf * light->getColor() * light->getIntensity() * nDotl
        / (float) (4 * dist2) // with square falloff
        ;
    }
  }
  if (scene_->hasAmbientLight()) {
    Light* ambientLight = scene_->getAmbientLight();
    outRadiance += ambientLight->getColor() * ambientLight->getIntensity() * cdiff;
  }
  return outRadiance;
}


bool SphereTracer::sphereTraceShadow(const Ray& r, const ImplicitShape *shapeToShadow) {
  bool &D = DEBUG_sphereTraceShadow;

  DEBUG_message(D,
      "[-------------------------\ninside sphereTraceShadow"
      );

  float t = 0;
  while (t < max_distance_) {
    float min_distance = infinity;
    Point3 from = r.at(t);
    for (auto shape : scene_->getShapes()) {
      float d = shape->getDistance(from);

      // Self-Hit Shadowing Error

      //// First method // Problems where actualy should be in shadow
      //if (shape == shapeToShadow && d < 10e-7)
      ////if (shape == shapeToShadow)
      //  continue;

      // Second method // Best so far
      if (shape == shapeToShadow && d <= hit_threshold_ * t) {
        // move "from" a bit over the surface (along the normal direction)
        d = shape->getDistance(
            from + shape->getNormalAt(from) * 10e-7
            );
      }

      // Third method
      // Try to descend into the CSG and exclude only current shape


      if (d < min_distance) {
        min_distance = d;
        if (min_distance <= hit_threshold_ * t) {
  DEBUG_message(D,
      "exiting sphereTraceShadow (true)\n-------------------------]\n"
      );
          return true;
        }
      }
    }
    t += min_distance;
  }

  DEBUG_message(D,
      "exiting sphereTraceShadow (false)\n-------------------------]\n"
      );

  return false;
}
// END /// SphereTracer --------------------------------------------------- ///




/// OctreeTracer ---------------------------------------------------------- ///
Color child_color(int idx) {
  if (DEBUG_octTrace) {
    std::cout << "idx for child_color = " << idx << std::endl;
  }
  // utility fuction for debug
  if (idx == 8) { // called sphere trace
    return Color(0.25, 0.25, 0);
  }
  if (idx < 0) { // miss
    return Color(0.25); // grey
  }
  if (
      idx != 0 &&
      idx != 1 &&
      idx != 2 &&
      idx != 3 &&
      idx != 4 &&
      idx != 5 &&
      idx != 6 &&
      idx != 7
      ) {
    return Color(0);
  }

  Color c(
      (idx & 4) ? 1 : 0,
      (idx & 2) ? 1 : 0,
      (idx & 1) ? 1 : 0
      );
  //std::cout <<
  //  "idx = " <<
  //  "c = " << c <<
  //  std::endl;
  return c;
  //return Color(idx/8.f);
}

Color OctreeTracer::trace(const Ray& r) {
  if (
      DEBUG_octTrace          ||
      DEBUG_octTrace_return   ||
      DEBUG_octTrace_ancestor ||
      DEBUG_oct_sphereTrace   )
    std::cout << "\n================= " << r.direction() << " =================" << std::endl;

  OctreeTracer::HitRecord ht = octTrace(&r);

  if (ht.isMiss()) {
    return Color(0);
  }
  Color c = shade(&ht);
  return c;

  //return child_color(ht.t_);
}

OctreeTracer::HitRecord OctreeTracer::octTrace(const Ray *r) {
  RayInfo *ray_info = new RayInfo(r);
  NodeInfo *parent_info = new NodeInfo( // root_info
      Point3(
        -oct_scene_->getRootDimension()/2.f,
        -oct_scene_->getRootDimension()/2.f,
        oct_scene_->getRootDimension()/2.f
        ), // p0 // (-d/2,-d/2, d/2) bottom corner
      Point3(
        oct_scene_->getRootDimension()/2.f,
        oct_scene_->getRootDimension()/2.f,
        -oct_scene_->getRootDimension()/2.f
        ), // p1 // ( d/2, d/2,-d/2) top corner
      ray_info,
      oct_scene_->getRootDimension(),  // node size
      0,                              // depth
      oct_scene_->getRoot()
      );
  Pos pos(oct_scene_->getHeight());
  Span t = project_cube(parent_info, ray_info);
  float h=t.max;
  int child_idx = select_child(parent_info, ray_info, t.min);
  
  if (0 <= child_idx && child_idx <= 7) {
    pos.step_in(child_idx);
  }

  if (DEBUG_octTrace) {
    std::cout <<
      "\n--- Initialization ---" <<
      "\nray_info = " << *ray_info <<
      "\nparent_info (ptr) = " << parent_info <<
      "\nparent_info = " << *parent_info <<
      "\nt = " << t <<
      "\nh = " << h <<
      "\nchild_idx = " << child_idx <<
      //"\nchild_info = " << *child_info <<
      "\npos =\n" << pos <<
      std::endl;
    //exit(1);
    //return HitRecord(child_idx);
  }
  if (child_idx < 0 || child_idx > 7) { // select_child already dettected a miss
    if (DEBUG_octTrace || DEBUG_octTrace_return) {
      std::cout <<
        "Exiting via select_child miss! Awesome!" <<
        std::endl;
    }
    //return t.max;
    return HitRecord(); // miss
  }

  const float exit_root_t = t.max;

  std::pair<NodeInfo*,float> stack[oct_scene_->getHeight()+1];
  stack[0] = std::make_pair(parent_info, exit_root_t); // root info at the first position
  if (DEBUG_octTrace) {
    std::cout <<
      "\noct_scene_->getHeight() = " << oct_scene_->getHeight() <<
      "\nstack[0] =" << 
      "(" << stack[0].first << ", " << stack[0].second << ")" << 
      "\nstack[1] =" << 
      "(" << stack[1].first << ", " << stack[1].second << ")" << 
      std::endl;
    //exit(1);
  }


  while (!pos.is_at_depth(oct_scene_->getHeight()+1)) // && t.min < exit_root_t && t.max < exit_root_t) // or voxel hit or exit octree
  {
    if (DEBUG_octTrace) {
      std::cout << "\n--- New While Cycle ---" <<
        "\nt = " << t <<
        "\nparent_info = " << *parent_info;
    }

    Span tc = project_cube(parent_info, child_idx, ray_info);

    if (DEBUG_octTrace) {
      std::cout <<
        "\nchild_idx = " << child_idx <<
        "\ntc = " << tc <<
        std::endl;
      //exit(1);
    }

    if (parent_info->doesChildExist(child_idx) && t.min <= t.max)  {
      if (false) { std::cout << "ERROR : LOD not implemented!" << std::endl;  exit(1); } //return t.min; // voxel is small enough // TODO use another formula // LOD related stuff // here disabled
      // INTERSECT
      if (DEBUG_octTrace) std::cout << "\n--- INTERSECT ---" << std::endl;

      Span tv(fmaxf(tc.min, t.min), fminf(tc.max,t.max)); // intersect(tc,t); // current voxel span
      if (DEBUG_octTrace) {
        std::cout << "\ntv = " << tv << std::endl;
        //exit(1);
      }


      NodeInfo *child_info_tmp = get_child_info(parent_info, child_idx); // TODO work to remove child_info
      HitRecord ht;
      if (!child_info_tmp->isEmpty() && parent_info->depth == oct_scene_->getHeight()-1) {
        float t_min = -1;

        if (DEBUG_octTrace) {
          std::cout <<
            "\nbefore assignement" <<
            "\nht" << ht <<
            std::endl;
        }
        ht = sphereTrace(r, child_info_tmp->getShape(), tv); 
        if (DEBUG_octTrace) {
          std::cout <<
            "\nafter assignement" <<
            "\nht" << ht <<
            std::endl;
        }


        t_min = ht.t_;
        tv = Span(fmaxf(tv.min, t_min), tv.max);
        if (DEBUG_octTrace) {
          std::cout << "\nsphereTrace result" <<
            "\nht.isMiss() = " << ht.isMiss() <<
            "\nt_min = " << t_min <<
            "\ntv = " << tv <<
            std::endl;
        }
        //return child_idx; // for debugging // TODO REMOVEME

      } else if (DEBUG_octTrace) {
        std::cout << "\nNon leaf empty node" <<
          std::endl;
      }

      if (tv.min <= tv.max) { // non empty intersection
        if (parent_info->depth == oct_scene_->getHeight()-1) {
          if (DEBUG_octTrace || DEBUG_octTrace_return) {
            std::cout <<
              "Intersection found via sphere tracing in leaf node!" <<
              std::endl;
          }
          if (ht.isMiss()) {
            std::cout <<
              "HitRecord shouldn't be a MISS!!" <<
              std::endl;
            exit(1); // TODO if here... FIX IT
          }
          return ht; // because sphere trace has not moved tmin past the current leaf
        }

        if (DEBUG_octTrace) {
          std::cout <<
            "\nNeed to visit inside" <<
            "\nparent_info->depth = " << parent_info->depth <<
            "\noct_scene_->getHeight() = " << oct_scene_->getHeight() << std::endl;
          //exit(1);
        }

        // PUSH
        if (DEBUG_octTrace) std::cout << "\n--- PUSH ---" << std::endl;


        if (tc.max < h) {// exit from voxel different from parent one
          if (DEBUG_octTrace) {
            std::cout <<
              "\nAdding to stack" <<
              "\npair = (" << parent_info << ", " << t.max << ")" <<
              "\nat = " << parent_info->depth <<
              "\noct_scene_->getHeight() = " << oct_scene_->getHeight() <<
              "\nbefore : stack[" << parent_info->depth << "] =" << 
              "(" << stack[parent_info->depth].first << ", " << stack[parent_info->depth].second << ")" << 
              std::endl;
          }
          stack[parent_info->depth] = std::make_pair(parent_info, t.max);
          if (DEBUG_octTrace) {
            std::cout <<
              "\nafter  : stack[" << parent_info->depth << "] =" << 
              "(" << stack[parent_info->depth].first << ", " << stack[parent_info->depth].second << ")" << 
              std::endl;
          }
        } else if (DEBUG_octTrace) {
          std::cout <<
            "Not adding to stack" <<
            std::endl;
          //exit(1);
        }

        h = tc.max;
        parent_info = get_child_info(parent_info, child_idx);
        child_idx = select_child(parent_info, ray_info, tv.min);

        //return child_idx; // just for debugging // TODO REMOVEME

        if (child_idx < 0 || child_idx > 7) {
          std::cout << "select_child miss inside while!" << std::endl;
          exit(1);
        }

        pos.step_in(child_idx);
        t = tv;

        if (DEBUG_octTrace) {
          std::cout <<
            "\nAfter Push" <<
            "\nt = " << t <<
            "\nh = " << h <<
            "\nparent_info (ptr) = " << parent_info <<
            "\nparent_info = " << *parent_info <<
            "\nchild_idx = " << child_idx <<
            "\npos =\n" << pos <<
            std::endl;
        }
        continue; // restart loop to process current voxel
      } else { // empty intersection
        if (DEBUG_octTrace)
          std::cout <<
            "\nNOT tv.min <= tv.max" <<
            "\nsphereTracing made me skip over this voxel" <<
            "\ntv = " << tv <<
            std::endl;
      }
    } else if (DEBUG_octTrace) {
      std::cout <<
        "Child doesn't exist" <<
        std::endl;
    }
    // ADVANCE
    if (DEBUG_octTrace) std::cout << "\n--- ADVANCE ---" << std::endl;
    Pos old_pos(pos);
    // step along ray // assumed always possible then check

    NodeInfo *child_info_tmp = get_child_info(parent_info, child_idx); // TODO work to remove child_info
    int step_mask = 0;
    if (tc.max == child_info_tmp->tx1) step_mask^=4;
    if (tc.max == child_info_tmp->ty1) step_mask^=2;
    if (tc.max == child_info_tmp->tz1) step_mask^=1;
    int positive_mask = 0;
    if (ray_info->direction.x() > 0) positive_mask^=4;
    if (ray_info->direction.y() > 0) positive_mask^=2;
    if (ray_info->direction.z() > 0) positive_mask^=1;
    int negative_mask = 0;
    if (ray_info->direction.x() < 0) negative_mask^=4;
    if (ray_info->direction.y() < 0) negative_mask^=2;
    if (ray_info->direction.z() < 0) negative_mask^=1;

    int step_mask_pos = step_mask & positive_mask;
    int step_mask_neg = step_mask & negative_mask;


    if (DEBUG_octTrace) {
      std::cout <<
        "\nray = " << r->direction() <<
        "\nstep_mask = " << step_mask <<
        "\nnegative_mask = " << negative_mask <<
        "\npositive_mask = " << positive_mask <<
        "\nstep_mask_neg = " << step_mask_neg <<
        "\nstep_mask_pos = " << step_mask_pos <<
        std::endl;
    }

    bool positive_add_ok = pos.add(step_mask_neg, -1);
    bool negative_add_ok = pos.add(step_mask_pos,  1);
    //bool positive_add_ok = pos.add(negative_mask, -1);
    //bool negative_add_ok = pos.add(positive_mask,  1);
    int     parent_idx = pos.get_idx_at(parent_info->depth);
    int old_parent_idx = old_pos.get_idx_at(parent_info->depth);

    child_idx = pos.get_idx_at(parent_info->depth+1);

    t.min = tc.max; // actual time advancement

    bool pop_needed =
      parent_idx != old_parent_idx;
    // if at least the parent changes, we need a pop
    // TODO is this completely safe? What if parent's parent chages but not direct parent? it's possible?

    if (DEBUG_octTrace) {
      std::cout <<
        "\nold_pos =\n" << old_pos <<
        "\npositive_mask = " << positive_mask <<
        "\nnegative_mask = " << negative_mask <<
        "\nchild_idx = " << child_idx <<
        "\npos =\n" << pos <<
        "\ntc.max = " << tc.max <<
        "\nt = " << t <<
        "\nexit_root_t = " << exit_root_t <<
        "\npop_needed = " << pop_needed <<
        "\nparent_idx = " << parent_idx <<
        "\nold_parent_idx = " << old_parent_idx <<
        //"\npositive_add_ok = " << positive_add_ok <<
        //"\nnegative_add_ok = " << negative_add_ok <<
        std::endl;
    }
    if (!(positive_add_ok && negative_add_ok)) {
      if (DEBUG_octTrace || DEBUG_octTrace_return) {
        std::cout <<
          "Exiting gently from root via add" <<
          std::endl;
      }
      return HitRecord(exit_root_t);
    }

    if (t.min == exit_root_t) {
      if (DEBUG_octTrace) {
        std::cout <<
          "Exiting from root" <<
          "\nt = " << t <<
          "\nexit_root_t = " << exit_root_t <<
          std::endl;
        exit(1);
      }
      if (DEBUG_octTrace_return) {
        std::cout <<
          "Exiting from root by t.min" <<
          std::endl;
        exit(1);
      }
      return HitRecord(exit_root_t);
    }

    if (pop_needed) { // need pop and sure not to exit from root
      // POP 
      if (DEBUG_octTrace) {
        std::cout << "\n--- POP ---" <<
          "\noct_scene_->getHeight() = " << oct_scene_->getHeight() <<
          "\nstack before";
        for (int i=0; i<=oct_scene_->getHeight(); i++) {
          std::cout <<
            "\nstack[" << i << "] =" << 
            "(" << stack[i].first << ", " << stack[i].second << ")";
        }
        std::cout << std::endl;
      }


      // if advance was wrong means we needed a pop
      int ancestor_depth = old_pos.highest_ancestor_depth(&pos);
      if (DEBUG_octTrace) {
        std::cout <<
          "\nancestor_depth = " << ancestor_depth <<
          std::endl;
      }

      // pulire codice sotto
      if (ancestor_depth > oct_scene_->getHeight()) {
        if (DEBUG_octTrace_return)
          std::cout <<
            "\nExit by ancestor_depth > oct_scene_->getHeight()" <<
            std::endl;
        //return exit_root_t; // miss
        return HitRecord(exit_root_t); // miss
      }

      if (ancestor_depth == 0) {
        std::cout <<
          "\nExit by ancestor_depth > oct_scene_->getHeight()" <<
          std::endl;
      }

      std::pair<NodeInfo*,float> tmp_pair = stack[ancestor_depth]; // [anch_depth-1] // TODO check index correctness
      if (DEBUG_octTrace)
        std::cout <<
          "tmp_pair = [" << tmp_pair.first << ", " << tmp_pair.second << "]" <<
          std::endl;
      parent_info = tmp_pair.first;
      if (DEBUG_octTrace)
        std::cout <<
          "parent_info = " << parent_info <<
          std::endl;
      t.max = tmp_pair.second;
      if (DEBUG_octTrace)
        std::cout <<
          "t.max = " << t.max <<
          std::endl;
      //pos.round_position(ancestor_depth);
      pos.round_position(ancestor_depth+1);
      if (DEBUG_octTrace)
        std::cout <<
          "rounded pos at " << ancestor_depth << "\n" <<
          pos <<
          std::endl;

      //child_idx = select_child(parent_info, ray_info, t.min);
      child_idx = pos.get_idx_at(ancestor_depth+1);
      if (DEBUG_octTrace)
        std::cout <<
          "child_idx" << child_idx <<
          std::endl;
      h = 0;
    }
  }
  if (DEBUG_octTrace_return)
    std::cout <<
      "Exiting last return" <<
      "\nt.min = " << t.min <<
      std::endl;
  return HitRecord(t.min); // here miss
}

OctreeTracer::HitRecord OctreeTracer::sphereTrace(const Ray *r, const ImplicitShape *shape, const Span& tv) {
  if (DEBUG_oct_sphereTrace) {
    std::cout <<
      "\n--- sphereTrace ---" <<
      "\ntv = " << tv <<
      std::endl;
  }

  //float t = tv.min;
  float t = 0;

  float minDistance = infinity;
  float d = infinity;
  Point3 p(0);
  while (t < tv.max)
  //while (t < 100) 
  {
    p = r->at(t);
    d = shape->getDistance(p);

    if (DEBUG_oct_sphereTrace) std::cout << "\nd = " << d;

    if (d < minDistance) {
      if (DEBUG_oct_sphereTrace)
        std::cout << "\nupdate minDistance" << std::endl;
      minDistance = d;
    }
    // did we intersect the shape?
    if (minDistance <= hit_threshold_ * t) {
      if (DEBUG_oct_sphereTrace)
        std::cout <<
          "\nHIT" <<
          "\nminDistance = " << minDistance <<
          "\nt = " << t <<
          "\nhit_threshold_ * t = " << hit_threshold_ * t <<
          "\n--- sphereTrace END ---";
      //exit(1);
      //return t;
      return HitRecord(
          r,
          shape,
          t,
          p,
          shape->getNormalAt(p),
          shape->getAlbedo(p)
          );
    }
    t += minDistance;
  }
  if (DEBUG_oct_sphereTrace)
    std::cout <<
      "\nMISS" <<
      "\nt = " << t <<
      //"\nHit max distance " << tv.max <<
      "\n--- sphereTrace END ---";
  //return tv.max;
  //return t; // better because t > t.max and this will induce advance or pop
  return HitRecord(t); // this encodes a miss
}

Color OctreeTracer::shade(const HitRecord *hit_record) {
  /// tmp stupid shading
  //Color outRadiance = hit_record->alb_;
  //return outRadiance;

  Point3 p = hit_record->p_;
  Vec3 viewDir = hit_record->r_->direction();

  Vec3 n = hit_record->n_;

  Color outRadiance = Color(0);

  Vec3 lightDir, h;
  float nDotl, nDotv, nDoth, vDoth;
  Color brdf;

  //bool shadow;
  float dist2 = 0;

  Color cdiff = hit_record->alb_;
  //float shininess_factor = shape->getShininess(p);
  //Color cspec = shape->getSpecular(p);

  for (const auto& light : scene_->getLights()) {
    lightDir = (light->getPosition() - p);
    dist2 = lightDir.length2(); // squared dist
    lightDir.normalize();
    nDotl = n.dot(lightDir);

    if (nDotl > 0) {
      h = (viewDir+lightDir).normalize();
      vDoth = viewDir.dot(h);
      nDotv = viewDir.dot(h);
      nDoth = n.dot(h);

      Vec3 r = 2 * nDotl * n - lightDir;
      float vDotr = viewDir.dot(r);

      brdf =
        cdiff / M_PI // + cspec * powf(vDotr, shininess_factor)
        ;

      // With shadows below
      //shadow = sphereTraceShadow(Ray(p,lightDir), shape);
      //outRadiance += (1-shadow) * brdf * light->getColor() * light->getIntensity() * nDotl
      //  / (float) (4 * dist2) // with square falloff
      //  ;
      outRadiance += brdf * light->getColor() * light->getIntensity() * nDotl
        / (float) (4 * dist2) // with square falloff
        ;
    }
  }
  if (scene_->hasAmbientLight()) {
    Light* ambientLight = scene_->getAmbientLight();
    outRadiance += ambientLight->getColor() * ambientLight->getIntensity() * cdiff;
  }
  return outRadiance;
}


//inline?
OctreeTracer::Span OctreeTracer::project_cube(const NodeInfo *ni, const RayInfo *ri) {
  // TODO // coordinate mirroring
  // TODO // flippare z per compensare left-handedness del cubo?
  // vado avanti senza flip e poi sistemo se e' tutto specchiato rispetto il piano xy
  // per ora funziona sse la camera e' nel positivo e guarda nel negativo

  // TODO clamp to zero?

  //std::cout <<
  //  "\n------------------------------------------------" <<
  //  "\ntx0 = " << tx0 <<
  //  "\nty0 = " << ty0 <<
  //  "\ntz0 = " << tz0 <<
  //  "\n" <<
  //  "\ntx1 = " << tx1 <<
  //  "\nty1 = " << ty1 <<
  //  "\ntz1 = " << tz1 <<
  //  std::endl;

  float t_min = fmaxf(ni->tx0, fmaxf(ni->ty0, ni->tz0));
  float t_max = fminf(ni->tx1, fminf(ni->ty1, ni->tz1));

  return Span(t_min, t_max);
}

OctreeTracer::Span OctreeTracer::project_cube(const NodeInfo *p_info, const int child_idx, const RayInfo *ri) {
  const NodeInfo *c_info = get_child_info(p_info, child_idx);
  float t_min = fmaxf(c_info->tx0, fmaxf(c_info->ty0, c_info->tz0));
  float t_max = fminf(c_info->tx1, fminf(c_info->ty1, c_info->tz1));

  if (!(t_min <= t_max)) {
    std::cout <<
      "\nWARNING in project_cube : not t_min <= t_max" <<
      //"\nERROR in project_cube : not t_min <= t_max" <<
      //"\nt_min = " << t_min <<
      //"\nt_max = " << t_max <<
      //"\nray_info = " << *ri << 
      //"\nc_info = " << *c_info <<
      std::endl;
    //exit(1);
  }


  return Span(t_min, t_max);
}


int OctreeTracer::select_child(const NodeInfo *p_info, const RayInfo *ri, float t_min) {
  const int no_child = -1;

  int idx = 0;

  float x_center = (p_info->x0 + p_info->x1)/2;
  float y_center = (p_info->y0 + p_info->y1)/2;
  float z_center = (p_info->z0 + p_info->z1)/2;

  float tx_center = x_center*ri->tx_coef + ri->tx_bias ;
  float ty_center = y_center*ri->ty_coef + ri->ty_bias ;
  float tz_center = z_center*ri->tz_coef + ri->tz_bias ;

  if (
      p_info->tx0 < 0 ||
      p_info->ty0 < 0 ||
      p_info->tz0 < 0 
     )
  {
    return no_child;
  }

  // ASSUMPTIONS // IMPORTANT
  // ray origin in positivie x,y,z quadrant looking towards origin
  // completely *to the right*, *above* and *+out the screen* w.r.t. the root cube
  // so the cam origin can never be between the center and one defining plane of a voxel
  // in other words: the 3 cam org defining planes will *always* be greater than any voxel's plane
  //
  // Beware because this influence the scene setup!

  if (
      tx_center >= t_min
     ) {
    idx ^= 4;
  }
  if (
      ty_center >= t_min
     ) {
    idx ^= 2;
  }
  if (
      tz_center >= t_min
     ) {
    idx ^= 1;
  }
  if (DEBUG_octTrace) {
    std::cout <<
      "\nSelect Child = " <<
      //"\nx_ok = " << x_ok <<
      //"\ny_ok = " << y_ok <<
      //"\nz_ok = " << z_ok <<
      "\nt_min = " << t_min <<
      "\nbox_center = " << Point3(x_center, y_center, z_center) <<
      "\nt_box_center = " << Point3(tx_center, ty_center, tz_center) <<
      "\np_info (ptr) = " << p_info <<
      "\np_info = " << *p_info <<
      "\nr_info = " << *ri <<
      "\nidx = " << idx <<
      std::endl;
  }
  return idx;
}

OctreeTracer::NodeInfo* OctreeTracer::get_child_info(const NodeInfo *p_info, int child_idx) {
  if (child_idx < 0 || child_idx > 7) {
    std::cout << "ERROR: get_child_info: child_idx must be in [0,7]! Instead got " << child_idx << std::endl;
    exit(1);
  }

  float half_psize  = p_info->size/2.f;

  Node *node_ptr = nullptr;
  unsigned char child_mask = p_info->getChildMask();

  /// Catch a possible error
  if (p_info->getFirstChild() == nullptr) {
    std::cout <<
      "ERROR: get_child_info: Impossible to get child info because first child of parent i nullptr!" <<
      p_info <<
      std::endl;
    exit(1);
  }

  if (child_mask & 1<<(7-child_idx)) { // if child exists
    // retrieve its pointer using its siblings if necessary
    int idx=0;
    while (idx <= child_idx) {
      if (!(child_mask & 1<<(7-idx))){
        // if this child doesn't exist, try next one
        idx++;
        continue;
      }
      // if it exists
      if (node_ptr == nullptr) node_ptr = p_info->getFirstChild(); // if it is the first to be discovered
      else node_ptr = node_ptr->getNextSibling();
      // else

      idx++;
    }
  }

  /// // DEBUG stuff
  /// Node *dbg_ptr = nullptr;
  /// for (int idx=0; idx<8; idx++) {
  ///   if (!(child_mask & 1<<(7-idx))) {
  ///     std::cout <<
  ///       "\nidx = " << idx <<
  ///       "\tempty";
  ///     continue;
  ///   }
  ///   // if it exists
  ///   if (dbg_ptr == nullptr)
  ///     dbg_ptr = p_info->getFirstChild();
  ///   else
  ///     dbg_ptr = dbg_ptr->getNextSibling();
  ///   std::cout <<
  ///     "\nidx = " << idx <<
  ///     "\tptr = " << dbg_ptr;
  /// }
  /// std::cout <<
  ///   "\nchild_idx = " << child_idx <<
  ///   "\tnode_ptr = " << node_ptr <<
  ///   std::endl;
  /// // END // DEBUG stuff


  //float offset_x0 =  (child_idx & 4) ?  half_psize : 0;
  //float offset_y0 =  (child_idx & 2) ?  half_psize : 0;
  //float offset_z0 = !(child_idx & 1) ? -half_psize : 0;

  float x_center = (p_info->x0 + p_info->x1)/2;
  float y_center = (p_info->y0 + p_info->y1)/2;
  float z_center = (p_info->z0 + p_info->z1)/2;

  float offset_x0 =  (child_idx & 4) ?  half_psize : -half_psize;
  float offset_y0 =  (child_idx & 2) ?  half_psize : -half_psize;
  float offset_z0 =  (child_idx & 1) ?  half_psize : -half_psize;
      // for reference
     //   -oct_scene_->getRootDimension()/2.f,
     //   -oct_scene_->getRootDimension()/2.f,
     //    oct_scene_->getRootDimension()/2.f
     //   ), // p0 // (-d/2,-d/2, d/2) bottom corner
     // Point3(
     //    oct_scene_->getRootDimension()/2.f,
     //    oct_scene_->getRootDimension()/2.f,
     //   -oct_scene_->getRootDimension()/2.f
     //   ), // p1 // ( d/2, d/2,-d/2) top corner
      // END // for reference

  return new OctreeTracer::NodeInfo (
      // here bottom left
      // bottom corner and top corner not respected but
      // it's sufficient them to be opposite
      Point3(
        x_center+offset_x0,
        y_center+offset_y0,
        z_center+offset_z0
        ),
      Point3(x_center, y_center, z_center),
      p_info->getRayInfo(),
      half_psize,      // node size
      p_info->depth+1, // depth
      node_ptr
      );
}



int OctreeTracer::gen_step_mask(const NodeInfo *ni, const RayInfo *ri, float t_max) {
  // TODO // coordinate mirroring needed here??

  //float tx1 = ni->x1*ri->tx_coef + ri->tx_bias; tx1 = (tx1 < 0)? -infinity : tx1;
  //float ty1 = ni->y1*ri->ty_coef + ri->ty_bias; ty1 = (ty1 < 0)? -infinity : ty1;
  //float tz1 = ni->z1*ri->tz_coef + ri->tz_bias; tz1 = (tz1 < 0)? -infinity : tz1;
  float tx1 = ni->tx1;
  float ty1 = ni->ty1;
  float tz1 = ni->tz1;

  int step_mask=0;
  //if (tx1 <= t_max) // safer?
  if (tx1 == t_max) step_mask^=4;
  if (ty1 == t_max) step_mask^=2;
  if (tz1 == t_max) step_mask^=1;

  return step_mask;
}


// END /// OctreeTracer --------------------------------------------------- ///

