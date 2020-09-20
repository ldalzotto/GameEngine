#include "TextureReader.h"

#include "stb_image.h"

void TextureReader_load(const char* p_textureAbsolutePath, Texture3c_PTR out_texture)
{
	int comp;
	out_texture->Pixels.Memory = (Color3c*)stbi_load(p_textureAbsolutePath, (int*)&out_texture->Width, (int*)&out_texture->Height, &comp, STBI_rgb);
	out_texture->Pixels.Capacity = (size_t)out_texture->Width * out_texture->Height;
	out_texture->Pixels.Size = out_texture->Pixels.Capacity;
};