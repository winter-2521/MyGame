#include "Option.hpp"

Option::Option(const InitData& init)
	: IScene{ init } {}

void Option::update()
{
	m_startTransition.update(m_startButton.mouseOver());
	m_rankingTransition.update(m_optionButton.mouseOver());
	m_exitTransition.update(m_exitButton.mouseOver());

	if (m_startButton.mouseOver() || m_optionButton.mouseOver() || m_exitButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}
	if (m_exitButton.leftClicked())
	{
		// 終了
		System::Exit();
	}
}

void Option::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

	FontAsset(U"TitleFont")(U"BREAKOUT")
		.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.2, 0.6, 0.2 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ 400, 100 });

	m_startButton.draw(ColorF{ 1.0, m_startTransition.value() }).drawFrame(2);
	m_optionButton.draw(ColorF{ 1.0, m_rankingTransition.value() }).drawFrame(2);
	m_exitButton.draw(ColorF{ 1.0, m_exitTransition.value() }).drawFrame(2);

	FontAsset(U"Menu")(U"あそぶ").drawAt(m_startButton.center(), ColorF{ 0.25 });
	FontAsset(U"Menu")(U"オプション").drawAt(m_optionButton.center(), ColorF{ 0.25 });
	FontAsset(U"Menu")(U"やめる").drawAt(m_exitButton.center(), ColorF{ 0.25 });
}
