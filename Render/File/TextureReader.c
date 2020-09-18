#include "TextureReader.h"

#include "DataStructures/STRING.h"
#include "Read/File/File.h"
#include "Objects/Texture/Texture.h"
#include "Texture/TextureAsset_def.h"

void TextureReader_load(const char* p_textureAbsolutePath, Texture3c_PTR out_texture)
{
	String l_file;
	File_readFile_byte(p_textureAbsolutePath, &l_file);	
	{
		TextureAssetHeader* l_textureAssetHeader = (TextureAssetHeader*)l_file.Memory;
		Texture_Alloc_3C(out_texture, l_textureAssetHeader->ImageWidth, l_textureAssetHeader->ImageHeight);
		memcpy(out_texture->Pixels.Memory, (char*)l_file.Memory + sizeof(TextureAssetHeader), l_textureAssetHeader->ImageSizeInByte);
	}
	String_Free(&l_file);
#if 0
	if (l_compressedImage)
	{
		// fread(l_compressedImage, sizeof(char), l_textureAssetHeader->ImageCompressedSizeInByte, l_fs.Stream);
		if (uncompress(out_texture->Pixels.Memory, &l_textureAssetHeader->ImageSizeInByte, l_compressedImage, l_textureAssetHeader->ImageCompressedSizeInByte) < 0)
		{
			printf("Error, failed to uncompress texture.");
			abort();
		};
	}
#endif
};