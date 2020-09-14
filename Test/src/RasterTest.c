#include "Renderer/Draw/DrawFunctions.h"
#include "Raster/Rasterizer.h"
#include "v2/_interface/VectorStructuresC.h"

int main()
{
	Polygon2i l_polygon =
	{
		.v1 = {0,0},
		.v2 = {3, 1},
		.v3 = {1, 3}
	};
	Recti l_rect = { .Min = {0,0}, .Max = {100,100} };
	Array_Vector2i l_pixels;
	Arr_Alloc_Vector2i(&l_pixels, 0);

	Rasterize_PolygonClipped(&l_polygon, &l_pixels, &l_rect);
	//Draw_PolygonClipped(&l_polygon, );

	return 0;
}