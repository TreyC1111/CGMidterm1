#include "BloomEffect.h"

void BloomEffect::Init(unsigned width, unsigned height)
{
	int index = int(_buffers.size());
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);
	index++;
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(unsigned(width / m_downscale), unsigned(height / m_downscale));
	index++;
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(unsigned(width / m_downscale), unsigned(height / m_downscale));
	index++;
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(width, height);

	index = int(_shaders.size());
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/bloom_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/blurHorizontal_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/blurVertical_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/bloomComposite_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	lightDirection = glm::vec2(1.0f / width, 1.0f / height);
}

void BloomEffect::ApplyEffect(PostEffect* buffer)
{
	BindShader(0);
	buffer->BindColorAsTexture(0, 0, 0);
	_buffers[0]->RenderToFSQ();
	buffer->UnbindTexture(0);
	UnbindShader();

	BindShader(1);
	_shaders[1]->SetUniform("u_threshHold", m_threshHold);
	BindColorAsTexture(0, 0, 0);
	_buffers[1]->RenderToFSQ();
	UnbindTexture(0);
	UnbindShader();

	for (unsigned i = 0; i < m_passes; i++)
	{
		BindShader(2);
		_shaders[2]->SetUniform("u_lightDirection", lightDirection.x);
		BindColorAsTexture(1, 0, 0);
		_buffers[2]->RenderToFSQ();
		UnbindTexture(0);
		UnbindShader();
		
		BindShader(3);
		_shaders[3]->SetUniform("u_lightDirection", lightDirection.y);
		BindColorAsTexture(2, 0, 0);
		_buffers[1]->RenderToFSQ();
		UnbindTexture(0);
		UnbindShader();
	}

	BindShader(4);
	buffer->BindColorAsTexture(0, 0, 0);
	BindColorAsTexture(1, 0, 1);
	_buffers[0]->RenderToFSQ();
	UnbindTexture(1);
	UnbindTexture(0);
	UnbindShader();
}

float BloomEffect::GetDownscale() const
{
	return m_downscale;
}

float BloomEffect::GetThreshHold() const
{
	return m_threshHold;
}

unsigned BloomEffect::GetPasses() const
{
	return m_passes;
}

void BloomEffect::SetThreshHold(float threshHold)
{
	m_threshHold = threshHold;
}

void BloomEffect::SetDownscale(float downscale)
{
	m_downscale = downscale;
}

void BloomEffect::SetPasses(unsigned passes)
{
	m_passes = passes;
}
