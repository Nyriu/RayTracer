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
bool DEBUG_oct_sphereTrace   = false;

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
      return shade(r.at(t), r.direction(), intersectedShape);  // use lights
      //return intersectedShape->color_;        // use only surf color
      //return Color(float(n_steps)/tmax,0, 0); // color by pixel comput cost
      //return Color(1,0,0);                    // fixed color
    }
    t += minDistance;
  }
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
Color OctreeTracer::trace(const Ray& r) {

  /// // END // DEBUG STUFF
  /// Pos pos(oct_scene_->getHeight());
  /// pos.step_in(3);
  /// pos.step_in(7);
  /// pos.step_in(3);

  /// std::cout <<
  ///   pos <<
  ///   "reached_max_depth = " <<
  ///   pos.reached_max_depth() <<
  ///   std::endl;

  /// Pos old_pos(pos);
  /// pos.add(4,  1);
  /// pos.add(2, -1);

  /// int ancestor_depth = old_pos.highest_ancestor_depth(&pos);
  /// int ancestor_idx   = old_pos.get_idx_at(ancestor_depth);

  /// std::cout <<
  ///   "old_pos\n" <<
  ///   old_pos <<
  ///   "pos\n" <<
  ///   pos <<
  ///   "\nancestor_depth = " <<
  ///   ancestor_depth <<
  ///   "\nancestor_idx = " <<
  ///   ancestor_idx <<
  ///   std::endl;

  /// pos.round_position(ancestor_depth);
  /// std::cout <<
  ///   "round pos at ancestor_depth\n" <<
  ///   pos <<
  ///   std::endl;

  /// exit(1);
  /// // END // DEBUG STUFF


  Vec3 dir = r.direction();
  //if (dir.x() < 0 && dir.y() > 0) {
  //  DEBUG_octTrace          = true;
  //  DEBUG_octTrace_return   = true;
  //  DEBUG_octTrace_ancestor = true;
  //  DEBUG_oct_sphereTrace   = false;
  //}
  float t_min = octTrace(&r);
  //if (dir.x() < 0 && dir.y() > 0) {
  //exit(1);
  //}
    

  //if (t_min == -1) exit(1);

  //std::cout << t_min << std::endl;
  //exit(1);
  if (t_min < 0) // miss
    return Color(0);
  Color c(t_min, 0, 0);
  //std::cout << c << std::endl;
  return c;
}

Color child_color(int idx) {
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

float OctreeTracer::octTrace(const Ray *r) {
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
  NodeInfo *child_info = get_child_info(parent_info, child_idx); // really necessary?
  pos.step_in(child_idx);
  //int depth = 1; // already in pos

  if (DEBUG_octTrace)
    std::cout <<
      "\n--- Initialization ---" <<
      "\nray_info = " << *ray_info <<
      "\nparent_info = " << *parent_info <<
      "\nt = " << t <<
      "\nh = " << h <<
      "\nchild_idx = " << child_idx <<
      "\nchild_info = " << *child_info <<
      "\npos =\n" << pos <<
      std::endl;

  const float exit_root_t = t.max;

  std::pair<NodeInfo*,float> stack[oct_scene_->getHeight()+1];
  stack[0] = std::make_pair(parent_info, exit_root_t); // root info at the first position
  if (DEBUG_octTrace)
    std::cout <<
      "\noct_scene_->getHeight() = " << oct_scene_->getHeight() <<
      "\nstack[0] =" << 
      "(" << stack[0].first << ", " << stack[0].second << ")" << 
      "\nstack[1] =" << 
      "(" << stack[1].first << ", " << stack[1].second << ")" << 
      //"\nstack[2] =" << 
      //"(" << stack[2].first << ", " << stack[2].second << ")" << 
      std::endl;


  while (!pos.reached_max_depth() && t.min <= t.max) // && t.min < exit_root_t && t.max < exit_root_t) // or voxel hit or exit octree
  {
    //TODO check OCTREE EXIT after miss or during POP

    if (DEBUG_octTrace) std::cout << "\n--- New While Cycle ---";

    Span tc = project_cube(child_info, ray_info);

    if (DEBUG_octTrace)
      std::cout <<
        "\nchild_info = " << child_info <<
        "\ntc = " << tc <<
        std::endl;

    if (child_info->doesNodeExist() && t.min <= t.max) { // use child bit mask of parent
      //if voxel small enough return t.min // TODO define "small enough"
      // INTERSECT
      if (DEBUG_octTrace) std::cout << "\n--- INTERSECT ---" << std::endl;

      Span tv(fmaxf(tc.min, t.min), fminf(tc.max,t.max)); // intersect(tc,t); // current voxel span
      if (DEBUG_octTrace) std::cout << "\ntv = " << tv << std::endl;


      if (tv.min <= tv.max) { // non empty intersection

        if (DEBUG_octTrace)
          std::cout <<
            "\nNon empty intersection" <<
            "\nIs leaf? " << (child_info->depth == oct_scene_->getHeight()) <<
            "\nchild_info->depth = " << child_info->depth <<
            "\noct_scene_->getHeight() = " << oct_scene_->getHeight() << std::endl;


        if (child_info->depth == oct_scene_->getHeight()-1) { // is a leaf // TODO better way? // parent leaf bit mask

          // launch sphere trace iff inside non empty LEAF!!!!!!
          if (!child_info->isEmpty()) { //if (voxel has a contour)
            //t' = project contour; //tv = intersect(tc,t');
            //if (DEBUG_octTrace) std::cout << "\nNot Empty so Launch sphereTrace" << std::endl;

            //float t_min = sphereTrace(r, child_info->getShape(), tv);
            //tv = Span(fmaxf(tv.min, t_min), tv.max);
            //if (DEBUG_octTrace)
            //  std::cout << "\nsphereTrace result" <<
            //    "\nt_min = " << t_min <<
            //    "\ntv.min = " << tv.min <<
            //    "\ntv.max = " << tv.min <<
            //    "\ntv = " << tv <<
            //    std::endl;



            float t_min = sphereTrace(r, child_info->getShape(), tv);
            tv = Span(fmaxf(tv.min, t_min), tv.max);
          }
          if (DEBUG_octTrace)
            std::cout <<
              "\nFound intersection  at " << tv.min << std::endl;
          if (DEBUG_octTrace_return)
            std::cout <<
              "\nExiting by found intersection " << tv.min << std::endl;
          return tv.min; // found desired intersection
        }
        // PUSH
        if (DEBUG_octTrace) std::cout << "\n--- PUSH ---" << std::endl;

        if (tc.max < h) {// exit from voxel different from parent one

          if (DEBUG_octTrace)
            std::cout <<
              "\nAdding to stack" <<
              "\npair = (" << parent_info << ", " << t.max << ")" <<
              "\nat = " << parent_info->depth <<
              "\nstack size = " << oct_scene_->getHeight() << std::endl;
          stack[parent_info->depth] = std::make_pair(parent_info, t.max);
        }
        if (DEBUG_octTrace && !(tc.max < h))
          std::cout <<
            "Not adding to stack" <<
            std::endl;

        h = tc.max;
        parent_info = child_info;
        child_idx = select_child(parent_info, ray_info, t.min);
        child_info = get_child_info(parent_info, child_idx); // really necessary? // TODO work to remove child_info
        pos.step_in(child_idx);
        t = tv;

        if (DEBUG_octTrace)
          std::cout <<
            "\nAfter Push" <<
            "\nt = " << t <<
            "\nh = " << h <<
            "\nparent_info = " << *parent_info <<
            "\nchild_idx = " << child_idx <<
            "\nchild_info = " << *child_info <<
            "\npos =\n" << pos <<
            std::endl;
        continue; // restart loop to process current voxel
      } else {
        if (DEBUG_octTrace)
          std::cout <<
            "\nNOT tv.min <= tv.max" <<
            "\ntv = " << tv <<
            std::endl;
        if (DEBUG_octTrace_return)
        std::cout <<
          "Exiting by NOT tv.min <= tv.max" <<
          std::endl;
      //return exit_root_t; // TODO here miss
      return -1; // TODO here miss
      }
    }
    // ADVANCE
    if (DEBUG_octTrace) std::cout << "\n--- ADVANCE ---" << std::endl;
    Pos old_pos(pos);
    // step along ray // assumed always possible
    //int step_mask=0;
    //if (child_info->tx1 == tc.max) step_mask^=4;
    //if (child_info->ty1 == tc.max) step_mask^=2;
    //if (child_info->tz1 == tc.max) step_mask^=1;
    int positive_mask = 0;
    if (ray_info->direction.x() > 0) positive_mask^=4;
    if (ray_info->direction.y() > 0) positive_mask^=2;
    if (ray_info->direction.z() > 0) positive_mask^=1;
    int negative_mask = 0;
    if (ray_info->direction.x() < 0) negative_mask^=4;
    if (ray_info->direction.y() < 0) negative_mask^=2;
    if (ray_info->direction.z() < 0) negative_mask^=1;

    pos.add(negative_mask, -1); // here can get child_idx from pos instead
    pos.add(positive_mask,  1);
    int     parent_idx = pos.get_idx_at(parent_info->depth);
    int old_parent_idx = old_pos.get_idx_at(parent_info->depth);

    //child_idx ^= step_mask;     // TODO better to do all without step_mask
    child_idx = pos.get_idx_at(child_info->depth);

    t.min = tc.max; // actual time advancement

    bool pop_needed =
      //!( ((child_idx & step_mask & positive_mask) ^ ( (child_idx^7) & step_mask & negative_mask)) & step_mask);
      parent_idx != old_parent_idx;
    // if at least the parent changes, we need a pop

    if (DEBUG_octTrace)
      std::cout <<
        "\nAfter Advance" <<
        "\nold_pos =\n" << old_pos <<
        //"\nstep_mask = " << step_mask <<
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
        std::endl;
    //exit(1);

    if (t.min == exit_root_t) {
      if (DEBUG_octTrace)
        std::cout <<
          "Exiting from root" <<
          "\nt = " << t <<
          "\nexit_root_t = " << exit_root_t <<
          std::endl;
      if (DEBUG_octTrace_return)
        std::cout <<
          "Exiting from root by t.min" <<
          std::endl;
      //return exit_root_t; // TODO here miss
      return -1; // TODO here miss
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
      int ancestor_depth = pos.highest_ancestor_depth(&old_pos);
      if (DEBUG_octTrace)
        std::cout <<
          "\nancestor_depth = " << ancestor_depth <<
          std::endl;

      // questa e' una cagata
      //if (ancestor_depth == 0) {
      //  if (DEBUG_octTrace_ancestor)
      //    std::cout <<
      //      "\npos =\n" << pos <<
      //      "\nold_pos =\n" << old_pos <<
      //      "\nancestor_depth = " << ancestor_depth <<
      //      std::endl;
      //  if (DEBUG_octTrace_return)
      //    std::cout <<
      //      "Exiting from root by ancestor depth" <<
      //      std::endl;
      //  //return exit_root_t; // TODO here miss
      //  return -1; // TODO here miss
      //}

      if (ancestor_depth > oct_scene_->getHeight()) {
        if (DEBUG_octTrace_return)
          std::cout <<
            "\nExit by ancestor_depth > oct_scene_->getHeight()" <<
            std::endl;
        return -1; // miss // TODO decide the value
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
      child_idx = pos.round_position(ancestor_depth);
        if (DEBUG_octTrace)
          std::cout <<
            "rounded pos at " << ancestor_depth << "\n" <<
            pos <<
            std::endl;

      child_info = get_child_info(parent_info, child_idx); // really necessary? // TODO work to remove child_info
        if (DEBUG_octTrace)
          std::cout <<
            "child_info = " <<
            child_info <<
            std::endl;

      h = 0;
    }
        //exit(1);
  }
  if (DEBUG_octTrace_return)
    std::cout <<
      "Exiting last return" <<
      std::endl;
  return t.min; // TODO here miss?
  //return -1; // TODO here miss?
}

float OctreeTracer::sphereTrace(const Ray *r, const ImplicitShape *shape, const Span& tv) {
  if (DEBUG_oct_sphereTrace) std::cout << "\n--- sphereTrace ---";

  float t = tv.min;

  while (t < tv.max) {
    float minDistance = infinity;

    float d = shape->getDistance(r->at(t));

    if (DEBUG_oct_sphereTrace) std::cout << "\nd = " << d;

    if (d < minDistance) {
      minDistance = d;
    }
    // did we intersect the shape?
    if (minDistance <= hit_threshold_ * t) {

      if (DEBUG_oct_sphereTrace)
        std::cout <<
          "\nminDistance = " << minDistance <<
          "\nt = " << t <<
          "\nhit_threshold_ * t = " << hit_threshold_ * t <<
          "\n--- sphereTrace END ---";
      return t;
    }
    t += minDistance;
  }
  if (DEBUG_oct_sphereTrace)
    std::cout <<
      "\nt = " << t <<
      "\nHit max distance " << tv.max <<
      "\n--- sphereTrace END ---";
  return tv.max;
  //return t;
  //return -1; // TODO encode miss
  //return (tv.min + tv.max)/2.f;
}



Color OctreeTracer::octTrace(const Ray& r) {
  // initial cube is supposed to start at t=0 and end at t=1 // not true for now
  //float t_min = 0, t_max = 1; // root active span
  //Span t(0,1); // root active span
 
  RayInfo *ray_info = new RayInfo(&r);
  std::cout << "ray info = " << *ray_info << "\n";
  //std::cout << //"ray { " <<
  //  //"\norig = " << ray_info->origin <<
  //  "\ndir  = " << ray_info->direction <<
  //  //"\n}\n";
  //  "\n";

  Pos pos(oct_scene_->getHeight());
  NodeInfo *root_info = new NodeInfo(
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
      oct_scene_->getRootDimension(),              // node size
      0.f,                                          // depth
      oct_scene_->getRoot()
      );
  //std::cout << "root node info = " << *root_info << "\n";
  // root active span
  Span t = project_cube(root_info, ray_info); // TODO think about the right p0 and p1 and
  //Span t1 = project_cube(&root_info, &r); // TODO think about the right p0 and p1 and
  //                                                // how to apply their offset/bias
  //std::cout << "span t1 = " << t1 << "\n"; // << std::cout;
  //t = Span(fmaxf(t.min,t1.min), fminf(t.max,t1.max));

  std::cout << "root span t = " << t << "\n"; // << std::cout;
  float h = t.max; // prevent unnecessary writes to stack

  //Node *parent = oct_scene_->getRoot();
  NodeInfo *parent_info = root_info;
  int child_idx = select_child(parent_info, ray_info, t.min);
  NodeInfo *child_info = get_child_info(parent_info, child_idx);
  std::cout << "parent_info = " << *parent_info << "\n"; // << std::cout;
  std::cout << "child_idx = " << child_idx << "\n"; // << std::cout;
  std::cout << "child_info = " << *child_info << "\n"; // << std::cout;
  //std::cout << "child span = " << project_cube(child_info, ray_info) << "\n"; // << std::cout;

  pos.step_in(child_idx);
  std::cout << "pos = \n" << pos << "\n";

  std::pair<NodeInfo*,float> stack[oct_scene_->getHeight()]; 
  //std::cout << "root t span = " << t << std::endl;
  int debug_counter = 0;
  //while (child_info->depth <= oct_scene_->getHeight()) 
  while (child_info->depth <= oct_scene_->getHeight() && debug_counter < 1) {
    std::cout << "new while cicle (" << debug_counter << ")" << std::endl;
    Span tc = project_cube(child_info, ray_info); // child t span
    std::cout << "tc span = " << tc << std::endl;

    //// if (child_info->doesNodeExist() && tc.min <= t.max) {
    ////   // if voxel is small enough return tmin // is ~ pixel wide

    ////   // intersect active span with the cube
    ////   //Span tv = intersect(tc,t);// current voxel span
    ////   Span tv(fmaxf(tc.min,t.min), fminf(tc.max,t.max));// current voxel span

    ////   if (child_info->depth == oct_scene_->getHeight()) { // it's a leaf
    ////     //std::cout << "leaf 1" << std::endl;
    ////     // here sphere trace the shape and shade
    ////     // here update t.min and t.max // TODO check INTERSECT in papaer's pseudo-code
    ////     return Color(0.5,0.5,0.3); // temp color // TODO remove
    ////   }

    ////   if (tv.min <= tv.max) { // is intersection non empty
    ////     if (child_info->depth == oct_scene_->getHeight()) { // it's a leaf
    ////       // return tv.min // found desired intersection // shade here?
    ////       //std::cout << "leaf 2" << std::endl;
    ////       return Color(0.5,0.3,0.5); // temp color // TODO remove
    ////     }
    ////     std::cout << "PUSH" << std::endl;
    ////     if (tc.max < h) { // se uscita dal voxel e' diversa da quella del padre
    ////       //stack[scale] = (parent,tmax)
    ////       stack[child_info->depth] = std::make_pair(parent_info, t.max));
    ////     }
    ////     // child must become parent
    ////     h = tc.max;
    ////     parent_info = child_info;
    ////     // and we descend into child's child
    ////     child_idx  = select_child(parent_info, ray_info, tv.min);
    ////     t = tv;
    ////     child_info = get_child_info(parent_info, child_idx);
    ////     continue; // restart loop to provess current voxel (new child's child)
    ////   }
    //// }

    std::cout << "ADVANCE" << std::endl;
    // ADVANCE // we assume always possible
    int step_mask=0;
    if (child_info->tx1 == tc.max) step_mask^=4;
    if (child_info->ty1 == tc.max) step_mask^=2;
    if (child_info->tz1 == tc.max) step_mask^=1;

    Pos old_pos(pos);

    std::cout <<
      "\nstep_mask = " << step_mask <<
      "\nchild_idx (before) = " << child_idx;
    child_idx ^= step_mask;
    std::cout <<
      "\nchild_idx ( after) = " << child_idx;


    //check if update disagree? // do we need a POP?
    int positive_mask = 0;
    if (ray_info->direction.x() > 0) positive_mask^=4;
    if (ray_info->direction.y() > 0) positive_mask^=2;
    if (ray_info->direction.z() > 0) positive_mask^=1;
    int negative_mask = 0;
    if (ray_info->direction.x() < 0) negative_mask^=4;
    if (ray_info->direction.y() < 0) negative_mask^=2;
    if (ray_info->direction.z() < 0) negative_mask^=1;
    std::cout <<
      "\npositive_mask = " << positive_mask <<
      "\nnegative_mask = " << negative_mask;
    bool pop_needed =
      !( ((child_idx & step_mask & positive_mask) ^ ( (child_idx^7) & step_mask & negative_mask)) & step_mask);
    std::cout << "\nneed pop " << pop_needed << std::endl;

    //can pop_needed be calc using highest_differing_bit?
    pos.add(negative_mask, -1);
    pos.add(positive_mask,  1);

    std::cout << "old_pos = \n" << old_pos << "\n";
    std::cout << "pos = \n" << pos << "\n";
    int ancestor_depth = pos.highest_ancestor_depth(&old_pos);
    std::cout <<
      "highest_differing_bit = " <<
      ancestor_depth <<
      std::endl;

    t.min = tc.max; // actual time advancement

    // POP // if advance was wrong means we needed a pop
    if (pop_needed) {
      std::cout << "POP" << std::endl;
      // TODO here check correctness
      if (ancestor_depth >= oct_scene_->getHeight()) return Color(0); // miss
      std::pair<NodeInfo*,float> tmp_pair = stack[ancestor_depth-1];
      parent_info = tmp_pair.first;
      t.max = tmp_pair.second;
      child_idx = pos.round_position(ancestor_depth);
      // TODO here collect child_info?
      h = 0;

      std::cout <<
        "ancestor info = \n" << parent_info <<
        "restored t.max= " << t.max <<
        "child_idx = " << child_idx <<
        // TODO ? "child_info = \n" << child_idx <<
        "rounded pos =\n " << pos <<
        "h = " << h <<
        std::endl;
    }

    debug_counter++;
  }

  exit(1);
  return child_color(child_idx);
}


Color OctreeTracer::shade(const Point3& p, const Vec3& viewDir, const ImplicitShape *shape) {
  // tmp stupid shading
  Color outRadiance = shape->getAlbedo(p);
  return outRadiance;

  //Vec3 n = shape->getNormalAt(p);

  //Color outRadiance = Color(0);

  //Vec3 lightDir, h;
  //float nDotl, nDotv, nDoth, vDoth;
  //Color brdf;

  //bool shadow;
  //float dist2 = 0;

  //Color cdiff = shape->getAlbedo(p);
  //float shininess_factor = shape->getShininess(p);
  //Color cspec = shape->getSpecular(p);

  //for (const auto& light : scene_->getLights()) {
  //  lightDir = (light->getPosition() - p);
  //  dist2 = lightDir.length2(); // squared dist
  //  lightDir.normalize();
  //  nDotl = n.dot(lightDir);

  //  if (nDotl > 0) {
  //    h = (viewDir+lightDir).normalize();
  //    vDoth = viewDir.dot(h);
  //    nDotv = viewDir.dot(h);
  //    nDoth = n.dot(h);

  //    Vec3 r = 2 * nDotl * n - lightDir;
  //    float vDotr = viewDir.dot(r);

  //    brdf =
  //      cdiff / M_PI +
  //      cspec * powf(vDotr, shininess_factor);

  //    // With shadows below
  //    shadow = sphereTraceShadow(Ray(p,lightDir), shape);
  //    outRadiance += (1-shadow) * brdf * light->getColor() * light->getIntensity() * nDotl
  //      / (float) (4 * dist2) // with square falloff
  //      ;
  //  }
  //}
  //if (scene_->hasAmbientLight()) {
  //  Light* ambientLight = scene_->getAmbientLight();
  //  outRadiance += ambientLight->getColor() * ambientLight->getIntensity() * cdiff;
  //}
  //return outRadiance;
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

int OctreeTracer::select_child(const NodeInfo *p_info, const RayInfo *ri, float t_min) {
  //work here
  int idx = 0;
  // t_min vs tx,ty,tz center of parent

  float x_center = (p_info->x0 + p_info->x1)/2;
  float y_center = (p_info->y0 + p_info->y1)/2;
  float z_center = (p_info->z0 + p_info->z1)/2;

  float tx_center = x_center*ri->tx_coef + ri->tx_bias ;
  //tx_center = (tx_center < 0)? 0 : tx_center;
  float ty_center = y_center*ri->ty_coef + ri->ty_bias ;
  //ty_center = (ty_center < 0)? 0 : ty_center;
  float tz_center = z_center*ri->tz_coef + ri->tz_bias ;
  //tz_center = (tz_center < 0)? 0 : tz_center;

  //std::cout <<
  //  "\nt_min = " << t_min <<
  //  "\nbox_center = " << Point3(x_center, y_center, z_center) <<
  //  "\nt_box_center = " << Point3(tx_center, ty_center, tz_center) <<
  //  std::endl;

  // TODO make adaptable to camera position and orientation
  // for now camera all positive (>0,>0,>0)
  if (
      tx_center < 0 || // given hypothesis camera on the right looking right
      tx_center >= t_min // camera looks through plane
     ) {
    idx ^= 4;
  }
  if (
      ty_center < 0 || // given hypothesis camera above looking up
      ty_center >= t_min
      ) {
    idx ^= 2;
  }
  if (
      tz_center < 0 || // given hypothesis camera "in front of the screen" looking "through screen"
      tz_center >= t_min
      ) {
    idx ^= 1;
  }

  return idx;
}

OctreeTracer::NodeInfo* OctreeTracer::get_child_info(const NodeInfo *p_info, int child_idx) {
  if (child_idx < 0 || child_idx > 7) std::cout << "ERROR: get_child_info: child_idx must be in [0,7]!" << std::endl;

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

