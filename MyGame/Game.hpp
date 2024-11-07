#pragma once
#include "Common.hpp"
#include "StageData.hpp"

class Game : public App::Scene
{
public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;

private:

	// ゲームをクリアしたか判定
	bool game_clear = false;

	// 移動アニメーション描画中か判定
	bool is_moving = false;

	// ゲーム盤面の幅と高さ
	const int32 game_field_width = 600;
	const int32 game_field_height = 400;

	// ステージデータの読み込み
	StageData stage_data;
	Board board;

	// マス一つ当たりの大きさ
	int32 cell_width;

	// マスに描画する絵文字のテクスチャ
	const Texture player_texture{U"👤"_emoji};
	const Texture goal_texture{ U"🚩"_emoji };
	const Texture bomb_texture{ U"💣"_emoji };

	// アニメーション速度 (1フレームごとに移動するピクセル数)
	const double speed = 15.0;

	/*
	*　中心をScene::Center.movedBy(0,-30)にセットしながらゲームの盤面を表すグリッドを書きたい
	*  各グリッドのマスを独立して描画しているので，そのような制御が難しい
	*  一度ゲームの盤面サイズのRectを配置してみて，その左上座標を取得 -> その座標を基準としてグリッドを描画という処理にする
	*/
	Rect simulate_game_grid;
	Vec2 grid_tl;

	// 戻るボタン用
	Rect m_back_Button{ Arg::center = Scene::Center().movedBy(250, 250) , 150, 50 };
	Transition m_back_Transition{ 0.4s, 0.2s };
};
