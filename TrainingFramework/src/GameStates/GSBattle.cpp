#include "GSBattle.h"
#include "Character.h"
#include "Font.h"
GSBattle::GSBattle()
{
}

GSBattle::~GSBattle()
{
}

void GSBattle::Init()
{
	currentState = 0;
	battler1 = AssetManager::GetInstance()->battler1;
	battler2 = AssetManager::GetInstance()->battler2;
	eva1 = AssetManager::GetInstance()->eva1;
	def1 = AssetManager::GetInstance()->def1;
	res1 = AssetManager::GetInstance()->res1;
	eva2 = AssetManager::GetInstance()->eva2;
	def2 = AssetManager::GetInstance()->def2;
	res2 = AssetManager::GetInstance()->res2;

	background = std::make_shared<Sprite2D>(AssetManager::GetInstance()->model2D, AssetManager::GetInstance()->shaderTexture, AssetManager::GetInstance()->brownBox);
	box1 = std::make_shared<Sprite2D>(AssetManager::GetInstance()->model2D, AssetManager::GetInstance()->shaderTexture, AssetManager::GetInstance()->brownBox);
	box2 = std::make_shared<Sprite2D>(AssetManager::GetInstance()->model2D, AssetManager::GetInstance()->shaderTexture, AssetManager::GetInstance()->brownBox);

	background->Set2DPosition(Globals::screenWidth / 2.0, Globals::screenHeight / 2.0);
	background->SetSize(Globals::screenWidth + 100, Globals::screenHeight + 100);

	box1->Set2DPosition(Globals::screenWidth / 4.0, 2 * Globals::screenHeight / 3.0);
	box1->SetSize(Globals::screenWidth / 2, 2 * Globals::screenHeight / 3.0);

	box2->Set2DPosition(3 * Globals::screenWidth / 4.0, 2 * Globals::screenHeight / 3.0);
	box2->SetSize(Globals::screenWidth / 2, 2 * Globals::screenHeight / 3.0);

	battler1->Set2DPosition(Globals::screenHeight / 6.0, Globals::screenHeight / 6.0);
	battler1->SetSize(Globals::screenHeight / 3, Globals::screenHeight / 3);

	battler2->getSecondFace()->Set2DPosition(Globals::screenWidth - Globals::screenHeight / 6.0, Globals::screenHeight / 6.0);
	battler2->getSecondFace()->SetSize(Globals::screenHeight / 3, Globals::screenHeight / 3);

	Prediction();
	auto textShader = AssetManager::GetInstance()->shaderText;
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("Ramaraja-Regular.ttf");

	auto text = std::make_shared<Text>(textShader, font, "Damage: " + std::to_string(dmg1), TextColor::WHITE, 2.0);
	text->Set2DPosition(30, Globals::screenHeight / 3.0 + 100);
	fixedText.push_back(text);

	text = std::make_shared<Text>(textShader, font, "Hit:    " + std::to_string(hit1), TextColor::WHITE, 2.0);
	text->Set2DPosition(30, Globals::screenHeight / 3.0 + 200);
	fixedText.push_back(text);

	text = std::make_shared<Text>(textShader, font, "Crit:   " + std::to_string(crit1), TextColor::WHITE, 2.0);
	text->Set2DPosition(30, Globals::screenHeight / 3.0 + 300);
	fixedText.push_back(text);

	text = std::make_shared<Text>(textShader, font, "Damage: " + std::to_string(dmg2), TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenWidth/2 + 30, Globals::screenHeight / 3.0 + 100);
	fixedText.push_back(text);

	text = std::make_shared<Text>(textShader, font, "Hit:    " + std::to_string(hit2), TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenWidth / 2 + 30, Globals::screenHeight / 3.0 + 200);
	fixedText.push_back(text);

	text = std::make_shared<Text>(textShader, font, "Crit:   " + std::to_string(crit2), TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenWidth / 2 + 30, Globals::screenHeight / 3.0 + 300);
	fixedText.push_back(text);
	
	lvl1 = std::make_shared<Text>(textShader, font, "", TextColor::WHITE, 2.0);
	lvl1->Set2DPosition(30, Globals::screenHeight / 3.0 + 400);
	exp1 = std::make_shared<Text>(textShader, font, "", TextColor::WHITE, 2.0);
	exp1->Set2DPosition(230, Globals::screenHeight / 3.0 + 400);

	lvl2 = std::make_shared<Text>(textShader, font, "Lvl: " + std::to_string(battler2->getLevel()), TextColor::WHITE, 2.0);
	lvl2->Set2DPosition(Globals::screenWidth / 2 + 30, Globals::screenHeight / 3.0 + 400);
	exp2 = std::make_shared<Text>(textShader, font, "Exp: " + std::to_string(battler2->getExp()), TextColor::WHITE, 2.0);
	exp2->Set2DPosition(Globals::screenWidth / 2 + 230, Globals::screenHeight / 3.0 + 400);

	prediction = std::make_shared<Text>(textShader, font, "Prediction", TextColor::WHITE, 2.0);
	prediction->Set2DPosition(Globals::screenWidth / 2 - 100, 30);

	enterToBegin = std::make_shared<Text>(textShader, font, "Enter to start", TextColor::WHITE, 1.0);
	enterToBegin->Set2DPosition(Globals::screenWidth / 2 - 60, Globals::screenHeight / 3 - 30);

	hp1 = std::make_shared<Text>(textShader, font, "", TextColor::WHITE, 2.0);
	hp1->Set2DPosition(Globals::screenHeight / 3 + 60, 100);
	text = std::make_shared<Text>(textShader, font, "/", TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenHeight / 3 + 60, 150);
	fixedText.push_back(text);
	text = std::make_shared<Text>(textShader, font, std::to_string(battler1->getMaxHealth()), TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenHeight / 3 + 60, 200);
	fixedText.push_back(text);
	
	hp2 = std::make_shared<Text>(textShader, font, "", TextColor::WHITE, 2.0);
	hp2->Set2DPosition(Globals::screenWidth - Globals::screenHeight / 6.0 - 200, 100);
	text = std::make_shared<Text>(textShader, font, "/", TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenWidth - Globals::screenHeight / 6.0 - 200, 150);
	fixedText.push_back(text);
	text = std::make_shared<Text>(textShader, font, std::to_string(battler2->getMaxHealth()), TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenWidth - Globals::screenHeight / 6.0 - 200, 200);
	fixedText.push_back(text);

}

void GSBattle::Exit()
{
}

void GSBattle::Pause()
{
}

void GSBattle::Resume()
{
}

void GSBattle::HandleEvents()
{
}

void GSBattle::HandleKeyEvents(int key, bool bIsPressed)
{
	if (bIsPressed) {
		switch (key) {
		case KEY_ENTER:
			GameStateMachine::GetInstance()->PopState();
			break;
		}
	}
}

void GSBattle::HandleTouchEvents(int x, int y, bool bIsPressed)
{
}

void GSBattle::HandleMouseMoveEvents(int x, int y)
{
}

void GSBattle::Update(float deltaTime)
{
	hp1->SetText(std::to_string(battler1->getHealthPoint()));
	hp2->SetText(std::to_string(battler2->getHealthPoint()));
	lvl1->SetText("Lvl: "+std::to_string(battler1->getLevel()));
	exp1->SetText("Exp: "+std::to_string(battler2->getExp()));
}

void GSBattle::Draw()
{
	background->Draw();
	box1->Draw();
	box2->Draw();
	battler1->Draw();
	battler2->getSecondFace()->Draw();

	hp1->Draw(); lvl1->Draw(); exp1->Draw();
	hp2->Draw(); lvl2->Draw(); exp2->Draw();
	for (auto it : fixedText) {
		it->Draw();
	}

	if (currentState == 0) {
		prediction->Draw();
		enterToBegin->Draw();
	}
}

void GSBattle::Prediction()
{
	if (battler1->isPhysical()) {
		dmg1 = battler1->getPower() - battler2->getDef() - def2;
	}
	else {
		dmg1 = battler1->getPower() - battler2->getRes() - res2;
	}
	if (battler2->isPhysical()) {
		dmg2 = battler2->getPower() - battler1->getDef() - def1;
	}
	else {
		dmg2 = battler2->getPower() - battler1->getRes() - res1;
	}
	if (dmg1 < 0) dmg1 = 0;
	if (dmg2 < 0) dmg2 = 0;

	hit1 = battler1->getHitRate() - battler2->getEvasion() - eva2;
	hit2 = battler2->getHitRate() - battler1->getEvasion() - eva1;

	crit1 = battler1->getCritRate();
	crit2 = battler2->getCritRate();
}
