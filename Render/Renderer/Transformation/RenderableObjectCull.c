#include "RenderableObjectCull.h"

#include "Cull/ObjectCulling.h"
#include "v2/_interface/MatrixC.h"

void RenderableObject_CullObject(RENDEREDOBJECT_BUFFER_PTR p_renderableObjects, RenderHeap_PTR p_renderHeap, CAMERABUFFER_PTR p_cameraBuffer)
{
	for (size_t i = 0; i < p_renderableObjects->RenderedObjects.Size; i++)
	{
		RenderedObject_PTR l_renderableObject = &p_renderHeap->RenderedObjectAllocator.array.Memory[p_renderableObjects->RenderedObjects.Memory[i].Handle];
		Matrix4f l_object_to_camera;
		Mat_Mul_M4F_M4F(&p_cameraBuffer->ViewMatrix, &l_renderableObject->ModelMatrix, &l_object_to_camera);

		l_renderableObject->IsCulled = ObjectCulled_Boxf(l_renderableObject->MeshBoundingBox, (Matrix4f_PTR)&l_renderableObject->ModelMatrix, (Matrix4f_PTR)&l_object_to_camera, &p_cameraBuffer->CameraFrustum);
	}
};