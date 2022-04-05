#pragma once
#include "GameStateBase.h"

class MapSquare;
class MapPointer;
class Sprite2D;
class Text;
class GameButton;
class SpriteAnimation;

class GSMap :
    public GameStateBase
{
public:
	GSMap();
	~GSMap();

	void	Init() override;
	void	Exit() override;

	void	Pause() override;
	void	Resume() override;

	void	HandleEvents() override;
	void	HandleKeyEvents(int key, bool bIsPressed) override;
	void	HandleTouchEvents(int x, int y, bool bIsPressed) override;
	void	HandleMouseMoveEvents(int x, int y) override;
	void	Update(float deltaTime) override;
	void	Draw() override;
	
private:
	std::shared_ptr<MapSquare>	m_mapMatrix[20][20];
	std::shared_ptr<Text>		m_debug;
	std::shared_ptr<BOOLEAN>    m_isPause;
	std::list<std::shared_ptr<GameButton>>	m_listButton;
	std::list<std::shared_ptr<GameButton>>	m_listButtonOnPause;
	std::list<std::shared_ptr<SpriteAnimation>>	m_listAnimation;
	std::shared_ptr<MapPointer> m_mapPointer;
};
