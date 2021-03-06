#pragma once
#include "Sprite2D.h"
#include <Character.h>
class Character;

class MapSquare : public Sprite2D
{
public:
	MapSquare() : Sprite2D(), m_posX(0), m_posY(0), m_evasion(0),m_defense(0), m_resistance(0), m_maptype("unknown") , character(nullptr), isPassable(true) {}
	MapSquare(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture);
	~MapSquare();
	//virtual bool isPassable(Character user) = 0;
	GLint getPosX();
	GLint getPosY();
	GLint getEvasion();
	GLint getDefense();
	GLint getResistance();
	std::string getMapType();
	bool getPassable();
	std::shared_ptr<Character> getCharacter();
	void setCharacter(std::shared_ptr<Character> character);
	void setPassable(bool pass);

	void setPosXY(GLint x, GLint y, GLint xOffset, GLint yOffset);
	void setDrawPos(GLint x, GLint y);
	bool isChess;

protected:
	GLint m_posX;
	GLint m_posY;
	GLint m_evasion;
	GLint m_defense;
	GLint m_resistance;
	std::string m_maptype;
	std::shared_ptr<Character> character;
	bool isPassable;
};
