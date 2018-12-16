// The code is modified from external/simple_irrcache.cpp https://github.com/githole/simple-irradiance-caching
#pragma once

#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

#include "../utils/Vec3.h"

// KD-tree
class KDTree {
public:
	// k-NN searchのクエリ
	struct Query {
		double max_radius; // 探索の最大半径
        // max_search_num == 0 is special case for taking every valid point
		size_t max_search_num; // 最大探索点数
		Vec3 search_position; // 探索中心
		Vec3 normal; // 探索中心における法線
		Query(const Vec3 &search_position_, const Vec3 &normal_, const double max_radius_, const size_t max_search_num_) :
		max_radius(max_radius_), max_search_num(max_search_num_), search_position(search_position_), normal(normal_) {}
	};

    KDTree() {
		root = NULL;
	}
	virtual ~KDTree() {
        for (int i = 0; i < point_ptrs.size(); i ++)
			delete point_ptrs[i];
		delete_kdtree(root);
	}
	size_t Size() {
		return point_ptrs.size();
	}
	void SearchKNN(std::vector<Vec3*> res, typename KDTree::Query &query) {
		locate_points(res, root, query);
	}

    // Passing pointer such that the passing value can contain more than Vec3
    // by passing pointer to derived class
	void AddPoint(Vec3* point_ptr) {
		point_ptrs.push_back(point_ptr);
	}
	void CreateKDtree() {
		root = create_kdtree_sub(point_ptrs.begin(), point_ptrs.end(), 0);
	}

    std::vector<Vec3*> GetRawVector() { return point_ptrs; };

protected:
    struct KDTreeNode {
		Vec3* point;
		KDTreeNode* left;
		KDTreeNode* right;
		int axis;

		KDTreeNode() : point(NULL), left(NULL), right(NULL) {
		}
	};

	std::vector<Vec3*> point_ptrs;
	KDTreeNode* root;
	void delete_kdtree(KDTreeNode* node);

	// KD search, passing res as reference to modify it in place
    // such that we don't have to append the results if search branches out
	void locate_points(std::vector<Vec3*>& res, KDTreeNode* node, KDTree::Query &query);
    KDTreeNode* create_kdtree_sub(std::vector<Vec3*>::iterator begin, std::vector<Vec3*>::iterator end, int depth);
	
	static bool kdtree_less_operator_x(const Vec3* left, const Vec3* right) {
		return left->x < right->x;
	}
	static bool kdtree_less_operator_y(const Vec3* left, const Vec3* right) {
		return left->y < right->y;
	}
	static bool kdtree_less_operator_z(const Vec3* left, const Vec3* right) {
		return left->z < right->z;
	}
};