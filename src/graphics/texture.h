#pragma once


struct Texture
{
	int32_t Width = 0;
	int32_t Height = 0;
	int32_t Channels = 0;
	uint32_t Index = 0;
};


Texture CreateTexture(const char* filepath);
