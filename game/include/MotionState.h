#pragma once
#include "btBulletDynamicsCommon.h"
#include "mat4/mat4.h"

using namespace lm;
class MotionState : public btDefaultMotionState
{
public:

	MotionState(const btTransform& transform) : btDefaultMotionState(transform) {}

	void GetWorldTransform(btScalar* transform);
	void setWorldTransform(const btTransform& centerOfMassWorldTrans) override;
	void getWorldTransform(btTransform& centerOfMassWorldTrans) const override;
	void setGraphMat(lm::mat4* m);
	lm::mat4* GraphicMatrix = nullptr;
	lm::mat4 ScalarToMat4(btScalar* btsc);
};
