
The ECS engine is a functional object that stores entities and their associated components.

# Architecture

<svg-inline src="ecs_architecture.svg"></svg-inline>

# Entity

An Entity by itself doesn't do anything, it is a bag of components. To be fully defined and interact with the world, it must have components attached to it.

# Component

In the point of view of the ECS engine, components are a type with a pointer to an externally allocated resource. <br/>
When we allocate a component, the external ressource has already been allocated, we provide a pointer to it so that can be retrieved back in the system.

# Global events

All operations on the ECS engine or components are not executed immediately, instead, they are pushed to the ECSGlobalEvent stack. These events must be consumed before any system are executed. <br/>
The events are :

**Entity allocation/release:**

Entities are pushed or removed from the to the entity container once the event is consumed. <br/>
On entity release, the entity components are scanned and the registered entity filter are notified.  

**Component added/removed:**

Upon consumption of component events, registered entity filters are notified if type is matching.

# Entity filter

The Entity engine doesn't hold any logic based on components. Instead, the ECS engine allows it's consumer to filter entities by their attached component types. These components events are triggered when a specified component type is added or removed to an entity. These views are called entity filters. <br/>
For example, an entity filter is set to listen to event of component types A and B. Once a node is allocated and a component of type A is attached to it, the entity filter doesn't push the entity because it needs to have component A and B. It is when the component of type B is attached to it that the entity is pushed to the filter. The same logic is applied when a component is detached.

# System

Systems are the interface between the ECS and the internal systems. <br/>
A system usually holds one or more entity filters. By consuming filter events, the system can build arrays of entity that are ensured to have the required component to work with.
