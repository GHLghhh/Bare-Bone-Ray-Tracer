// The code is modified from external/simple_irrcache.cpp https://github.com/githole/simple-irradiance-caching
#include "IrradianceCache.h"

#include "../utils/constants.h"

void IrradianceCache::locate_irradiance_points(IrradianceCache::ResultIrradianceQueue& pqueue, KDTreeNode* node, IrradianceQuery &query)
{
	if (node == NULL)
		return;
	const int axis = node->axis;

	double delta;
	switch (axis) {
	case 0: delta = query.search_position.x - node->point->x; break;
	case 1: delta = query.search_position.y - node->point->y; break;
	case 2: delta = query.search_position.z - node->point->z; break;
	}

	Irradiance* point_ptr = static_cast<Irradiance*>(node->point);

	Vec3 dir = (*point_ptr) - query.search_position;
	const double dt = Vec3::Dot(query.normal, dir.Unit());

	bool samePoint = 
	  (fabs(point_ptr->x - query.search_position.x) < kEPSILON)
	  && (fabs(point_ptr->y - query.search_position.y) < kEPSILON)
	  && (fabs(point_ptr->z - query.search_position.z) < kEPSILON);

	// イラディアンスキャッシュの重み
	const double weight = 1.0 / (dir.Length() / point_ptr->R + sqrt(1.0 - Vec3::Dot(query.normal, point_ptr->normal)));
	// "in front" check
	double front =
	  Vec3::Dot((query.search_position - *point_ptr),
	    (query.normal + point_ptr->normal));
    // also handle the case where exact same point is cached
	if (samePoint) {
		pqueue.push(ElementForIrradianceQueue(point_ptr, -1));
		return;
	}
	if ((weight > query.threashold && -2 * kEPSILON < front)) {
		pqueue.push(ElementForIrradianceQueue(point_ptr, weight));
	}
	
	if (delta > 0.0) { // みぎ
		locate_irradiance_points(pqueue, node->right, query);
		if (fabs(delta) < query.max_radius) {
			locate_irradiance_points(pqueue, node->left, query);
		}
	} else { // ひだり
		locate_irradiance_points(pqueue,node->left, query);
		if (fabs(delta) < query.max_radius) {
			locate_irradiance_points(pqueue, node->right, query);
		}
	}
}
	
void IrradianceCache::add_new_point(KDTreeNode* node, const Irradiance& point)
{
	const int axis = node->axis;
	double delta;
	switch (axis) {
	case 0: delta = point.x - node->point->x; break;
	case 1: delta = point.y - node->point->y; break;
	case 2: delta = point.z - node->point->z; break;
	}

	if (delta > 0.0) { // みぎ
		if (node->right != NULL) {
			add_new_point(node->right, point);
		} else {
			KDTreeNode* newnode = new KDTreeNode;
			newnode->axis = (axis + 1) % 3;
			newnode->point = new Irradiance(point);
			point_ptrs.push_back(newnode->point);
			node->right = newnode;
		}
	} else { // ひだり
		if (node->left != NULL) {
			add_new_point(node->left, point);
		} else {
			KDTreeNode* newnode = new KDTreeNode;
			newnode->axis = (axis + 1) % 3;
			newnode->point = new Irradiance(point);
			point_ptrs.push_back(newnode->point);
			node->left = newnode;
		}
	}
}

void IrradianceCache::AddPointToTree(const Irradiance &point) {
	if (unbalanced_root != NULL) {
		add_new_point(unbalanced_root, point);
	} else {
		KDTreeNode* newnode = new KDTreeNode;
		newnode->axis = 0;
		newnode->point = new Irradiance(point);
		point_ptrs.push_back(newnode->point);
		unbalanced_root = newnode;
	}
}