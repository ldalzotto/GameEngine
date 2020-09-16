#include "AssetPaths.h"
#include <string.h>
#include <stdio.h>
#include "DataStructures/STRING.h"

Assetpath ASSET_FOLDER_ROOT = { "" };

void AssetPath_Initialize(const char* ExecutablePath)
{
#if ASSETPATH_DEV
	strcat_s(ASSET_FOLDER_ROOT.Path, MAX_FILEPATH_LENGTH, EXECUTABLEPATH_DEV);
#else
	StringSLICE l_executablePathSlice = (StringSLICE){ .Memory = ExecutablePath, .Begin = 0, .End = strlen(ExecutablePath) + 1 };
	StringSLICE l_antislashSlice = (StringSLICE){ .Memory = "\\", .Begin = 0, .End = 1 };
	size_t tmp_index;
	while (String_Find(&l_executablePathSlice, &l_antislashSlice, &tmp_index))
	{
		tmp_index += 1;
		l_executablePathSlice.Begin = tmp_index;
	}
	// ASSET_FOLDER_ROOT = { 1 };
	memset(ASSET_FOLDER_ROOT.Path, ' ', MAX_FILEPATH_LENGTH);
	ASSET_FOLDER_ROOT.Path[MAX_FILEPATH_LENGTH - 1] = (char)NULL;

	String l_assetFolderRootString = String_FromRaw(ASSET_FOLDER_ROOT.Path);
	String_ClearNoRealloc(&l_assetFolderRootString);
	l_executablePathSlice.End = l_executablePathSlice.Begin;
	l_executablePathSlice.Begin = 0;
	String_InsertSliceNoRealloc(&l_assetFolderRootString, &l_executablePathSlice);
#endif

	strcat_s(ASSET_FOLDER_ROOT.Path, MAX_FILEPATH_LENGTH, "Assets/");
	ASSET_FOLDER_ROOT.Path[MAX_FILEPATH_LENGTH - 1] = (char)NULL;
};

void AssetPath_GetAbsolutePath(const char* p_localPath, Assetpath_PTR out_absolutepath)
{
	*out_absolutepath = ASSET_FOLDER_ROOT;
	strcat_s(out_absolutepath->Path, MAX_FILEPATH_LENGTH, p_localPath);
};