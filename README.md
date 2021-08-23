# ImTricks

ImTricks combines useful functions for quickly creating animated menus on ImGui.


```cpp

// This turns into

static std::map<ImGuiID, Animation> AnimationMap;
auto AnimationHandle = AnimationMap.find(ImGui::GetItemID());

if (AnimationHandle == AnimationMap.end())
{
	AnimationMap.insert({ ImGui::GetItemID(), {0.0f} });
	AnimationHandle = AnimationMap.find(ImGui::GetItemID());
}

if (*v)
{
	if (AnimationHandle->second.size < 1.0f)
		  AnimationHandle->second.size += 0.1f * (60 / ImGui::GetIO().Framerate);
}
else if (!(*v))
{
	if (AnimationHandle->second.size > 0.0f)
  		AnimationHandle->second.size -= 0.1f * (60 / ImGui::GetIO().Framerate);
}

// Into this

float trickFloat = ImTricks::Animations::FastLerpInt("trickFloat", enable_animation, 0.f, 1.f, 0.1f);

```

## List of functions

```cpp
int - ImTricks::Animations::FastLerpInt(const char* identifier, bool state, int min, int max, int speed);
float - ImTricks::Animations::FastLerpFloat(const char* identifier, bool state, float min, float max, float speed);
ImColor - ImTricks::Animations::FastColorLerp(ImColor start, ImColor end, float stage);
```
