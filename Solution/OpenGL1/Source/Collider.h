#ifndef COLLIDER_H
#define COLLIDER_H

#include "GameObject.h"
#include "Mtx44.h"

class Collider : public GameObject //just to render, eventually remove
{
public:
	Collider();
	void PushToHullPoints(Vector3);
	void UpdateHull(Vector3 translate, Vector3 rotate); //aplly rotate and translate
	~Collider();
private:
	std::vector<Vector3> hullPoints; //defining the enclosing convex hull
	std::vector<Vector3> startingPoints; //defining the default convex hull 

	std::vector<Vector3> lineStart, lineEnd; //for drawing the colliding box
	
	Mtx44 rotationMatrix[3]; //xyz
};
#endif
