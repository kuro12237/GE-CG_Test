#pragma once
#include"Particle/Particle.h"
#include"Input.h"
#include"VectorTransform.h"


enum SParticleUV
{
	RT,
	RB,
	LT,
	LB
};

class FireParticle
{
public:
	FireParticle() {};
	~FireParticle() {};

	void Initialize(Vector3 pos);

	void Update(Vector3 pos);

	void Draw(ViewProjection view);


private:

	void UVSet(Vector3& s, Vector3& t);

	unique_ptr<Particle>particle_ = nullptr;
	list<Particle_param> particleList_{};

	Vector3 position_ = {};
	Vector3 prevPos_ = {};

	uint32_t smokeTexHandle = 0;
	bool Trriger = false;
	uint32_t spownTimer_ = 0;

	random_device seedGenerator;
};

