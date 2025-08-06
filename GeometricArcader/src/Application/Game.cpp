#include "Game.h"

#include "FlyFishTools/FlyFishUtils.h"

using namespace Engine;

Game::Game()
	: m_BladePosition{ 0.f, 0.f, 0.f }
	, m_Window{ Application::Get().GetWindow() }
{
}

void Game::Update(float deltaTime)
{
	// movespeed
	constexpr float baseMoveSpeed{ 2000.f };
	bool sprinting{ Input::IsKeyPressed(Key::LeftShift) };
	const float moveSpeed{ sprinting ? baseMoveSpeed * 2.f : baseMoveSpeed };
	const float deltaSpeed{ moveSpeed * deltaTime };

	if (Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::Up))
	{
		const Motor upMotor{ Motor::Translation(deltaSpeed, FlyFishUtils::yAxis) };
		//std::cout << "UpMotor: " << upMotor.toString() << "\n";
		m_BladePosition = MultiVector{ upMotor * m_BladePosition * ~upMotor }.Grade3();
	}
	if (Input::IsKeyPressed(Key::S) || Input::IsKeyPressed(Key::Down))
	{
		const Motor downMotor{ Motor::Translation(-deltaSpeed, FlyFishUtils::yAxis) };
		//std::cout << "UpMotor: " << downMotor.toString() << "\n";
		m_BladePosition = MultiVector{ downMotor * m_BladePosition * ~downMotor }.Grade3();
	}
	if (Input::IsKeyPressed(Key::A) || Input::IsKeyPressed(Key::Left))
	{
		const Motor leftMotor{ Motor::Translation(-deltaSpeed, FlyFishUtils::xAxis) };
		//std::cout << "UpMotor: " << leftMotor.toString() << "\n";
		m_BladePosition = MultiVector{ leftMotor * m_BladePosition * ~leftMotor }.Grade3();
	}
	if (Input::IsKeyPressed(Key::D) || Input::IsKeyPressed(Key::Right))
	{
		const Motor rightMotor{ Motor::Translation(deltaSpeed, FlyFishUtils::xAxis) };
		//std::cout << "UpMotor: " << rightMotor.toString() << "\n";
		m_BladePosition = MultiVector{ rightMotor * m_BladePosition * ~rightMotor }.Grade3();
	}
}

void Game::Render() const
{
	const float windowWidth{ static_cast<float>(m_Window.GetWidth()) };
	const float windowHeight{ static_cast<float>(m_Window.GetHeight()) };
	const float halfWidth{ windowWidth * 0.5f };
	const float halfHeight{ windowHeight * 0.5f };

	Renderer2D::SetDrawColor(Color::magenta);
	Renderer2D::DrawRect(Rect2f{ 0.f, 0.f, windowWidth, 200.f });
	Renderer2D::DrawRect(Rect2f{ 0.f, 0.f, 200.f, windowHeight });

	Renderer2D::SetDrawColor(Color::orange);
	FlyFishUtils::DrawFillRect(m_BladePosition, 20.f, 20.f);
	Renderer2D::SetDrawColor(Color::red);
	FlyFishUtils::DrawRect(m_BladePosition, 25.f, 25.f);

	//Renderer2D::SetDrawColor({ 1.f, 1.f, 1.f, 1.f });
	//Renderer2D::DrawRect(Rect2f{ 0.f, 0.f, 200.f, 200.f });

	//const BiVector xAxis{ BiVector::LineFromPoints(-halfWidth, 0.f, 1.f, halfWidth, 0.f, 1.f)};
	//const BiVector yAxis{ BiVector::LineFromPoints(0.f, -halfHeight, 1.f, 0.f, halfHeight, 1.f)};

	//Renderer2D::SetDrawColor(Color::red);
	//FlyFishUtils::DrawLine(xAxis);
	//Renderer2D::SetDrawColor(Color::green);
	//FlyFishUtils::DrawLine(yAxis);

	//Renderer2D::SetDrawColor(Color::red);
	//FlyFishUtils::FillRect(m_BladePosition, 50.f, 50.f);

	//Renderer2D::SetDrawColor(Color::magenta);
	//Renderer2D::DrawPoint(glm::vec3{ 0.f, 0.f, 1.f });
	//Renderer2D::SetDrawColor(Color::white);
	//Renderer2D::DrawFilledRect(glm::vec3{}, glm::vec2{ 50.f, 50.f });
}