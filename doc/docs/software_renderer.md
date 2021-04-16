
The software renderer is the algorithm that draw 3D objects on the screen without using the GPU. This means that all graphics pipeline steps must be implemented from scratch, from colouring pixels to object culling. <br/>

The renderer supports :

* Object culling.
* Polygon culling.
* Polygon clipping.
* Polygon rasterization.
* Polygon perspective interpolation. 
* Pixel depth buffer test.
* Pixel texture mapping
* Pixel shading.

The whole algorithm consists of breaking objects to polygons to pixels by filtering the maximum number of data for each steps.

The algorithm can be divided in three steps :

1. Breaking objects to polygons.
2. Breaking polygons to pixels.
3. Pixel shading.

# Data model

<svg-inline src="software_renderer_data_model.svg""></svg-inline>

**Material:**

The rendered color of the object with an optional texture. The material is used in the pixel shading stage. <br/>
Materials can be typed to be assigned to different render pipeline :

* NotShaded_NotTextured
* NotShaded_Textured
* FlatShaded_Textured
* FlatShaded_NotTextured

Flat shaded means that polygon faces act as if they are flat surface.

**RederedObject:**

The rendered object is the 3D object definition, it contains all informations to provide to the render pipeline to draw a 3D object.

# Render pipeline

A render pipeline is the algorithm that draw 3D objects on a target texture. For every pipeline, all steps are the same, but the pixel shading step may differ depending of the material type. Is there texture mapping ? Do we need perspective interpolation ? Is flat shade enabled ? 
Different answers to these questions will produce different render pipeline. Even if this imply some small code duplication sometimes.
 
This choice has been made to :

1. Avoid branching inside inside pipeline as mush as possible. All pipelines are straightforward, more readable and performance problems may be easy to find and fix.
2. Having no performance impact of introducing a new render pipeline.

<svg-inline src="software_renderer_flow.svg""></svg-inline>

Every render pipeline take the following inputs :

* A camera with view matrix, projection matrix and precomputed frustum.
* A depth texture buffer.
* The target rendered texture.
* An array of rendered objects.

# Simplifications

* We assume that polygons are always triangles. This makes the polygon rasterization and interpolation straightforward.
* The render texture is defined by floating point and is the target texture used as output of the renderer.

### 1. Breaking objects to polygons

#### 1.1 Camera space projection

Every rendered objects positions are projected into camera space by using the rendered object model matrix and the camera view matrix.

#### 1.2 Object culling

Rendered objects are filtered by checking if the mesh bounding sphere intersects with the camera frustum.

Once the bounding sphere is projected in camera space, we create three segments along the X,Y,Z (camera local space) and we check if each segments intersect or are contained within the camera frustum planes. 
If at least one segment is not, then the entire object is ensured to not be visible, so it is discarded.

#### 1.3 Objects break to polygons

For every objects, we extract all polygons and vertices from the mesh. Because every polygons are unique and may share the same vertices, camera and world positions of vertices are calculated only if they haven't been calculated already, 
this avoids to do the same matrix projection multiple times.

#### 1.4 Polygon backface culling

Only one side of polygons are rendered. To avoid pushing polygons that will not be rendered all polygons that are not facing the camera are discarded. <br/>

This is done by calculating the oriented normal of the polygon and project it on the camera forward direction. 

### 2. Breaking polygons to pixels

#### 2.1 Polygon clipping

The polygon clipping allows us to remove polygons that are beyond the camera. Projecting a point beyond the camera in clip space will produce garbage values so we filter them.

This is done by ensuring that for every polygons in camera space, the forward (Z axis) value of every vertices are > 0.

> **WARNING:** This solution makes polygons popping in and out when they move across the camera. A smarter solution would be to cut the polygon with the near plane and discard only the cutted part that is behind the camera.  

#### 2.2 Polygon sorting

Polygons are sorted by their distance from camera to render them from near to far. <br/> 
This step is required to avoid overdrawing as much as possible. Polygons near to the camera are more likely to hide others.

#### 2.3 Rasterization

Once the polygon has been projected to screen space, the rasterization function tells us if one pixel is inside the polygon. <br/>
The rasterization can be divided in three step.

**Polygon bounding box calculation:**

To avoid to check all pixel of the rendered texture, we calculate the bounding box of the polygon that will discard all other pixels of the iteration.

**Polygon edge function check:**

To know if the pixel is inside the polygon, we calculate the edge function relative to the pixel for the three edges of the polygon.<br/>
The edge function calculates the signed area of the parallelogram defined by the edge and pixel point. If all areas are positive, then this means that the pixel is inside.

**Interpolation factors:**

We want to be able to interpolate data associated to vertices to all pixels. An interpolated value for a pixel is defined as (V1 * I1) + (V2 * I2) + (V3 * I3) where V1, V2 and V3 are the datas associated to vertices. I1, I2 and I3 are the interpolation factors.

For vertex V1, the interpolation factor is the ratio between the area calculated by the edge function relative to the (V2-V3) edge and the pixel and the area calculated by the edge function relative to the (V2-V3) edge and the V1 position. <br/>
If the pixel is positioned at V1, then I1 will be 1 and I2 and I3 will be zero because their areas is shrank to a segment. 

<svg-inline src="software_renderer_interpolation.svg"></svg-inline>

#### 2.4 Depth testing

Multiple polygon can write to the same pixel. We must be sure that the final pixel color corresponds to the pixel that is the nearest from the camera. To do so, we interpolate vertices depth along the polygon and compare it's value against a depth texture that have the same size as the render target.
If the pixel pass the test, it's depth is written to the depth texture.

Using a linear interpolation with the interpolation factors will lead to distortion. Because the depth no longer exist in the render target texture, a pixel with X,Y coordinate will be interpolated the same way if it's depth is 0.5 or 1. <br/>
To take the perspective into account, we must linearly interpolate the value with a 1.0f/z factor.  

### 3 Pixel shading

The pixel shading will write the calculated color of every pixel to the render target texture. Color inputs are provided by the material of the rendered object.

### 3.1 Texture mapping

Texture mapping is done by assigning uv coordinates to vertices. So by using the perspective interpolation, we get UV coordinates per pixels and request back the texture.

> No filtering is applied.


# Resources

[polygon rasterization](https://1drv.ms/b/s!AsTOe2fn2yZvhWEU_lpIeQxNa7I5?e=vXK9aV)

[perspective-correct interpolation](https://1drv.ms/b/s!AsTOe2fn2yZvhVOD173Leo14ojcf?e=th27GE)