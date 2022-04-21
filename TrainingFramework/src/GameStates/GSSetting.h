#pragma once
#include "GameStateBase.h"
#include "GameButton.h"

class GSSetting :
    public GameStateBase
{
public:
    GSSetting();
    ~GSSetting();

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
	std::shared_ptr<Sprite2D> background;
	std::shared_ptr<Sprite2D> btnOn;
	std::shared_ptr<Sprite2D> btnOff;
	std::shared_ptr<GameButton> button;
};

