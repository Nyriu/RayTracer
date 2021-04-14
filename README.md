# Simple RayTracer in C++

The aim is to build from scratch a CPU ray tracer (sphere tracing) for implicit surfaces (Signed Distance Functions).

I'm taking inspiration from:

  * *Ray Tracing from the Ground Up* by Kevin Suffern
  * *Ray Tracing in One Weekend* by Peter Shirley
  * [scratchpixel](https://www.scratchapixel.com/lessons/advanced-rendering/rendering-distance-fields) tutorials

## Dependencies
* SDL2
* GLM (removable)


## DONE

* Constructive Solid Geometry (CSG)
  * [here](https://www.iquilezles.org/www/articles/smin/smin.htm)
    and [here](https://www.scratchapixel.com/lessons/advanced-rendering/rendering-distance-fields/constructive-solid-geometry-blends-deformers)
    for theory


## TODO

* obj transformations

* blending and anti-aliasing
  * AA with cone-tracing
  * [here for theory](https://www.scratchapixel.com/lessons/advanced-rendering/rendering-distance-fields/basic-sphere-tracer)


## Examples

### Without Blending and Anti-Aliasing
![](./imgs/img_00.png)
![](./imgs/img_01.png)
![](./imgs/2020_03_22_spheres.png)

![](./imgs/img_02.png)
![](./imgs/2020_03_22_toruses.png)

Simple CSG

![](./imgs/img_03.png)

CSG Shadow Debug Process

![](./imgs/funny_shadow_debug_process.gif)


### New Images

![](./imgs/time_1617089573.png)
![](./imgs/time_1617091499.png)
![](./imgs/time_1617092196.png)
![](./imgs/time_1617092777.png)
![](./imgs/time_1617093299.png)
![](./imgs/time_1617093337.png)

