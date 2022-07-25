#pragma once
#include "Core/Debug/Assertion.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Debug/Log.h"

#include "Mat4/Mat4.h"
#include "Vec3/Vec3.h"
 
#include "Resources/Model.h"
#include "Resources/Shader.h"
#include "LowRenderer/Camera.h"
#include "Resources/Resourcesmanager.h"
#include "LowRenderer/Mesh.h"
#include "LowRenderer/Light.h"
#include "Resources/Texture.h"
#include "WorldPhysics.h"
#include "PhysicsComponent.h"
#include "player.h"
#include "GameObject.h"
#include "Singleton.h"
#include "Button.h"
#include "DebugDrawer.h"
#include "CollisionSystem.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "Turret.h"
#include "UIManager.h"

#include "Oscillator.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cstdio>


#include <rapidjson/rapidjson.h>
#include <cstdio>
#include <ostream>
#include <iostream>

namespace Resources
{
	class Application
	{
	private:
		//var
		GLFWwindow* window;
		lm::Mat4<float> projectionMat;
		const int SCR_WIDTH;
		const int SCR_HEIGHT;
		WorldPhysics* world;
		DebugDrawer* m_pDebugDrawer;
		/*UIManager* UIManager;*/

		//Delta Time
		float currentTime;
		float lastTime;

		// Editor
		int selectedItem;
		int newGameObject;
		bool addComponentButton;
		bool renameGameObject;

		//Mouse Input
		double lastMouseX;
		double lastMouseY;
		double mouseX;
		double mouseY;
		double mouseOffSetX;
		double	mouseOffSetY;
		bool firstMouse;

		//private function
		void initWindow(const char* title);
		void initGlad();
		void initOpenGLOption();
		void initCam();
		void initLight();
		void initGameObject();
		void initImgui();
		void initMusic();

		void updateLights();
		void updateGameObject();

		void GameLoop();
		void updatePhysics();
	public:
		//constructor / destructor
		Application(
			char const* title,
			const int SCR_WIDTH,
			const int SCR_HEIGHT);

		//public function
		void Run();
		void updateDelta();
		void updateMouseInput();

		void save();
		void load();

		// Get
		GLFWwindow* getWindow();
		int getSelectedItem();
		int getNewGameObject();
		bool getAddComponentButton();
		bool getRenameGameObject();

		// Set
		void setCursor(bool cursor);
		void setSelectedItem(int selectedItem);
		void setNewGameObject(int newGameObject);
		void setAddComponentButton(bool addComponentButton);
		void setRenameGameObject(bool renameGameObject);

		void updateInput();
		void processInput(GLFWwindow* window);

		//gameObject
		std::vector<GameObject*> gameObjects;
		std::vector<GameObject*> gameObjectsCopy;

		//clear color
		lm::vec4 clear_color;

		//editor
		void editorShortcut();
		bool editor = true;
		bool showDebugDrawer = true;
		bool showImgui = true;

		//static function
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	};
}

void drawHierarchy(Application* app);
void drawAddGameObject(Application* app);
void addChildrenToHierarchy(Application* app, GameObject* parent);
void deleteChildrenFromHierarchy(Application* app, GameObject* parent);
void drawMainMenuBar(Application* app);
void drawInspector(Application* app, int selectedObject);
void drawComponent(Application* app, int selectedObject);
void drawPossibleComponent(Application* app, int selectedObject);
void renderImGui();
void addComponentFromLoad(GameObject* g, rapidjson::Value::ConstValueIterator i);
