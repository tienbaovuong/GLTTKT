#include "River.h"

River::River(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
	: MapSquare(model, shader, texture)
{
	this->m_evasion = 0;
	this->m_maptype = "River";
	this->isPassable = false;
}

River::~River()
{
}