#include "Texture/TextureAsset_def.h"
#include "DataStructures/STRING.h"
#include "Read/File/File.h"
#include "stb_image.h"
#include "stb_image_write.h"

#define PNGS_ARG_PNGPATH "PNG"
#define PNGS_ARG_DSTPATH "DST"

void PNG_To_Texture3cAsset(const char* p_pngAbsoluteFilePath, Texture3cAsset_PTR out_texture);

int main(int argc, char* argv[])
{
	char* p_pngAbsolutePath = NULL;
	char* p_destinationAbsolutepath = NULL;

#if 0
	for (size_t i = 0; i < argc; i++)
	{
		if (String_CompareRaw(argv[i], PNGS_ARG_PNGPATH, sizeof(PNGS_ARG_PNGPATH) - 1))
		{
			if (i + 1 < argc)
			{
				i += 1;
				p_pngAbsolutePath = argv[i];
			}
		}

		if (String_CompareRaw(argv[i], PNGS_ARG_DSTPATH, sizeof(PNGS_ARG_DSTPATH) - 1))
		{
			if (i + 1 < argc)
			{
				i += 1;
				p_destinationAbsolutepath = argv[i];
			}
		}
	}
#endif

#if 1
	p_pngAbsolutePath = "E:/GameProjects/GameEngine/Assets/Textures/Gradient2.jpg";
	p_destinationAbsolutepath = "E:/GameProjects/GameEngine/Assets/Textures/gradient2.dst";
#endif

	if (p_pngAbsolutePath && p_destinationAbsolutepath)
	{
		Texture3cAsset l_generatedTexture;
		PNG_To_Texture3cAsset(p_pngAbsolutePath, &l_generatedTexture);

		FileStream l_fs = FileStream_open(p_destinationAbsolutepath, FILESTREAM_MODE_WRITE);
		FileStream_WriteBytes(&l_fs, &l_generatedTexture, sizeof(l_generatedTexture.Header));
		// fseek(l_fs.Stream, 0, SEEK_END);
		FileStream_WriteBytes(&l_fs, l_generatedTexture.Pixels, l_generatedTexture.Header.ImageSizeInByte);
		FileStream_close(&l_fs);

		stbi_image_free(l_generatedTexture.Pixels);

	}

	return 0;
}

void PNG_To_Texture3cAsset(const char* p_pngAbsoluteFilePath, Texture3cAsset_PTR out_texture)
{
	int comp;
	// stbi_set_flip_vertically_on_load(1);
	out_texture->Pixels = stbi_load(p_pngAbsoluteFilePath, &out_texture->Header.ImageWidth, &out_texture->Header.ImageHeight, &comp, STBI_rgb);
	out_texture->Header.ImageSizeInByte = ((uint32_t)out_texture->Header.ImageWidth * out_texture->Header.ImageHeight) * sizeof(Color3c);
};