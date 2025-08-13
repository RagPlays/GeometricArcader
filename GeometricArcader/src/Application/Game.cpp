#include "Game.h"

#include "FlyFishTools/FlyFishUtils.h"

using namespace Engine;

Game::Game()
	: m_Window{ Application::Get().GetWindow() }
	, m_Player{}
{
}

void Game::OnEvent(Engine::Event& e)
{
	m_Player.OnEvent(e);
}

void Game::Update(float deltaTime)
{
	m_Player.Update(deltaTime);
}

void Game::Render() const
{
	DrawAxes();
	m_Player.Render();
	DrawLineTest();
	DrawCollisionLines();
}

void Game::TestMeetJoin()
{
	BiVector bivector{ FlyFishUtils::e3Gen ^ FlyFishUtils::e1Gen };
}

void Game::DrawAxes() const
{
	const float windowWidth{ static_cast<float>(m_Window.GetWidth()) };
	const float windowHeight{ static_cast<float>(m_Window.GetHeight()) };

	Renderer2D::SetDrawColor(Color::red);
	FlyFishUtils::DrawLine(FlyFishUtils::xAxis, windowWidth * 0.9f);
	Renderer2D::SetDrawColor(Color::green);
	FlyFishUtils::DrawLine(FlyFishUtils::yAxis, windowHeight * 0.9f);
}

void Game::DrawLineTest() const
{
	Renderer2D::SetDrawColor(Color::magenta);
	const glm::vec3 p1(-200.f, 200.f, 0.f);
	const glm::vec3 p2(425.f, 150.f, 0.f);
	float distance = glm::distance(p1, p2);
	const BiVector testLine = BiVector::LineFromPoints(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
	FlyFishUtils::DrawLine(testLine, distance);
	Renderer2D::SetDrawColor(Color::cyan);
	Renderer2D::DrawFilledCircle(p1, 5.f);
	Renderer2D::DrawFilledCircle(p2, 5.f);
}

void Game::DrawCollisionLines() const
{
	// Window sizes etc...
	const float windowWidth{ static_cast<float>(m_Window.GetWidth()) };
	const float windowHeight{ static_cast<float>(m_Window.GetHeight()) };
	const float halfWidth{ windowWidth * 0.5f };
	const float halfHeight{ windowHeight * 0.5f };

	const float collisionOffset{ 100.f };
	const float xOffset{ halfWidth - collisionOffset };
	const float yOffset{ halfHeight - collisionOffset };

	// Create planes that are the collision bounds (all pointing outwards)
	// Plane n = n1e1 + n2e2 + n3e3 + n0(-e0) -> So n0 needs to be -n0 to be correct

	const Vector leftBoundPlane		{ /*e0*/xOffset,	/*e1*/1.0f,		/*e2*/0.0f,		/*e3*/0.0f }; // normal -> right
	const Vector rightBoundPlane	{ /*e0*/xOffset,	/*e1*/-1.0f,	/*e2*/0.0f,		/*e3*/0.0f }; // normal -> left
	const Vector bottomBoundPlane	{ /*e0*/yOffset,	/*e1*/0.0f,		/*e2*/1.0f,		/*e3*/0.0f }; // normal -> up
	const Vector topBoundPlane		{ /*e0*/yOffset,	/*e1*/0.0f,		/*e2*/-1.0f,	/*e3*/0.0f }; // normal -> down

	// Create render lines (by meeting with e3)
	const BiVector leftBoundLine{ leftBoundPlane ^ FlyFishUtils::e3Gen };
	const BiVector rightBoundLine{ rightBoundPlane ^ FlyFishUtils::e3Gen };
	const BiVector bottomBoundLine{ bottomBoundPlane ^ FlyFishUtils::e3Gen };
	const BiVector topBoundLine{ topBoundPlane ^ FlyFishUtils::e3Gen };

	// Render lines
	Renderer2D::SetDrawColor(Color::yellow);
	FlyFishUtils::DrawLine(leftBoundLine, yOffset * 2.f);
	FlyFishUtils::DrawLine(rightBoundLine, yOffset * 2.f);
	FlyFishUtils::DrawLine(bottomBoundLine, xOffset * 2.f);
	FlyFishUtils::DrawLine(topBoundLine, xOffset * 2.f);

	////////////////////////////////////////
	// NEEDS TO BE MOVED OUT OF RENDERING //
	// Check if inside the collision bounds 
	const TriVector& playerPos{ m_Player.GetPosition() };
	const glm::vec2 playerSize{ m_Player.GetSize() };
	const glm::vec2 playerHalfSize{ playerSize * 0.5f };

	if (
		FlyFishUtils::SignedDistanceToPlane(leftBoundPlane, playerPos) < playerHalfSize.x ||
		FlyFishUtils::SignedDistanceToPlane(rightBoundPlane, playerPos) < playerHalfSize.x ||
		FlyFishUtils::SignedDistanceToPlane(bottomBoundPlane, playerPos) < playerHalfSize.y ||
		FlyFishUtils::SignedDistanceToPlane(topBoundPlane, playerPos) < playerHalfSize.y
		)
	{
		// Collision detected, render a debug circle
		Renderer2D::SetDrawColor(Color::darkGray);
		Renderer2D::DrawFilledCircle(Circle3f{ glm::vec3{ 0.f, 0.f, -1.f }, 50.f });
	}
	// NEEDS TO BE MOVED OUT OF RENDERING //
	////////////////////////////////////////
}