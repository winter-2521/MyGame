#include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{
	// ステージデータの読み込み
	stage_data = StageInfo[getData().stage_number][getData().level_number];
	board = Board(stage_data);

	// マス一つ当たりの大きさ
	cell_width = std::min(game_field_width / stage_data.stage_width, game_field_height / stage_data.stage_height);

	/*
	*　中心をScene::Center.movedBy(0,-30)にセットしながらゲームの盤面を表すグリッドを書きたい
	*  各グリッドのマスを独立して描画しているので，そのような制御が難しい
	*  一度ゲームの盤面サイズのRectを配置してみて，その左上座標を取得 -> その座標を基準としてグリッドを描画という処理にする
	*/
	simulate_game_grid = Rect{ Arg::center = Scene::Center().movedBy(0,-30) , cell_width * stage_data.stage_width, cell_width * stage_data.stage_height };
	grid_tl = simulate_game_grid.tl();

	// キャラの現在位置をグリッド基準の位置に合わせる．
	for (auto i : step(board.players.size()))
	{
		board.player_pos[i] = Vec2{grid_tl.x + board.players[i].pos.x * cell_width, grid_tl.y + board.players[i].pos.y * cell_width};
	}

}

void Game::update()
{
	// ゲームの盤面(グリッド)を描画
	for (auto w : step(stage_data.stage_width))
	{
		for (auto h : step(stage_data.stage_height))
		{
			if (board.is_wall(w, h)) continue;
			Rect cell(grid_tl.x + w * cell_width, grid_tl.y + h * cell_width, cell_width, cell_width);
			cell.draw(Palette::Skyblue);
			cell.drawFrame(1.0, 0.0, Palette::Lightslategray);
		}
	}

	// ゴールを描画
	for (auto p : board.goals) {
		Vec2 target_pos = Vec2(grid_tl.x + p.x * cell_width, grid_tl.y + p.y * cell_width);
		goal_texture.resized(cell_width).drawAt(target_pos + Vec2(cell_width / 2, cell_width / 2));
	}

	// 右回転マスを描画
	for (auto p : board.l_rots) {
		Vec2 target_pos = Vec2(grid_tl.x + p.x * cell_width, grid_tl.y + p.y * cell_width);
		l_rot_texture.resized(cell_width).drawAt(target_pos + Vec2(cell_width / 2, cell_width / 2));
	}

	// 左回転マスを描画
	for (auto p : board.r_rots) {
		Vec2 target_pos = Vec2(grid_tl.x + p.x * cell_width, grid_tl.y + p.y * cell_width);
		r_rot_texture.resized(cell_width).drawAt(target_pos + Vec2(cell_width / 2, cell_width / 2));
	}

	// プレイヤーの移動処理を描画する
	for (auto i : step(board.players.size())) {
		Vec2 target_pos = Vec2(grid_tl.x + board.players[i].pos.x * cell_width, grid_tl.y + board.players[i].pos.y * cell_width);

		// 目的地との距離が遠ければ微量だけ移動させる
		if (board.player_pos[i].distanceFrom(target_pos) > speed)
		{
			board.player_pos[i].moveBy((target_pos - board.player_pos[i]).setLength(speed));
		}
		else
		{
			// それ以外なら目的地に合わせる
			board.player_pos[i] = target_pos;
			is_moving = false;
		}

		// 目的地についている かつ 回転マスにいるとき，回転処理を行う
		if (board.player_pos[i] == target_pos && board.player_angle_rest[i] != 0.0)
		{
			if (board.player_angle_rest[i] > 0)
			{ // 正方向の回転
				if (board.player_angle_rest[i] > rotate_speed) {
					board.players[i].angle += rotate_speed;
					board.player_angle_rest[i] -= rotate_speed;
				}
				else {
					board.players[i].angle += board.player_angle_rest[i];
					board.player_angle_rest[i] = 0.0;
				}
			}
			if (board.player_angle_rest[i] < 0)
			{ // 負方向の回転
				if (-board.player_angle_rest[i] > rotate_speed) {
					board.players[i].angle -= rotate_speed;
					board.player_angle_rest[i] += rotate_speed;
				}
				else {
					board.players[i].angle += board.player_angle_rest[i];
					board.player_angle_rest[i] = 0.0;
				}
			}

			// 回転角が大きくなりすぎたり，小さくなりすぎないように処理
			if (board.players[i].angle > 360_deg) board.players[i].angle -= 360_deg;
			if (board.players[i].angle < 0_deg) board.players[i].angle += 360_deg;
		}

		// プレイヤーの位置に絵文字を描画
		player_texture.resized(cell_width).rotated(board.players[i].angle).drawAt(board.player_pos[i] + Vec2(cell_width / 2, cell_width / 2));
	}

	m_back_Transition.update(m_back_Button.mouseOver());

	if (m_back_Button.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	// ボタン押下でプレイヤーの移動処理
	// ゲームクリア時は処理を受け付けない
	if (!game_clear)
	{
		if (KeyD.down())
		{
			board.operate_player(0);
			is_moving = true;
		}

		if (KeyS.down())
		{
			board.operate_player(1);
			is_moving = true;
		}

		if (KeyA.down())
		{
			board.operate_player(2);
			is_moving = true;
		}

		if (KeyW.down())
		{
			board.operate_player(3);
			is_moving = true;
		}

		if (m_back_Button.leftClicked())
		{
			// ステージ選択に戻る
			changeScene(State::Level);
		}
	}
	else if(!is_moving){
		// ウィンドウの背景（クリア画面として半透明に）
		Rect clear_window = Rect(Scene::Center().movedBy(-200, -100), 400, 200);
		clear_window.draw(ColorF(0.0, 0.5));

		// メッセージの表示
		FontAsset(U"Clear")(U"クリア！").drawAt(clear_window.center(), Palette::Yellow);

		if (KeyEnter.down() || m_back_Button.leftClicked())
		{
			changeScene(State::Level);
		}
	}

	// もしゲームクリア済みならフラグを立てる (クリア画面表示用)
	if (board.is_clear()) game_clear = true;
}

void Game::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

	m_back_Button.draw(ColorF{ 1.0, m_back_Transition.value() }).drawFrame(2);

	FontAsset(U"Menu")(U"戻る").drawAt(m_back_Button.center(), ColorF{ 0.25 });
}
