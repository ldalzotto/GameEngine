
# The philosophy of the engine

The execution flow of the engine can be seen as a succession of shader pass execution. <br/>
For example, let's say that the engine needs to allocate some render objects. What the engine does is reading a buffer where is stored all events of allocation and execute them before continuing execution. <br/>
The whole engine comes together by chaining multiple execution passes in the right order to draw a frame.

# Execution graph

Here is the engine chain of execution represented as a graph:

<svg-inline src="architecture_overview_execution.svg"></svg-inline>

## Main loop

## ECS

An Engine instance is associated to a unique Scene. The Scene is a hierarchical collection of Nodes located in 3D space. Every node can have multiple components attached to them. These components act as a link between the user and the internal systems. <br/>
See ([scene](scene.md)).

<svg-inline src="architecture_overview_scene_tree.svg"></svg-inline>

## Scene middleware

Scene middlewares are where node component resources are stored. They act as the communication layer between the scene and the internal systems.

Because nodes can move in 3D space. The state of the Node is updated when it's position, rotation or scale has changed. <br/>
The Scene Middlewares are the consumers of this state. When a Node has moved, they send events to internal systems to take this change into account. <br/>
See ([scene middleware](scene_middleware.md)).

# Interaction with user

If we want to make a node display a 3D mesh, the user must :

1. Allocate a Node and set its 3D coordinates.
2. Allocate a mesh renderer component resource from the render middleware.
3. Add a component to the Node and link it with the mesh renderer token.
