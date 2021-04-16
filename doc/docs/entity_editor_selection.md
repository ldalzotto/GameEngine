
The entity editor selection is a system implemented to assert that the math library is correctly implemented. <br/>
The system allows :

* Select any entity with a mesh renderer component by clicking on it.
* Move, rotate and scale entity with 3D gizmos.

# Entity selection

The entity selection is done by casting a ray from the camera transform position in the direction pointed by the mouse against the physics world. The first intersected physics body bounding box is the entity that will be selected.

# Entity movement

The entity movement system is the most basic form of moving a 3D object in a scene with gizmo implemented by any 3D software. <br/>

The movement algorithm is composed of three parts :

## 1. The guiding plane

A guiding plane is either instantiated or positioned at the selected transform. The guiding plane will be used to project the screen space movement of the mouse in world space.<br/>

## 2. Mouse movement projection

The mouse movement delta between two frames is projected from the screen space to the guiding plane. Projection is done by finding the intersection points between the rays fired to mouse positions from the camera. All further calculations are based on this projection.

## 3. transform movement calculation

**Translation:**

The translation gizmo is represented by three arrows that points to the transform local axis. <br/>
For one selected direction, the guiding plane must contain the line defined by the direction. <br/>
Also, to allow the user to move the object, the plane orientation is facing "as much as possible" the camera. This means that the angle between the guiding plane normal and the camera direction must be minimal. 

The delta mouse position in plane space is projected to the selected axis. Resulting to the movement distance of the transform along the selected direction. 

**Rotation:**

The rotation gizmo is represented by three quarter circle facing the orientation axis. <br/>
The guiding plane is facing the camera. <br/>
The delta mouse position is converted to angle delta by taking the transform as point of origin. <br/>
The angle delta is applied to the rotation.

> This could have been entirely be done in screen space.

**Scale:**

Exact same as the translation, but instead of moving the transform, we scale it.