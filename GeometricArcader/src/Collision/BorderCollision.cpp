#include "BorderCollision.h"

#include "FlyFishTools/FlyFishUtils.h"
#include "Player/Player.h"

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
		const bool horizontalPlane{ FlyFishUtils::IsHorizontalPlane(plane) };
		const BiVector line{ plane ^ FlyFishUtils::e3 };
		FlyFishUtils::DrawLine(line, (horizontalPlane ? m_OffsetX : m_OffsetY) * 2.f);
	}
}

CollisionsData BorderCollision::HandleCollisions(TriVector& entityPos, const glm::vec2& entitySize) const
{
	CollisionsData collsData{};
	constexpr uint8_t maxHandles{ 2 };
	for (uint8_t handleIdx{}; handleIdx < maxHandles; ++handleIdx)
	{
		CollidingData collData{ TryGetCollision(entityPos, entitySize) };
		if (!collData.HasCollision()) break;
		ResolveCollision(entityPos, entitySize, collData);
		collsData.Collisions.emplace_back(collData);
	}
	return collsData;
}

CollidingData BorderCollision::TryGetCollision(const TriVector& entityPoint, const glm::vec2& entitySize) const
{
	const glm::vec2 halfSize{ entitySize * 0.5f };

	for (const auto& plane : m_BorderPlanes)
	{
		const bool horizontalPlane{ FlyFishUtils::IsHorizontalPlane(plane) };
		const float offsetDistance{ horizontalPlane ? halfSize.y : halfSize.x };
		if (float signedDistance{ FlyFishUtils::SignedDistanceToPlane(plane, entityPoint) }; signedDistance < offsetDistance)
		{
			return CollidingData{ signedDistance, &plane };
		}
	}

	return CollidingData{};
}

void BorderCollision::ResolveCollision(TriVector& entityPos, const glm::vec2& entitySize, const CollidingData& collision) const
{
	if (collision.CollidedPlane == nullptr) return;

	const Vector& collPlane{ *collision.CollidedPlane };
	const bool horizontalPlane{ FlyFishUtils::IsHorizontalPlane(collPlane) };
	const glm::vec2 halfSize{ entitySize * 0.5f };
	const float offsetDist{ horizontalPlane ? halfSize.y : halfSize.x };

	// Projected position on the collision plane
	TriVector projectedPos{ FlyFishUtils::Projection(entityPos, collPlane) };

	// Setting the position to the edge of the collision plane
	entityPos = projectedPos;

	// Translate the entity position away from the collision plane (using normal of the plane)
	FlyFishUtils::Translate(entityPos, collPlane, offsetDist);
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