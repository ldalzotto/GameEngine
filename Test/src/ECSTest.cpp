
extern "C"
{
#include "ECS.h"
}

ECS ecs;

size_t ComponentTestType = 1262;

typedef struct ComponentTest_TYP
{
	ECS_ComponentHeader Header;

}ComponentTest, * ComponentTest_Handle;

ComponentTest_Handle ComponentTest_Alloc() { return (ComponentTest_Handle)ECS_Component_Alloc(&ComponentTestType, sizeof(ComponentTest)); };

int main()
{
	ECS_Build(&ecs, NULL);
	ECS_Entity_HANDLE l_entity = ECS_AllocateEntity(&ecs);

	ComponentTest_Handle l_compTest = ComponentTest_Alloc();
	ECS_AddComponent(&ecs, l_entity, &l_compTest->Header);
	// ECS_AddComponent(&ecs, l_entity, &l_compTest->Header);
	//	ECS_Component_Alloc(&ComponentTestType, sizeof(ComponentTest));

	ECS_FreeEntity(&ecs, l_entity);
	ECS_Free(&ecs);
}