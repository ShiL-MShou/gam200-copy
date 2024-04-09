/************************************************************************************ /
/*
File Name:		Camera.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/

#include "stdafx.h"
#include "Camera.h"
#include "imgui.h"
#include "Graphics.h"
#include "MyGuiManager.h"
#include "CoreEngine.h"
#include "TimeManager.h"
#include "Logic.h"


Camera::Camera()
	:	m_transformPtr{}, screenWidth{ 0 }, screenHeight{ 0 }, limit{ 4559 / 2.0f, 3899 / 2.0f }, followSize{ 833 / 2.0f, 0 }, isMoving{ false }, 
		alignX{ CAMERA_DIRECTION::DEFAULT }, alignY{ CAMERA_DIRECTION::DEFAULT }, currAlignDiff{}, alignDiff{}
{
	screenWidth =	static_cast<float>(GRAPHICS->GetGraphicsWidth());
	screenHeight =	static_cast<float>(GRAPHICS->GetGraphicsHeight());
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	GRAPHICS->AddMainCamera(GetBase()->has(Camera));
	m_transformPtr = GetBase()->has(Transform);
	m_transformPtr->SetStartPosition(m_transformPtr->GetPosition());

	// test if it is working on bigger screens
	followTolorance.x = screenWidth / 3.0f;
	followTolorance.y = screenHeight / 3.0f;

	//MoveTo(VEC2(3400.0f, 400.0f));

	SetAlign(CAMERA_DIRECTION::NEUTRAL);

	FACTORY->CreateArcheType("BlackBarTop");
	barTopPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "BlackBarTop", "bb1", false)->has(SpriteComponent);
	barTopPtr.lock()->SetActive(false);

	FACTORY->CreateArcheType("BlackBarBot");
	barBotPtr = LOGIC->CreateObjectAt(VEC2(0, 0), 0, "BlackBarBot", "bb2", false)->has(SpriteComponent);
	barBotPtr.lock()->SetActive(false);

}

void Camera::MoveBarDown(std::weak_ptr<SpriteComponent> barPtr, float speed)
{
	barPtr.lock()->SetFixedPosition(barPtr.lock()->GetFixedPosition() - VEC2(0.0f, speed));
}

void Camera::MoveBarUp(std::weak_ptr<SpriteComponent> barPtr, float speed)
{
	barPtr.lock()->SetFixedPosition(barPtr.lock()->GetFixedPosition() + VEC2(0.0f, speed));
}

void Camera::BarUpdate()
{
	if (!moveList.empty())
	{
		barTopPtr.lock()->SetActive(true);
		barBotPtr.lock()->SetActive(true);

		if (barTopPtr.lock()->GetFixedPosition().gety() > 680.0f)
		{
			MoveBarDown(barTopPtr, 3.0f);
			MoveBarUp(barBotPtr, 3.0f);
		}
	}
	else if (barTopPtr.lock()->IsActive())
	{
		MoveBarUp(barTopPtr, 3.0f);
		MoveBarDown(barBotPtr, 3.0f);

		if (barTopPtr.lock()->GetFixedPosition().gety() > 950.0f)
		{
			barTopPtr.lock()->SetActive(false);
			barBotPtr.lock()->SetActive(false);
		}
	}
}

void Camera::Update()
{
	if ((CORE->GetEditorStatus() && MyGuiManager::isGamePaused) || CORE->GetPausedStatus())
		return;

	//std::cout << "Camera Pos" << m_transformPtr->GetPosition().x
	//	<< " | " << m_transformPtr->GetPosition().y << std::endl;

	//black bars
	BarUpdate();

	//std::cout << "Alignment diff " << currAlignDiff.x << " | " << currAlignDiff.y << std::endl;

	//std::cout << "Moving List size" << moveList.size() << std::endl;

	//if (m_followPtr && m_transformPtr)
	//{
	//	std::cout << "Start: " << -m_followPtr->GetPosition().x + currAlignDiff.x << " | "
	//		<< -m_followPtr->GetPosition().y + currAlignDiff.y << std::endl;
	//
	//	std::cout << "Start: " << m_transformPtr->GetPosition().x << " | "
	//		<< m_transformPtr->GetPosition().y << std::endl;
	//}

	// transition moving
	if (moveList.empty() == false)
	{
		if (INPUTMGR->GetKeyRelease(MVK_G))
		{
			while (!moveList.empty())
				moveList.pop();
			AddFollowPos(); 
		}
		//std::cout << "move list size : " << moveList.size() << std::endl;
		//std::cout << MyTimeManager.GetDt() << std::endl;`

		bool moved = false;
		// moving left?

		// snap X
		if (movingDiff.x < 0 && m_transformPtr->GetPosition().x < movingToPos.x || 
			movingDiff.x > 0 && m_transformPtr->GetPosition().x > movingToPos.x)
			m_transformPtr->SetPositionX(movingToPos.x);
		else
		{
			// move X
			if (movingToPos.x != m_transformPtr->GetPosition().x)
			{
				moved = true;
				// when the pos is too close that the round fn (at the end) will negate the changes.
				if (movingDiff.x * MyTimeManager.GetDt() < 0.5f && movingDiff.x * MyTimeManager.GetDt() > -0.5f)
					// -ve
					if (movingDiff.x < 0.0f)
						m_transformPtr->SetPositionX(m_transformPtr->GetPosition().x - 0.51f);
					// +ve
					else
						m_transformPtr->SetPositionX(m_transformPtr->GetPosition().x + 0.51f);
				else
					m_transformPtr->SetPositionX(m_transformPtr->GetPosition().x + movingDiff.x * MyTimeManager.GetDt());
			}
		}

		// snap Y
		if (movingDiff.y < 0 && m_transformPtr->GetPosition().y < movingToPos.y ||
			movingDiff.y > 0 && m_transformPtr->GetPosition().y > movingToPos.y)
			m_transformPtr->SetPositionY(movingToPos.y);
		else
		{
			// move Y
			if (movingToPos.y != m_transformPtr->GetPosition().y)
			{
				moved = true;
				// when the pos is too close that the round fn (at the end) will negate the changes.
				if (movingDiff.y * MyTimeManager.GetDt() < 0.5f && movingDiff.y * MyTimeManager.GetDt() > -0.5f)
					// -ve
					if (movingDiff.y < 0.0f)
						m_transformPtr->SetPositionY(m_transformPtr->GetPosition().y - 0.51f);
					// +ve
					else
						m_transformPtr->SetPositionY(m_transformPtr->GetPosition().y + 0.51f);
				else
					m_transformPtr->SetPositionY(m_transformPtr->GetPosition().y + movingDiff.y * MyTimeManager.GetDt());
			}
			
		}

		// reset
		if (moved == false)
		{
			if (timer <= 0.0f)
				moveList.pop();


			if (moveList.empty())
				isMoving = false;
			else
			{
				if (timer <= 0.0f)
				{
					//std::cout << "moving to " << moveList.front().
					MoveTo(moveList.front());
				}
				else
					timer -= MyTimeManager.GetDt();
			}
		}
			

		
		//m_transformPtr->SetPositionY(m_transformPtr->GetPosition().y + movingDiff.y);


		m_transformPtr->SetPosition(round(m_transformPtr->GetPosition().x),
									round(m_transformPtr->GetPosition().y));

		//std::cout << "Curr: " << m_transformPtr->GetPosition().x << " | "
		//						<< m_transformPtr->GetPosition().y << std::endl;
	}
	else

	// check if the camera is suppose to be following something
	if (m_followPtr != nullptr)
	{
		UpdateAlignment();

		//std::cout << "pos1 : " << m_transformPtr->GetPosition().x << "  ||  " << m_transformPtr->GetPosition().y << std::endl;
		//std::cout << "pos follow : " << m_followPtr->GetPosition().x << "  ||  " << m_followPtr->GetPosition().y << std::endl;
		//std::cout << "Alignment diff " << currAlignDiff.x << " | " << currAlignDiff.y << std::endl;
		m_transformPtr->SetPositionX(round (-m_followPtr->GetPosition().x + currAlignDiff.x));

		m_transformPtr->SetPositionY(round(-m_followPtr->GetPosition().y + currAlignDiff.y));

		//std::cout << "pos2 : " << m_transformPtr->GetPosition().x << "  ||  " << m_transformPtr->GetPosition().y << "\n" << std::endl;


		//std::cout << m_transformPtr->GetPosition().x << " | " << m_transformPtr->GetPosition().y  << std::endl;
		
		// follow
		// snap left
		//if (m_transformPtr->GetPosition().x < (-m_followPtr->GetPosition().x + currAlignDiff.x))
		//	m_transformPtr->SetPositionX(-m_followPtr->GetPosition().x + currAlignDiff.x);
		//
		//// snap right
		//else if (m_transformPtr->GetPosition().x > (-m_followPtr->GetPosition().x + screenWidth - currAlignDiff.x))
		//	m_transformPtr->SetPositionX(-m_followPtr->GetPosition().x + screenWidth - currAlignDiff.x);

		//if (m_transformPtr->GetPosition().y < (-m_followPtr->GetPosition().y + currAlignDiff.y))
		//	m_transformPtr->SetPositionY(-m_followPtr->GetPosition().y + currAlignDiff.y);
		//
		//else if (m_transformPtr->GetPosition().y > (-m_followPtr->GetPosition().y + screenHeight - currAlignDiff.y))
		//	m_transformPtr->SetPositionY(-m_followPtr->GetPosition().y + screenHeight - currAlignDiff.y);
		

		//// limit
		//if ( (m_transformPtr->GetPosition().x > limit.x * m_transformPtr->GetScale().x))
		//	m_transformPtr->SetPositionX(limit.x * m_transformPtr->GetScale().x);
		//else if ((m_transformPtr->GetPosition().x < (-limit.x + ((1 / m_transformPtr->GetScale().x) * WinWidth) )* m_transformPtr->GetScale().x))
		//	m_transformPtr->SetPositionX((-limit.x  + ((1 / m_transformPtr->GetScale().x) * WinWidth)  )* m_transformPtr->GetScale().x);

		//if ((m_transformPtr->GetPosition().y > limit.y * m_transformPtr->GetScale().y))
		//	m_transformPtr->SetPositionY(limit.y * m_transformPtr->GetScale().y);
		//else if ((m_transformPtr->GetPosition().y < (-limit.y + ((1 / m_transformPtr->GetScale().y) * WinHeight)) * m_transformPtr->GetScale().y))
		//	m_transformPtr->SetPositionY((-limit.y + ((1 / m_transformPtr->GetScale().y) * WinHeight)) * m_transformPtr->GetScale().y);
	}

	//if (m_followPtr != nullptr)
	//m_transformPtr->SetPosition(m_followPtr->GetPosition());

	//m_transformPtr->SetPosition(round(m_transformPtr->GetPosition().x),
	//							round(m_transformPtr->GetPosition().y));

}

std::shared_ptr<Component> Camera::Clone()
{
	std::shared_ptr<Camera> newComponent = std::make_shared<Camera>();
	newComponent->screenHeight = screenHeight;
	newComponent->screenWidth = screenWidth;
	newComponent->followTolorance = followTolorance;
	newComponent->alignX = alignX;
	newComponent->alignY = alignY;
	newComponent->currAlignDiff = currAlignDiff;
	newComponent->alignDiff = alignDiff;

	return newComponent;
}

void Camera::Inspect()
{
	//ImGui::SliderFloat2("Position", m_transformPtr->GetPosition().vec, 0.0f, static_cast<float>(WinWidth));
	//ImGui::SliderFloat2("Scale", m_transformPtr->GetScale().vec, 0.0f, 500.0f);
	//
	//ImGui::SliderFloat2("Limit", limit.vec, 0.0f, 500.0f);
	//ImGui::SliderFloat2("followTolorance", followTolorance.vec, 0.0f, 500.0f);

	if (ImGui::Button("Align Up"))
	{
		SetAlign(CAMERA_DIRECTION::UP);
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::Button("Align Down"))
	{
		SetAlign(CAMERA_DIRECTION::DOWN);
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::Button("Align Left"))
	{
		SetAlign(CAMERA_DIRECTION::LEFT);
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::Button("Align Right"))
	{
		SetAlign(CAMERA_DIRECTION::RIGHT);
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::Button("Align Neutral"))
	{
		SetAlign(CAMERA_DIRECTION::NEUTRAL_X);
		ImGui::CloseCurrentPopup();
	}


}

void Camera::Destroy()
{
}

void Camera::ResetPosition()
{
}

void Camera::MoveX(int _x)
{
	m_transformPtr->SetPositionX(m_transformPtr->GetPosition().x + _x);
}

void Camera::MoveY(int _y)
{
	m_transformPtr->SetPositionY(m_transformPtr->GetPosition().y + _y);
}

void Camera::Move(VEC2 _vec)
{
	m_transformPtr->SetPosition(_vec);
}

void Camera::AdjustZoom(float _zoom)
{
	m_transformPtr->SetScale(VEC2{ m_transformPtr->GetScale().x + _zoom, m_transformPtr->GetScale().y + _zoom});
}

VEC2 Camera::GetPosition()
{
	return m_transformPtr->GetPosition();
}

VEC2 Camera::GetScale()
{
	return m_transformPtr->GetScale();
}

float Camera::GetScreenWidth()
{
	return screenWidth;
}

float Camera::GetScreenHeight()
{
	return screenHeight;
}

glm::mat4 Camera::GetMatrix()
{
	return m_transformPtr->ConvertIntoMat4();
}

void Camera::SetFollowPtr(std::shared_ptr<Transform> _followptr)
{
	m_followPtr = _followptr;
}

void Camera::SetFollowSize(VEC2 _size)
{
	followSize = _size;
}

void Camera::SetLimit(VEC2 _limit)
{
	limit = _limit;
}

// Rounding in smaller intervals
float Camera::MyRound(float _val)
{
	float tmpVal = _val * 10;
	tmpVal = roundf(tmpVal);

	tmpVal /= 10;

	return tmpVal;

	//float tmpVal = _val - static_cast<int>(_val);

	/*if (tmpVal > 0.75f)
		return static_cast<int>(_val) + 0.75f;
	else if (tmpVal > 0.5f)
		return static_cast<int>(_val) + 0.5f;
	else if (tmpVal > 0.25f)
		return static_cast<int>(_val) + 0.25f;
	else 
		return static_cast<float>(static_cast<int>(_val));*/
}

void Camera::MoveTo(std::tuple<VEC2, float, float> _tuple)
{
	isMoving = true;

	movingToPos = -std::get<0>(_tuple);
	timer = std::get<1>(_tuple);

	movingDiff = (movingToPos - m_transformPtr->GetPosition()) / (MAX_MOVE_SPEED * std::get<2>(_tuple));
}

void Camera::SetAlign(CAMERA_DIRECTION _align)
{
	if (_align == CAMERA_DIRECTION::NEUTRAL)
	{
		SetAlign(CAMERA_DIRECTION::NEUTRAL_X);
		SetAlign(CAMERA_DIRECTION::NEUTRAL_Y);
		return;
	}


	float time = 1.50f;

	// align X
	if (_align == CAMERA_DIRECTION::LEFT && alignX != CAMERA_DIRECTION::LEFT)
	{
		alignDiff = {	(screenWidth/ 2.0f - CAMERA_HORIZONTAL_TOLORANCE - currAlignDiff.x)	/ time,
						0 };
		alignX = _align;
	}
	else if (_align == CAMERA_DIRECTION::RIGHT && alignX != CAMERA_DIRECTION::RIGHT)
	{
		alignDiff = { (screenWidth / 2.0f + CAMERA_HORIZONTAL_TOLORANCE - currAlignDiff.x) / time,
						0 };
		alignX = _align;
	}
	else if (_align == CAMERA_DIRECTION::NEUTRAL_X && alignX != CAMERA_DIRECTION::NEUTRAL_X)
	{
		alignDiff = { (screenWidth / 2.0f - currAlignDiff.x) / time,
						0 };
		alignX = _align;
	}

	// align Y
	if (_align == CAMERA_DIRECTION::UP && alignY != CAMERA_DIRECTION::UP)
	{
		alignDiff.y = (screenHeight / 2.0f - CAMERA_VERTICAL_TOLORANCE - currAlignDiff.y) / time;
		alignY = _align;
	}
	else if (_align == CAMERA_DIRECTION::DOWN && alignY != CAMERA_DIRECTION::DOWN)
	{
		alignDiff.y = (screenHeight / 2.0f + CAMERA_VERTICAL_TOLORANCE - currAlignDiff.y) / time;
		alignY = _align;
	}
	else if (_align == CAMERA_DIRECTION::NEUTRAL_Y && alignX != CAMERA_DIRECTION::NEUTRAL_Y)
	{
		alignDiff.y = (screenHeight / 2.0f - currAlignDiff.y) / time;
		alignY = _align;
	}


}

void Camera::AddToMoveList(VEC2 _pos, float _timer, float _speed)
{
	if (_speed > MAX_MOVE_SPEED)
		_speed = MAX_MOVE_SPEED;
	
	moveList.push(std::tuple<VEC2, float, float>(_pos, _timer, _speed));
	
	if (moveList.size() == 1)
		MoveTo(moveList.front());
}

void Camera::AddFollowPos(float speed)
{
	UpdateAlignment();
	// TESTING
	if (alignX == CAMERA_DIRECTION::NEUTRAL_X)
		currAlignDiff.x = screenWidth / 2.0f;
	
	if (alignY == CAMERA_DIRECTION::NEUTRAL_Y)
		currAlignDiff.y = screenHeight / 2.0f;

	// return to followPtr
 	if (m_followPtr != nullptr)
		moveList.push(std::tuple<VEC2, float, float>(VEC2(	round(m_followPtr->GetPosition().x - currAlignDiff.x),
															round(m_followPtr->GetPosition().y - currAlignDiff.y)),
													0.0f,
													speed));

	

	//std::cout << m_followPtr->GetPosition().x << " | " << m_followPtr->GetPosition().y << std::endl;
	//std::cout << "Alignment diff "  <<  currAlignDiff.x << " | " << currAlignDiff.y << std::endl;
	//
	//std::cout << "Start: "	<< -m_followPtr->GetPosition().x + screenWidth / 2.0f << " | "
	//						<< -m_followPtr->GetPosition().y + screenHeight / 2.0f << std::endl;
	//

	if (moveList.size() == 1)
		MoveTo(moveList.front());
}

void Camera::UpdateAlignment()
{
	//std::cout << "Align: " << (int)alignX << " | "<<  (int)alignY <<  std::endl;
	 
	// align X
	if (alignX == CAMERA_DIRECTION::LEFT)
	{
		
		// snap
		if (currAlignDiff.x < screenWidth / 2.0f - CAMERA_HORIZONTAL_TOLORANCE)
		{
			currAlignDiff.x = screenWidth / 2.0f - CAMERA_HORIZONTAL_TOLORANCE;
			//SetAlign(CAMERA_DIRECTION::NEUTRAL_X);
		}
		// move
		else if (currAlignDiff.x > screenWidth / 2.0f - CAMERA_HORIZONTAL_TOLORANCE)
		{
			currAlignDiff.x += (alignDiff.x * MyTimeManager.GetDt());
		}

	}
	else if (alignX == CAMERA_DIRECTION::RIGHT)
	{
		// snap
		if (currAlignDiff.x > screenWidth / 2.0f + CAMERA_HORIZONTAL_TOLORANCE)
		{
			currAlignDiff.x = screenWidth / 2.0f + CAMERA_HORIZONTAL_TOLORANCE;
			//SetAlign(CAMERA_DIRECTION::NEUTRAL_X);
		}
		// move
		else if (currAlignDiff.x < screenWidth / 2.0f + CAMERA_HORIZONTAL_TOLORANCE)
			currAlignDiff.x += (alignDiff.x * MyTimeManager.GetDt());
	}
	else if (alignX == CAMERA_DIRECTION::NEUTRAL_X)
	{
		if (alignDiff.x < 0.0f && currAlignDiff.x < screenWidth / 2.0f)
		{
			currAlignDiff.x = screenWidth / 2.0f;
		}
		else if (alignDiff.x > 0.0f && currAlignDiff.x > screenWidth / 2.0f)
		{
			currAlignDiff.x = screenWidth / 2.0f;
		}
		else if (currAlignDiff.x != screenWidth / 2.0f)
		{
			currAlignDiff.x += (alignDiff.x * MyTimeManager.GetDt());
		}
	}

	// align Y
	if (alignY == CAMERA_DIRECTION::UP)
	{
		// snap
		if (currAlignDiff.y < screenHeight / 2.0f - CAMERA_VERTICAL_TOLORANCE)
		{
			currAlignDiff.y = screenHeight / 2.0f - CAMERA_VERTICAL_TOLORANCE;
			//SetAlign(CAMERA_DIRECTION::NEUTRAL_X);
		}
		// move
		else if (currAlignDiff.y > screenHeight / 2.0f - CAMERA_VERTICAL_TOLORANCE)
		{
			currAlignDiff.y += (alignDiff.y * MyTimeManager.GetDt());
		}
	}
	else if (alignY == CAMERA_DIRECTION::DOWN)
	{
		// snap
		if (currAlignDiff.y > screenHeight / 2.0f + CAMERA_VERTICAL_TOLORANCE)
		{
			currAlignDiff.y = screenHeight / 2.0f + CAMERA_VERTICAL_TOLORANCE;
		}
		// move
		else if (currAlignDiff.y < screenHeight / 2.0f + CAMERA_VERTICAL_TOLORANCE)
		{
			currAlignDiff.y += (alignDiff.y * MyTimeManager.GetDt());
		}
	}
	else if (alignY == CAMERA_DIRECTION::NEUTRAL_Y)
	{
		if (alignDiff.y < 0.0f && currAlignDiff.y < screenHeight / 2.0f)
		{
			currAlignDiff.y = screenHeight / 2.0f;
		}
		else if (alignDiff.y > 0.0f && currAlignDiff.y > screenHeight / 2.0f)
		{
			currAlignDiff.y = screenHeight / 2.0f;
		}
		else if (currAlignDiff.y != screenHeight / 2.0f)
		{
			currAlignDiff.y += (alignDiff.y * MyTimeManager.GetDt());
		}
	}


	//std::cout << "Alignment diff " << currAlignDiff.x << " | " << currAlignDiff.y << std::endl;
}

void Camera::Shake()
{
	shakePos = -m_transformPtr->GetPosition();
	isShaking = true;

	float moveSpd = 0.01f;

	moveList.push(std::tuple<VEC2, float, float>(VEC2(shakePos.x + SHAKE_OFFSET_X, shakePos.y), 0.0f, moveSpd));
	moveList.push(std::tuple<VEC2, float, float>(shakePos, 0.0f, moveSpd));
	moveList.push(std::tuple<VEC2, float, float>(VEC2(shakePos.x - SHAKE_OFFSET_X, shakePos.y), 0.0f, moveSpd));
	moveList.push(std::tuple<VEC2, float, float>(shakePos, 0.0f, moveSpd));
	moveList.push(std::tuple<VEC2, float, float>(VEC2(shakePos.x + SHAKE_OFFSET_X, shakePos.y), 0.0f, moveSpd));
	moveList.push(std::tuple<VEC2, float, float>(shakePos, 0.0f, moveSpd));
	moveList.push(std::tuple<VEC2, float, float>(VEC2(shakePos.x - SHAKE_OFFSET_X, shakePos.y), 0.0f, moveSpd));
	moveList.push(std::tuple<VEC2, float, float>(shakePos, 0.0f, moveSpd));

	if (moveList.size() == 8)
		MoveTo(moveList.front());
}

// assume the shaking and the panning will never happen at the same time.
void Camera::ShakeHelper()
{



}
