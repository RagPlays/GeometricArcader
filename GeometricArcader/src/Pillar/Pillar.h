#ifndef PILLAR_H
#define PILLAR_H

#include <Engine.h>

#include "FlyFish.h"

class Player;

class Pillar final
{
public:

	Pillar();
	~Pillar() = default;

	Pillar(const Pillar& other) = delete;
	Pillar(Pillar&& other) noexcept = delete;
	Pillar& operator=(const Pillar& other) = delete;
	Pillar& operator=(Pillar&& other) noexcept = delete;

	void OnEvent(Engine::Event& e);

	void Update(Player& player, float deltaTime);
	void Render() const;

	const TriVector& GetPosition() const;

private:

	void UpdatePlayerGravity(Player& player, float deltaTime);

	bool OnMousePressed(const Engine::MouseButtonPressedEvent& e);

private:

	const Engine::Window& m_Window;

	TriVector m_Position;
	const glm::vec2 m_Size;


	const float m_InfluenceRadius;
	bool m_UsingGravity;
};

#endif // !PILLAR_H
