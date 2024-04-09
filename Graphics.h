/*************************************************************************************/
/*
File Name:		Graphics.h
Project Name:	WhereGotLate
Author(s):		Tay Qin Wen Alvin [100%]


Brief:			Graphics system manages and does all the rendering of images & text.


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/


#pragma once

#include "OpenGLIncludes.h"
//#include <gl/Gl.h>
#include <memory>

#include "System.h"
#include "Engine.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Renderer.h"
#include "Shader.h"
//#include "GraphicsLayer.h"
#include "Camera.h"
#include "Animator.h"

#include "TextRenderer.h"

#include "ShaderManager.h"
#include "SpriteManager.h"

#include <tuple>
#include <string>


#include "MyGraphicsFormat.h"
#include "SpriteList.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "AnimationComponent.h"

// Particles
#include "ParticleManager.h"

//#include "Scripts.h"

// --- FBO ---
#include "FrameBuffer.h"
#include "RenderBuffer.h"

#include "BoxCollider.h"

#define INSTANCELIMIT	100
#define INSTANCELIMIT2	200

#define ZNEAR			2.0f
#define ZFAR			-2.0f


class Graphics : public ISystem
{
	std::unique_ptr<Renderer>		rendererPtr;

	//ShaderManager					shaderManager;
	//SpriteManager					spriteManager;
	//AnimationManager				animationManager;

	//ParticleManager				particlesManager;


	std::shared_ptr<TextRenderer>	textRendererPtr;

	HDC m_windowDC;					//global Window-device context
	HGLRC m_wglDC;					//OpenGL-device context


	// ------- tmp for testing -----------
	float scaleFactor = 1.0f;
	float rotateRad = 0.0f;

	// ----- tmp data -----
	//int timeCount = 0;
	//float fps;
	//float dtSum = 0;

	float CameraX = 1;
	float CameraY = 1;
	float CameraScale = 1;


	//World Projection and view matrices
	glm::mat4 m_projMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_viewProjMatrix;

	std::shared_ptr<Camera> m_mainCameraPtr;

	unsigned int m_MaxLayerNum;


	std::map<std::string, MyGraphicsFormat>		m_graphicsMap;

	//std::map<std::string, std::shared_ptr<Texture>> m_graphicsMap;

	// to resource manager?
	//std::vector<std::shared_ptr<Texture>>		resourceListTexture; // textureList;
	std::vector<std::shared_ptr<SpriteList>>	resourceListSprite; // spriteList;
	std::vector<std::shared_ptr<Animation>>		resourceListAnimation; // animationList;


	std::shared_ptr<std::array<glm::mat4, INSTANCELIMIT2>> modelMatrices;


	// --- for FBO ---
	std::shared_ptr<VertexArray>	vertexArrayPtr;
	std::unique_ptr<VertexBuffer>	vertexBufferPtr;
	std::shared_ptr<IndexBuffer>	indexBufferPtr;
	std::unique_ptr<FrameBuffer>	frameBufferPtr;
	std::unique_ptr<RenderBuffer>	renderBufferPtr;
	std::shared_ptr<Texture>		texturePtr;


	int		graphicsHeight;
	int		graphicsWidth;

	float	tmpAlpha;

public:
	unsigned int buffer;

	HWND hWnd;


	Graphics(HWND _hWnd);
	~Graphics();

	virtual void Initialize() override;
	virtual void Update(float _dt) override;
	virtual void Clear() override;
	virtual std::string GetName() override { return "Graphics"; }
	
	void DrawWorld();

	// Drawing for debug
	inline void DrawDebug();

	// loading
	void LoadAssets(std::string _filePath);
	std::shared_ptr<Texture> LoadTexture(std::string _filePath);

	void LoadNewAsset();

	void LoadAsset(std::string _path, std::string _jsonKey1);
	void LoadPrimaryAssets();
	void LoadSecondaryAssets();
	bool LoadAssets(JsonReader& _jr, std::string _jsonKey1, int _index);

	// ----------- from eg ---------------------
	bool InitializeRenderingEnvironment();
	void CleanRenderingEnvironment();
	void SwapBuffers();
	// ----------- from eg ---------------------

	void AddMainCamera(std::shared_ptr<Camera> _camera);


	bool isDebug = true;
	void ToggleDebugInfo();

	// --- Sprite Manager bypass ---
	//unsigned int AddSprite(std::shared_ptr<SpriteComponent> _sprPtr);
	//void normalToFixed(int _sprID);
	//void RemoveSprite(unsigned int _id);

	VEC2 GetTextureSize(std::string _name);
	unsigned int GetTextureFrameMax(std::string _name, std::string _subName);
	std::shared_ptr<Texture> GetTexture(std::string _name);
	std::shared_ptr<Texture> GetFBOTexture();

	//std::vector<std::shared_ptr<Texture>>&		GetTextureList();
	std::vector<std::shared_ptr<SpriteList>>&	GetSpriteList();
	std::vector<std::shared_ptr<Animation>>&	GetAnimationList();
	std::map<std::string, MyGraphicsFormat>&	GetGraphicsMap();


	 // --- conversion ---
	VEC2 ScreenToWorld(VEC2 _coords);
	VEC2 WorldToScreen(VEC2 _coords);



	// ---- for testing -------
	void TestRotate(float _scale)
	{
		rotateRad += _scale;
	}

	void TestScale(float _scale)
	{
		scaleFactor += _scale;
	}

	void TestCameraX(float _pos)
	{
		m_mainCameraPtr->MoveX(static_cast<int>(_pos));
	}

	void TestCameraY(float _pos)
	{
		m_mainCameraPtr->MoveY(static_cast<int>(_pos));
	}

	void TestCameraZoom(float _zoom)
	{
		m_mainCameraPtr->AdjustZoom(_zoom);
	}

	// not used
	void MouseCollide()
	{
		// do smth
	}


	void UpdateScreenSize();

	std::shared_ptr<Camera> GetMainCameraPtr();


	//int AddTexture(std::shared_ptr<Texture> _texture);

	void AddSpriteList(std::shared_ptr<SpriteList> _spriteListPtr);

	std::shared_ptr<SpriteList> GetSpriteList(unsigned int _id);
	std::shared_ptr<SpriteList> GetSpriteList(std::string _name);
	int GetSpriteListID(std::string _name);
	std::string GetSpriteName(unsigned int _id);

	unsigned int GetRenderID(const std::string& texturename);


	void AddAnimaton(std::shared_ptr<Animation> _animationPtr);
	void AddAnimationComponent(std::shared_ptr<AnimationComponent> _animationCompPtr);
	
	std::shared_ptr<Animation> GetAnimation(int _id);
	int GetAnimationID(std::string _name);
	std::string GetAnimationName(int _id);
	//AnimationManager& GetAnimationManager();

	void InitLevelMap(const std::string& _path);


	// Getters and Setters
	std::shared_ptr<TextRenderer> GetTextRenderer();
	glm::mat4 GetProjMatrix();
	glm::mat4 GetViewMatrix();

	std::shared_ptr<std::array<glm::mat4, INSTANCELIMIT2>> GetModelMatrices();

	void LoadLevel();

	void CreateEmitter(	EmitterType _type, VEC2 _position, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed,
						float _emitInterval, float _emitNum, VEC2 _emitArea, float _angle, float _movementSpeed, std::string _archetype);

	void CreateEmitter(	EmitterType _type, std::shared_ptr<Transform> transformPtr, VEC2 _position, float _startDelay, float _lifetime, float _particleLifetime, float _fadeSpeed,
						float _emitInterval, float _emitNum, VEC2 _emitArea, float _angle, float _movementSpeed, std::string _archeType);
	
	int GetGraphicsWidth();
	int GetGraphicsHeight();

	AABB GetCameraAABB();

	void SetAlpha(float _a) { tmpAlpha = _a; }
};

//A global pointer to the instance of the graphics
extern std::shared_ptr<Graphics> GRAPHICS;


