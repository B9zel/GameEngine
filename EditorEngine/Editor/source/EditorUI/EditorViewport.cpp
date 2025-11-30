#include <Editor/includes/EditorUI/EditorViewport.h>
#include <Render/includes/Framebuffer.h>
#include <Editor/includes/EditorEngine.h>
#include <Editor/includes/EditorViewportClient.h>
#include <Runtime/includes/SceneComponent.h>
#include <Core/includes/InputDevice.h>
#include <imgui/imgui.h>
#include <glad/glad.h>
#include <ImGuizmo/ImGuizmo.h>
#include <Core/includes/World.h>

namespace Editor
{

	static float NormalizeDeg(float a)
	{
		float x = std::fmod(a + 180.0f, 360.0f);
		if (x < 0.0f) x += 360.0f;
		return x - 180.0f;
	}

	static FVector NormalizeDegVec3(const FVector& v)
	{
		return FVector(NormalizeDeg(v.GetX()), NormalizeDeg(v.GetY()), NormalizeDeg(v.GetZ()));
	}


	static bool IsDraw = false;
	static int CrentDraw = 0;
	EditorViewport::EditorViewport()
	{
		m_GuizmoOpiration = ImGuizmo::OPERATION::TRANSLATE;
	}
	void EditorViewport::Draw()
	{
		ImGui::Begin("Viewport", 0, ImGuiWindowFlags_Modal);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		static ImVec2 lastSize = ImGui::GetWindowSize();

		uint32 Texture = FrameBuffer->GetColorAttachmentID(0);

		ImVec2 avail = ImGui::GetContentRegionAvail();
		ImGuiIO& io = ImGui::GetIO();

		
		ImVec2 texLogicalSize = ImVec2((float)FrameBuffer->GetSpecifiction().Width / io.DisplayFramebufferScale.x,
			(float)FrameBuffer->GetSpecifiction().Height / io.DisplayFramebufferScale.y);
		
		
		bool fitPreserveAspect = true;

		
		ImVec2 displaySize = texLogicalSize;
		if (fitPreserveAspect) {
			if (displaySize.x > avail.x || displaySize.y > avail.y) {
				float scale = std::min(avail.x / displaySize.x, avail.y / displaySize.y);
				displaySize.x *= scale;
				displaySize.y *= scale;
			}
		}
		else {
		
			displaySize = avail;
		}
		

		// ѕоложим Image в child (чтобы не по€вл€лс€ scroll)
		ImGui::BeginChild("ViewportChild", displaySize, false,
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground);
		// optionally выровн€ть по центру
		ImVec2 cur = ImGui::GetCursorPos();
		float padX = (ImGui::GetContentRegionAvail().x - displaySize.x) * 0.5f;
		if (padX > 0.0f) ImGui::SetCursorPosX(cur.x + padX);

		ImGui::Image((void*)Texture, displaySize, ImVec2(0, 1), ImVec2(1, 0)); //, ImVec2(0, 1), ImVec2(1, 0)
		m_IsFocusedViewport = ImGui::IsItemHovered();

		ImVec2 Space = avail;
		if (Space.x != lastSize.x || Space.y != lastSize.y)
		{
			FrameBuffer->Resize(abs(Space.x), abs(Space.y));
			lastSize = Space;
			EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{0} {1}", abs(Space.x), (Space.y));
		}




		//glBindFramebuffer(GL_READ_FRAMEBUFFER, 1);
		//glReadBuffer(GL_COLOR_ATTACHMENT0);
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
		//glBlitFramebuffer(0, 0, FrameBuffer->GetSpecifiction().Width, FrameBuffer->GetSpecifiction().Height,
		//	0, 0, FrameBuffer->GetSpecifiction().Width, FrameBuffer->GetSpecifiction().Height,
		//	GL_COLOR_BUFFER_BIT, GL_NEAREST);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//FrameBuffer->Bind();
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 mouse = ImGui::GetMousePos();
		float localX = mouse.x - pos.x;
		float localY = abs(mouse.y - pos.y);

		static int NumDraw = 2;


		ImVec2 p0 = ImGui::GetItemRectMin();
		ImVec2 p1 = ImGui::GetItemRectMax();
		//ImVec2 size = ImVec2(p1.x - p0.x, p1.y - p0.y);
		const int Width = FrameBuffer->GetSpecifiction().Width;
		const int Height = FrameBuffer->GetSpecifiction().Height;

		//if (ImGui::IsKeyPressed(ImGuiKey_MouseLeft) && ImGui::IsKeyDown(ImGuiKey_1))
		if (false)
		{
			//localY = FrameBuffer->GetSpecifiction().Height - (localY);
			//localX -=  ImGui::GetScrollX();
			//localY -=  ImGui::GetScrollY();
			FrameBuffer->Bind();
			EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{0}, {1}, {2}", FrameBuffer->ReadPixel(1, localX, localY), localX, localY);


			IsDraw = true;

		}
		if (IsDraw)
		{

			FrameBuffer->Bind();
			for (int i = 0; i < Width; i++)
			{
				for (int j = 0; j < Height; j++)
				{
					// верхний левый угол картинки
					ImVec2 size = ImGui::GetItemRectSize();   // размер картинки


					//relY = Height - relY;
					//  оординаты мыши относительно картинки
					int a = FrameBuffer->ReadPixel(1, i, Height - j);
					if (a != 0)
					{
						float relX = i + p0.x + ImGui::GetScrollX();
						float relY = j + p0.y + ImGui::GetScrollY();
						ImDrawList* draw_list = ImGui::GetWindowDrawList();
						draw_list->AddCircle(ImVec2(relX, relY), 1.0f, IM_COL32(255, 0, 0, 255));

						EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{0}, {1} : {2}", relX, relY, a);
					}

				}
			}
			if (CrentDraw > NumDraw)
			{
				IsDraw = false;
			}
			CrentDraw++;
		}

		// Guizmo
		FrameBuffer->Bind();
		if (m_IsFocusedViewport)
		{
			if (CoreEngine::InputDevice::GetIsButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver())
			{
				ImVec2 pos = ImGui::GetCursorScreenPos();
				ImVec2 mouse = ImGui::GetMousePos();

				float localX = mouse.x - pos.x;
				float localY = abs(mouse.y - pos.y);


				int32 IdActor = FrameBuffer->ReadPixel(1, localX, localY);

				auto* FindedActor = OwnerEditor->GetWorld()->GetActorPredicate([&](CoreEngine::Runtime::Actor* Actor)
					{
						return Actor->GetUUID().GetID() == IdActor;
					});
				if (FindedActor)
				{
					OwnerEditor->SetSelectedObject(FindedActor);
				}
			}



			if (m_CanChangeOpirations)
			{
				if (CoreEngine::InputDevice::GetIsKeyPressed(GLFW_KEY_Q))
				{
					m_GuizmoOpiration = ImGuizmo::OPERATION::BOUNDS;
				}
				if (CoreEngine::InputDevice::GetIsKeyPressed(GLFW_KEY_W))
				{
					m_GuizmoOpiration = ImGuizmo::OPERATION::TRANSLATE;
				}
				if (CoreEngine::InputDevice::GetIsKeyPressed(GLFW_KEY_E))
				{
					m_GuizmoOpiration = ImGuizmo::OPERATION::ROTATE;
				}
				if (CoreEngine::InputDevice::GetIsKeyPressed(GLFW_KEY_R))
				{
					m_GuizmoOpiration = ImGuizmo::OPERATION::SCALE;
				}
			}
		}

		


		if (auto* WorldObject = GetSceneComponentFromSelected())
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			static FVector LastGuizmoRotate;

			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
			EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Pre change {0} {1} {2}", WorldObject->GetComponentRotation().GetX(), WorldObject->GetComponentRotation().GetY(), WorldObject->GetComponentRotation().GetZ())
			
			FMatrix4x4 ObjectMatrix = WorldObject->GetTransform().ToMatrix();
			FMatrix4x4 ViewMatrix = (OwnerEditor->GetViewpoertClient()->GetViewMatrix());
			FMatrix4x4 ProjectionMatrix = OwnerEditor->GetViewpoertClient()->CreateProjection();
			//ImGuizmo::DrawGrid(Math::GetValuePtr((ViewMatrix)), Math::GetValuePtr(ProjectionMatrix), Math::GetValuePtr(ObjectMatrix), 10);
			ImGuizmo::Manipulate(Math::GetValuePtr(ViewMatrix), Math::GetValuePtr(ProjectionMatrix), static_cast<ImGuizmo::OPERATION>(m_GuizmoOpiration), ImGuizmo::MODE::LOCAL, Math::GetValuePtr(ObjectMatrix));
			if (ImGuizmo::IsUsing())
			{
				FVector Location, Rotation, Scale;
				ImGuizmo::DecomposeMatrixToComponents(Math::GetValuePtr(ObjectMatrix), Math::GetValuePtr(Location.vector), Math::GetValuePtr(Rotation.vector), Math::GetValuePtr(Scale.vector));

				WorldObject->SetComponentLocation(Location);

				EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{0} {1} {2}", Rotation.GetX(), Rotation.GetY(), Rotation.GetZ());
				EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Last {0} {1} {2}", LastGuizmoRotate.GetX(), LastGuizmoRotate.GetY(), LastGuizmoRotate.GetZ());
				EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Minus {0} {1} {2}", (Rotation - LastGuizmoRotate).GetX(), (Rotation - LastGuizmoRotate).GetY(), (Rotation - LastGuizmoRotate).GetZ());
				WorldObject->SetComponentRotation(Rotation - LastGuizmoRotate + WorldObject->GetComponentRotation());
				EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Post change {0} {1} {2}\n", WorldObject->GetComponentRotation().GetX(), WorldObject->GetComponentRotation().GetY(), WorldObject->GetComponentRotation().GetZ())

				LastGuizmoRotate = WorldObject->GetComponentRotation();
				
				WorldObject->SetComponentScale(Scale);
			}
		
		}

		FrameBuffer->UnBind();
		
		ImGui::EndChild();
		ImGui::PopStyleVar();
		ImGui::End();
		//ImGui::Begin("Drag and Drop Example");

		/*static int sourceValue = 42;
		static int targetValue = 0;*/
		//////
		// »сточник
		//static const char* items[] = { "Cube", "Sphere", "Light" };
		//static const char* selected = nullptr;

		//ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));
		//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
		//ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
		//	if (ImGui::Selectable("Drag"))
		//	{
		//		EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Drag Started");
		//		//ImGui::SetDragDropPayload("OBJECT_NAME", items[i], strlen(items[i]) + 1);
		//	}
		//	ImGui::PopStyleColor(3);
		//	ImGui::PopStyleVar(2);
		//for (int i = 0; i < IM_ARRAYSIZE(items); i++)
		//{
		//	ImGui::Selectable(items[i]);
		//	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		//	{
		//		ImGui::SetDragDropPayload("OBJECT_NAME", items[i], strlen(items[i]) + 1);
		//		ImGui::Text("Dragging %s", items[i]);
		//		ImGui::EndDragDropSource();
		//	}

		//	if (ImGui::BeginDragDropTarget())
		//	{
		//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBJECT_NAME"))	
		//		{
		//			const char* dropped = (const char*)payload->Data;
		//			selected = dropped;
		//		}
		//		ImGui::EndDragDropTarget();
		//	}
		//}

		//if (selected)
		//	ImGui::Text("Selected object: %s", selected);
		/////////

		//ImGui::End();
	}

	void EditorViewport::SetFrameBuffer(const SharedPtr<CoreEngine::Render::Framebuffer>& Buffer)
	{
		FrameBuffer = Buffer;
	}

	bool EditorViewport::GetIsFocused() const
	{
		return m_IsFocusedViewport;
	}

	void EditorViewport::OnActiveMoveCamera(bool IsActive)
	{
		m_CanChangeOpirations = !IsActive;
	}

	CoreEngine::Runtime::SceneComponent* EditorViewport::GetSceneComponentFromSelected() const
	{
		if (!OwnerEditor || !OwnerEditor->GetSelectedObject()) return nullptr;

		if (OwnerEditor->GetSelectedObject()->GetClass()->IsChildClassOf(CoreEngine::Runtime::Actor::GetStaticClass()))
		{
			return dynamic_cast<CoreEngine::Runtime::Actor*>(OwnerEditor->GetSelectedObject())->GetRootComponent();
		}
		return dynamic_cast<CoreEngine::Runtime::SceneComponent*>(OwnerEditor->GetSelectedObject());
	}

}