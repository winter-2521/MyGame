#pragma once
#include <Siv3D.hpp>

// 扱うシーンの名前を列挙
enum class State
{
	Title,
	Game,
	Option,
	Stage,
	Level,
};

// ステージの情報を表すデータの構造体
struct StageData
{
	//ステージの縦横サイズ
	int32 stage_height;
	int32 stage_width;

	//クリアに必要な最短手数
	int32 minimum_clear_turn;

	//マスに何がおかれているか？の情報
	String grid_info[50];

	StageData(int32 h, int w, int turn, std::initializer_list<String> grid = {}) : stage_height(h), stage_width(w), minimum_clear_turn(turn)
	{
		std::copy(grid.begin(), grid.end(), grid_info);
	}
};

// シーン間で共有するデータの構造体
struct GameData
{
	// 選択したステージ番号
	int32 stage_number = 0;

	// 選択したレべル番号
	int32 level_number = 0;
};

using App = SceneManager<State, GameData>;
