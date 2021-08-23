#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#ifndef IMGUI_TRICK_ONCE
#define IMGUI_TRICK_ONCE

enum NotifyState : int {
	ImTrickNotify_Success = 0,
	ImTrickNotify_Warning = 1,
	ImTrickNotify_Danger = 2,
	ImTrickNotify_Default = 3
};

namespace ImTricks {

	/*
	// The Animations namespace contains everything you need to easily create animations in your ImGui menus.
	*/
	namespace Animations {

		/*
		// Usage:
		// int trickInt = ImTricks::Animations::FastLerpInt("trickInt", enable_animation, 0, 255, 15);
		// draw->AddRectFilled(p, p + ImVec2(30, 30), ImColor(255, 255, 255, trickInt));
		*/
		inline int FastLerpInt(const char* identifier, bool state, int min, int max, int speed) {

			static std::map<const char*, int> valuesMapInt;
			auto value = valuesMapInt.find(identifier);

			if (value == valuesMapInt.end()) {
				valuesMapInt.insert({ identifier,  0 });
				value = valuesMapInt.find(identifier);
			}

			const float frameRateSpeed = speed * (1.f - ImGui::GetIO().DeltaTime);

			if (state) {
				if (value->second < max)
					value->second += frameRateSpeed;
			}
			else {
				if (value->second > min)
					value->second -= frameRateSpeed;
			}

			value->second = std::clamp(value->second, min, max);

			return value->second;
		}

		/*
		// Usage:
		// float trickFloat = ImTricks::Animations::FastLerpInt("trickFloat", enable_animation, 0.f, 1.f, 0.05f);
		// draw->AddRectFilled(p, p + ImVec2(30, 30), ImColor(1.f, 1.f, 1.f, trickFloat));
		*/
		inline float FastLerpFloat(const char* identifier, bool state, float min, float max, float speed) {

			static std::map<const char*, float> valuesMapFloat;
			auto value = valuesMapFloat.find(identifier);

			if (value == valuesMapFloat.end()) {
				valuesMapFloat.insert({ identifier, 0.f });
				value = valuesMapFloat.find(identifier);
			}

			const float frameRateSpeed = speed * (1.f - ImGui::GetIO().DeltaTime);

			if (state) {
				if (value->second < max)
					value->second += frameRateSpeed;
			}
			else {
				if (value->second > min)
					value->second -= frameRateSpeed;
			}

			value->second = std::clamp(value->second, min, max);

			return value->second;
		}

		/*
		// Usage:
		//	float trickFloat = ImTricks::Animations::FastLerpFloat("header", check, 0.f, 1.f, 0.05f);
		//	draw->AddRectFilled(p, p + ImVec2(513, 30), ImTricks::Animations::FastColorLerp(ImColor(255, 0, 0), ImColor(0, 255, 0), trickFloat));
		*/
		inline ImColor FastColorLerp(ImColor start, ImColor end, float stage)
		{
			ImVec4 lerp = ImLerp(
				ImVec4(start.Value.x, start.Value.y, start.Value.z, start.Value.w),
				ImVec4(end.Value.x, end.Value.y, end.Value.z, end.Value.w),
				stage);

			return ImGui::ColorConvertFloat4ToU32(lerp);
		}
	}

	/*
	// The NotifyManager namespace contains everything you need to easily create notifications in your interface.
	*/
	namespace NotifyManager {

		struct NotifyStruct {
			const char* message;
			NotifyState state;
			ULONGLONG time;
		};

		std::vector<NotifyStruct> notifies;
		inline int duration = 3000;

		/*
		// Description:
		// Responsible for adding notifications to the display queue.
		//
		//	if (ImGui::Button("Create Notify", { 120, 25 }))
		//		ImTricks::NotifyManager::AddNotify("The notification was displayed successfully.", ImTrickNotify_Success);
		*/
		inline void AddNotify(const char* message, NotifyState state) {
			notifies.push_back({ message, state, GetTickCount64() + duration });
		}

		/*
		// Description:
		// Call a function in your render. This function is responsible for displaying and monitoring notifications.
		//
		// Usage:
		// ImTricks::NotifyManager::HandleNotifies(ImGui::GetOverlayDrawList());
		*/
		inline void HandleNotifies(ImDrawList* draw) {

			if (notifies.empty())
				return;

			const auto ScreenSize = ImGui::GetIO().DisplaySize;
			ImVec2 NotifyPos = ScreenSize - ImVec2(320.f, 50.f);

			auto DrawNotify = [&draw, &NotifyPos](NotifyStruct notify) {

				const auto NotifyEndPos = NotifyPos + ImVec2(300, 30);

				draw->AddRectFilled(NotifyPos, NotifyEndPos, ImGui::GetColorU32(ImGuiCol_PopupBg), ImGui::GetStyle().PopupRounding);

				auto StateColor = ImColor(45, 45, 45);

				switch (notify.state) {
				case ImTrickNotify_Success: StateColor = ImColor(0, 255, 0); break;
				case ImTrickNotify_Warning: StateColor = ImColor(130, 255, 0); break;
				case ImTrickNotify_Danger: StateColor = ImColor(255, 0, 0); break;
				case ImTrickNotify_Default:
				default:
					StateColor = ImColor(45, 45, 45);
					break;
				}

				draw->AddRectFilled(NotifyPos, ImVec2(NotifyPos.x + 5.f, NotifyPos.y + 30), StateColor, 1.f);

				const auto TextSize = ImGui::CalcTextSize(notify.message);
				const auto TextPos = NotifyPos + ImVec2(15.f, 15.f - TextSize.y / 2.f);

				draw->AddText(TextPos, ImGui::GetColorU32(ImGuiCol_Text), notify.message);

				NotifyPos.y -= 40.f;
			};

			for (auto notify : notifies) {
				if (notify.time < GetTickCount64())
					continue;

				DrawNotify(notify);
			}
		}
	}


	namespace Widgets {

		// I don't understand why ocornut didn't add ImColor support to ColorEdit.
		inline void ColorEdit3(const char* label, ImColor& color, ImGuiColorEditFlags flags) {
			static float col[3] = { color.Value.x, color.Value.y, color.Value.z };

			if (ImGui::ColorEdit3(label, col, flags))
				color = ImColor(col[0], col[1], col[2]);
		}

		inline void ColorEdit4(const char* label, ImColor& color, ImGuiColorEditFlags flags) {
			static float col[4] = { color.Value.x, color.Value.y, color.Value.z , color.Value.w };

			if (ImGui::ColorEdit4(label, col, flags))
				color = ImColor(col[0], col[1], col[2], col[3]);
		}
	}
}

#endif