#ifndef MAINLAYER_H
#define	MAINLAYER_H

#include <Engine.h>

class Font;

class MainLayer final : public Engine::Layer
{
public:

	MainLayer();
	~MainLayer();

	virtual void OnAttach() override;

	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;

	virtual void OnEvent(Engine::Event& e) override;

private:

	void Update();
	void Render() const;

private:


};

#endif // !MAINLAYER_H
