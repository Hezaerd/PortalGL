#include <cassert>
#include <btBulletDynamicsCommon.h>

class DynamicCharacterController : public btActionInterface
{
public:

	DynamicCharacterController(btRigidBody* body, const btCapsuleShape* shape);
	virtual ~DynamicCharacterController() = default;
	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep) override;
	void debugDraw(btIDebugDraw* debugDrawer) override {}
	void setMovementDirection(const btVector3& walkDirection);
	const btVector3& getMovementDirection() const;
	void resetStatus();
	bool canJump() const;
	void jump(const btVector3& dir = btVector3(0, 0, 0));
	const btRigidBody* getBody() const;
	btScalar mMaxLinearVelocity2 = pow(15 / 3.6, 2);
	btScalar mWalkAccel = 25.0;
	btScalar mJumpSpeed = 4.5;
	/// The damp to use to stop movements
	btScalar mSpeedDamping = 0.1;

	btScalar mShapeRadius;
	btScalar mShapeHalfHeight;
	btVector3 mGravity;
	btScalar mMaxCosGround = -SIMDSQRT12;
	btScalar mRadiusThreshold = 1e-2;

protected:

	/**
	 * Default constructor, to be used by child classes.
	 * \warning The child classes must set the rigid body when using this, and
	 * set the shape dimensions.
	 */
	DynamicCharacterController();
	void setupBody();
	void updateVelocity(float dt);
	btRigidBody* mRigidBody;
	btVector3 mMoveDirection;
	bool mJump;
	btVector3 mJumpDir;
	bool mOnGround = false;
	btVector3 mGroundPoint;
};

class FindGround : public btCollisionWorld::ContactResultCallback {
public:
	btScalar FindGround::addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0, int partId0, int index0,
		const btCollisionObjectWrapper* colObj1, int partId1, int index1)
	{
		if (colObj0->m_collisionObject == mMe && !mHaveGround) {
			const btTransform& transform = mMe->getWorldTransform();
			// Orthonormal basis (just rotations) => can just transpose to invert
			btMatrix3x3 invBasis = transform.getBasis().transpose();
			btVector3 localPoint = invBasis * (cp.m_positionWorldOnB - transform.getOrigin());
			localPoint[2] += mShapeHalfHeight;
			float r = localPoint.length();
			float cosTheta = localPoint[2] / r;

			if (fabs(r - mController->mShapeRadius) <= mRadiusThreshold && cosTheta < mMaxCosGround) {
				mHaveGround = true;
				mGroundPoint = cp.m_positionWorldOnB;
			}
		}
		return 0;
	}

	btRigidBody* mMe;
	DynamicCharacterController* mController;
	// Assign some values, in some way
	float mShapeHalfHeight;
	float mRadiusThreshold;
	float mMaxCosGround;
	bool mHaveGround = false;
	btVector3 mGroundPoint;
};