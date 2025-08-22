#ifndef ULTRAPICKUP_H
#define ULTRAPICKUP_H

#include <Engine.h>

#include "FlyFish.h"

class UltraPickup final
{
public:

	UltraPickup();
	~UltraPickup() = default;

	void Update(float deltaTime);
	void Render() const;

	const TriVector& GetPosition() const;
	const glm::vec2& GetSize() const;

	void RandomizePosition();

private:

	const Engine::Window& m_Window;

	TriVector m_Position;
	std::array<TriVector, 4> m_RotatePositions;

	glm::vec2 m_Size;
	glm::vec2 m_RenderCubeSize;
};

#endif // !ULTRAPICKUP_H
