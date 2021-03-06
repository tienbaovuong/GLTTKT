#include "GSBattle.h"
#include "Character.h"
#include "SpriteAnimation.h"
#include "Font.h"
#include <ctime>
GSBattle::GSBattle()
{
}

GSBattle::~GSBattle()
{
}

void GSBattle::Init()
{
	//so many text aaaaaaaaaaaaaaaaaaaaaaa
	std::srand(std::time(0));
	currentState = 0;
	m_time = 0;
	expGain = 0;
	battler1 = AssetManager::GetInstance()->battler1;
	battler2 = AssetManager::GetInstance()->battler2;
	eva1 = AssetManager::GetInstance()->eva1;
	def1 = AssetManager::GetInstance()->def1;
	res1 = AssetManager::GetInstance()->res1;
	eva2 = AssetManager::GetInstance()->eva2;
	def2 = AssetManager::GetInstance()->def2;
	res2 = AssetManager::GetInstance()->res2;
	isAssist = isAssisting();
	cantFightBack = (battler2->getEquipment()->getType() == "staff");

	slashAni = std::make_shared<SpriteAnimation>(AssetManager::GetInstance()->model2D, AssetManager::GetInstance()->shaderAnimation, AssetManager::GetInstance()->slashAnimation, 5, 1, 0, 0.2);
	slashAni->SetSize(2*Globals::screenHeight / 3, 2*Globals::screenHeight / 3);
	slashAni->Set2DPosition(Globals::screenWidth - Globals::screenHeight / 6.0, Globals::screenHeight / 6.0);
	healAni = std::make_shared<SpriteAnimation>(AssetManager::GetInstance()->model2D, AssetManager::GetInstance()->shaderAnimation, AssetManager::GetInstance()->healAnimation, 5, 1, 0, 0.2);
	healAni->SetSize(2*Globals::screenHeight / 3, 2*Globals::screenHeight / 3);
	healAni->Set2DPosition(Globals::screenWidth - Globals::screenHeight / 6.0, Globals::screenHeight / 6.0);

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

	if(!cantFightBack) {
		text = std::make_shared<Text>(textShader, font, "Damage: " + std::to_string(dmg2), TextColor::WHITE, 2.0);
		text->Set2DPosition(Globals::screenWidth / 2 + 30, Globals::screenHeight / 3.0 + 100);
		fixedText.push_back(text);

		text = std::make_shared<Text>(textShader, font, "Hit:    " + std::to_string(hit2), TextColor::WHITE, 2.0);
		text->Set2DPosition(Globals::screenWidth / 2 + 30, Globals::screenHeight / 3.0 + 200);
		fixedText.push_back(text);

		text = std::make_shared<Text>(textShader, font, "Crit:   " + std::to_string(crit2), TextColor::WHITE, 2.0);
		text->Set2DPosition(Globals::screenWidth / 2 + 30, Globals::screenHeight / 3.0 + 300);
		fixedText.push_back(text);
	}
	
	lvl1 = std::make_shared<Text>(textShader, font, "", TextColor::WHITE, 2.0);
	lvl1->Set2DPosition(30, Globals::screenHeight / 3.0 + 450);
	exp1 = std::make_shared<Text>(textShader, font, "", TextColor::WHITE, 2.0);
	exp1->Set2DPosition(230, Globals::screenHeight / 3.0 + 450);

	lvl2 = std::make_shared<Text>(textShader, font, "Lvl: " + std::to_string(battler2->getLevel()), TextColor::WHITE, 2.0);
	lvl2->Set2DPosition(Globals::screenWidth / 2 + 30, Globals::screenHeight / 3.0 + 450);
	exp2 = std::make_shared<Text>(textShader, font, "Exp: " + std::to_string(battler2->getExp()), TextColor::WHITE, 2.0);
	exp2->Set2DPosition(Globals::screenWidth / 2 + 230, Globals::screenHeight / 3.0 + 450);

	prediction = std::make_shared<Text>(textShader, font, "Prediction", TextColor::WHITE, 2.0);
	prediction->Set2DPosition(Globals::screenWidth / 2 - 100, 30);

	enterToBegin = std::make_shared<Text>(textShader, font, "Enter to start", TextColor::WHITE, 1.0);
	enterToBegin->Set2DPosition(Globals::screenWidth / 2 - 60, Globals::screenHeight / 3 - 30);
	
	if (isAssist) {
		assistText = std::make_shared<Text>(textShader, font, battler1->getEquipment()->getDescription(), TextColor::WHITE, 2.0);
		assistText->Set2DPosition(30, Globals::screenHeight / 3.0 + 200);
	}

	hp1 = std::make_shared<Text>(textShader, font, "", TextColor::WHITE, 2.0);
	hp1->Set2DPosition(Globals::screenHeight / 3 + 60, 100);
	text = std::make_shared<Text>(textShader, font, "/", TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenHeight / 3 + 60, 150);
	fixedText2.push_back(text);
	text = std::make_shared<Text>(textShader, font, std::to_string(battler1->getMaxHealth()), TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenHeight / 3 + 60, 200);
	fixedText2.push_back(text);
	text = std::make_shared<Text>(textShader, font, battler1->getCharName(), TextColor::WHITE, 2.0);
	text->Set2DPosition(30, Globals::screenHeight / 3.0 + 400);
	fixedText2.push_back(text);
	

	hp2 = std::make_shared<Text>(textShader, font, "", TextColor::WHITE, 2.0);
	hp2->Set2DPosition(Globals::screenWidth - Globals::screenHeight / 6.0 - 200, 100);
	text = std::make_shared<Text>(textShader, font, "/", TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenWidth - Globals::screenHeight / 6.0 - 200, 150);
	fixedText2.push_back(text);
	text = std::make_shared<Text>(textShader, font, std::to_string(battler2->getMaxHealth()), TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenWidth - Globals::screenHeight / 6.0 - 200, 200);
	fixedText2.push_back(text);
	text = std::make_shared<Text>(textShader, font, battler2->getCharName(), TextColor::WHITE, 2.0);
	text->Set2DPosition(Globals::screenWidth / 2 + 30, Globals::screenHeight / 3.0 + 400);
	fixedText2.push_back(text);

	numbers = std::make_shared<Text>(textShader, font, "" , TextColor::RED, 2.0);
	numbers->Set2DPosition(Globals::screenWidth - Globals::screenHeight / 6.0 - 50, Globals::screenHeight / 6.0);

	hitStatus = std::make_shared<Text>(textShader, font, "", TextColor::RED, 2.0);
	hitStatus->Set2DPosition(Globals::screenWidth - Globals::screenHeight / 6.0 - 50, Globals::screenHeight / 6.0 + 50);

	expText = std::make_shared<Text>(textShader, font, "", TextColor::WHITE, 2.0);
	expText->Set2DPosition(Globals::screenWidth / 2 - 100, Globals::screenHeight / 6.0);
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
			if (currentState == 0) {
				if (!isAssist) {
					currentState = 1;
					int random = rand() % 100 + 1;
					if (hit1 < random) {
						numbers->SetText("");
						hitStatus->SetText("Miss");
						if (!battler1->isEnemy()) expGain = 0;
					}
					else {
						random = rand() % 100 + 1;
						if (crit1 >= random) {
							hitStatus->SetText("Crititcal");
							numbers->SetText(std::to_string(dmg1 * 2));
							battler2->setHealthPoint(battler2->getHealthPoint() - dmg1 * 2);
						}
						else {
							hitStatus->SetText("Hit");
							numbers->SetText(std::to_string(dmg1));
							battler2->setHealthPoint(battler2->getHealthPoint() - dmg1);
						}
						if (!battler1->isEnemy()) expGain = 1;
					}
				}
				else {
					currentState = 1;
					if (!battler1->isEnemy()) expGain = 1.5;
					battler1->getEquipment()->effect(battler2);
				}
				auto temp = battler1->getEquipment();
				temp->reduceDurability(1);
				if (temp->getDurability() == 0) {
					battler1->unequip(temp);
					battler1->m_itemList.remove(temp);
				}
			}
			else if (currentState == 4) {
				GameStateMachine::GetInstance()->PopState();
				AssetManager::GetInstance()->battleEnd = true;
			}
			break;
		case KEY_BACK:
			if (currentState == 0 && !battler1->isEnemy()) {
				//AssetManager::GetInstance()->escapeBattle = true;
				GameStateMachine::GetInstance()->PopState();
			}
		default:
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
	exp1->SetText("Exp: "+std::to_string(battler1->getExp()));
	lvl2->SetText("Lvl: " + std::to_string(battler2->getLevel()));
	exp2->SetText("Exp: " + std::to_string(battler2->getExp()));

	switch (currentState) {
	case 1:
		m_time += deltaTime;
		if (isAssist) healAni->Update(deltaTime);
		else slashAni->Update(deltaTime);

		if (m_time > 1.0) {
			m_time = 0;
			if (!battler2->getAlive() || isAssist || cantFightBack) {
				currentState = 3;
				giveExp();
			}
			else {
				currentState = 2;
				slashAni->Set2DPosition(Globals::screenHeight / 6.0, Globals::screenHeight / 6.0);
				numbers->Set2DPosition(Globals::screenHeight / 6.0 - 50, Globals::screenHeight / 6.0);
				hitStatus->Set2DPosition(Globals::screenHeight / 6.0 - 50, Globals::screenHeight / 6.0 + 50);
				int random = rand() % 100 + 1;
				if (hit2 < random) {
					numbers->SetText("");
					hitStatus->SetText("Miss");
					if (!battler2->isEnemy()) expGain = 0;
				}
				else {
					random = rand() % 100 + 1;
					//printf("%d - %d", crit2, random);
					if (crit2 >= random) {
						hitStatus->SetText("Crititcal");
						numbers->SetText(std::to_string(dmg2 * 2));
						battler1->setHealthPoint(battler1->getHealthPoint() - dmg2 * 2);
					}
					else {
						hitStatus->SetText("Hit");
						numbers->SetText(std::to_string(dmg2));
						battler1->setHealthPoint(battler1->getHealthPoint() - dmg2);
					}
					if (!battler2->isEnemy()) expGain = 1;
				}
				auto temp = battler2->getEquipment();
				temp->reduceDurability(1);
				if (temp->getDurability() == 0) {
					battler2->unequip(temp);
					battler2->m_itemList.remove(temp);
				}
			}
		}
		break;
	case 2:
		m_time += deltaTime;
		slashAni->Update(deltaTime);

		if (m_time > 1.0) {
			m_time = 0;
			currentState = 3;
			giveExp();
		}
		break;

	case 3:
		m_time += deltaTime;
		if (m_time > 0.6) {
			m_time = 0;
			currentState = 4;
		}
	default:
		break;
	}
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

	if (isAssist) {
		assistText->Draw();
	}
	else {
		for (auto it : fixedText) {
			it->Draw();
		}
	}
	for (auto it : fixedText2) {
		it->Draw();
	}

	if (currentState == 0) {
		prediction->Draw();
		enterToBegin->Draw();
	}
	else if (currentState == 1 || currentState == 2) {
		if (isAssist) healAni->Draw();
		else {
			slashAni->Draw();
			numbers->Draw();
			hitStatus->Draw();
		}
	}
	else if (currentState == 3) {
		expText->Draw();
	}
}

void GSBattle::Prediction()
{
	if (isAssist) {
		return;
	}
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

bool GSBattle::isAssisting()
{
	if (battler1->getEquipment()->getType() == "staff") {
		return true;
	}
	else return false;
}

void GSBattle::giveExp()
{
	int base;
	int result;
	if (!battler1->isEnemy()) {
		int lvlRange = battler2->getLevel() - battler1->getLevel();
		if (lvlRange <= -2) base = 10 + (lvlRange + 2) * 3;
		else if (lvlRange <= 1) base = 10;
		else base = 10 + (lvlRange - 1) * 3;

		if (base <= 1) base = 1;
		if (base >= 30) base = 30;
		if (!battler2->getAlive()) expGain = 3;
		result = expGain * base;
		if (result == 0) result = 1;
		if (!battler1->getAlive()) result = 0;
		battler1->gainExp(result);
	}
	else {
		int lvlRange = battler1->getLevel() - battler2->getLevel();
		if (lvlRange <= -2) base = 10 + (lvlRange + 2) * 3;
		else if (lvlRange <= 1) base = 10;
		else base = 10 + (lvlRange - 1) * 3;

		if (base <= 1) base = 1;
		if (base >= 30) base = 30;
		if (!battler1->getAlive()) expGain = 3;
		result = expGain * base;
		if (result == 0) result = 1;
		if (!battler2->getAlive()) result = 0;
		battler2->gainExp(result);
	}
	if (result != 0) {
		expText->SetText("+ " + std::to_string(result) + "exp");
	}
	else {
		expText->SetText("No exp");
	}
}
