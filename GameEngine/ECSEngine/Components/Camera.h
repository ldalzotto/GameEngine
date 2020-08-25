#pragma once

#include "Camera_def.h"
#include "ECS.h"
#include "Types_def.h"

void Camera_init(Camera_PTR p_camera, RenderV2Interface* p_renderInterface);
void Camera_OnComponentDetached(Camera_PTR p_camera);
void Camera_buildProjectionMatrix(Camera_PTR p_camera);

Matrix4f Camera_worldToClipMatrix(Camera_PTR p_camera);

Segment_Vector3f Camera_buildWorldSpaceRay(Camera_PTR p_camera, Vector2f_PTR p_screenPoint);

ECS_GETCOMPONENT_FUNCTION(Camera, CAMERA_TYPE, Camera_PTR);