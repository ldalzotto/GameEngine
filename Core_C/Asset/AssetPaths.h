#pragma once

#include "AssetPaths_def.h"
#include "DataStructures/String.h"

void AssetPath_Initialize(const char* ExecutablePath);
void AssetPath_GetAbsolutePath(const char* p_localPath, Assetpath_PTR out_absolutepath);