#include "GSMap.h"

#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"
#include "GameButton.h"
#include "MapSquare.h"
#include "MapPointer.h"
#include "Character.h"
#include "SpriteAnimation.h"
#include <Level/LevelZero.h>
#include "MapTile/MapTileHeader.h"

GSMap::GSMap()
	: currentState(0)
{
	m_mapMatrix = new std::shared_ptr<MapSquare>* [Globals::mapWidth];
	for (int i = 0; i < Globals::mapWidth; i++) {
		m_mapMatrix[i] = new std::shared_ptr<MapSquare>[Globals::mapHeight];
	}
}

GSMap::~GSMap()
{
	//for (int i = 0; i < Globals::mapWidth; i++) {
		//delete m_mapMatrix[i];
	//}
	delete m_mapMatrix;
}

void GSMap::Init()
{
	m_isPause = false;
	m_chosenCharacter = nullptr;
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");

	
	//tile init
	auto bridgeVer = ResourceManagers::GetInstance()->GetTexture("tileset/Bridge_Vertical.tga");	//checked
	auto bridgeHor = ResourceManagers::GetInstance()->GetTexture("tileset/Bridge_Horizontal.tga");	//checked
	auto camp = ResourceManagers::GetInstance()->GetTexture("tileset/Camp.tga");		//checked
	auto castle = ResourceManagers::GetInstance()->GetTexture("tileset/Castle.tga");
	auto chess = ResourceManagers::GetInstance()->GetTexture("tileset/Chess.tga");		//checked
	auto emptyChess = ResourceManagers::GetInstance()->GetTexture("tileset/Opened_Chess.tga");
	auto snowField = ResourceManagers::GetInstance()->GetTexture("tileset/SnowField.tga"); //checked
	auto ground = ResourceManagers::GetInstance()->GetTexture("tileset/Ground.tga");		//checked
	auto river = ResourceManagers::GetInstance()->GetTexture("tileset/River.tga");			//checked
	auto frontEdge = ResourceManagers::GetInstance()->GetTexture("tileset/Front_Edge.tga");	//checked
	auto leftEdge = ResourceManagers::GetInstance()->GetTexture("tileset/Left_Edge.tga");	//checked
	auto leftCorner = ResourceManagers::GetInstance()->GetTexture("tileset/Leftcorner_Edge.tga");	//checked
	auto rightEdge = ResourceManagers::GetInstance()->GetTexture("tileset/Right_Edge.tga");	//checked
	auto rightCorner = ResourceManagers::GetInstance()->GetTexture("tileset/Rightcorner_Edge.tga");  //checked
	auto snowTree = ResourceManagers::GetInstance()->GetTexture("tileset/Snow_Tree.tga");	//checked
	auto tree = ResourceManagers::GetInstance()->GetTexture("tileset/Normal_Tree.tga");		//checked

	//Level init
	LevelZero lvl0 = LevelZero::LevelZero(model);


	//map matrix
	switch (Globals::gameLevel) 
	{
	case 0:
		for (int i = 0; i < Globals::mapWidth; i++) {
			for (int j = 0; j < Globals::mapHeight; j++) {
				m_map[j][i] = lvl0.mapping[i][j];
			}
		}
		break;
	default:
		break;
	}
	for (int i = 0; i < Globals::mapWidth; i++) {
		for (int j = 0; j < Globals::mapHeight; j++) {
			switch (m_map[i][j])
			{
			case SNOW_FIELD:
				m_mapMatrix[i][j] = std::make_shared<SnowField>(model, shader, snowField);
				break;
			case GROUND:
				m_mapMatrix[i][j] = std::make_shared<Ground>(model, shader, ground);
				break;
			case RIVER:
				m_mapMatrix[i][j] = std::make_shared<River>(model, shader, river);
				break;
			case SNOW_TREE:
				m_mapMatrix[i][j] = std::make_shared<SnowTree>(model, shader, snowTree);
				break;
			case TREE:
				m_mapMatrix[i][j] = std::make_shared<NormalTree>(model, shader, tree);
				break;
			case FRONT_EDGE:
				m_mapMatrix[i][j] = std::make_shared<Edge>(model, shader, frontEdge);
				break;
			case LEFT_EDGE:
				m_mapMatrix[i][j] = std::make_shared<Edge>(model, shader, leftEdge);
				break;
			case RIGHT_EDGE:
				m_mapMatrix[i][j] = std::make_shared<Edge>(model, shader, rightEdge);
				break;
			case BRIDGE_VER:
				m_mapMatrix[i][j] = std::make_shared<Bridge>(model, shader, bridgeVer);
				break;
			case BRIDGE_HOR:
				m_mapMatrix[i][j] = std::make_shared<Bridge>(model, shader, bridgeHor);
				break;
			case CAMP:
				m_mapMatrix[i][j] = std::make_shared<Camp>(model, shader, camp);
				break;
			case CHESS:
				m_mapMatrix[i][j] = std::make_shared<Chess>(model, shader, chess);
				break;
			case LEFTCRNR:
				m_mapMatrix[i][j] = std::make_shared<Edge>(model, shader, leftCorner);
				break;
			case RIGHTCRNR:
				m_mapMatrix[i][j] = std::make_shared<Edge>(model, shader, rightCorner);
				break;
			case EMPTYCHESS:
				m_mapMatrix[i][j] = std::make_shared<Chess>(model, shader, emptyChess);
				break;
			case CASTLE:
				m_mapMatrix[i][j] = std::make_shared<Castle>(model, shader, castle);
				break;
			default:
				break;
			}
			m_mapMatrix[i][j]->setPosXY(i,j);
			m_mapMatrix[i][j]->SetSize(Globals::squareLength, Globals::squareLength);
		}
	}


	//map pointer
	auto texture = ResourceManagers::GetInstance()->GetTexture("pointersquare.tga");
	m_mapPointer = std::make_shared<MapPointer>(model, shader, texture);
	m_mapPointer->setPosXY(5, 5);
	m_mapPointer->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	m_mapPointer->SetSize(Globals::squareLength, Globals::squareLength);


	//possible move marker
	texture = ResourceManagers::GetInstance()->GetTexture("blueCircle.tga");
	m_moveMarker = std::make_shared<Sprite2D>(model, shader, texture);
	m_moveMarker->SetSize(Globals::squareLength, Globals::squareLength);

	// button list
	// button pause
	texture = ResourceManagers::GetInstance()->GetTexture("btn_pause.tga");
	std::shared_ptr<GameButton>  button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Globals::screenWidth - 45, 50);
	button->SetSize(75, 75);
	button->SetOnClick([this]() {
		//GameStateMachine::GetInstance()->PopState();
		Pause();
		});
	m_listButton.push_back(button);

	// button list in pause mode
	// button continue
	texture = ResourceManagers::GetInstance()->GetTexture("btn_resume.tga");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Globals::screenWidth / 2, 350);
	button->SetSize(200, 100);
	button->SetOnClick([this]() {
		Resume();
		//GameStateMachine::GetInstance()->PopState();
		});
	m_listButtonOnPause.push_back(button);


	// button return to main menu
	texture = ResourceManagers::GetInstance()->GetTexture("btn_quit.tga");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Globals::screenWidth / 2, 450);
	button->SetSize(200, 100);
	button->SetOnClick([this]() {
		GameStateMachine::GetInstance()->PopState();
		});
	m_listButtonOnPause.push_back(button);


	//character
	auto character = std::make_shared<Character>(model);
	character->setPosXY(5, 5);
	int x = 5, y = 5;
	
	character->getFieldAnimation()->SetSize(Globals::squareLength * 2, Globals::squareLength * 2);
	m_mapMatrix[x][y]->setCharacter(character);
	character->getFieldAnimation()->Set2DPosition(m_mapMatrix[x][y]->GetPosition().x, m_mapMatrix[x][y]->GetPosition().y);
	m_listCharacter.push_back(character);

	auto character2 = std::make_shared<Character>(model);
	character2->setPosXY(6, 19);
	x = 6;  y = 19;

	character2->getFieldAnimation()->SetSize(Globals::squareLength * 2, Globals::squareLength * 2);
	m_mapMatrix[x][y]->setCharacter(character2);
	character2->getFieldAnimation()->Set2DPosition(m_mapMatrix[x][y]->GetPosition().x, m_mapMatrix[x][y]->GetPosition().y);
	m_listCharacter.push_back(character2);

	//debug
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("Ramaraja-Regular.ttf");
	m_debug = std::make_shared<Text>(shader, font, "", TextColor::RED, 1.0);
	m_debug->Set2DPosition(Vector2(5, 25));
	
	m_numMovement = std::make_shared<Text>(shader, font, "", TextColor::RED, 1.0);
	m_numMovement->Set2DPosition(Globals::screenWidth - 110, Globals::screenHeight - 10);
	//Evasion info
	m_infoTile = std::make_shared<Text>(shader, font, "", TextColor::RED, 1.0);
	m_infoTile->Set2DPosition(Vector2(10, Globals::screenHeight - 10));
}

void GSMap::Exit()
{
}

void GSMap::Pause()
{
	m_isPause = std::make_shared<BOOLEAN>(TRUE);
}

void GSMap::Resume()
{
	if (m_isPause.get()) {
		m_isPause = false;
		//std::wcout << "Yes";
	}
}

void GSMap::HandleEvents()
{
}

void GSMap::HandleKeyEvents(int key, bool bIsPressed)
{
	if (bIsPressed) {
		int xtemp = m_mapPointer->getPosX(), ytemp = m_mapPointer->getPosY();
		switch (key)
		{
		case KEY_MOVE_LEFT:
		case KEY_LEFT:
			//pointer transition
			if (m_mapPointer->getPosX() == 0) break;
			m_mapPointer->setPosXY(m_mapPointer->getPosX() - 1, m_mapPointer->getPosY());

			//map transition
			if (m_mapMatrix[0][0]->GetPosition().x != Globals::squareLength / 2 && m_mapPointer->canMoveLeft()) {
				for (int i = 0; i < Globals::mapWidth; i++) {
					for (int j = 0; j < Globals::mapHeight; j++) {
						auto temp = m_mapMatrix[i][j];
						temp->setDrawPos(1, 0);
					}
				}
			}
			//m_mapPointer->Set2DPosition(m_mapMatrix[xtemp][ytemp]->GetPosition().x, m_mapMatrix[xtemp][ytemp]->GetPosition().y);

			break;
		case KEY_MOVE_BACKWORD:
		case KEY_DOWN:
			//pointer transition
			if (m_mapPointer->getPosY() == Globals::mapHeight-1) break;
			m_mapPointer->setPosXY(m_mapPointer->getPosX(), m_mapPointer->getPosY()+1);

			//map transition
			if (m_mapMatrix[19][Globals::mapHeight - 1]->GetPosition().y != Globals::screenHeight - Globals::squareLength/2 && m_mapPointer->canMoveDown())
				for (int i = 0; i < Globals::mapWidth; i++) {
					for (int j = 0; j < Globals::mapHeight; j++) {
						auto temp = m_mapMatrix[i][j];
						temp->setDrawPos(0, - 1);
					}
				}
			//m_mapPointer->Set2DPosition(m_mapMatrix[xtemp][ytemp]->GetPosition().x, m_mapMatrix[xtemp][ytemp]->GetPosition().y);
			break;
		case KEY_MOVE_RIGHT:
		case KEY_RIGHT:
			//pointer transition
			if (m_mapPointer->getPosX() == Globals::mapWidth-1) break;
			m_mapPointer->setPosXY(m_mapPointer->getPosX() + 1, m_mapPointer->getPosY());

			//map transition
			if (m_mapMatrix[19][19]->GetPosition().x != Globals::screenWidth - Globals::squareLength/2 && m_mapPointer->canMoveRight())
				for (int i = 0; i < Globals::mapWidth; i++) {
					for (int j = 0; j < Globals::mapHeight; j++) {
						auto temp = m_mapMatrix[i][j];
						temp->setDrawPos(- 1, 0);
					}
				}
			//m_mapPointer->Set2DPosition(m_mapMatrix[xtemp][ytemp]->GetPosition().x, m_mapMatrix[xtemp][ytemp]->GetPosition().y);
			break;
		case KEY_MOVE_FORWORD:
		case KEY_UP:
			//pointer transition
			if (m_mapPointer->getPosY() == 0) break;
			m_mapPointer->setPosXY(m_mapPointer->getPosX(), m_mapPointer->getPosY() - 1);

			//map transition
			if (m_mapMatrix[0][0]->GetPosition().y != Globals::squareLength/2 && m_mapPointer->canMoveUp())
				for (int i = 0; i < Globals::mapWidth; i++) {
					for (int j = 0; j < Globals::mapHeight; j++) {
						auto temp = m_mapMatrix[i][j];
						temp->setDrawPos(0, 1);
					}
				}
			//m_mapPointer->Set2DPosition(m_mapMatrix[xtemp][ytemp]->GetPosition().x, m_mapMatrix[xtemp][ytemp]->GetPosition().y);
			break;

		case KEY_ENTER:
			switch (currentState)
			{
			case 0:
				if (m_mapMatrix[xtemp][ytemp]->getCharacter() == nullptr) {
					break;
				}
				else {
					m_chosenCharacter = m_mapMatrix[xtemp][ytemp]->getCharacter();
					if (m_chosenCharacter->getDisableButton()) break;
					m_chosenCharacter->calculateMovementMap(m_mapMatrix);
					currentState = 1;
				}
				break;

			case 1:
				//printf("%d", m_chosenCharacter->getMovementMap()[xtemp][ytemp].mark);
				if (m_chosenCharacter->getMovementMap()[xtemp][ytemp].mark) {
					//currentState = 2;
					currentState = 0;
					m_mapMatrix[m_chosenCharacter->getPosX()][m_chosenCharacter->getPosY()]->setCharacter(nullptr);
					m_mapMatrix[xtemp][ytemp]->setCharacter(m_chosenCharacter);
					m_chosenCharacter->move(xtemp, ytemp);
				}
				break;

			case 2:
				//m_mapMatrix[xtemp][ytemp]->getCharacter()->setFinishTurn(true);
				if (checkEndTurn()) currentState = 3;
				else currentState = 0;
				break;
			}

			
		default:
			break;
		}
	}
}

void GSMap::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto button : m_listButton)
	{
		if (button->HandleTouchEvents(x, y, bIsPressed))
		{
			break;
		}
	}
	if (m_isPause.get())
		for (auto button : m_listButtonOnPause)
		{
			if (button->HandleTouchEvents(x, y, bIsPressed))
			{
			break;
			}
		}

}

void GSMap::HandleMouseMoveEvents(int x, int y)
{
}


void GSMap::Update(float deltaTime)
{
	if (m_isPause.get()) {
		for (auto it : m_listButtonOnPause)
		{
			it->Update(deltaTime);
		}
		return;
	}
	
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
	int xtemp = m_mapPointer->getPosX(), ytemp = m_mapPointer->getPosY();
	m_mapPointer->Set2DPosition(m_mapMatrix[xtemp][ytemp]->GetPosition().x, m_mapMatrix[xtemp][ytemp]->GetPosition().y);

	//text
	m_debug->SetText(std::to_string(xtemp) + " - " + std::to_string(ytemp));
	m_infoTile->SetText("Evasion: " + std::to_string(m_mapMatrix[xtemp][ytemp]->getEvasion()));
	if (m_mapMatrix[xtemp][ytemp]->getCharacter() == nullptr) {
		m_numMovement->SetText("");
	}
	else m_numMovement->SetText("Move: " + std::to_string(m_mapMatrix[xtemp][ytemp]->getCharacter()->getMove()));

	for (auto it : m_listAnimation)
	{
		it->Update(deltaTime);
		
	}

	for (auto it : m_listCharacter)
	{
		it->Update(deltaTime);
		xtemp = it->getPosX(); ytemp = it->getPosY();
		it->getFieldAnimation()->Set2DPosition(m_mapMatrix[xtemp][ytemp]->GetPosition().x, m_mapMatrix[xtemp][ytemp]->GetPosition().y);
		it->getFieldAnimation()->Update(deltaTime);
	}
}

void GSMap::Draw()
{
	
	for (int i = 0; i < Globals::mapWidth; i++) {
		for (int j = 0; j < Globals::mapHeight; j++) {
			m_mapMatrix[i][j]->Draw();
		}
	}
	if (currentState == 1) {
		MoveList** temp = m_chosenCharacter->getMovementMap();
		for (int i = 0; i < Globals::mapWidth; i++) {
			for (int j = 0; j < Globals::mapHeight; j++) {
				if (temp[i][j].mark == true) {
					m_moveMarker->Set2DPosition(m_mapMatrix[i][j]->GetPosition().x, m_mapMatrix[i][j]->GetPosition().y);
					m_moveMarker->Draw();
				}
			}
		}
	}
	m_mapPointer->Draw();
	
	for (auto it : m_listAnimation)
	{
		it->Draw();
	}
	m_debug->Draw();
	m_numMovement->Draw();
	m_infoTile->Draw();
	for (auto it : m_listButton)
	{
		it->Draw();
	}
	for (auto it : m_listCharacter)
	{
		it->getFieldAnimation()->Draw();
	}

	if (m_isPause.get()) {
		for (auto it : m_listButtonOnPause)
		{
			it->Draw();
		}
	}
	
}

bool GSMap::checkEndTurn()
{
	for (auto it : m_listCharacter) {
		if (it->getFinishTurn() == false)
			return false;
	}
	return true;
}

