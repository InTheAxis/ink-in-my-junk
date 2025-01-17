#ifndef SCENE_EXAMPLE_CAR_H
#define SCENE_EXAMPLE_CAR_H

#include "Scene.h"
#include "Car.h"

class SceneExampleCar : public Scene
{
public:
	SceneExampleCar();
	~SceneExampleCar();
protected:
	GameObject floor;
	GameObject ramp;
	Car car;

	virtual void InitDerived();
	virtual void RenderDerived();
	virtual void UpdateDerived(double dt);
	virtual void UpdateDerivedBounced(double dt);
};

#endif //! SCENE_EXAMPLE_CAR_H