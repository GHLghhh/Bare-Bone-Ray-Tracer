// The code is modified from external/simple_irrcache.cpp https://github.com/githole/simple-irradiance-caching
#include "KDTree.h"


void KDTree::delete_kdtree(KDTreeNode* node)
{
	if (node == NULL)
		return;
	delete_kdtree(node->left);
	delete_kdtree(node->right);
	delete node;
}

// KD search, passing res as reference to modify it in place
// such that we don't have to append the results if search branches out
void KDTree::locate_points(std::vector<Vec3*>& res, KDTreeNode* node, KDTree::Query &query) {
	if (node == NULL)
		return;
	const int axis = node->axis;

	double delta;
	switch (axis) {
		case 0: delta = query.search_position.x - node->point->x; break;
		case 1: delta = query.search_position.y - node->point->y; break;
		case 2: delta = query.search_position.z - node->point->z; break;
	}

	// 対象点<->探索中心の距離が設定半径以下　かつ　対象点<->探索中心の法線方向の距離が一定以下　という条件ならその対象点格納
	Vec3 dir = (*node->point) - query.search_position;
	const double dt = Vec3::Dot(query.normal, dir.Unit());
	if (dir.Length() < query.max_radius && fabs(dt) <= query.max_radius * query.max_radius * 0.01) {
		if (query.max_search_num == 0 || res.size() < query.max_search_num)
			res.push_back(node->point);
	}
	if (delta > 0.0) { // みぎ
		locate_points(res, node->right, query);
		if (fabs(delta) < query.max_radius) {
			locate_points(res, node->left, query);
		}
	} else { // ひだり
		locate_points(res, node->left, query);
		if (fabs(delta) < query.max_radius) {
			locate_points(res, node->right, query);
		}
	}

}
	
KDTree::KDTreeNode* KDTree::create_kdtree_sub(std::vector<Vec3*>::iterator begin, std::vector<Vec3*>::iterator end, int depth) {
	if (begin >= end) {
		return NULL;
	}
	const int axis = depth % 3;
	// 中央値
	switch (axis) {
	case 0: std::sort(begin, end, kdtree_less_operator_x); break;
	case 1: std::sort(begin, end, kdtree_less_operator_y); break;
	case 2: std::sort(begin, end, kdtree_less_operator_z); break;
	}
	const int median = (end - begin) / 2;
	KDTreeNode* node = new KDTreeNode;
	node->axis = axis;
	node->point = (*(begin + median));
	// 子供
	node->left  = create_kdtree_sub(begin,              begin + median, depth + 1);
	node->right = create_kdtree_sub(begin + median + 1, end,            depth + 1);
	return node;
}