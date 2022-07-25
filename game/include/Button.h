#pragma once
#include "GameObject.h"

class Button : public Component
{
public:
	Button() {
		name = "Button";
	}
	void Activate();
};