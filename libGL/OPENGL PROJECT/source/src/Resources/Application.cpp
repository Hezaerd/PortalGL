#include "Resources/Application.h"

using namespace irrklang;
using namespace Resources;

void Application::initWindow(const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(this->SCR_WIDTH, this->SCR_HEIGHT, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Application::framebuffer_size_callback);

	// Enable VSYNC
	glfwSwapInterval(1);

	this->window = window;
}

void Application::initGlad()
{
	// glad: load all OpenGL function pointers
// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	GLint flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(Application::glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}

void Resources::Application::initOpenGLOption()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Application::initCam()
{
	GameObject* editorCamera = new GameObject("Main Camera");
	editorCamera->addComponent(new LowRenderer::Camera(editorCamera));
	editorCamera->addComponent(new rotateWithMouse(0.5f, true));
	editorCamera->addComponent(new freeMovement(10));
	SINGLETON.editorCam = editorCamera;
	SINGLETON.setCam(editorCamera);
}

void Application::initLight()
{
	GameObject* dirLight = new GameObject("Directional Light 1");
	dirLight->addComponent(new LowRenderer::DirectionLight(lm::vec3(1, 1, 1), 0.1f, 0.1f, 0.3f));
	gameObjects.emplace_back(dirLight);

	GameObject* pointLight = new GameObject("Point Light 1");
	pointLight->addComponent(new LowRenderer::PointLight(lm::vec3(0, 0, 1), 0.2f, 0.2f, 0.4f, 1, 0.2f, 0));
	pointLight->localTransform.translate(lm::vec3(0, 5, 0));
	gameObjects.emplace_back(pointLight);

	GameObject* pointLight2 = new GameObject("Point Light 2");
	pointLight2->addComponent(new LowRenderer::PointLight(lm::vec3(1, 0, 0), 0.2f, 0.2f, 0.4f, 1, 0.2f, 0));
	pointLight2->localTransform.translate(lm::vec3(0, 20, 0));
	gameObjects.emplace_back(pointLight2);

	GameObject* pointLight3 = new GameObject("Point Light 3");
	pointLight3->addComponent(new LowRenderer::PointLight(lm::vec3(1, 1, 1), 0.2f, 0.2f, 0.4f, 1, 0.2f, 0));
	pointLight3->localTransform.translate(lm::vec3(30, 30, 0));
	gameObjects.emplace_back(pointLight3);

	GameObject* spotLight = new GameObject("Spot Light 1");
	spotLight->addComponent(new LowRenderer::SpotLight(lm::vec3(0, 1, 0), 0.4f, 0.4f, 0.6f, 1, 0.1f, 0));
	spotLight->localTransform.translate(lm::vec3(20, 38, 0));
	spotLight->localTransform.setRotation(lm::vec3(0, 0, -90));
	gameObjects.emplace_back(spotLight);
}

void Application::initGameObject()
{
	GameObject* box1 = new GameObject("Box 1");
	GameObject* button = new GameObject("Button");
	GameObject* floor = new GameObject("Floor");
	GameObject* floor2 = new GameObject("Floor2");
	GameObject* floor3 = new GameObject("Floor3");
	GameObject* floor4 = new GameObject("Floor4");
	GameObject* floor5 = new GameObject("Floor5");
	GameObject* roof = new GameObject("roof");

	GameObject* wall = new GameObject("Wall");
	GameObject* wall2 = new GameObject("Wall2");
	GameObject* wall3 = new GameObject("Wall3");
	GameObject* wall4 = new GameObject("Wall4");
	GameObject* wall5 = new GameObject("Wall5");
	GameObject* wall6 = new GameObject("Wall6");

	GameObject* player = new GameObject("Player");
	GameObject* playerCam = new GameObject("PlayerCam");
	GameObject* gun = new GameObject("gun");
	GameObject* turret = new GameObject("turret");
	GameObject* bluePortal = new GameObject("bluePortal");
	GameObject* orangePortal = new GameObject("orangePortal");

	//same order pls
	gameObjects.emplace_back(box1);
	gameObjects.emplace_back(button);
	gameObjects.emplace_back(floor);
	gameObjects.emplace_back(floor2);
	gameObjects.emplace_back(floor3);
	gameObjects.emplace_back(floor4);
	gameObjects.emplace_back(floor5);
	gameObjects.emplace_back(roof);

	gameObjects.emplace_back(wall);
	gameObjects.emplace_back(wall2);
	gameObjects.emplace_back(wall3);
	gameObjects.emplace_back(wall4);
	gameObjects.emplace_back(wall5);
	gameObjects.emplace_back(wall6);

	gameObjects.emplace_back(player);
	gameObjects.emplace_back(playerCam);
	gameObjects.emplace_back(gun);
	gameObjects.emplace_back(turret);
	gameObjects.emplace_back(bluePortal);
	gameObjects.emplace_back(orangePortal);

	player->addComponent(new playerMovement(6, 2));
	player->addComponent(new PlayerComponent(3));
	player->localTransform.setScale(lm::vec3(1, 2, 1));
	player->localTransform.translate(lm::vec3(0, 1, 0));
	player->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::DYNAMICS, lm::vec3(1, 1, 1), 1.f, player, false, 1));

	turret->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/magenoir.obj", "libGL/OPENGL PROJECT/assets/textures/floor.jpg"));
	turret->localTransform.setScale(lm::vec3(1, 1, 1));
	turret->localTransform.translate(lm::vec3(24, 25, 0));
	turret->addComponent(new Turret());

	playerCam->addComponent(new LowRenderer::Camera(playerCam));
	playerCam->addComponent(new rotateWithMouse(0.5f, false));
	playerCam->localTransform.translate(lm::vec3(0, 2, 0));
	playerCam->setParent(player);

	gun->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/staff.obj", "libGL/OPENGL PROJECT/assets/textures/container.jpg"));
	gun->localTransform.setScale(lm::vec3(0.02, 0.02f,0.02f));
	gun->localTransform.setPosition(lm::vec3(1, 0.9f, 0.3f));
	gun->localTransform.setRotation(lm::vec3(0, 0, -60));
	gun->setParent(playerCam);

	bluePortal->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/pBlue.jpg"));
	bluePortal->localTransform.translate(lm::vec3(0, -100, 0));
	bluePortal->localTransform.setScale(lm::vec3(0.1f, 3, 1));
	bluePortal->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1, 1, 1), 0.f, bluePortal, false, 1));
	bluePortal->addComponent(new PortalCollision());

	orangePortal->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/pOrange.jpg"));
	orangePortal->localTransform.translate(lm::vec3(5, -100, 0));
	orangePortal->localTransform.setScale(lm::vec3(0.1f, 3, 1));
	orangePortal->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1, 1, 1), 0.f, orangePortal, false, 1));
	orangePortal->addComponent(new PortalCollision());

	box1->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/chest.obj", "libGL/OPENGL PROJECT/assets/textures/container.jpg"));
	box1->localTransform.translate(lm::vec3(20, 25, 0));
	box1->localTransform.setRotation(lm::vec3(0, 90, 0));
	box1->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::DYNAMICS, lm::vec3(1.f, 1.f, 1.f), 1.f, box1));
	box1->addComponent(new Grabable());

	button->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/orb.obj", "libGL/OPENGL PROJECT/assets/textures/pBlue.jpg"));
	button->localTransform.setScale(lm::vec3(0.5f, 0.5f, 0.5f));
	button->localTransform.translate(lm::vec3(15, 26, 7));
	button->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 1.f, button));
	button->addComponent(new LowRenderer::PointLight(lm::vec3(1, 1, 1), 1, 1, 1, 1, 0.5f, 0));
	button->addComponent(new Button());

	floor->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/stoneground.jpg"));
	floor->localTransform.setScale(lm::vec3(10, 1, 10));
	floor->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 0.f, floor));

	roof->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/stoneground.jpg"));
	roof->localTransform.setScale(lm::vec3(40, 1, 40));
	roof->localTransform.translate(lm::vec3(0, 45, 0));
	roof->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 0.f, roof));

	floor2->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/stoneground.jpg"));
	floor2->localTransform.translate(lm::vec3(6, 1, 0));
	floor2->localTransform.setScale(lm::vec3(4, 4, 12));
	floor2->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 0.f, floor2));

	floor3->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/stoneground.jpg"));
	floor3->localTransform.translate(lm::vec3(-5, 10, 0));
	floor3->localTransform.setScale(lm::vec3(4, 0.2, 12));
	floor3->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 0.f, floor3));

	floor4->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/stoneground.jpg"));
	floor4->localTransform.translate(lm::vec3(5, 15, 0));
	floor4->localTransform.setScale(lm::vec3(4, 0.2, 12));
	floor4->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 0.f, floor4));

	floor5->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/stoneground.jpg"));
	floor5->localTransform.translate(lm::vec3(39, 25, 0));
	floor5->localTransform.setScale(lm::vec3(30, 0.2, 12));
	floor5->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 0.f, floor5));

	wall->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/stonewall.jpg"));
	wall->localTransform.translate(lm::vec3(10, 10, 0));
	wall->localTransform.setScale(lm::vec3(1, 15, 12));
	wall->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 0.f, wall));
	wall->addComponent(new CanPlacePortal());

	wall2->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/stonewall.jpg"));
	wall2->localTransform.translate(lm::vec3(-10, 10, 0));
	wall2->localTransform.setScale(lm::vec3(1, 35, 12));
	wall2->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 0.f, wall2));
	wall2->addComponent(new CanPlacePortal());

	wall3->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/stonewall.jpg"));
	wall3->localTransform.translate(lm::vec3(0, 10, 10));
	wall3->localTransform.setScale(lm::vec3(42, 35, 1));
	wall3->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 0.f, wall3));
	wall3->addComponent(new CanPlacePortal());

	wall4->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/stonewall.jpg"));
	wall4->localTransform.translate(lm::vec3(0, 10, -10));
	wall4->localTransform.setScale(lm::vec3(42, 35, 1));
	wall4->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 0.f, wall4));
	wall4->addComponent(new CanPlacePortal());

	wall5->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/stonewall.jpg"));
	wall5->localTransform.translate(lm::vec3(39, 32, 0));
	wall5->localTransform.setRotation(lm::vec3(0,90,0));
	wall5->localTransform.setScale(lm::vec3(13, 13, 1));

	wall6->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/group.jpg"));
	wall6->localTransform.translate(lm::vec3(64, 40, 0));
	wall6->localTransform.setRotation(lm::vec3(180, 90, 0));
	wall6->localTransform.setScale(lm::vec3(13, 13, 1));

	SINGLETON.gameCam = playerCam;
}

void Application::initImgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	ImGui::StyleColorsDark();
}

void Application::initMusic()
{
	SINGLETON.soundEngine = createIrrKlangDevice();
}

void Application::updateLights()
{
	//send lights to shader
	int dirCounter = 0;
	int pointCounter = 0;
	int spotCounter = 0;

	//can we simplify that ??
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->getComponent<LowRenderer::DirectionLight>("dirLight") != nullptr) {
			gameObjects[i]->getComponent<LowRenderer::DirectionLight>("dirLight")->sendToShader(SINGLETON.getShader(), dirCounter, gameObjects[i]);
			dirCounter++;
		}
		if (gameObjects[i]->getComponent<LowRenderer::PointLight>("pointLight") != nullptr) {
			gameObjects[i]->getComponent<LowRenderer::PointLight>("pointLight")->sendToShader(SINGLETON.getShader(), pointCounter, gameObjects[i]);
			pointCounter++;
		}
		if (gameObjects[i]->getComponent<LowRenderer::SpotLight>("spotLight") != nullptr) {
			gameObjects[i]->getComponent<LowRenderer::SpotLight>("spotLight")->sendToShader(SINGLETON.getShader(), spotCounter, gameObjects[i]);
			spotCounter++;
		}
	}

	SINGLETON.getShader()->setInt("NR_DIR_LIGHTS", dirCounter);
	SINGLETON.getShader()->setInt("NR_POINT_LIGHTS", pointCounter);
	SINGLETON.getShader()->setInt("NR_SPOT_LIGHTS", spotCounter);
}

void Application::updateGameObject()
{
	if (editor) {
		for (const auto& gameObject : gameObjects)
		{
			//only update matrix, light, mesh
			gameObject->UpdateRender();
		}
	}
	else
	{
		for (const auto& gameObject : gameObjects)
		{
			gameObject->Update();
		}
	}
}

void Application::GameLoop()
{
	while (!glfwWindowShouldClose(this->window))
	{
		//time
		updateDelta();

		//input
		updateInput();

		//physics update
		updatePhysics();

		//render
		glClearColor(clear_color.X(), clear_color.Y(), clear_color.Z(), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use shader
		SINGLETON.getShader()->use();

		//update lights
		updateLights();

		//send cam
		SINGLETON.getShader()->setVec3f(SINGLETON.getCam()->worldTransform.getPosition(), "viewPos");

		//update GameObject
		updateGameObject();

		if (SINGLETON.getCam() == SINGLETON.editorCam)
			SINGLETON.getCam()->Update();

		SINGLETON.getShader()->unUse();

		// Render imgui
		if (showImgui) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			drawHierarchy(this);
			drawAddGameObject(this);
			drawMainMenuBar(this);
			renderImGui();
		}

		//debug drawer
		if (showDebugDrawer) {
			m_pDebugDrawer->SetViewProjection(this->projectionMat * SINGLETON.getCam()->getComponent<LowRenderer::Camera>("Camera")->getViewMatrix());
			world->getWorld()->debugDrawWorld();
		}

		glfwSwapBuffers(this->window);
	}
}

void Application::updatePhysics()
{
	if (editor) {
		WORLD_PHYSICS.getWorld()->stepSimulation(0);
	}
	else
	{
		WORLD_PHYSICS.getWorld()->stepSimulation(SINGLETON.delta);
	}
}

Application::Application(char const* title, const int SCR_WIDTH, const int SCR_HEIGHT)
	:
	window(nullptr),
	SCR_WIDTH(SCR_WIDTH),
	SCR_HEIGHT(SCR_HEIGHT)
{
	SINGLETON.setProjectionMatrix(SCR_WIDTH, SCR_HEIGHT);
	this->projectionMat = SINGLETON.getProjectionMatrix();
	this->world = &WORLD_PHYSICS;

	// Delta Time
	this->currentTime = 0.f;
	this->lastTime = 0.f;

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffSetX = 0.0;
	this->mouseOffSetY = 0.0;
	this->firstMouse = true;
	this->selectedItem = -1;
	this->newGameObject = 0;
	this->addComponentButton = false;
	this->renameGameObject = false;
	this->initWindow(title);
	this->initGlad();
	this->initOpenGLOption();
	m_pDebugDrawer = new DebugDrawer(btIDebugDraw::DBG_DrawWireframe);
	// add the debug drawer to the world
	this->world->getWorld()->setDebugDrawer(m_pDebugDrawer);

	SINGLETON.app = this;
}

void Application::Run()
{
	//music/sounds
	initMusic();

	//cam
	initCam();

	//shader
	SINGLETON.setShader(SINGLETON.getResources()->Create<Shader>("libGL/OPENGL PROJECT/source/shaders/core_vertex.glsl", "libGL/OPENGL PROJECT/source/shaders/core_fragment.glsl"));

	//lights
	initLight();

	//scene
	initGameObject();

	// Setup Dear ImGui context
	initImgui();

	//game loop
	GameLoop();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//end of function
	glfwTerminate();
}

void Resources::Application::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	SINGLETON.mouseOffSetX = this->mouseX - this->lastMouseX;
	SINGLETON.mouseOffSetY = this->mouseY - this->lastMouseY;

	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}

GLFWwindow* Resources::Application::getWindow()
{
	return window;
}

int Resources::Application::getSelectedItem()
{
	return this->selectedItem;
}

int Resources::Application::getNewGameObject()
{
	return this->newGameObject;
}

bool Resources::Application::getAddComponentButton()
{
	return this->addComponentButton;
}

bool Resources::Application::getRenameGameObject()
{
	return this->renameGameObject;
}

void Resources::Application::save()
{
	rapidjson::Document d;
	d.SetObject();

	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	rapidjson::Value v(rapidjson::kArrayType);

	//save all gameObjects
	for (int i = 0; i < gameObjects.size(); i++)
	{
		lm::vec3 pos(gameObjects[i]->worldTransform.getPosition());
		lm::vec3 rot(gameObjects[i]->worldTransform.getRotation());
		lm::vec3 scale(gameObjects[i]->worldTransform.getScale());

		rapidjson::Value g(rapidjson::kObjectType);

		// name
		g.AddMember("name", rapidjson::StringRef(gameObjects[i]->name.c_str()), allocator);
		//id
		g.AddMember("id", gameObjects[i]->id, allocator);
		//parent
		if (gameObjects[i]->parent != nullptr) {
			g.AddMember("parent", gameObjects[i]->parent->id, allocator);
		}

		//pos
		g.AddMember("Px", pos.X(), allocator);
		g.AddMember("Py", pos.Y(), allocator);
		g.AddMember("Pz", pos.Z(), allocator);

		//rotation
		g.AddMember("Rx", rot.X(), allocator);
		g.AddMember("Ry", rot.Y(), allocator);
		g.AddMember("Rz", rot.Z(), allocator);

		//scale
		g.AddMember("Sx", scale.X(), allocator);
		g.AddMember("Sy", scale.Y(), allocator);
		g.AddMember("Sz", scale.Z(), allocator);

		rapidjson::Value components(rapidjson::kArrayType);

		std::map<std::string, Component*>::iterator it;
		std::vector<std::string> keys;
		for (it = gameObjects[i]->components.begin(); it != gameObjects[i]->components.end(); it++)
		{
			keys.push_back(it->first);
		}
		for (int j = 0; j < keys.size(); j++)
		{
			it = gameObjects[i]->components.find(keys[j]);
			if (it != gameObjects[i]->components.end()) {
				//add to components
				components.PushBack(it->second->getJsonObj(allocator), allocator);
			}
		}

		//add components to this gameObject
		g.AddMember("components", components, allocator);
		//add to list
		v.PushBack(g, allocator);
	}

	d.AddMember("gameObjects", v, d.GetAllocator());

	d.AddMember("clearColorx", clear_color.X(), d.GetAllocator());
	d.AddMember("clearColory", clear_color.Y(), d.GetAllocator());
	d.AddMember("clearColorz", clear_color.Z(), d.GetAllocator());

	FILE* fp = fopen("save.json", "wb"); // non-Windows use "w"

	char writeBuffer[65536]{};
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	d.Accept(writer);

	fclose(fp);
}

void Resources::Application::load()
{
	FILE* fp = fopen("save.json", "rb");

	if (fp == nullptr) {
		return;
	}

	char readBuffer[65536]{};
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	//if value exist
	if (!d.HasMember("gameObjects")) {
		return;
	}

	// delete the actual scene
	//SINGLETON.stopAllSounds();
	WORLD_PHYSICS.removeAllRigidBodies();
	for (auto g : gameObjects)
	{
		delete g;
	}
	gameObjects.clear();
	SINGLETON.gmCount = 1;

	const rapidjson::Value& a = d["gameObjects"];

	//for all gameObjects
	for (rapidjson::Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr) {
		GameObject* g = new GameObject(itr->GetObj()["name"].GetString());
		g->name = itr->GetObj()["name"].GetString();
		g->id = itr->GetObj()["id"].GetInt();
		lm::vec3 pos(itr->GetObj()["Px"].GetFloat(), itr->GetObj()["Py"].GetFloat(), itr->GetObj()["Pz"].GetFloat());
		lm::vec3 rot(itr->GetObj()["Rx"].GetFloat(), itr->GetObj()["Ry"].GetFloat(), itr->GetObj()["Rz"].GetFloat());
		lm::vec3 scale(itr->GetObj()["Sx"].GetFloat(), itr->GetObj()["Sy"].GetFloat(), itr->GetObj()["Sz"].GetFloat());
		g->localTransform.setPosition(pos);
		g->localTransform.setRotation(rot);
		g->localTransform.setScale(scale);
		g->UpdateMatrix();

		rapidjson::GenericArray components = itr->GetObj()["components"].GetArray();
		for (rapidjson::Value::ConstValueIterator i = components.Begin(); i != components.End(); ++i)
		{
			addComponentFromLoad(g, i);
		}
		gameObjects.push_back(g);
	}
	int counter = 0;
	for (rapidjson::Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr) {
		if (itr->GetObj().HasMember("parent")) {
			gameObjects[counter]->setParent(gameObjects[static_cast<std::vector<GameObject*, std::allocator<GameObject*>>::size_type>(itr->GetObj()["parent"].GetInt()) - 1]);
		}
		counter++;
	}

	clear_color = lm::vec4(d["clearColorx"].GetFloat(), d["clearColory"].GetFloat(), d["clearColorz"].GetFloat(), 1);

	fclose(fp);
}

void Resources::Application::updateDelta()
{
	this->currentTime = static_cast<float>(glfwGetTime());
	SINGLETON.unscaledDelta = this->currentTime - this->lastTime;
	SINGLETON.delta = SINGLETON.timescale * SINGLETON.unscaledDelta;
	this->lastTime = this->currentTime;
}

void Resources::Application::updateInput()
{
	glfwPollEvents();
	processInput(window);
	updateMouseInput();
	editorShortcut();
}

void Resources::Application::setCursor(bool cursor)
{
	if (cursor)
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Resources::Application::setSelectedItem(int selectedItem)
{
	this->selectedItem = selectedItem;
}

void Resources::Application::setNewGameObject(int newGameObject)
{
	this->newGameObject = newGameObject;
}

void Resources::Application::setAddComponentButton(bool addComponentButton)
{
	this->addComponentButton = addComponentButton;
}

void Resources::Application::setRenameGameObject(bool renameGameObject)
{
	this->renameGameObject = renameGameObject;
}

void Resources::Application::editorShortcut()
{
	if (SINGLETON.CTRL_O) {
		load();
	}
	if (SINGLETON.CTRL_S) {
		save();
	}
}

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SINGLETON.setProjectionMatrix(width, height);
}

void Application::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (editor)
			glfwSetWindowShouldClose(window, true);
		else
		{
			setCursor(false);
			showImgui = true;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		SINGLETON.W = true;
	}
	else
	{
		SINGLETON.W = false;
	}

	if (SINGLETON.CTRL_S == false)
	{
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			if (SINGLETON.LCTRL) {
				SINGLETON.CTRL_S = true;
			}
			else
			{
				SINGLETON.S = true;
			}
		}
		else
		{
			SINGLETON.S = false;
		}
	}

	if (SINGLETON.CTRL_O == false)
	{
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			if (SINGLETON.LCTRL) {
				SINGLETON.CTRL_O = true;
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		SINGLETON.A = true;
	}
	else
	{
		SINGLETON.A = false;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		SINGLETON.E = true;
	}
	else
	{
		SINGLETON.E = false;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		SINGLETON.D = true;
	}
	else
	{
		SINGLETON.D = false;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		SINGLETON.SPACE = true;
	}
	else
	{
		SINGLETON.SPACE = false;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		SINGLETON.LCTRL = true;
	}
	else
	{
		SINGLETON.LCTRL = false;
		SINGLETON.CTRL_O = false;
		SINGLETON.CTRL_S = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		SINGLETON.LMB = true;
	}
	else
	{
		SINGLETON.LMB = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		SINGLETON.RMB = true;
	}
	else
	{
		SINGLETON.RMB = false;
	}
}

void APIENTRY Application::glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

void drawHierarchy(Application* app)
{
	ImGui::SetNextWindowPos(ImVec2(0, 19));
	ImGui::SetNextWindowSize(ImVec2(300, 680));
	ImGui::Begin("PortalGL :", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	ImGui::Text("Scene :");

	addChildrenToHierarchy(app, nullptr);

	ImGui::End();
}

void drawAddGameObject(Application* app)
{
	ImGui::SetNextWindowPos(ImVec2(0, 699));
	ImGui::SetNextWindowSize(ImVec2(300, 320));
	ImGui::Begin("Objects :", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	if (ImGui::Button("Add gameObject")) {
		app->gameObjects.emplace_back(new GameObject("new object " + std::to_string(app->gameObjects.size())));
	}

	ImGui::End();
}

void addChildrenToHierarchy(Application* app, GameObject* parent) {
	for (auto child : app->gameObjects) {
		if (child->parent == parent) {
			if (ImGui::TreeNode(child->getName().c_str())) {
				app->setSelectedItem(child->id - 1);
				addChildrenToHierarchy(app, child);
				drawInspector(app, app->getSelectedItem());
				ImGui::TreePop();
				if (child->id < 0) {
					return;
				}
			}
		}
	}
}

void deleteChildrenFromHierarchy(Application* app, GameObject* parent) {
	for (auto child : app->gameObjects) {
		if (child->parent == parent) {
			GameObject* g = child;
			deleteChildrenFromHierarchy(app, g);
			app->gameObjects.erase(app->gameObjects.begin() + g->id - 1);
			for (int i = g->id - 1; i < app->gameObjects.size(); i++)
			{
				app->gameObjects[i]->id--;
			}
			delete g;
		}
	}
}

void drawMainMenuBar(Application* app)
{
	const char* btText;
	if (app->editor) {
		btText = "Start";
	}
	else
	{
		btText = "Stop";
	}

	//ImGui::PushStyleVar()
	ImGui::BeginMainMenuBar();

	// Add a file menu
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
		{
			// do something
		}
		if (ImGui::MenuItem("Open", "Ctrl+O"))
		{
			app->load();
		}
		if (ImGui::MenuItem("Save", "Ctrl+S"))
		{
			app->save();
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Exit", "Escape"))
		{
			glfwSetWindowShouldClose(app->getWindow(), true);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Game"))
	{
		if (ImGui::MenuItem(btText))
		{
			app->editor = !app->editor;
			if (app->editor) {
				app->load();
				SINGLETON.setCam(SINGLETON.editorCam);
			}
			else
			{
				app->save();
				if (SINGLETON.gameCam != nullptr) {
					SINGLETON.setCam(SINGLETON.gameCam);
					app->setCursor(true);
					app->showImgui = false;
					app->showDebugDrawer = false;
				}
				for (auto g : app->gameObjects)
				{
					g->Start();
				}
			}
		}
		if (ImGui::MenuItem("change Camera")) {
			if (SINGLETON.getCam() == SINGLETON.editorCam) {
				SINGLETON.setCam(SINGLETON.gameCam);
			}
			else
			{
				SINGLETON.setCam(SINGLETON.editorCam);
			}
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Show"))
	{
		if (ImGui::MenuItem("Show debug drawer")) {
			app->showDebugDrawer = !app->showDebugDrawer;
		}
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void drawInspector(Application* app, int selectedObject)
{
	ImGui::SetNextWindowPos(ImVec2(300, 699));
	ImGui::SetNextWindowSize(ImVec2(300, 320));
	ImGui::Begin("Inspector", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	lm::vec3 pos(app->gameObjects[selectedObject]->localTransform.getPosition());
	lm::vec3 rot(app->gameObjects[selectedObject]->localTransform.getRotation());
	lm::vec3 scal(app->gameObjects[selectedObject]->localTransform.getScale());

	ImGui::Text(app->gameObjects[selectedObject]->name.c_str());
	ImGui::SameLine();
	if (ImGui::Button("Rename"))
	{
		app->setRenameGameObject(true);
	}
	if (app->getRenameGameObject())
	{
		char buf[128]{};
		strcpy(buf, app->gameObjects[selectedObject]->name.c_str());

		ImGui::SetNextWindowPos(ImVec2(300, 665));
		ImGui::SetNextWindowSize(ImVec2(300, 35));
		ImGui::Begin("Rename", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

		if (ImGui::InputText(" ", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			bool sameName = false;
			int test = 0;
			for (auto& g : app->gameObjects)
			{
				if (buf == g->name)
				{
					sameName = true;
					break;
				}
				test++;
			}

			if (sameName)
			{
				std::string newBuf;
				newBuf = buf;
				newBuf += std::to_string(test);
				app->gameObjects[selectedObject]->setName(newBuf);
				app->setRenameGameObject(false);
			}
			else
			{
				app->gameObjects[selectedObject]->setName(buf);
				app->setRenameGameObject(false);
			}
		}

		ImGui::End();
	}

	ImGui::Text("id : % d", app->gameObjects[selectedObject]->id);
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
	{
		GameObject* g = app->gameObjects[selectedObject];
		selectedObject = -1;
		deleteChildrenFromHierarchy(app, g);
		app->gameObjects.erase(app->gameObjects.begin() + g->id - 1);
		for (int i = g->id - 1; i < app->gameObjects.size(); i++)
		{
			app->gameObjects[i]->id--;
		}
		delete g;
		ImGui::End();
		return;
	}
	ImGui::Separator();
	ImGui::Text("Position :");
	ImGui::PushItemWidth(250);
	ImGui::DragFloat("Px", pos.Xptr());
	ImGui::DragFloat("Py", pos.Yptr());
	ImGui::DragFloat("Pz", pos.Zptr());
	ImGui::Text("Rotation :");
	ImGui::DragFloat("Rx", rot.Xptr());
	ImGui::DragFloat("Ry", rot.Yptr());
	ImGui::DragFloat("Rz", rot.Zptr());
	ImGui::Text("Scale :");
	ImGui::DragFloat("Sx", scal.Xptr());
	ImGui::DragFloat("Sy", scal.Yptr());
	ImGui::DragFloat("Sz", scal.Zptr());

	app->gameObjects[selectedObject]->localTransform.setPosition(pos);
	app->gameObjects[selectedObject]->localTransform.setRotation(rot);
	app->gameObjects[selectedObject]->localTransform.setScale(scal);

	ImGui::End();

	drawComponent(app, app->getSelectedItem());
}

void drawComponent(Application* app, int selectedObject)
{
	ImGui::SetNextWindowPos(ImVec2(600, 699));
	ImGui::SetNextWindowSize(ImVec2(300, 320));
	ImGui::Begin("Component", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	//add component
	ImGui::Text("Components : ");
	ImGui::SameLine();
	if (ImGui::Button("Add"))
	{
		app->setAddComponentButton(true);
	}
	if (app->getAddComponentButton())
	{
		drawPossibleComponent(app, selectedObject);
	}
	ImGui::Separator();

	//remove component
	for (auto& component : app->gameObjects[selectedObject]->components)
	{
		ImGui::Text(component.first.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Remove"))
		{
			app->gameObjects[selectedObject]->removeComponent(component.first);
			break;
		}
	}

	//parent
	ImGui::Text("Parent :");
	static int parent_id = -1;
	if (app->gameObjects[selectedObject]->parent != nullptr) {
		ImGui::Text("parent name : %s", app->gameObjects[selectedObject]->parent->name);
		ImGui::Text("parent id : %d", app->gameObjects[selectedObject]->parent->id);
		parent_id = app->gameObjects[selectedObject]->parent->id;
		if (ImGui::Button("Remove parent")) {
			app->gameObjects[selectedObject]->setParent(nullptr);
		}
	}
	ImGui::InputInt("change parent : ", &parent_id);
	if (parent_id > 0 && static_cast<unsigned long long>(parent_id) - 1 < app->gameObjects.size() && parent_id != app->gameObjects[selectedObject]->id) {
		if (ImGui::Button("Set parent"))
		{
			app->gameObjects[selectedObject]->setParent(app->gameObjects[static_cast<std::vector<GameObject*, std::allocator<GameObject*>>::size_type>(parent_id) - 1]);
		}
	}

	ImGui::End();
}

void drawPossibleComponent(Application* app, int selectedObject)
{
	ImGui::SetNextWindowPos(ImVec2(675, 549));
	ImGui::SetNextWindowSize(ImVec2(150, 150));
	ImGui::Begin("Component List", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	ImGui::Text("Components   ");
	ImGui::SameLine();
	if (ImGui::SmallButton("X"))
	{
		app->setAddComponentButton(false);
	}
	ImGui::Separator();
	if (ImGui::Button("dirLight"))
	{
		app->gameObjects[selectedObject]->addComponent(new LowRenderer::DirectionLight(lm::vec3(1.f, 1.f, 1.f), 0.5f, 0.5f, 1.0f));

		app->setAddComponentButton(false);
	}
	if (ImGui::Button("spotLight"))
	{
		app->gameObjects[selectedObject]->addComponent(new LowRenderer::SpotLight(lm::vec3(1, 1, 1), 0.5f, 0.5f, 1.0f, 1, 0.2f, 0));

		app->setAddComponentButton(false);
	}
	if (ImGui::Button("pointLight"))
	{
		app->gameObjects[selectedObject]->addComponent(new LowRenderer::PointLight(lm::vec3(1, 1, 1), 0.5f, 0.5f, 1.0f, 1, 0.2f, 0));

		app->setAddComponentButton(false);
	}
	if (ImGui::Button("Mesh"))
	{
		std::cout << "Mesh component" << std::endl;
		app->gameObjects[selectedObject]->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/container.jpg"));
		app->setAddComponentButton(false);
	}
	if (ImGui::Button("physics"))
	{
		app->gameObjects[selectedObject]->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::STATIC, lm::vec3(1.f, 1.f, 1.f), 0, app->gameObjects[selectedObject]));

		app->setAddComponentButton(false);
	}
	if (ImGui::Button("Oscillator"))
	{
		std::cout << "Oscillator component" << std::endl;
		app->gameObjects[selectedObject]->addComponent(new Oscillator(lm::vec3(1.f, 0.f, 0.f), app->gameObjects[selectedObject]->localTransform.getPosition(), 10.f, 5.f));

		app->setAddComponentButton(false);
	}

	ImGui::End();
}

void renderImGui()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void addComponentFromLoad(GameObject* g, rapidjson::Value::ConstValueIterator i) {
	if (i->GetObj().HasMember("name")) {
		std::string name = i->GetObj()["name"].GetString();
		if (name == "dirLight")
		{
			lm::vec3 col(i->GetObj()["Colorx"].GetFloat(), i->GetObj()["Colory"].GetFloat(), i->GetObj()["Colorz"].GetFloat());
			g->addComponent(new LowRenderer::DirectionLight(col, i->GetObj()["diffuse"].GetFloat(), i->GetObj()["ambient"].GetFloat(), i->GetObj()["specular"].GetFloat()));
		}
		else if (name == "spotLight")
		{
			lm::vec3 col(i->GetObj()["Colorx"].GetFloat(), i->GetObj()["Colory"].GetFloat(), i->GetObj()["Colorz"].GetFloat());
			g->addComponent(new LowRenderer::SpotLight(col, i->GetObj()["diffuse"].GetFloat(), i->GetObj()["ambient"].GetFloat(), i->GetObj()["specular"].GetFloat(),
				i->GetObj()["constant"].GetFloat(), i->GetObj()["linear"].GetFloat(), i->GetObj()["quadratic"].GetFloat(), i->GetObj()["cutOff"].GetFloat(), i->GetObj()["outerCutOff"].GetFloat()));
		}
		else if (name == "pointLight")
		{
			lm::vec3 col(i->GetObj()["Colorx"].GetFloat(), i->GetObj()["Colory"].GetFloat(), i->GetObj()["Colorz"].GetFloat());
			g->addComponent(new LowRenderer::PointLight(col, i->GetObj()["diffuse"].GetFloat(), i->GetObj()["ambient"].GetFloat(), i->GetObj()["specular"].GetFloat(),
				i->GetObj()["constant"].GetFloat(), i->GetObj()["linear"].GetFloat(), i->GetObj()["quadratic"].GetFloat()));
		}
		else if (name == "Mesh")
		{
			g->addComponent(new LowRenderer::Mesh(i->GetObj()["modelPath"].GetString(), i->GetObj()["texturePath"].GetString()));
		}
		else if (name == "Test")
		{
			g->addComponent(new testComponent(i->GetObj()["speed"].GetFloat()));
		}
		else if (name == "physics")
		{
			g->addComponent(new PhysicsComponent(
				i->GetObj()["flag"].GetInt(),
				lm::vec3(i->GetObj()["halfExtentX"].GetFloat(), i->GetObj()["halfExtentY"].GetFloat(), i->GetObj()["halfExtentZ"].GetFloat()),
				i->GetObj()["mass"].GetFloat(),
				g,
				i->GetObj()["sleep"].GetBool(),
				i->GetObj()["rigidBody"].GetBool()));
		}
		else if (name == "Oscillator") {
			g->addComponent(new Oscillator(
				lm::vec3(i->GetObj()["direction.x"].GetFloat(), i->GetObj()["direction.y"].GetFloat(), i->GetObj()["direction.z"].GetFloat()),
				i->GetObj()["speed"].GetFloat(),
				i->GetObj()["amplitude"].GetFloat(),
				i->GetObj()["frequency"].GetFloat()));
		}
		else if (name == "playerMovement")
		{
			g->addComponent(new playerMovement(
				i->GetObj()["speed"].GetFloat(),
				i->GetObj()["force"].GetFloat()
			));
		}
		else if (name == "rotateWithMouse")
		{
			g->addComponent(new rotateWithMouse(
				i->GetObj()["sensi"].GetFloat(),
				i->GetObj()["editor"].GetBool()
			));
		}
		else if (name == "Camera")
		{
			g->addComponent(new LowRenderer::Camera(g));
			SINGLETON.gameCam = g;
		}
		else if (name == "portalCollision")
		{
			g->addComponent(new PortalCollision());
		}
		else if (name == "CanPlacePortal")
		{
			g->addComponent(new CanPlacePortal());
		}
		else if (name == "Grabable")
		{
			g->addComponent(new Grabable());
		}
		else if (name == "Button")
		{
			g->addComponent(new Button());
		}
		else if (name == "Turret")
		{
			g->addComponent(new Turret());
		}
		else if (name == "PlayerComponent") {
			g->addComponent(new PlayerComponent(
				i->GetObj()["life"].GetInt()
			));
		}
	}
}