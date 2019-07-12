#include "Texture.h"



void Texture::setTexture(unsigned int id)
{
	texturesID.push_back(id);
	count++;
}

unsigned int Texture::getTextureID(int position)
{
	return texturesID[position];
}

int Texture::getNumberTexture()
{
	return count;
}

Texture::Texture()
{
	count = 0;
}


Texture::~Texture()
{
}
