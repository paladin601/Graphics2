#pragma once
#include <vector>
using std::vector;
using namespace std;

class Texture
{
private:
	vector<unsigned int> texturesID;
	int count;
public:
	void setTexture(unsigned int);
	unsigned int getTextureID(int);
	int getNumberTexture();
	Texture();
	~Texture();
};

