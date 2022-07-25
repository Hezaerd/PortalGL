//#include "Resources/Application.h"
//
//UIManager::UIManager()
//{
//}
//
//// Get methodes
//int UIManager::getSelectedItem()
//{
//	return this->selectedItem;
//}
//
//bool UIManager::getAddComponentButton()
//{
//	return this->addComponentButton;
//}
//
//// Set methodes
//void UIManager::setSelectedItem(int selectedItem)
//{
//	this->selectedItem = selectedItem;
//}
//
//void UIManager::setAddComponentButton(bool addComponentButton)
//{
//	this->addComponentButton = addComponentButton;
//}
//
//// Drawing methodes
//
//void UIManager::drawHierarchy(Application* app) 
//{
//	ImGui::SetNextWindowPos(ImVec2(0, 19));
//	ImGui::SetNextWindowSize(ImVec2(300, 680));
//	ImGui::Begin("PortalGL :", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
//
//	ImGui::Text("Scene :");
//
//	addChildrenToHierarchy(app, nullptr);
//
//	ImGui::Separator();
//	if (ImGui::Button("Add gameObject")) {
//		app->gameObjects.emplace_back(new GameObject("new object " + std::to_string(app->gameObjects.size())));
//	}
//
//	ImGui::End();
//}
//
//void UIManager::addChildrenToHierarchy(Application* app, GameObject* parent) {
//	for (auto child : app->gameObjects) {
//		if (child->parent == parent) {
//			if (ImGui::TreeNode(child->getName().c_str())) {
//				app->setSelectedItem(child->id - 1);
//				drawInspector(app, app->getSelectedItem());
//				addChildrenToHierarchy(app, child);
//				ImGui::TreePop();
//			}
//		}
//	}
//}
//
//void UIManager::drawMainMenuBar(Application* app)
//{
//	const char* btText;
//	if (app->editor) {
//		btText = "Start";
//	}
//	else
//	{
//		btText = "Stop";
//	}
//
//	ImGui::BeginMainMenuBar();
//
//	// Add a file menu
//	if (ImGui::BeginMenu("File"))
//	{
//		if (ImGui::MenuItem("New"))
//		{
//			// should open a new empty scene!
//		}
//		if (ImGui::MenuItem("Open"))
//		{
//			app->load();
//		}
//		if (ImGui::MenuItem("Save"))
//		{
//			app->save();
//		}
//		ImGui::Separator();
//		if (ImGui::MenuItem("Exit"))
//		{
//			glfwSetWindowShouldClose(app->getWindow(), true);
//		}
//		ImGui::EndMenu();
//	}
//
//	if (ImGui::BeginMenu("Game"))
//	{
//		if (ImGui::MenuItem(btText))
//		{
//			app->editor = !app->editor;
//			if (app->editor) {
//				app->load();
//				SINGLETON.setCam(SINGLETON.editorCam);
//			}
//			else
//			{
//				app->save();
//				SINGLETON.setCam(SINGLETON.gameCam);
//				for (auto g : app->gameObjects)
//				{
//					g->Start();
//				}
//			}
//		}
//		if (ImGui::MenuItem("change Camera")) {
//			if (SINGLETON.getCam() == SINGLETON.editorCam) {
//				SINGLETON.setCam(SINGLETON.gameCam);
//			}
//			else
//			{
//				SINGLETON.setCam(SINGLETON.editorCam);
//			}
//		}
//		ImGui::EndMenu();
//	}
//
//	ImGui::EndMainMenuBar();
//}
//
//void UIManager::drawInspector(Application* app, int selectedObject)
//{
//	// Window settings & start window
//	ImGui::SetNextWindowPos(ImVec2(300, 699));
//	ImGui::SetNextWindowSize(ImVec2(300, 320));
//	ImGui::Begin("Inspector", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
//
//	// Var
//	lm::vec3 pos(app->gameObjects[selectedObject]->localTransform.getPosition());
//	lm::vec3 rot(app->gameObjects[selectedObject]->localTransform.getRotation());
//	lm::vec3 scal(app->gameObjects[selectedObject]->localTransform.getScale());
//
//	// ImGui stuff
//	ImGui::Text(app->gameObjects[selectedObject]->name.c_str());
//	ImGui::SameLine();
//	ImGui::Text("   |   id : %d", app->gameObjects[selectedObject]->id);
//	ImGui::Separator();
//	ImGui::Text("Position :");
//	ImGui::PushItemWidth(250);
//	ImGui::DragFloat("Px", pos.Xptr());
//	ImGui::DragFloat("Py", pos.Yptr());
//	ImGui::DragFloat("Pz", pos.Zptr());
//	ImGui::Text("Rotation :");
//	ImGui::DragFloat("Rx", rot.Xptr());
//	ImGui::DragFloat("Ry", rot.Yptr());
//	ImGui::DragFloat("Rz", rot.Zptr());
//	ImGui::Text("Scale :");
//	ImGui::DragFloat("Sx", scal.Xptr());
//	ImGui::DragFloat("Sy", scal.Yptr());
//	ImGui::DragFloat("Sz", scal.Zptr());
//
//	// Applying transformation
//	app->gameObjects[selectedObject]->localTransform.setPosition(pos);
//	app->gameObjects[selectedObject]->localTransform.setRotation(rot);
//	app->gameObjects[selectedObject]->localTransform.setScale(scal);
//
//	// End this window
//	ImGui::End();
//
//	drawComponent(app, app->getSelectedItem());
//}
//
//void UIManager::drawComponent(Application* app, int selectedObject)
//{
//	// Window settings & start window
//	ImGui::SetNextWindowPos(ImVec2(600, 699));
//	ImGui::SetNextWindowSize(ImVec2(300, 320));
//	ImGui::Begin("Component", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
//
//	// ImGui stuff
//	// Draw every components of the gameObject
//	ImGui::Text("Components : ");
//	ImGui::SameLine();
//	if (ImGui::Button("Add"))
//	{
//		setAddComponentButton(true);
//	}
//	if (getAddComponentButton())
//	{
//		drawPossibleComponent(app, selectedObject);
//	}
//	ImGui::Separator();
//
//	//remove component
//	for (auto& component : app->gameObjects[selectedObject]->components)
//	{
//		ImGui::Text(component.first.c_str());
//		ImGui::SameLine();
//		if (ImGui::Button("Remove"))
//		{
//			app->gameObjects[selectedObject]->removeComponent(component.first);
//			break;
//		}
//	}
//
//	//parent
//	ImGui::Text("Parent :");
//	static int parent_id = -1;
//	if (app->gameObjects[selectedObject]->parent != nullptr) {
//		ImGui::Text("parent name : %s", app->gameObjects[selectedObject]->parent->name);
//		ImGui::Text("parent id : %d", app->gameObjects[selectedObject]->parent->id);
//		parent_id = app->gameObjects[selectedObject]->parent->id;
//		if (ImGui::Button("Remove parent")) {
//			app->gameObjects[selectedObject]->setParent(nullptr);
//		}
//	}
//	ImGui::InputInt("change parent : ", &parent_id);
//	if (parent_id > 0 && parent_id - 1 < app->gameObjects.size() && parent_id != app->gameObjects[selectedObject]->id) {
//		if (ImGui::Button("Set parent"))
//		{
//			app->gameObjects[selectedObject]->setParent(app->gameObjects[parent_id - 1]);
//		}
//	}
//
//	ImGui::End();
//}
//
//void UIManager::drawPossibleComponent(Application* app, int selectedObject)
//{
//	ImGui::SetNextWindowPos(ImVec2(675, 549));
//	ImGui::SetNextWindowSize(ImVec2(100, 150));
//	ImGui::Begin("Component List", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
//
//	ImGui::Text("Components : ");
//	ImGui::Separator();
//	if (ImGui::Button("dirLight"))
//	{
//		//app->gameObjects[selectedObject]->addComponent(new LowRenderer::DirectionLight());
//
//		app->setAddComponentButton(false);
//	}
//	if (ImGui::Button("spotLight"))
//	{
//		//app->gameObjects[selectedObject]->addComponent(new LowRenderer::SpotLight());
//
//		app->setAddComponentButton(false);
//	}
//	if (ImGui::Button("pointLight"))
//	{
//		//app->gameObjects[selectedObject]->addComponent(new LowRenderer::PointLight());
//
//		app->setAddComponentButton(false);
//	}
//	if (ImGui::Button("Mesh"))
//	{
//		std::cout << "Mesh component" << std::endl;
//		app->gameObjects[selectedObject]->addComponent(new LowRenderer::Mesh("libGL/OPENGL PROJECT/assets/meshes/f.obj", "libGL/OPENGL PROJECT/assets/textures/container.jpg"));
//		app->setAddComponentButton(false);
//	}
//	if (ImGui::Button("physics"))
//	{
//		//app->gameObjects[selectedObject]->addComponent(new PhysicsComponent());
//
//		app->setAddComponentButton(false);
//	}
//	if (ImGui::Button("Oscillator"))
//	{
//		std::cout << "Oscillator component" << std::endl;
//		app->gameObjects[selectedObject]->addComponent(new Oscillator(lm::vec3(1.f, 0.f, 0.f), app->gameObjects[selectedObject]->localTransform.getPosition(), 10.f, 5.f));
//
//		app->setAddComponentButton(false);
//	}
//
//	ImGui::End();
//}
//
//void UIManager::renderImGui()
//{
//	ImGui::EndFrame();
//	ImGui::Render();
//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//}