/*************************************************************************************/
/*
File Name:		Graphics.cpp
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Graphics system manages and does all the rendering of images & text.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#include "stdafx.h"
#include "Graphics.h"

#include "GAM200.h"

#include <iostream>			// std::cout

// --- glm ---
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

// --- freetype ---
#include <ft2build.h>
#include FT_FREETYPE_H

// --- serialize ---
#include "TextSerialization.h"

// --- debuging ---
#include "DebugDraw.h"

// Engine
#include "ComponentCreator.h"

// --- for testing ---
#include "Logic.h"

// seralise
#include "RapidjsonSerialization.h"

// broadcast
#include "CoreEngine.h"
#include "Message.h"
#include "Physics.h"

// GraphicsVerticesType
#include "GraphicsVerticesType.h"

#include "TimeManager.h"


//#include "CoreEngine.h"
//#include "WindowsSystem.h"

//#include <execution>


//A global pointer to the graphics system
std::shared_ptr<Graphics> GRAPHICS;

Graphics::Graphics(HWND _hWnd)
	: hWnd{ _hWnd }, isDebug{false}
{
	InitializeRenderingEnvironment();

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);//RGBA

	GLint res = glewInit();
	if (res != GLEW_OK)
	{
		//Log it
		return;
	}

	// need to create environment first.
	rendererPtr = std::make_unique<Renderer>();
}

Graphics::~Graphics()
{
	CleanRenderingEnvironment();
}

void Graphics::Initialize()
{
	// set BG color
	float clearColor[4] = { 1.0f, 0.4f, 0.5f, 1.0f };
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	// -----------------------------------------------------------------------------------------

	// depth
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Blending - setting for alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

	// --- instancing --- [still testing]

	modelMatrices = std::make_shared<std::array<glm::mat4, INSTANCELIMIT2>>();

	for (int i = 0; i < INSTANCELIMIT2; ++i)
	{
		(*modelMatrices)[i] = glm::mat4(1.0f);
	}

	glGenBuffers(1, &buffer);
	

	// --- init managers ---
	MyAnimationManager.Initialize();
	MyShaderManager.Initialize();
	MySpriteManager.Initialize();
	MyParticleManager.Initialize();

	// load all shaders
	//MyShaderManager.init();
	MyShaderManager.LoadAllShaders(CORE->mPath + "Graphics/shader.txt");

	// load all the textures
	LoadPrimaryAssets();
	//LoadAssets(CORE->mPath + "Graphics/textures.json");

	// --- init text renderer ---
	textRendererPtr = std::make_shared<TextRenderer>();
	textRendererPtr->Initialize(MyShaderManager.GetShader(ShaderType::SHADER_TXT));

	// --- Register components ---
	RegisterComponent(Camera);
	RegisterComponent(SpriteComponent);
	//RegisterComponent(Animator);
	RegisterComponent(AnimationComponent);

	// --- init matrix ---
	m_projMatrix = glm::ortho(	0.0f, static_cast<float>(WinWidth),		// left right
								0.0f, static_cast<float>(WinHeight),	// bot top
								ZNEAR, ZFAR);							// znear, zfar

	graphicsHeight = WinHeight;
	graphicsWidth = WinWidth;

	// setting viewport size
	glViewport(0, 0, WinWidth, WinHeight);

	// safety checks for camera
	if (m_mainCameraPtr != nullptr)
	{
		// init matrix
		m_viewMatrix = m_mainCameraPtr->GetMatrix();
		m_viewProjMatrix = m_projMatrix * m_viewMatrix;
	}

	// --- init debug draw ---
	MyDebugDraw.Initalize();

	// --- init tint for shader ---
	MyShaderManager.BindShader(ShaderType::SHADER_IMG);
	MyShaderManager.GetShader(ShaderType::SHADER_IMG)->SetUniform4f("u_tint", 0.0f, 0.0f, 0.0f, 1.0f);
	//MyShaderManager.GetShader(ShaderType::SHADER_IMG)->SetUniform1f("u_alpha", 0.0f, 0.0f, 0.0f, 1.0f);


	// --- FBO ---
	GraphicsVerticesType quadVertices = {
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3 // 2, 3, 0
	};

	vertexArrayPtr = std::make_shared<VertexArray>();
	vertexBufferPtr = std::make_unique<VertexBuffer>(quadVertices);

	// "add" vertexBuffer into vertexArray
	VertexBufferLayout layout;
	layout.Push<float>(3);		// vertex/ length
	layout.Push<float>(2);		// texture coords
	//layout.Push<float>(1);		// alpha
	vertexArrayPtr->AddBuffer(*vertexBufferPtr, layout);

	indexBufferPtr = std::make_shared<IndexBuffer>(indices, 6);
	frameBufferPtr = std::make_unique<FrameBuffer>();
	renderBufferPtr = std::make_unique<RenderBuffer>(WinWidth, WinHeight);
	texturePtr = std::make_unique<Texture>(WinWidth, WinHeight);

	tmpAlpha = 1.0f;
}

void Graphics::Update(float _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	// clearing
	glClear(GL_DEPTH_BUFFER_BIT); 
	rendererPtr->Clear();

	frameBufferPtr->Bind();

	// clear framebuffer
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// --------------------------------------------------------------------------------------------

	// --- update matrix ---
	assert(m_mainCameraPtr);

	m_mainCameraPtr->Update();

	m_viewMatrix = m_mainCameraPtr->GetMatrix();
	m_viewProjMatrix = m_projMatrix * m_viewMatrix;

	// update all animations
	MyAnimationManager.Update(_dt);

	// update all the fixed sprites
	for (auto it = std::begin(MySpriteManager.spriteList); it != std::end(MySpriteManager.spriteList); ++it)
	{
		if (it->first->GetMovementType() == SpriteMovementType::FIXED)
		{
			// setting the position with respect to the camera
			it->first->GetBase()->has(Transform)->SetPosition(	it->first->GetFixedPosition().x - GRAPHICS->GetMainCameraPtr()->GetPosition().x,
																it->first->GetFixedPosition().y - GRAPHICS->GetMainCameraPtr()->GetPosition().y);
		}
	}
	
	// update all particles
	MyParticleManager.Update(_dt);

	// drawing all the sprites
	DrawWorld();

	// tmp for effiency testing
	//textRendererPtr->AddText("FPS: " + std::to_string(static_cast<int>(MyTimeManager.GetAvgDt())), VEC2(10.0f, 680.0f), 0.750f, VEC3(1.0f, 0.0f, 0.0f));


	// debug drawing
	if (isDebug)
	{
		DrawDebug();
		// cast to int to remove decimal points
		textRendererPtr->AddText("FPS: " + std::to_string(static_cast<int>(MyTimeManager.GetAvgDt())), VEC2(10.0f, 680.0f), 0.750f, VEC3(0.0f, 0.0f, 0.0f));
		//textRendererPtr->AddText("FPS: " + std::to_string(static_cast<int>(fps)), VEC2(10.0f, 680.0f));
		//textRendererPtr->RenderText(0.750f, glm::vec3(0.0f, 0.0f, 0.0f), m_projMatrix);
	}

	//textRendererPtr->AddText(std::to_string(static_cast<int>(INPUTMGR->GetMousePosition().x)) + " \t " + std::to_string(static_cast<int>(INPUTMGR->GetMousePosition().y))
	//	, VEC2(1000.0f, 640.0f), 0.750f, VEC3(0.0f, 0.0f, 0.0f));
		
	// Render all text
	textRendererPtr->RenderText(m_projMatrix);

	// end of drawing to frame buffer
	frameBufferPtr->Unbind();

	// drawing data from the frame buffer
	rendererPtr->Clear();

	// updating shader
	Transform tmp;
	MyShaderManager.BindShader(ShaderType::SHADER_IMG);
	MyShaderManager.GetShader(ShaderType::SHADER_IMG)->SetUniformMat4f(std::string("u_MVPArr[" + std::to_string(0) + "]"), tmp.ConvertTest());
	MyShaderManager.GetShader(ShaderType::SHADER_IMG)->SetUniform1f(std::string("u_alphaArr[" + std::to_string(0) + "]"), tmpAlpha);

	// binding
	vertexArrayPtr->Bind();
	indexBufferPtr->Bind();
	texturePtr->Bind();
		
	// draw
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, 1);
}

void Graphics::Clear()
{
	MySpriteManager.spriteList.clear();
	MyAnimationManager.Clear();
	MyParticleManager.Clear();
}

void Graphics::DrawWorld()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	MyShaderManager.BindShader(ShaderType::SHADER_IMG);

	std::string currTexture = "";
	std::string currSubTexture = "";
	float depth = 0.0f;

	int currFrameNum = 0;
	int num = 0;

	std::shared_ptr<VertexBuffer> tmpVB;
	std::shared_ptr<Texture> tmptexturePtr;
	std::shared_ptr<AnimationComponent> animationCompPtr;

	//AABB cameraAABB = m_mainCameraPtr->GetPosition()

	float tolorance = 0.0f;

	AABB camAABB(	VEC2(-m_mainCameraPtr->GetPosition().x - tolorance, -m_mainCameraPtr->GetPosition().y - tolorance),
					VEC2(-m_mainCameraPtr->GetPosition().x + m_mainCameraPtr->GetScreenWidth() + tolorance, -m_mainCameraPtr->GetPosition().y + m_mainCameraPtr->GetScreenHeight() + tolorance));

	//std::cout << camAABB.minPt.x << " | " << camAABB.minPt.y << "  |  " << camAABB.maxPt.x << " | " << camAABB.maxPt.y << std::endl;

	//std::cout << " ===================" << std::endl;

	//MyShaderManager.BindShader(ShaderType::SHADER_IMG);
	//MyShaderManager.GetShader(ShaderType::SHADER_IMG)->SetUniform4f("u_tint", 0.0f, 0.0f, 0.0f, 1.0f);
	//std::cout << "reset Alpha" << std::endl;

	for (auto [sprPtr, id] : MySpriteManager.spriteList)
	{
		VEC2 tmpTextureDiff;

		std::shared_ptr<Transform> transformPtr = sprPtr->GetBase()->has(Transform);

		AABB rectAABB (	VEC2(transformPtr->GetPosition().x - std::abs(transformPtr->GetScale().x), transformPtr->GetPosition().y - std::abs(transformPtr->GetScale().y)),
						VEC2(transformPtr->GetPosition().x + std::abs(transformPtr->GetScale().x), transformPtr->GetPosition().y + std::abs(transformPtr->GetScale().y))	);
		

		//MyDebugDraw.DrawRect(transformPtr->GetPosition(), transformPtr->GetScale());
		/*MyDebugDraw.DrawRect(	VEC2(	-m_mainCameraPtr->GetPosition().x - tolorance + m_mainCameraPtr->GetScreenWidth() / 2.0f, 
										-m_mainCameraPtr->GetPosition().y - tolorance + m_mainCameraPtr->GetScreenHeight() / 2.0f),
								VEC2(m_mainCameraPtr->GetScreenWidth() + tolorance , m_mainCameraPtr->GetScreenWidth() + tolorance) );*/

		//MyDebugDraw.DrawRect(	VEC2(-m_mainCameraPtr->GetPosition().x + m_mainCameraPtr->GetScreenWidth()/ 2.0f, -m_mainCameraPtr->GetPosition().y + m_mainCameraPtr->GetScreenHeight() / 2.0f),
		//						VEC2(m_mainCameraPtr->GetScreenWidth(), m_mainCameraPtr->GetScreenHeight()));



		//if (((std::string)(sprPtr->GetBase())->GetName()).find("Prefect") != std::string::npos)
		//{
		//	std::cout << "RECT AABB" << rectAABB.minPt.x << " | " << rectAABB.minPt.y << "  |  " << rectAABB.maxPt.x << " | " << rectAABB.maxPt.y << std::endl;

		//	std::cout << transformPtr->GetPosition().x << " ||| " << transformPtr->GetPosition().y << std::endl;
		//}

		//std::cout << "Checking" << sprPtr->GetBase()->GetName() << std::endl;

		//std::cout << (bool)(sprPtr->GetMovementType() == SpriteMovementType::FREE) << " | " << (bool)(camAABB.CheckCollided(&rectAABB) == false) << " | " << (bool)(sprPtr->IsActive() == false) << std::endl;

		// simple culling using AABB checking
		if (sprPtr->GetMovementType() == SpriteMovementType::FREE && camAABB.CheckCollided(&rectAABB) == false)
			continue;

		// checking for inactive sprites
		if (sprPtr->IsActive() == false)
			continue;

		//if (sprPtr->GetAlpha() != tmpAlpha)
		{
			//tmpAlpha = sprPtr->GetAlpha();
			//MyShaderManager.BindShader(ShaderType::SHADER_IMG);
			//MyShaderManager.GetShader(ShaderType::SHADER_IMG)->SetUniform4f("u_tint", 1.0f, 0.0f, 0.0f, 1 - sprPtr->GetAlpha());
			//std::cout << sprPtr->GetBase()->GetName() << " | " << sprPtr->GetAlpha() << std::endl;
		}
		
		

		// Animated
		if (sprPtr->GetType() == SpriteType::ANIMATED)
		{
			depth = 0;

			// setting up base information to work with
			animationCompPtr = sprPtr->GetBase()->has(AnimationComponent);

			// safety checks
			if (animationCompPtr == nullptr)
				continue;

			std::shared_ptr<Animation> animationPtr = GetAnimation(animationCompPtr->animationList.at(animationCompPtr->currAnimation));

			// not first loop
			//if (currTexture != "")
			{
				//if (currTexture != m_graphicsMap.find(animationPtr->textureName)->first)// || depth != sprPtr->GetDepth())
				{
					//std::cout << "num" << num << std::endl;
					//depth = sprPtr->GetDepth;
					glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, num);

					// reset
					num = 0;
					currTexture = "";
					depth = 0.0f;
				}
			}
			
			if (currTexture != m_graphicsMap.find(animationPtr->textureName)->first)
			{

				currTexture = m_graphicsMap.find(animationPtr->textureName)->first;

				auto [name, va, vb, ib, texture] = m_graphicsMap.find(animationPtr->textureName)->second;

				// setting tmps
				tmptexturePtr = texture;
				tmpVB = vb;

				// binding
				texture->Bind();
				vb->Bind();
				va->Bind();
				ib->Bind();

				// safety checks
				if (tmpVB == nullptr)
					return;

				// setting datas
				currSubTexture = sprPtr->GetCurrSubTextureName();
				currFrameNum = sprPtr->GetCurrFrameNum();
				//tmpVB->SetAlpha(sprPtr->GetAlpha());
				tmpVB->SetDepth(sprPtr->GetDepth());
				depth = sprPtr->GetDepth();
				tmpVB->SetUV(GetAnimation(animationCompPtr->animationList.at(animationCompPtr->currAnimation))->frames.at(animationCompPtr->currFrame).uv);
				tmpVB->UpdateData();
			}

			//if (depth != sprPtr->GetDepth())
			//{
			//	depth = sprPtr->GetDepth();

			//	tmpVB->SetDepth(sprPtr->GetDepth());
			//	tmpVB->UpdateData();

			//}
		}
		else // Not animated (IE SpriteList)
		{
			// not the same texture, update the current texture and bind the texture
			if (currTexture != sprPtr->GetCurrTextureName())
			{
				// not first loop
				if (currTexture != "")
				{
					//std::cout << "Draw : " << currTexture << std::endl;
					//std::cout << num << std::endl;
					//std::cout << "Drawing " << sprPtr->GetBase()->GetName() << " | " << sprPtr->GetAlpha() << std::endl;
					glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, num);

					// reset
					num = 0;
				}

				// update current texture
				currTexture = sprPtr->GetCurrTextureName();

				auto graphicsMapItr = m_graphicsMap.find(currTexture);

				// safety checks
				if (graphicsMapItr == std::end(m_graphicsMap))
				{
					std::cout << "ERROR: Graphics \"" << currTexture << "\" cant be found." << std::endl;
					assert(0);
				}

				auto [name, va, vb, ib, texture] = graphicsMapItr->second;

				// setting tmps
				tmptexturePtr = texture;
				tmpVB = vb;

				// binding
				texture->Bind();
				vb->Bind();
				va->Bind();
				ib->Bind();

				// safety checks
				if (tmpVB == nullptr)
					return;

				currSubTexture = sprPtr->GetCurrSubTextureName();
				currFrameNum = sprPtr->GetCurrFrameNum();
				tmpVB->SetDepth(sprPtr->GetDepth());

				depth = sprPtr->GetDepth();

				//std::shared_ptr<SpriteList> tmp = GetSpriteList(sprPtr->GetCurrSubTextureName());
				//int frameNum = sprPtr->GetCurrFrameNum();

				//std::cout << sprPtr->GetCurrSubTextureName() << " " << GetSpriteList(sprPtr->GetCurrSubTextureName())->sprites.size() << std::endl;

				tmpVB->SetUV(GetSpriteList(sprPtr->GetCurrSubTextureName())->sprites.at(sprPtr->GetCurrFrameNum()).uv);

				//tmpVB->SetUV(GetSpriteList(sprPtr->GetCurrSubTextureName())->sprites.at(sprPtr->GetCurrFrameNum()).uv);
					/*tmptexturePtr->GetUVs(	sprPtr->GetCurrSubTextureName(),
													sprPtr->GetCurrFrameNum()));*/
				tmpVB->UpdateData();
			}

			if (currSubTexture != sprPtr->GetCurrSubTextureName() || currFrameNum != sprPtr->GetCurrFrameNum())
			{
				// not first loop
				if (currSubTexture != "")
				{
					//std::cout << "Draw : " << currTexture << std::endl;
					//std::cout << num << std::endl;
					//std::cout << "Drawing " << sprPtr->GetBase()->GetName() << " | " << sprPtr->GetAlpha() << std::endl;
					glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, num);

					// reset
					num = 0;
				}


				currSubTexture = sprPtr->GetCurrSubTextureName();
				currFrameNum = sprPtr->GetCurrFrameNum();

				/*std::cout	<< sprPtr->GetCurrSubTextureName() << " Size: " 
							<< GetSpriteList(sprPtr->GetCurrSubTextureName())->sprites.size() 
							<< " currFrameNum " << currFrameNum << std::endl;*/

				tmpVB->SetDepth(sprPtr->GetDepth());
				depth = sprPtr->GetDepth();
				tmpVB->SetUV(GetSpriteList(sprPtr->GetCurrSubTextureName())->sprites.at(sprPtr->GetCurrFrameNum()).uv);

				//tmpVB->SetUV(tmptexturePtr->GetUVs(	sprPtr->GetCurrSubTextureName(),
				//									sprPtr->GetCurrFrameNum()));
				tmpVB->UpdateData();
			}

			if (depth != sprPtr->GetDepth())
			{
				depth = sprPtr->GetDepth();

				tmpVB->SetDepth(sprPtr->GetDepth());
				tmpVB->UpdateData();

			}
		}

		Transform tmpTransform = *sprPtr->GetTransform();
		tmpTransform.SetPosition(tmpTransform.GetPosition() + sprPtr->GetOffset());
		glm::mat4 mvp = m_viewProjMatrix * tmpTransform.ConvertIntoMat4();

		MyShaderManager.GetShader(ShaderType::SHADER_IMG)->SetUniformMat4f(std::string("u_MVPArr[" + std::to_string(num) + "]"), mvp);
		MyShaderManager.GetShader(ShaderType::SHADER_IMG)->SetUniform1f(std::string("u_alphaArr[" + std::to_string(num) + "]"), sprPtr->GetAlpha());

		//std::cout << sprPtr->GetBase()->GetName() << " | " << sprPtr->GetAlpha() << std::endl;

		++num;

		//limitCounter
		if (num >= INSTANCELIMIT)
		{
			//std::cout << num << std::endl;
			//std::cout << "Drawing " << sprPtr->GetBase()->GetName() << " | " << sprPtr->GetAlpha() << std::endl;
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, num);
			
			num = 0;
		}
	}

	//std::cout << num << std::endl;
	//std::cout << "Drawing " << tmpAlpha << std::endl;
	if (num != 0)
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, num);

	//std::cout << " -------------- " << std::endl;
}


inline void Graphics::DrawDebug()
{
	//MyDebugDraw.DrawLine(VEC2(-m_mainCameraPtr->GetPosition().x, -m_mainCameraPtr->GetPosition().y),
	//	VEC2(-m_mainCameraPtr->GetPosition().x + m_mainCameraPtr->GetScreenWidth(), -m_mainCameraPtr->GetPosition().y + m_mainCameraPtr->GetScreenHeight()));

	MyDebugDraw.DrawAll(MyShaderManager.GetShader(ShaderType::SHADER_COLOR), m_viewProjMatrix);


}

void Graphics::LoadAssets(std::string _filePath)
{
	//InitLevelMap("test");


	/*{
		JsonReader jr(CORE->mPath + "Tilemaps/Episode.json");
		bool isFileOpened = jr.IsFileOpened();
		assert(isFileOpened);

		jr.ParseStreamToDoc();
		jr.doc.Accept(jr.prettyReaderBuffer);

		if (!isFileOpened)
		{
			assert(0 && "Error Loading Assets!");
		}
		else
		{
			// needed to reset the ifstream good() flag, otherwise for some reason the below jr.IsGood() will be false
			jr.stream.clear();
		}

		std::string testStr;
		jr.ReadStringMember("Row0", "Col0", "Texture name", testStr);
		std::cout << "Test: " << testStr << std::endl;

		int testInt;
		jr.ReadIntMember("Row0", "Col0", "CurrFrame", testInt);
		std::cout << "Test" << testInt << std::endl;

	}*/
	// ---------------------------------

	JsonReader jr(_filePath);
	bool isFileOpened = jr.IsFileOpened();
	assert(isFileOpened);

	jr.ParseStreamToDoc();
	jr.doc.Accept(jr.prettyReaderBuffer);
	
	if (!isFileOpened)
	{
		assert(0 && "Error Loading Assets!");
	}
	else
	{
		// needed to reset the ifstream good() flag, otherwise for some reason the below jr.IsGood() will be false
		jr.stream.clear();
	}

	//std::vector<std::string> testVec;
	std::cout << _filePath << std::endl;
	//jr.ReadStringArrayMember("ArrTest", testVec);


	std::string jsonKey1 = "Texture";

	std::string name, path, tmpKey;
	int i = 1;

	while (jr.IsGood())
	{
		tmpKey = jsonKey1 + std::to_string(i);

		if (jr.doc.HasMember(tmpKey.c_str()))
		{
			jr.ReadStringMember(tmpKey, "TextureName", name);
			jr.ReadStringMember(tmpKey, "FilePath", path);


			if (strcmp(path.substr(path.find_last_of(".") + 1, std::string::npos).c_str(), "png") != 0)
			{
				std::string errormsg = path + " is not in .png format";
				MessageBoxA(NULL, errormsg.c_str(), "Error", MB_OK);
				++i;
				continue;
			}

			// load texture and add it into the list
			m_graphicsMap.insert({ name, Create_MyGraphicsFormat(name, path, jr, tmpKey) });


			std::cout << "Loaded texture: " << name << std::endl;
		}
		else
			break;

		++i;
	}
}

std::shared_ptr<Texture> Graphics::LoadTexture(std::string _filePath)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(_filePath);

	if (texture)
	{
		// NO ID FOR NOW?!
		//tmp no id
		//texture->SetTextureID(AddTexture(texture));
		
		return texture;
	}
	else
	{
		std::cout << "ERROR: Texture failed to load: " << _filePath << std::endl;
		assert(0);	// force assert
		return nullptr;
	}
}

void Graphics::LoadNewAsset()
{
	JsonReader jr(CORE->mPath + "Graphics/textures.json");
	bool isFileOpened = jr.IsFileOpened();
	assert(isFileOpened);

	jr.ParseStreamToDoc();
	jr.doc.Accept(jr.prettyReaderBuffer);

	if (!isFileOpened)
	{
		assert(0 && "Error Loading Assets!");
	}
	else
	{
		// needed to reset the ifstream good() flag, otherwise for some reason the below jr.IsGood() will be false
		jr.stream.clear();
	}


	std::string jsonKey1 = "Texture";

	std::string name, path, tmpKey;
	int i = static_cast<int>(m_graphicsMap.size());

	while (jr.IsGood())
	{
		tmpKey = jsonKey1 + std::to_string(i);

		if (jr.doc.HasMember(tmpKey.c_str()))
		{
			jr.ReadStringMember(tmpKey, "TextureName", name);
			jr.ReadStringMember(tmpKey, "FilePath", path);

			if (strcmp(path.substr(path.find_last_of(".") + 1, std::string::npos).c_str(), "png") != 0)
			{
				std::string errormsg = path + " is not in .png format";
				MessageBoxA(NULL, errormsg.c_str(), "Error", MB_OK);
				continue;
			}

			// load texture and add it into the list
			m_graphicsMap.insert({ name, Create_MyGraphicsFormat(name, path, jr, tmpKey) });



			std::cout << "Loaded texture: " << name << std::endl;
		}
		else
			break;

		++i;
	}
}

void Graphics::LoadAsset(std::string _path, std::string _jsonKey1)
{
	JsonReader jr(_path);
	bool isFileOpened = jr.IsFileOpened();
	assert(isFileOpened);

	jr.ParseStreamToDoc();
	jr.doc.Accept(jr.prettyReaderBuffer);

	if (!isFileOpened)
	{
		assert(0 && "Error Loading Assets!");
	}
	else
	{
		// needed to reset the ifstream good() flag, otherwise for some reason the below jr.IsGood() will be false
		jr.stream.clear();
	}

	std::string name, path, tmpKey;
	int i = static_cast<int>(m_graphicsMap.size());

	while (jr.IsGood())
	{
		tmpKey = _jsonKey1 + std::to_string(i);

		if (jr.doc.HasMember(tmpKey.c_str()))
		{
			jr.ReadStringMember(tmpKey, "TextureName", name);
			jr.ReadStringMember(tmpKey, "FilePath", path);

			if (strcmp(path.substr(path.find_last_of(".") + 1, std::string::npos).c_str(), "png") != 0)
			{
				std::string errormsg = path + " is not in .png format";
				MessageBoxA(NULL, errormsg.c_str(), "Error", MB_OK);
				continue;
			}

			// load texture and add it into the list
			m_graphicsMap.insert({ name, Create_MyGraphicsFormat(name, path, jr, tmpKey) });



			std::cout << "Loaded texture: " << name << std::endl;
		}
		else
			break;

		++i;
	}
}

void Graphics::LoadPrimaryAssets()
{
	LoadAsset(CORE->mPath + "Graphics/textures.json", "LoadingTexture");
}

void Graphics::LoadSecondaryAssets()
{
	LoadAsset(CORE->mPath + "Graphics/textures.json", "Texture");
}

// returns true if asset is loaded properly
bool Graphics::LoadAssets(JsonReader& _jr, std::string _jsonKey1, int _index)
{
	//JsonReader jr(_path);
	bool isFileOpened = _jr.IsFileOpened();
	assert(isFileOpened);

	_jr.ParseStreamToDoc();
	_jr.doc.Accept(_jr.prettyReaderBuffer);

	if (!isFileOpened)
	{
		assert(0 && "Error Loading Assets!");
	}
	else
	{
		// needed to reset the ifstream good() flag, otherwise for some reason the below jr.IsGood() will be false
		_jr.stream.clear();
	}

	std::string name, path, tmpKey;
	//int i = static_cast<int>(m_graphicsMap.size());

	// json reader is valid
	if (_jr.IsGood())
	{
		tmpKey = _jsonKey1 + std::to_string(_index);

		// check if it exists
		if (_jr.doc.HasMember(tmpKey.c_str()))
		{
			_jr.ReadStringMember(tmpKey, "TextureName", name);
			_jr.ReadStringMember(tmpKey, "FilePath", path);

			// check for png format
			if (strcmp(path.substr(path.find_last_of(".") + 1, std::string::npos).c_str(), "png") != 0)
			{
				std::string errormsg = path + " is not in .png format";
				MessageBoxA(NULL, errormsg.c_str(), "Error", MB_OK);

				return false;
			}

			// load texture and add it into the list
			m_graphicsMap.insert({ name, Create_MyGraphicsFormat(name, path, _jr, tmpKey) });



			std::cout << "Loaded texture: " << name << std::endl;

			return true;
		}
		// key does not exist
		else
			return false;
	}
	// json reader is not valid
	else
		return false;

	return true;
}

bool Graphics::InitializeRenderingEnvironment()
{
	//create rendering window
	m_windowDC = GetDC(hWnd);

	DEVMODE devMode = { 0 };
	devMode.dmSize = sizeof(DEVMODE);
	BOOL b = EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &devMode);
	if (b == 0)
		return false;

	//drawing surface format
	PIXELFORMATDESCRIPTOR pfdesc;
	memset(&pfdesc, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfdesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfdesc.nVersion = 1;
	pfdesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
	pfdesc.iPixelType = PFD_TYPE_RGBA;
	pfdesc.cColorBits = (BYTE)devMode.dmBitsPerPel;//32; //24 bit color for front and back buffer
	pfdesc.cDepthBits = 24;//24 bit depth buffer - not used in this demo
	pfdesc.cStencilBits = 8; //8 bit stencil buffer - not used in this demo

	int pf = ChoosePixelFormat(m_windowDC, &pfdesc);//checks if the graphics card can support the pixel format requested
	if (pf == 0)
	{
		ReleaseDC(hWnd, m_windowDC);
		return false;
	}


	BOOL ok = SetPixelFormat(m_windowDC, pf, &pfdesc);
	if (!ok)
	{
		ReleaseDC(hWnd, m_windowDC);
		return false;
	}


	//set the OpenGL context
	m_wglDC = wglCreateContext(m_windowDC);
	if (!m_wglDC)
	{
		ReleaseDC(hWnd, m_windowDC);
		return false;
	}


	ok = wglMakeCurrent(m_windowDC, m_wglDC);
	if (!ok)
	{
		wglDeleteContext(m_wglDC);
		ReleaseDC(hWnd, m_windowDC);
		return false;
	}

	return true;
}

void Graphics::CleanRenderingEnvironment()
{
	if (m_wglDC)
	{
		if (!wglMakeCurrent(NULL, NULL))
		{
			//log
		}
	}

	if (!wglDeleteContext(m_wglDC))
	{
		//log
	}
	m_wglDC = NULL;

	if (m_windowDC && !ReleaseDC(hWnd, m_windowDC))
	{
		m_windowDC = NULL;
	}
}

void Graphics::SwapBuffers()
{
	::SwapBuffers(m_windowDC); //using double buffering
}

void Graphics::AddMainCamera(std::shared_ptr<Camera> _camera)
{
	m_mainCameraPtr = _camera;
}



void Graphics::ToggleDebugInfo()
{
	if (isDebug)
		isDebug = false;
	else
		isDebug = true;
}

//// --- Sprite Manager bypass ---
//unsigned int Graphics::AddSprite(std::shared_ptr<SpriteComponent> _sprPtr)
//{
//	return spriteManager.AddSprite(_sprPtr);
//}
//
//void Graphics::normalToFixed(int _sprID)
//{
//	spriteManager.normalToFixed(_sprID);
//}
//
//void Graphics::RemoveSprite(unsigned int _id)
//{
//	spriteManager.RemoveSprite(_id);
//}

void Graphics::UpdateScreenSize()
{
	//CleanRenderingEnvironment();
	//assert(InitializeRenderingEnvironment() && "Failed to update screen size");

	//// set BG color
	//float clearColor[4] = { 1.0f, 0.4f, 0.5f, 1.0f };
	//glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	//glClear(GL_COLOR_BUFFER_BIT);

	//// -----------------------------------------------------------------------------------------

	//// depth
	//glEnable(GL_DEPTH_TEST);
	//glClear(GL_DEPTH_BUFFER_BIT);

	//// Blending - setting for alpha
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	//glBlendEquation(GL_FUNC_ADD);


	// updating matrix
	m_projMatrix = glm::ortho(	0.0f, static_cast<float>(WinWidth),	// left right
								0.0f, static_cast<float>(WinHeight),	// bot top
								ZNEAR, ZFAR);	// znear, zfar

	m_viewProjMatrix = m_projMatrix * m_viewMatrix;

	// updating viewport size
	glViewport(0, 0, WinWidth, WinHeight);
}

std::shared_ptr<Camera> Graphics::GetMainCameraPtr()
{
	return m_mainCameraPtr;
}

//int Graphics::AddTexture(std::shared_ptr<Texture> _texture)
//{
//	resourceListTexture.push_back(_texture);
//
//	return static_cast<int>(resourceListTexture.size() - 1);
//}

void Graphics::AddSpriteList(std::shared_ptr<SpriteList> _spriteListPtr)
{
	resourceListSprite.push_back(_spriteListPtr);
}

//std::vector<std::shared_ptr<SpriteList>>& Graphics::GetFullSpriteList()
//{
//	// TODO: insert return statement here
//	return resourceListSprite;
//}

std::shared_ptr<SpriteList> Graphics::GetSpriteList(unsigned int _id)
{
	// safety checks
	assert(resourceListSprite.size() > _id && "ERROR: SpriteList out of range");

	return resourceListSprite.at(_id);
}

std::shared_ptr<SpriteList> Graphics::GetSpriteList(std::string _name)
{
	for (int i = 0; i < resourceListSprite.size(); ++i)
	{
		if (resourceListSprite.at(i)->name == _name)
			return resourceListSprite.at(i);
	}

	assert(0 && "ERROR: Sprite not found!");

	return nullptr;
}

int Graphics::GetSpriteListID(std::string _name)
{
	for (int i = 0; i < resourceListSprite.size(); ++i)
	{
		if (resourceListSprite.at(i)->name == _name)
			return i;
	}

	assert(0 && "ERROR: Sprite not found!");

	return -1;
}

std::string Graphics::GetSpriteName(unsigned int _id)
{
	// safety checks
	assert(resourceListSprite.size() > _id && "ERROR: SpriteList out of range");

	return resourceListSprite.at(_id)->name;
}

unsigned int Graphics::GetRenderID(const std::string& texturename)
{
	return GetTexture(texturename)->GetRenderID();
}

void Graphics::AddAnimaton(std::shared_ptr<Animation> _animationPtr)
{
	resourceListAnimation.push_back(_animationPtr);
}

void Graphics::AddAnimationComponent(std::shared_ptr<AnimationComponent> _animationCompPtr)
{
	MyAnimationManager.AddAnimationComponent(_animationCompPtr);
}

std::shared_ptr<Animation> Graphics::GetAnimation(int _id)
{
	return resourceListAnimation.at(_id);
}

int Graphics::GetAnimationID(std::string _name)
{
	for (int i = 0; i < resourceListAnimation.size(); ++i)
	{
		if (resourceListAnimation.at(i)->name == _name)
			return i;
	}

	assert(0 && "Animation not found!");

	return -1;
}

std::string Graphics::GetAnimationName(int _id)
{
	return resourceListAnimation.at(_id)->name;
}

void Graphics::InitLevelMap(const std::string& _path)
{
	//CORE->GetCurrLevel()

	//std::string tmpPath = CORE->mPath + "Tilemaps/Episode.json";
	//std::string tmpPath = CORE->mPath + "Tilemaps/" + CORE->GetCurrLevel() + "Wall.json";
	//std::string tmpPath2 = CORE->mPath + "Tilemaps/" + CORE->GetCurrLevel() + "Floor.json";

	bool isWall = (_path.find("Wall") != std::string::npos);

	JsonReader jr(_path);
	bool isFileOpened = jr.IsFileOpened();
	//assert(isFileOpened);
	if (!isFileOpened)
		return;

	jr.ParseStreamToDoc();
	jr.doc.Accept(jr.prettyReaderBuffer);

	if (!isFileOpened)
	{
		//assert(0 && "Error Loading Assets!");
	}
	else
	{
		// needed to reset the ifstream good() flag, otherwise for some reason the below jr.IsGood() will be false
		jr.stream.clear();
	}

	int width = 0;
	int height = 0;

	std::string textureName = "";
	int currFrame = 0;

	jr.ReadIntMember("Size", "Width", width);
	jr.ReadIntMember("Size", "Height", height);

	int gridSize = 64;

	FACTORY->CreateArcheType("BgTile");

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			// for now no need for texture name?
			jr.ReadStringMember("Row" + std::to_string(j), "Col" + std::to_string(i), "Texture name", textureName);
			jr.ReadIntMember("Row" + std::to_string(j), "Col" + std::to_string(i), "CurrFrame", currFrame);

			// default for no texture.
			if (textureName == "")
				continue;

			std::shared_ptr<GOC> tmpObj = LOGIC->CreateObjectAt(	VEC2(i * gridSize + 0.5f * gridSize, j * gridSize + 0.5f * gridSize), 0, "BgTile",
																	"Tile R" + std::to_string(j) + "  xC" + std::to_string(i), false);

			std::shared_ptr<SpriteComponent> tmpSpr = tmpObj->has(SpriteComponent);
			
			if (isWall)
				tmpSpr->SetDepth(tmpSpr->GetDepth() - 0.01f);

			tmpSpr->SetStartFrame(currFrame);

			//std::cout << "Test: " << textureName << " | " << currFrame << std::endl;
		}
	}

	//std::string testStr;
	//jr.ReadStringMember("Row0", "Col0", "Texture name", testStr);
	//std::cout << "Test: " << testStr << std::endl;
	//
	//int testInt;
	//jr.ReadIntMember("Row0", "Col0", "CurrFrame", testInt);
	//std::cout << "Test" << testInt << std::endl;
}

//AnimationManager& Graphics::GetAnimationManager()
//{
//	//return animationManager;
//}

std::shared_ptr<TextRenderer> Graphics::GetTextRenderer()
{
	return textRendererPtr;
}

glm::mat4 Graphics::GetProjMatrix()
{
	return m_projMatrix;
}

glm::mat4 Graphics::GetViewMatrix()
{
	return m_viewMatrix;
}
std::shared_ptr<std::array<glm::mat4, INSTANCELIMIT2>> Graphics::GetModelMatrices()
{
	return modelMatrices;
}

void Graphics::LoadLevel()
{
	SetAlpha(1.0f);
	MyParticleManager.Initialize();
}

void Graphics::CreateEmitter(	EmitterType _type, VEC2 _position, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed,
								float _emitInterval, float _emitNum, VEC2 _emitArea, float _angle, float _movementSpeed, std::string _archeType)
{
	MyParticleManager.CreateEmitter(_type, _position, _startDelay, _lifetime, _particleLifetime, _fadeSpeed, _emitInterval, _emitNum, _emitArea, _angle, _movementSpeed, _archeType);
}

void Graphics::CreateEmitter(EmitterType _type, std::shared_ptr<Transform> transformPtr, VEC2 _offset, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed,
	float _emitInterval, float _emitNum, VEC2 _emitArea, float _angle, float _movementSpeed, std::string _archeType)
{
	MyParticleManager.CreateEmitter(_type, transformPtr, _offset, _startDelay, _lifetime, _particleLifetime, _fadeSpeed, _emitInterval, _emitNum, _emitArea, _angle, _movementSpeed, _archeType);
}

int Graphics::GetGraphicsWidth()
{
	return graphicsWidth;
}

int Graphics::GetGraphicsHeight()
{
	return graphicsHeight;
}

AABB Graphics::GetCameraAABB()
{
	return AABB (VEC2(-m_mainCameraPtr->GetPosition().x, -m_mainCameraPtr->GetPosition().y),
		VEC2(-m_mainCameraPtr->GetPosition().x + m_mainCameraPtr->GetScreenWidth(), -m_mainCameraPtr->GetPosition().y + m_mainCameraPtr->GetScreenHeight()));
}

VEC2 Graphics::GetTextureSize(std::string _name)
{
	std::shared_ptr<Texture> tmpTexture = GetTexture(_name);
	
	// safety checks
	if (tmpTexture == nullptr)
		assert(0 && "Texture not found!");

	return VEC2(static_cast<float>(tmpTexture->GetWidth()), static_cast<float>(tmpTexture->GetHeight()));
}

unsigned int Graphics::GetTextureFrameMax(std::string _name, std::string _subName)
{
	return GetTexture(_name)->GetMaxFrame(_subName);
}

std::shared_ptr<Texture> Graphics::GetTexture(std::string _name)
{
	auto itr = m_graphicsMap.find(_name);

	// safety check
	if (itr == std::end(m_graphicsMap))
		assert(0);

	auto [name, va, vb, ib, texture] = itr->second;

	return texture;
}

std::shared_ptr<Texture> Graphics::GetFBOTexture()
{
	return texturePtr;
}

//std::vector<std::shared_ptr<Texture>>& Graphics::GetTextureList()
//{
//	return resourceListTexture;// textureList;
//}

std::vector<std::shared_ptr<SpriteList>>& Graphics::GetSpriteList()
{
	return resourceListSprite;
}

std::vector<std::shared_ptr<Animation>>& Graphics::GetAnimationList()
{
	return resourceListAnimation; // animationList;
}

std::map<std::string, MyGraphicsFormat>& Graphics::GetGraphicsMap()
{
	return m_graphicsMap;
}

VEC2 Graphics::ScreenToWorld(VEC2 _coords)
{
	//return VEC2{ (_coords.x - m_mainCameraPtr->GetPosition().x) / m_mainCameraPtr->GetScale().x,
	//			(WinHeight - _coords.y - m_mainCameraPtr->GetPosition().y) / m_mainCameraPtr->GetScale().y };

	return VEC2{ (-m_mainCameraPtr->GetPosition().x + _coords.x), (-m_mainCameraPtr->GetPosition().y + _coords.y) };
}

VEC2 Graphics::WorldToScreen(VEC2 _coords)
{
	return VEC2{ _coords.x * m_mainCameraPtr->GetScale().x + m_mainCameraPtr->GetPosition().x,
				-(_coords.y * m_mainCameraPtr->GetScale().y + m_mainCameraPtr->GetPosition().y - WinHeight) };
};
