#pragma once
#include "Common.hpp"

class Stage : public App::Scene
{
public:

	Stage(const InitData& init);

	void update() override;

	void draw() const override;

private:

	Rect m_stage1_Button{ Arg::center = Scene::Center().movedBy(0, -150), 250, 100 };
	Transition m_stage1_Transition{ 0.4s, 0.2s };

	Rect m_stage2_Button{ Arg::center = Scene::Center(), 250, 100 };
	Transition m_stage2_Transition{ 0.4s, 0.2s };

	Rect m_stage3_Button{ Arg::center = Scene::Center().movedBy(0, 150), 250, 100 };
	Transition m_stage3_Transition{ 0.4s, 0.2s };

	Rect m_back_Button{ Arg::center = Scene::Center().movedBy(250, 250) , 150, 50};
	Transition m_back_Transition{ 0.4s, 0.2s };
};
