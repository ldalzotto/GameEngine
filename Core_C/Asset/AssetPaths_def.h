#pragma once

#include "Include/PlatformInclude.h"

#ifdef _WIN32
#define MAX_FILEPATH_LENGTH MAX_PATH
#endif

#if ASSETPATH_DEV
#define EXECUTABLEPATH_DEV "E:/GameProjects/GameEngine/"
#endif

typedef struct Assetpath_TYP
{
	char Path[MAX_FILEPATH_LENGTH];
}Assetpath, * Assetpath_PTR;

extern Assetpath ASSET_FOLDER_ROOT;