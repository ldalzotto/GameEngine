
extern "C"
{
#include "ECS.h"
#include "DataStructures/HASHMAP.h"
}

ECS ecs;

#define ComponentTestType 1262

typedef struct ComponentTest_TYP
{
	ECS_ComponentHeader Header;

}ComponentTest, * ComponentTest_Handle;

ComponentTest_Handle ComponentTest_Alloc() { return (ComponentTest_Handle)ECS_Component_Alloc(ComponentTestType, sizeof(ComponentTest)); };

void FunctionTest(ECS_ComponentHeader_HANDLE p_comp, void*)
{
	switch (p_comp->ComponentType)
	{
	case ComponentTestType:
		break;
	}
}

size_t Hasgggg(size_t* p_key)
{
	return *p_key;
}


int main()
{
	HashMap l_hashMapTest; HashMap_Alloc(&l_hashMapTest, sizeof(size_t), sizeof(int), (HashMap_HashFn)Hasgggg, 0);
	size_t l_key = 8441511610;
	int l_value = 65;
	HashMap_PushKeyValueRealloc(&l_hashMapTest, sizeof(size_t), sizeof(int), (char*)&l_key, (char*)&l_value);
	l_key = 844151118;
	l_value = 66;
	HashMap_PushKeyValueRealloc(&l_hashMapTest, sizeof(size_t), sizeof(int), (char*)&l_key, (char*)&l_value);
	HashMap_PushKeyValueRealloc(&l_hashMapTest, sizeof(size_t), sizeof(int), (char*)&l_key, (char*)&l_value);
	// l_key += 1;
	int* l_val = NULL;
	if (HashMap_GetValue(&l_hashMapTest, sizeof(size_t), sizeof(int), (char*)&l_key, (char**)&l_val))
	{
		
	};


	ECS_Build(&ecs, NULL);

	ECS_OnComponentDestroyedStaticCallback l_callback = { ComponentTestType, FunctionTest , NULL };
	ECS_RegisterGlobalComponentDestroyedEvent(&ecs, &l_callback);

	ECS_Entity_HANDLE l_entity = ECS_AllocateEntity(&ecs);

	ComponentTest_Handle l_compTest = ComponentTest_Alloc();
	ECS_AddComponent(&ecs, l_entity, &l_compTest->Header);
	// ECS_AddComponent(&ecs, l_entity, &l_compTest->Header);
	//	ECS_Component_Alloc(&ComponentTestType, sizeof(ComponentTest));

	ECS_FreeEntity(&ecs, l_entity);
	ECS_Free(&ecs);
}