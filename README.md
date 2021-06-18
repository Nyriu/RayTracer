# Simple RayTracer in C++

The aim is to build from scratch a CPU ray tracer (sphere tracing) for implicit surfaces (Signed Distance Functions).

I'm taking inspiration from:

  * *Ray Tracing from the Ground Up* by Kevin Suffern
  * *Ray Tracing in One Weekend* by Peter Shirley
  * [scratchpixel](https://www.scratchapixel.com/lessons/advanced-rendering/rendering-distance-fields) tutorials

## Dependencies
* GLFW and OpenGL for window

## DONE

* Constructive Solid Geometry (CSG)
  * [here](https://www.iquilezles.org/www/articles/smin/smin.htm)
    and [here](https://www.scratchapixel.com/lessons/advanced-rendering/rendering-distance-fields/constructive-solid-geometry-blends-deformers)
    for theory

* obj transformations

## TODO

* blending and anti-aliasing
  * AA with cone-tracing
  * [here for theory](https://www.scratchapixel.com/lessons/advanced-rendering/rendering-distance-fields/basic-sphere-tracer)


## Examples

## Debugging "fun"
### how it should be...
![](./imgs/octree_stuff/time_1623085255.png)
![](./imgs/octree_stuff/time_1623230427.png)
![](./imgs/octree_stuff/time_1623913907.png)
### showing svo root's children
![](./imgs/octree_stuff/time_1623914189.png)
![](./imgs/octree_stuff/time_1623916258.png)
### showing children of the 7-th root's children
![](./imgs/octree_stuff/time_1623916940.png)
### debug images
![](./imgs/octree_stuff/time_1623929507.png)
![](./imgs/octree_stuff/time_1623929869.png)
![](./imgs/octree_stuff/time_1623937119.png)
![](./imgs/octree_stuff/time_1623938505.png)

## Example with SVO enabled
![](./imgs/octree_stuff/time_1624009245.png)
![](./imgs/octree_stuff/time_1624009367.png)
### shadows can be enabled but their computation doesn't take advantage of the SVO at the moment
