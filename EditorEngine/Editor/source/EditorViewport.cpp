#include <Editor/includes/EditorViewport.h>
#include <Render/includes/Framebuffer.h>
#include <imgui/imgui.h>
#include <glad/glad.h>

namespace Editor
{
	static bool IsDraw = false;
	static int CrentDraw = 0;
	EditorViewport::EditorViewport()
	{
	}
	void EditorViewport::DrawViewport(SharedPtr<CoreEngine::Render::Framebuffer>& FrameBuffer)
	{
		ImGui::Begin("Viewport");
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		static ImVec2 lastSize = ImGui::GetWindowSize();
		
		uint32 Texture = FrameBuffer->GetColorAttachmentID(0);

		ImVec2 avail = ImGui::GetContentRegionAvail();
		ImGuiIO& io = ImGui::GetIO();
		
		// реальный размер текстуры в логических (ImGui) пикселях
		ImVec2 texLogicalSize = ImVec2((float)FrameBuffer->GetSpecifiction().Width / io.DisplayFramebufferScale.x,
			(float)FrameBuffer->GetSpecifiction().Height / io.DisplayFramebufferScale.y);

		// выберите поведение: "fit" (вписать в avail) или "fill" (растянуть по ширине/высоте)
		bool fitPreserveAspect = true;

		// вычисляем displaySize
		ImVec2 displaySize = texLogicalSize;
		if (fitPreserveAspect) {
			if (displaySize.x > avail.x || displaySize.y > avail.y) {
				float scale = std::min(avail.x / displaySize.x, avail.y / displaySize.y);
				displaySize.x *= scale;
				displaySize.y *= scale;
			}
		}
		else {
			// fill - растянуть в доступную область (можно сохранить aspect или нет)
			displaySize = avail;
		}

		// Положим Image в child (чтобы не появлялся scroll)
		ImGui::BeginChild("ViewportChild", displaySize, false,
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		// optionally выровнять по центру
		ImVec2 cur = ImGui::GetCursorPos();
		float padX = (ImGui::GetContentRegionAvail().x - displaySize.x) * 0.5f;
		if (padX > 0.0f) ImGui::SetCursorPosX(cur.x + padX);

		ImGui::Image((void*)Texture, displaySize, ImVec2(0, 1), ImVec2(1, 0)); //, ImVec2(0, 1), ImVec2(1, 0)

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
			float localX =  mouse.x - pos.x;
			float localY =  abs(mouse.y - pos.y);
		
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
						// Координаты мыши относительно картинки
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
			FrameBuffer->UnBind();
			ImGui::EndChild();
			ImGui::PopStyleVar();
			ImGui::End();
			ImGui::Begin("Drag and Drop Example");

			static int sourceValue = 42;
			static int targetValue = 0;
			//////
			// Источник
			static const char* items[] = { "Cube", "Sphere", "Light" };
			static const char* selected = nullptr;


			for (int i = 0; i < IM_ARRAYSIZE(items); i++)
			{
				ImGui::Selectable(items[i]);

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("OBJECT_NAME", items[i], strlen(items[i]) + 1);
					ImGui::Text("Dragging %s", items[i]);
					ImGui::EndDragDropSource();
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBJECT_NAME"))
					{
						const char* dropped = (const char*)payload->Data;
						selected = dropped;
					}
					ImGui::EndDragDropTarget();
				}
			}

			if (selected)
				ImGui::Text("Selected object: %s", selected);
			///////

			ImGui::End();
	}
}