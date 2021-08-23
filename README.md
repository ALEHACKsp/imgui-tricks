# ImTricks

ImTricks is a collection of useful functions for expanding / improving the functionality of the ImG ui.

At the moment it has in itself:
* Functions to speed up the creation of animations.
* Functions for creating and displaying notifications.

# Notifies Usage

```cpp

void RenderMyGUI() {

   ...
   if (ImGui::Button("Create Notify", { 120, 25 }))
   	ImTricks::NotifyManager::AddNotify("The notification was displayed successfully.", ImTrickNotify_Success);
   ...
   
   ImTricks::NotifyManager::HandleNotifies(ImGui::GetOverlayDrawList());
}

```


# Animations Usage

```cpp

void RenderMyGUI() {
   ...
   ImGui::Checkbox("Enable Animation", &enable_animation);
   float trickFloat = ImTricks::Animations::FastLerpInt("trickFloat", enable_animation, 0.f, 1.f, 0.05f);
   ImColor colorLerped = ImTricks::Animations::FastColorLerp(ImColor(255, 0, 0), ImColor(0, 255, 0), trickFloat);
   ImGui::GetWindowDrawList()->AddRectFilled(p, p + ImVec2(500, 30), colorLerped);
   ...
}

```

