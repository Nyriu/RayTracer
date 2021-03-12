# Simple RayTracer in C++

The aim is to build from scratch a CPU ray tracer (sphere tracing) with focus on implicit surfaces (Signed Distance Functions).

I'm taking inspiration from:

  * *Ray Tracing from the Ground Up* by Kevin Suffern
  * *Ray Tracing in One Weekend* by Peter Shirley
  * [scratchpixel](https://www.scratchapixel.com/lessons/advanced-rendering/rendering-distance-fields) tutorials

First I'll put together something ugly but that get's the job done.
Then I'll improve it or move on to graphical APIs (thinking of Vulkan)

## Dependencies
* SDL2
* GLM (maybe removable, depends)



## TODO

* Constructive Solid Geometry (CSG)
  * [here](https://www.iquilezles.org/www/articles/smin/smin.htm)
    and [here](https://www.scratchapixel.com/lessons/advanced-rendering/rendering-distance-fields/constructive-solid-geometry-blends-deformers) (C++ impl. with *functors* and *variadic arguments*)
    for theory

* transformations
  * [here for theory](https://www.scratchapixel.com/lessons/advanced-rendering/rendering-distance-fields/basic-sphere-tracer)
  * keep translation and rotation separate to avoid 4x4 matrix, that can also scale (scaling is tricky)
  * we need to move the ray into obj space to calc the ray

* blending and anti-aliasing
  * multi-sampling or with cones (maybe better with sphere tracer)
  * [here for theory](https://www.scratchapixel.com/lessons/advanced-rendering/rendering-distance-fields/basic-sphere-tracer)

* create Image class

* *look_at* with position instead of cam direction

## Examples

### Without Blending and Anti-Aliasing
![](./imgs/img_00.png)
![](./imgs/img_01.png)
![](./imgs/img_02.png)

Simple CSG

![](./imgs/img_03.png)
