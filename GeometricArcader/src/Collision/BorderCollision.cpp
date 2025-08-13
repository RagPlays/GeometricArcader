#include "BorderCollision.h"

#include "FlyFishTools/FlyFishUtils.h"

#include "Entities/Player.h"

using namespace Engine;

BorderCollision::BorderCollision(uint32_t borderOffset)
	: m_BorderOffset{ borderOffset }
	, m_OffsetX{}
	, m_OffsetY{}
	, m_BorderPlanes
	{
		Vector{ /*e0*/0.f, /*e1*/1.0f,  /*e2*/0.0f,  /*e3*/0.0f },	// plane normal -> right
		Vector{ /*e0*/0.f, /*e1*/-1.0f, /*e2*/0.0f,  /*e3*/0.0f },	// plane normal -> left
		Vector{ /*e0*/0.f, /*e1*/0.0f,  /*e2*/1.0f,  /*e3*/0.0f },	// plane normal -> up
		Vector{ /*e0*/0.f, /*e1*/0.0f,  /*e2*/-1.0f, /*e3*/0.0f }	// plane normal -> down
	}
{
	const Window& window{ Application::Get().GetWindow() };
	UpdateBorderPlanes(window.GetWidth(), window.GetHeight());
}

void BorderCollision::OnEvent(Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(BorderCollision::OnWindowResized));
}

void BorderCollision::Render() const
{
	Renderer2D::SetDrawColor(Color::yellow);

	for (const auto& plane : m_BorderPlanes)
	{
		const bool horizontalPlane{ FlyFishUtils::IsHorzontalPlane(plane) };
		const BiVector line{ plane ^ FlyFishUtils::e3Gen };
		FlyFishUtils::DrawLine(line, (horizontalPlane ? m_OffsetX : m_OffsetY) * 2.f);
	}
}

void BorderCollision::HandleCollision(TriVector& entityPos, const glm::vec2& entitySize) const
{
	const glm::vec2 halfSize{ entitySize * 0.5f };

	if (auto collData{ IsColliding(entityPos, entitySize) }; collData.HasCollision)
	{
		const Vector& collPlane{ *collData.CollidedPlane };
		const bool horizontalPlane{ FlyFishUtils::IsHorzontalPlane(collPlane) };
		const float offsetDist{ horizontalPlane ? halfSize.y : halfSize.x };

		// Projected position on the collision plane
		const TriVector projectedPos{ FlyFishUtils::Projection(entityPos, collPlane) };

		// Create translation line
		const BiVector normalLine{ collPlane.e1(), collPlane.e2(), collPlane.e3(), 0.f, 0.f, 0.f};

		// Build the translation motor
		const Motor translateMotor{ Motor::Translation(offsetDist, normalLine) };

		// Translate the entity position using the motor
		entityPos = MultiVector{ translateMotor * projectedPos * ~translateMotor }.Grade3();

		// Recusively check for further collisions
		HandleCollision(entityPos, entitySize);
	}
}

BorderCollisionData BorderCollision::IsColliding(const TriVector& entityPoint, const glm::vec2& entitySize) const
{
	const glm::vec2 halfSize{ entitySize * 0.5f };

	for (const auto& plane : m_BorderPlanes)
	{
		const bool horizontalPlane{ FlyFishUtils::IsHorzontalPlane(plane) };
		const float offsetDistance{ horizontalPlane ? halfSize.y : halfSize.x };
		if (float signedDistance{ FlyFishUtils::SignedDistanceToPlane(plane, entityPoint) }; signedDistance < offsetDistance)
		{
			return BorderCollisionData{ true, signedDistance, &plane };
		}
	}

	return BorderCollisionData{};
}

void BorderCollision::UpdateBorderPlanes(uint32_t windowWidth, uint32_t windowHeight)
{
	const uint32_t halfWidth{ windowWidth / 2 };
	const uint32_t halfHeight{ windowHeight / 2 };
	m_OffsetX = static_cast<float>(halfWidth - m_BorderOffset);
	m_OffsetY = static_cast<float>(halfHeight - m_BorderOffset);

	m_BorderPlanes[0][0] = m_BorderPlanes[1][0] = m_OffsetX; // set e0 to offsetX (for left and right planes)
	m_BorderPlanes[2][0] = m_BorderPlanes[3][0] = m_OffsetY; // set e0 to offsetY (for top and bottom planes)
}

bool BorderCollision::OnWindowResized(WindowResizeEvent& e)
{
	const uint32_t width{ e.GetWidth() };
	const uint32_t height{ e.GetHeight() };
	if (width < m_BorderOffset || height < m_BorderOffset) return false;
	
	UpdateBorderPlanes(width, height);

	return false;
}