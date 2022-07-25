#include "Button.h"
#include "Singleton.h"
#include "AIMesh.h"

void Button::Activate()
{
	SINGLETON.playSound(5, false); //button activate
	GameObject* t = SINGLETON.findObjectWithName("turret");
	if (t != nullptr) {
		t->removeComponent("Turret");
	}
}