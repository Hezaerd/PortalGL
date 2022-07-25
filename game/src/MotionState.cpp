#include "MotionState.h"

void MotionState::setWorldTransform(const btTransform& centerOfMassWorldTrans)
{
	btDefaultMotionState::setWorldTransform(centerOfMassWorldTrans);
	if (GraphicMatrix != nullptr)
	{
		btScalar worldMatrix[16];
		btTransform worldTrans;
		getWorldTransform(worldTrans);
		worldTrans.getOpenGLMatrix(worldMatrix);
		*GraphicMatrix = ScalarToMat4(worldMatrix);
	}
}

void MotionState::getWorldTransform(btTransform& centerOfMassWorldTrans) const
{
	btDefaultMotionState::getWorldTransform(centerOfMassWorldTrans);
}

void MotionState::setGraphMat(lm::mat4* m)
{
	this->GraphicMatrix = m;
}

void MotionState::GetWorldTransform(btScalar* transform)
{
	btTransform trans;
	getWorldTransform(trans);
	trans.getOpenGLMatrix(transform);
}

lm::mat4 MotionState::ScalarToMat4(btScalar* btsc)
{
	Vec4<float> r1(btsc[0], btsc[1], btsc[2], btsc[3]);
	Vec4<float> r2(btsc[4], btsc[5], btsc[6], btsc[7]);
	Vec4<float> r3(btsc[8], btsc[9], btsc[10], btsc[11]);
	Vec4<float> r4(btsc[12], btsc[13], btsc[14], btsc[15]);

	return mat4(r1, r2, r3, r4);
}