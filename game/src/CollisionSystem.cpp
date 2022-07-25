#include "CollisionSystem.h"

void CollisionSystem::CheckForCollisionEvents() {
	//check for collisions
	CollisionPairs pairsThisUpdate;

	// iterate through all of the manifolds in the dispatcher
	for (int i = 0; i < WORLD_PHYSICS.getDispatcher()->getNumManifolds(); ++i) {
		// get the manifold
		btPersistentManifold* pManifold = WORLD_PHYSICS.getDispatcher()->getManifoldByIndexInternal(i);

		// get the two bodies that are involved in the collision
		if (pManifold->getNumContacts() > 0) {
			// get the two rigid bodies involved in the collision
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(pManifold->getBody0());
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(pManifold->getBody1());

			// add the pair to the set of pairs
			bool const swapped = pBody0 > pBody1;
			const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
			const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;

			// create the pair
			CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);

			// insert the pair into the current list
			pairsThisUpdate.insert(thisPair);

			// check if the pair is in the last update
			if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end()) {
				__raise onCollisionEnter((btRigidBody*)pSortedBodyA, (btRigidBody*)pSortedBodyB);
			}
		}
	}

	CollisionPairs removedPairs;

	std::set_difference(m_pairsLastUpdate.begin(), m_pairsLastUpdate.end(),
		pairsThisUpdate.begin(), pairsThisUpdate.end(),
		std::inserter(removedPairs, removedPairs.begin()));

	for (CollisionPairs::const_iterator iter = removedPairs.begin(); iter != removedPairs.end(); ++iter) {
		__raise onCollisionExit((btRigidBody*)iter->first, (btRigidBody*)iter->second);
	}

	m_pairsLastUpdate = pairsThisUpdate;
}