#include "FireParticle.h"

void FireParticle::Initialize(Vector3 pos)
{
	smokeTexHandle = TextureManager::LoadPngTexture("smoke.png");
	particle_ = make_unique<Particle>();
	particle_->SetTexHandle(smokeTexHandle);
	particle_->Initialize(new ParticlePlaneState, 1000);
	particle_->SetBlendMode(BlendAdd);
	position_ = pos;
}

void FireParticle::Update(Vector3 pos)
{
	prevPos_ = position_;
	position_ = pos;
	const float kSize = 0.5f;

	particleList_ = particle_->begin();
	int  count = 0;
	for (Particle_param particle : particleList_)
	{
	    particle.color_.w -= 0.01f;
		particle.color_.x -= 0.015f;

		particle.worldTransform_.translate.y += particle.Velocity.y;
		particle.worldTransform_.translate.x += particle.Velocity.x;

		if (particle.color_.w <= 0.0f)
		{
			
			continue;
		}

		count++;
		particle_->PushList(particle);
	}

	spownTimer_++;
	if (spownTimer_ > 5)
	{
		for (int i = 0; i < 10; i++)
		{
			Particle_param p1{};
			//p1.worldTransform_.Initialize();
			mt19937 randomEngine(seedGenerator());
			uniform_real_distribution<float>distribution(0.025f, 0.1f);

			Vector3 randpos = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };
			p1.worldTransform_.translate = VectorTransform::Add(VectorTransform::Add(p1.worldTransform_.translate,pos), randpos);
			p1.worldTransform_.scale = { kSize, kSize, kSize };

			uniform_real_distribution<float>distributionAlpha(0.2f, 1.0f);
			Vector4 color = { 1,1,1,1 };
			p1.color_ = { color.x,color.y,color.z,255 };
			p1.color_ = { color.x,color.y,color.z,distributionAlpha(randomEngine) };
		
			UVSet(p1.uvTransform_.scale, p1.uvTransform_.translate);

			uniform_real_distribution<float>distributionVelocityX(-0.01f, 0.01f);
			uniform_real_distribution<float>distributionVelocityY(0.0f, 0.03f);
			p1.Velocity.x = distributionVelocityX(randomEngine);
			p1.Velocity.y = distributionVelocityY(randomEngine);
			
	
			particle_->PushList(p1);
		}
		spownTimer_ = 0;

	}

}

void FireParticle::Draw(ViewProjection view)
{
	particle_->Draw(view);
}

void FireParticle::UVSet(Vector3 &s, Vector3 &t)
{
	mt19937 randomEngine(seedGenerator());
	uniform_int_distribution<int>distribution(0, 4);
	const int UVrand = distribution(randomEngine);
	s = { 0.5f,0.5f,0.5f };
	
	if (RT == UVrand)
	{
		t = { 0.5f,0,0 };
	}
	if (RB == UVrand)
	{
		t = { 0.5f,0.5f,0 };
	}
	if (LT == UVrand)
	{
		t = { 0.0f,0.0f,0 };
	}
	if (LB == UVrand)
	{
		t = { 0.0f,0.5f,0 };
	}
}
