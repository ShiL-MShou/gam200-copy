/*************************************************************************************/
/*
File Name:		MyGui_Gizmo.cpp
Project Name:	WhereGotLate
Author(s):		Kok Zhe Yan			 [100%]


Copyright Information:
All content © 2019 DigiPen (SINGAPORE) Corporation, all rights reserved.
*/
/*************************************************************************************/
#include "stdafx.h"

#include "MyGui_Gizmo.h"
#include "MyGuiManager.h"
#include "..\GAM200\Vendor\ImGuizmo\ImGuizmo.h"

#include "Graphics.h"
#include "Mat4x4.h"

void MyGui_Gizmo::LoadGizmo(bool* _open)
{
	if (!MyGuiManager::isGamePaused)
		return;

	if (!ImGui::Begin("Gizmo", _open))
	{
		ImGui::End();
		return;
	}

	if (MyGuiManager::curr_obj_id == -1)
	{
		ImGui::End();
		return;
	}

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;
	glm::mat4 glm_obj = MyResource.GetObjectWithID(MyGuiManager::curr_obj_id)->has(Transform)->ConvertIntoMat4();
	std::shared_ptr<Transform> transform = MyResource.GetObjectWithID(MyGuiManager::curr_obj_id)->has(Transform);
	std::shared_ptr<SpriteComponent> sprite = MyResource.GetObjectWithID(MyGuiManager::curr_obj_id)->has(SpriteComponent);
	glm::mat4 glm_view = GRAPHICS->GetViewMatrix();
	glm::mat4 glm_proj = GRAPHICS->GetProjMatrix();

	MAT4 view_matrix{};
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			view_matrix[i][j] = glm_view[i][j];

	MAT4 proj_matrix{};
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			proj_matrix[i][j] = glm_proj[i][j];


	MAT4 obj_matrix{};
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			obj_matrix[i][j] = glm_obj[i][j];

	
	MAT4 viewtranpose{};
	MAT4 projtranpose{};
	MAT4 objtranpose{};
	
	view_matrix.Transpose(viewtranpose);
	proj_matrix.Transpose(projtranpose);
	obj_matrix.Transpose(objtranpose);

	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		if (INPUTMGR->GetKeyRelease(MVK_Z))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (INPUTMGR->GetKeyRelease(MVK_X))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (INPUTMGR->GetKeyRelease(MVK_C))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
	}

	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(obj_matrix.m, matrixTranslation, matrixRotation, matrixScale);

	if (sprite && sprite->GetMovementType() == SpriteMovementType::FIXED)
	{
		matrixTranslation[0] = sprite->GetFixedPosition().x;
		matrixTranslation[1] = sprite->GetFixedPosition().y;
		matrixTranslation[2] = 0;
	}

	ImGui::InputFloat3("Tr", matrixTranslation, 3);
	ImGui::InputFloat3("Rt", matrixRotation, 3);
	ImGui::InputFloat3("Sc", matrixScale, 3);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, obj_matrix.m);


	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}

	if (INPUTMGR->GetKeyRelease(MVK_V))
		useSnap = !useSnap;
	ImGui::Checkbox("", &useSnap);
	ImGui::SameLine();

	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		ImGui::InputFloat3("Snap", &snap[0]);
		break;
	case ImGuizmo::ROTATE:
		ImGui::InputFloat("Angle Snap", &snap[0]);
		break;
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &snap[0]);
		break;
	}
	ImGui::Checkbox("Bound Sizing", &boundSizing);
	if (boundSizing)
	{
		ImGui::PushID(3);
		ImGui::Checkbox("", &boundSizingSnap);
		ImGui::SameLine();
		ImGui::InputFloat3("Snap", boundsSnap);
		ImGui::PopID();
	}

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(view_matrix.m, proj_matrix.m, mCurrentGizmoOperation, mCurrentGizmoMode, obj_matrix.m, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
	ImGuizmo::DecomposeMatrixToComponents(obj_matrix.m, matrixTranslation, matrixRotation, matrixScale);
	if (sprite && sprite->GetMovementType() == SpriteMovementType::FIXED)
	{
		sprite->SetFixedPosition(VEC2{ matrixTranslation[0], matrixTranslation[1] });
	}
	else
		transform->SetPosition(VEC2{ matrixTranslation[0], matrixTranslation[1] });
	transform->SetRotation(matrixRotation[2]);
	transform->SetScale(VEC2{ matrixScale[0], matrixScale[1] });

	ImGui::End();
}

void MyGui_Gizmo::Open()
{
}

void MyGui_Gizmo::Update()
{
	if (MyGuiManager::show_gizmo && MyGuiManager::curr_obj_id != 0) LoadGizmo(&MyGuiManager::show_gizmo);
}

void MyGui_Gizmo::Exit()
{
}
