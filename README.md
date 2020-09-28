# GameEngine

Supported platform : Windows only.

![](https://i.imgur.com/SPNdNuE.gif)
![](https://i.imgur.com/8LRG2xY.gif)

## External dependencies

* <b>STB_IMAGE</b> : For loading textures.

## Milestone 0.0.4 features

This milestone added the following features :

### Software Renderer
1/ <b>Texture Mapping</b> : adding support for texture mapping, with perspective corrected interpolation.

2/ <b>ZBuffer</b> : implementing a ZBuffer for visibility problem.

3/ <b>Smart rasterizer</b> : improving the rasterizer algorithm that iterate over the polygon bounding box in a smarter way.

4/ <b>RenderPipeline</b> : adding support for multiple RenderPipeline (unlit_textured - flatlit_textured - flatlit_nottextured).

5/ <b>Window scaling</b> : the renderer now have an internal rendered texture. This texture is then stretched to fit the window size.
