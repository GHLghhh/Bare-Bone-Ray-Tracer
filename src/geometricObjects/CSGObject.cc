#include "CSGObject.h"
#include "../materials/Material.h"
#include "../utils/ShadeRec.h"

CSGObject::CSGObject(GeometricObject* leftOperand, GeometricObject* rightOperand, CSGOperation operation)
  : GeometricObject((leftOperand->Position()+rightOperand->Position()) / 2,
      nullptr), leftOperand_(leftOperand), rightOperand_(rightOperand), operation_(operation)
{
  for (int i = 0; i < 3; i++) {
    boundingBox_.first[i] =
      (leftOperand->boundingBox_.first[i] < rightOperand->boundingBox_.first[i])
        ? leftOperand->boundingBox_.first[i] : rightOperand->boundingBox_.first[i];
    boundingBox_.second[i] =
      (leftOperand->boundingBox_.second[i] > rightOperand->boundingBox_.second[i])
        ? leftOperand->boundingBox_.second[i] : rightOperand->boundingBox_.second[i];
  }
}

// [TODO] finish all the function below
bool CSGObject::Hit(const Ray& ray, double& tMin, ShadeRec& sr)
{
  std::vector<HitRangeElement> res = HitRange(ray);
  if (res.size() == 0) {
    return false;
  }
  tMin = res[0].first;
  sr.hitPosition = res[0].second.hitPosition;
  sr.material = res[0].second.material;
  sr.normal = res[0].second.normal;
  sr.objectHit = res[0].second.objectHit;
  return true;
}

std::vector<HitRangeElement> CSGObject::HitRange(const Ray& ray)
{
  // Changing leftRes in place, the end result of leftRes is res
  std::vector<HitRangeElement> res;

  std::vector<HitRangeElement> mergeRes;
  std::vector<bool> resFromLeft;
  std::vector<bool> resEnter;

  std::vector<HitRangeElement> leftRes = leftOperand_->HitRange(ray);
  std::vector<HitRangeElement> rightRes = rightOperand_->HitRange(ray);
  int leftIdx = 0;
  int rightIdx = 0;
  while (leftIdx < leftRes.size() && rightIdx < rightRes.size()) {
    double leftT = leftRes[leftIdx].first;
    double rightT = rightRes[rightIdx].first;
    if (leftT < rightT) {
      mergeRes.push_back(leftRes[leftIdx]);
      resFromLeft.push_back(true);
      resEnter.push_back((leftIdx % 2) == 0);
      leftIdx++;
    } else {
      mergeRes.push_back(rightRes[rightIdx]);
      resFromLeft.push_back(false);
      resEnter.push_back((rightIdx % 2) == 0);
      rightIdx++;
    }
  }

  bool inLeftRange = false;
  bool inRightRange = false;        
  switch (operation_) {
    case CSGOperation::DIFFERENCE:
      {
        for (int i = 0; i < mergeRes.size(); i++) {
          if (resFromLeft[i]) {
            inLeftRange = resEnter[i];
            if (inLeftRange && !inRightRange) {
              res.push_back(mergeRes[i]);
            }
          } else {
            inRightRange = resEnter[i];
            if (inLeftRange) {
              res.push_back(mergeRes[i]);
            }
          }
        }
        // if (leftRes.size() != 0) {
        //   while (leftIdx < leftRes.size()) {
        //     double enterT = leftRes[leftIdx].first;
        //     double exitT = leftRes[leftIdx+1].first;
        //     if (rightIdx < rightRes.size()) {
        //       double diffEnterT = rightRes[rightIdx].first;
        //       double diffExitT = rightRes[rightIdx+1].first;
        //       // All part will be cut
        //       if (diffEnterT <= enterT && diffExitT >= exitT) {
        //         leftRes.erase(leftRes.begin()+leftIdx, leftRes.begin() + leftIdx + 2);
        //       }
        //       // No part in leftRes will be cut
        //       else if (diffExitT <= enterT) {
        //         rightIdx += 2;
        //       }
        //       // Some part in rightRes may be used
        //       else if (diffEnterT >= exitT) {
        //         leftIdx += 2;
        //       }
        //       // Some part will be cut
        //       else {
        //         // Cut the beginning of the section
        //         if (diffEnterT < enterT && diffExitT < exitT) {
        //           leftRes[leftIdx] = rightRes[rightIdx+1];
        //           rightIdx += 2;
        //         }
        //         // Cut the end of the section
        //         else if (diffEnterT > enterT && diffExitT > exitT) {
        //           leftRes[leftIdx+1] = rightRes[rightIdx];
        //           leftIdx += 2;
        //         }
        //         // Cut in the middle section
        //         else {
        //           leftRes.insert(leftRes.begin()+leftIdx+1, rightRes[rightIdx+1]);
        //           leftRes.insert(leftRes.begin()+leftIdx+1, rightRes[rightIdx]);
        //           rightIdx += 2;
        //         }
        //       }
        //     }
        //   }
        // }
      }
      break;
    case CSGOperation::INTERSECTION:
      {
        for (int i = 0; i < mergeRes.size(); i++) {
          if (inLeftRange && inRightRange && !resEnter[i]) {
            res.push_back(mergeRes[i]);
          }

          if (resFromLeft[i]) {
            inLeftRange = resEnter[i];
          } else {
            inRightRange = resEnter[i];
          }

          if (inLeftRange && inRightRange) {
            res.push_back(mergeRes[i]);
          }
        }
        // if (leftRes.size() != 0 && rightRes.size() != 0) {
        //   while (leftIdx < leftRes.size()) {
        //     double enterT = leftRes[leftIdx].first;
        //     double exitT = leftRes[leftIdx+1].first;
        //     if (rightIdx < rightRes.size()) {
        //       double diffEnterT = rightRes[rightIdx].first;
        //       double diffExitT = rightRes[rightIdx+1].first;
        //       // All part in leftRes is kept
        //       if (diffEnterT <= enterT && diffExitT >= exitT) {
        //         leftIdx += 2;
        //       }
        //       // Some part in leftRes may be kept
        //       else if (diffExitT <= enterT) {
        //         rightIdx += 2;
        //       }
        //       // no part in leftRes will be kept
        //       else if (diffEnterT >= exitT) {
        //         leftRes.erase(leftRes.begin()+leftIdx, leftRes.begin() + leftIdx + 2);
        //       }
        //       // Some part will be kept [TODO TODO]
        //       else {
        //         // Keep the beginning of the section
        //         if (diffEnterT < enterT && diffExitT < exitT) {
        //           leftRes[leftIdx] = rightRes[rightIdx+1];
        //           rightIdx += 2;
        //         }
        //         // Cut the end of the section
        //         else if (diffEnterT > enterT && diffExitT > exitT) {
        //           leftRes[leftIdx+1] = rightRes[rightIdx];
        //           leftIdx += 2;
        //         }
        //         // Cut in the middle section
        //         else {
        //           leftRes.insert(leftRes.begin()+leftIdx+1, rightRes[rightIdx+1]);
        //           leftRes.insert(leftRes.begin()+leftIdx+1, rightRes[rightIdx]);
        //           rightIdx += 2;
        //         }
        //       }
        //     }
        //   }
        // }
      }
      break;
    case CSGOperation::UNION:
      {
        for (int i = 0; i < mergeRes.size(); i++) {
          if ((!inLeftRange && !inRightRange) && resEnter[i]) {
            res.push_back(mergeRes[i]);
          }

          if (resFromLeft[i]) {
            inLeftRange = resEnter[i];
          } else {
            inRightRange = resEnter[i];
          }

          if (!inLeftRange && !inRightRange) {
            res.push_back(mergeRes[i]);
          }
        }
      }
      break;
  }
  return res;
}