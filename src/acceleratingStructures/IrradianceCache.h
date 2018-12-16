// The code is modified from external/simple_irrcache.cpp https://github.com/githole/simple-irradiance-caching
#pragma once

#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

#include "../utils/Vec3.h"
#include "../utils/RGBColor.h"
#include "KDTree.h"

class Irradiance : public Vec3 {
public:
	RGBColor irradiance;
	Vec3 normal;
	double R;

	Irradiance(const Vec3& position_, const RGBColor& irradiance_, const Vec3& normal_, const double R_)
	  : Vec3(position_), irradiance(irradiance_), normal(normal_), R(R_) {
	}
};

class IrradianceCache : public KDTree {
public:
	
	// k-NN searchのクエリ
	struct IrradianceQuery : public KDTree::Query {
		double threashold;

		IrradianceQuery(const Vec3& search_position_, const Vec3& normal_, const double max_radius, const double threashold_)
		 : KDTree::Query(search_position_, normal_, max_radius, 0), threashold(threashold_) {}
	};

	// 結果のQueueに乗せるためのデータ構造。
	// Keep the result in this form because weight will be calculated
	// when gathering the points
	struct ElementForIrradianceQueue {
		const Irradiance *point;
		double distance2;
		double weight;

		ElementForIrradianceQueue(const Irradiance *point_, const double weight_) : point(point_), weight(weight_) {}
		bool operator<(const ElementForIrradianceQueue &b) const {
			return distance2 < b.distance2;
		}
		
		ElementForIrradianceQueue& operator=(const ElementForIrradianceQueue& obj) {
			this->point = obj.point;
			this->distance2 = obj.distance2;
			this->weight = obj.weight;
			return (*this);
		}
	};
	// KNNの結果を格納するキュー
	typedef std::priority_queue<ElementForIrradianceQueue, std::vector<ElementForIrradianceQueue> > ResultIrradianceQueue;

	void SearchCachedPoints(ResultIrradianceQueue& iqueue, IrradianceQuery &query) {
		if (root != NULL)
			locate_irradiance_points(iqueue, root, query);
		else
			locate_irradiance_points(iqueue, unbalanced_root, query);
	}
	void AddPointToTree(const Irradiance &point);

private:
    // keep a new set of roots such that
	// two pass irradiance caching is relatively efficient:
	// First pass, adding nodes to unbalanced KD tree structure
	// to gain some efficiency on gathering points without taking
	// time to balance the tree.
	// Second pass, create a balance tree and gather irradiance
	// through it.
	KDTreeNode* unbalanced_root;

	// フツーのk-NN search。
	// Slightly different from KDTree::locate_points()
	// the result wanted contains more information
	// [TODO] generalize and make it abstract
	void locate_irradiance_points(IrradianceCache::ResultIrradianceQueue& pqueue, KDTreeNode* node, IrradianceQuery &query);
	
	void add_new_point(KDTreeNode* node, const Irradiance& point);
};