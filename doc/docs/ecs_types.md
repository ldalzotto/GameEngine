This page describe all component and system types.

# Transform component

The transform component defines a 3D position of the entity.

When updating an entity transform, what we change is the local transform. The local to world matrix is recalculated on the fly, only when needed. The local to world matrix value is cached until the transform change again.

Because transforms have a parent/child relationship, the parent movement affect the child position. When a transform parent position is set, this change is propagated recursively into the children. This propagation is done instantaneously.

When a transform has changed, the internal state of the transform indicates that the node has changed. This state can be consumed to check if a node has moved or not.

# MeshRenderer component

The MeshRenderer component holds references to a render material object and mesh resource. Material and mesh are allocated instantaneously on demand.<br/>
MeshRenderer also computes the bounding box the the allocated mesh.

# Mesh draw system

The mesh draw system updates render model matrices when an entity has moved.

**ECS event consumption:**

The system allocate an entity filter that listen to the presence of Transform and MeshRenderer components. When an entity met the criteria, the system allocates a rendered object from the render system and link it to the mesh renderer component. 
The rendered object is deallocated when criteria are no more met.

# Camera component

The Camera component holds projection and view matrices.

# Camera render system

The camera render system updates view and projection matrices when an entity has moved. 

**ECS event consumption:**

The system allocate an entity filter that listen to the presence of Transform and Camera components.

# PhysicsBody component

The PhysicsBody component holds a reference to :

* The BoxCollider object of the physics system.
* The transform component to get entity position. 
* The mesh renderer bounding box to get the shape geometry. 

# Physics system

**ECS event consumption:**
The system allocate an entity filter that listen to the presence of Transform, PhysicsBody and MeshRenderer components. When an entity met the criteria, the system allocates a box collider object and push it to the physics world.