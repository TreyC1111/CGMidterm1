#pragma once

#include "Graphics/Post/PostEffect.h"

class BloomEffect : public PostEffect
{
public:
	void Init(unsigned width, unsigned height) override;
	void ApplyEffect(PostEffect* buffer) override;

	float GetDownscale() const;
	float GetThreshHold() const;
	unsigned GetPasses() const;

	void SetThreshHold(float threshHold);
	void SetDownscale(float downscale);
	void SetPasses(unsigned passes);
private:
	float m_downscale = 2.0f;
	float m_threshHold = 0.01f;
	unsigned m_passes = 10;
	glm::vec2 lightDirection;
};