#include "DynamicCharacterController.h"

#include "Singleton.h"


DynamicCharacterController::DynamicCharacterController(btRigidBody* body,
													   const btCapsuleShape* shape)
{
	mRigidBody = body;

	mShapeRadius = shape->getRadius();
	mShapeHalfHeight = shape->getHalfHeight();

	setupBody();
	resetStatus();
}

DynamicCharacterController::DynamicCharacterController()
{
	mRigidBody = nullptr;
	resetStatus();
}

void DynamicCharacterController::setupBody()
{
	assert(mRigidBody);
	mRigidBody->setSleepingThresholds(0.0, 0.0);
	mRigidBody->setAngularFactor(0.0);
	mGravity = mRigidBody->getGravity();
}

void DynamicCharacterController::updateAction(btCollisionWorld* collisionWorld,
	btScalar deltaTimeStep)
{
	FindGround callback;
	collisionWorld->contactTest(mRigidBody, callback);
	mOnGround = callback.mHaveGround;
	mGroundPoint = callback.mGroundPoint;

	updateVelocity(deltaTimeStep);


	if (mOnGround ) {
		/* Avoid going down on ramps, if already on ground, and clearGravity()
		is not enough */
		mRigidBody->setGravity({ 0, 0, 0 });
	}
	else {
		mRigidBody->setGravity(mGravity);
	}
}

void DynamicCharacterController::updateVelocity(float dt)
{
	btTransform transform;
	mRigidBody->getMotionState()->getWorldTransform(transform);
	btMatrix3x3& basis = transform.getBasis();
	/* Orthonormal basis - can just transpose to invert.
	Also Bullet does this in the btTransform::inverse */
	btMatrix3x3 inv = basis.transpose();

	btVector3 linearVelocity = inv * mRigidBody->getLinearVelocity();

	if (mMoveDirection.fuzzyZero() && mOnGround) {
		linearVelocity *= mSpeedDamping;
	}
	else if (mOnGround || linearVelocity[2] > 0) {
		btVector3 dv = mMoveDirection * (mWalkAccel * dt);
		linearVelocity += dv;

		btScalar speed2 = pow(linearVelocity.x(), 2)
			+ pow(linearVelocity.y(), 2);
		if (speed2 > mMaxLinearVelocity2) {
			btScalar correction = sqrt(mMaxLinearVelocity2 / speed2);
			linearVelocity[0] *= correction;
			linearVelocity[1] *= correction;
		}
	}

	if (mJump) {
		linearVelocity += mJumpSpeed * mJumpDir;
		mJump = false;

	}

	mRigidBody->setLinearVelocity(basis * linearVelocity);
}





void DynamicCharacterController::setMovementDirection(
	const btVector3& walkDirection)
{
	mMoveDirection = walkDirection;
	mMoveDirection.setZ(0);
	if (!mMoveDirection.fuzzyZero()) {
		mMoveDirection.normalize();
	}
}

const btVector3& DynamicCharacterController::getMovementDirection() const
{
	return mMoveDirection;
}

void DynamicCharacterController::resetStatus()
{
	mMoveDirection.setValue(0, 0, 0);
	mJump = false;
	mOnGround = false;

}

bool DynamicCharacterController::canJump() const
{
	return mOnGround;
}

void DynamicCharacterController::jump(const btVector3& dir)
{
	if (!canJump()) {
		return;
	}

	mJump = true;

	mJumpDir = dir;
	if (dir.fuzzyZero()) {
		mJumpDir.setValue(0, 0, 1);
	}
	mJumpDir.normalize();
}

const btRigidBody* DynamicCharacterController::getBody() const
{
	return mRigidBody;
}



