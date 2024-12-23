﻿#pragma once
#include "Common.hpp"

//TODO ; ミスってヘッダファイルに関数の中身まで全部書いちゃった，あとでソースと分割して書く

// グリッドデータ用のキャラクタをメモ
const String PLAYER	= Unicode::Widen("S");
const String GOAL	= Unicode::Widen("G");
const String WALL	= Unicode::Widen("#");
const String L_ROT  = Unicode::Widen("L");
const String R_ROT  = Unicode::Widen("R");
const String BOMB	= Unicode::Widen("B");

// グリッドの移動方向の差分を表す列
// L , D , R , U の順 (プレイヤーの回転操作を実装するとき用の仕様)
const int dy[4] = {0,1,0,-1};
const int dx[4] = {1,0,-1,0};

// ステージの情報を表すデータの構造体
struct StageData
{
	//ステージの縦横サイズ
	int32 stage_height;
	int32 stage_width;

	//クリアに必要な最短手数 -> 直接計算する関数を内部に実装するので必要なくなりそう
	int32 minimum_clear_turn;

	//マスに何がおかれているか？の情報
	Array<String> grid_info;

	StageData() : stage_height(0), stage_width(0) {}

	StageData(int32 h, int32 w, int32 turn, std::initializer_list<String> grid = {})
		: stage_height(h), stage_width(w), minimum_clear_turn(turn), grid_info(grid) {}
};


// ステージデータを入力として，クリアまでの最短ターン数を計算する
// プレイヤーの人数を k とし，盤面の広さを HW としたとき，O((4HW)^k) くらいの計算が必要
// 定数倍はかなり重そうかも
int32 calc_minimum_turn(StageData stage)
{
	// ステージの高さと幅
	const int32 height = stage.stage_height;
	const int32 width = stage.stage_width;

	// 番兵として無限を入れるため十分大きい値をセット
	const int32 INF = 1000000;

	// プレイヤーの状態を管理する Array に名前を付ける
	using players_pos = Array<std::pair<int,int> >; 

	// 答えを格納する Array を定義
	std::map<players_pos,int32> distance_state;

	// BFS 探索に用いる queue
	std::queue<players_pos> q;

	// 座標 (x,y) を x*w+y で変換して int32 の数字一つで持てるようにする．(高速化のための処理)
	auto get_coord = [&width](int32 x, int32 y)->int {return x * width + y; };

	// 座標 x*w+y を 座標 (x,y) に戻す．
	auto get_coord = [&width](int32 x, int32 y)->int {return x * width + y; };

	// プレイヤーの初期地点
	players_pos first_pos;

	for (auto i : step(height))
	{
		for (auto j : step(width))
		{
			if (String(1, stage.grid_info[i][j]) == PLAYER)
			{
				first_pos.push_back(std::make_pair(get_coord(i, j), 0));
			}
		}
	}

	q.push(first_pos);

	distance_state[first_pos] = 0;

	// いける状態が空でない間，BFSを行う．
	// すべてのキャラがゴールマスにいるとき，距離をそのまま返す．
	while (!q.empty())
	{
		players_pos now_position = q.front();
		q.pop();

		for (auto k : step(4))
		{
			players_pos next_position = now_position;
			for (auto player_index : step(now_position.size()))
			{
			}
		}
	}

	return INF;
}

// 実際に動かす Player の構造体
struct Player {
	// Playerの座標
	Point pos;

	// Playerの回転角
	double angle;

	// Playerの向き (回転マス等を実装するときに必要)
	int32 dir;
};

// 盤面の状態管理を行う構造体
// 現状,この構造体で盤面情報を管理 -> Board の情報を用いて実際のゲーム画面へ反映 という流れ．
// 描画周りまでこちらに任せてしまう形式のほうがよいか？
struct Board {

	// ステージデータの読み込み・保存用
	// 最初の状態に戻すボタンの実装時などに使う
	StageData stage_info;

	/////////////////////////////
	// 各オブジェクトを表す配列 //
	////////////////////////////
	
	// プレイヤーの座標
	Array<Player> players;
	// ゴール地点の座標
	Array<Point> goals;
	// 右回りマスの座標
	Array<Point> l_rots;
	// 左回りマスの座標
	Array<Point> r_rots;

	// アニメーション用に，プレイヤーの現在位置を示す double 型の配列も定義
	Array<Vec2> player_pos;

	// 回転アニメーション用に，プレイヤーの残り回転角度を示す double 型の配列
	Array<double> player_angle_rest;

	// 現在のターン数
	int32 now_turn;

	Board() : now_turn(0) {}

	Board(StageData data) : stage_info(data), now_turn(0)
	{
		for (auto w : step(data.stage_width))
		{
			for (auto h : step(data.stage_height))
			{
				if (String(1, data.grid_info[h][w]) == PLAYER) {
					players.push_back(Player(Point(w, h), 0.0, 0));
					player_pos.push_back(Vec2(w, h));
					player_angle_rest.push_back(0.0);
				}
				else if (String(1, data.grid_info[h][w]) == GOAL) {
					goals.push_back(Point(w, h));
				}
				else if (String(1, data.grid_info[h][w]) == L_ROT) {
					l_rots.push_back(Point(w, h));
				}
				else if (String(1, data.grid_info[h][w]) == R_ROT) {
					r_rots.push_back(Point(w, h));
				}
			}
		}

		if (players.size() != goals.size())
		{
			// 仕様変更によりこれがあり得るステージを作る可能性あり．
			Logger << U"ステージデータ読み込みエラー：プレイヤー数とゴール数が異なる\n";
		}
	}

	// 盤面を初期状態にする．
	// 最初の状態に戻すボタンの実装時に使う．
	void init()
	{
		players.clear();
		goals.clear();
		player_pos.clear();
		player_angle_rest.clear();
		now_turn = 0;
		for (auto w : step(stage_info.stage_width))
		{
			for (auto h : step(stage_info.stage_height))
			{
				if (String(1, stage_info.grid_info[h][w]) == PLAYER) {
					players.push_back(Player(Point(w, h), 0.0, 0));
					player_angle_rest.push_back(0.0);
				}
				else if (String(1, stage_info.grid_info[h][w]) == GOAL) {
					goals.push_back(Point(w, h));
				}
			}
		}

		for (const Player &p : players)
		{
			player_pos.push_back(Vec2(p.pos));
		}
	}

	// マス (h,w) が壁か判定する関数
	bool is_wall(int32 w, int32 h) const {
		return String(1, stage_info.grid_info[h][w]) == WALL;
	}

	// マス (h,w) にプレイヤーが移動できるか判定する関数
	bool is_go(int32 h, int32 w) const {
		if (h < 0 || w < 0 || h >= stage_info.stage_height || w >= stage_info.stage_width) return false;
		if (is_wall(w, h)) return false;
		return true;
	}

	// Point を引数に使うバージョンも定義
	bool is_go(Point p) const {
		int32 h = p.y;
		int32 w = p.x;
		return is_go(h, w);
	}

	// ゲームがクリア済みか判定する
	bool is_clear()
	{
		int32 cnt = 0;
		for (auto i : step(goals.size()))
		{
			for (auto j : step(players.size()))
			{
				if (goals[i] == players[j].pos)
				{
					cnt++;
					break;
				}
			}
		}
		return cnt == goals.size();
	}

	// プレイヤーを一手進める関数
	// int32 を引数に取り，0->L 1->D 2->R 3->U に対応する
	// players.dir を参照して向きも反映する
	void operate_player(int32 r) {
		Array<Player> calc = players;
		for (auto i : step(players.size())){

			int32 dir = (r + players[i].dir)%4;
			Point players_nxt_pos = players[i].pos;

			players_nxt_pos.x += dx[dir];
			players_nxt_pos.y += dy[dir];

			if (!is_go(players_nxt_pos)) continue;

			calc[i].pos = players_nxt_pos;
		}

		// 行き先で他のプレイヤーとぶつかるような移動に関しては移動を行わない
		Array<int32> del;
		for (auto i : step(players.size()))
		{
			for (auto j : step(players.size()))
			{
				if (i != j && calc[i].pos == calc[j].pos)
				{
					del.push_back(i);
					break;
				}
			}
		}
		for (auto i : del) calc[i].pos = players[i].pos;

		players = calc;

		// 回転マスに乗ったときは残り回転角の値を更新
		// dir(今の向きを表す変数)の更新もここで行う
		for (auto i : step(players.size())) {
			auto [w, h] = players[i].pos;
			if(String(1, stage_info.grid_info[h][w]) == L_ROT) {
				player_angle_rest[i] += 360_deg + 90_deg;
				players[i].dir = (players[i].dir + 1) % 4;
			}
			if (String(1, stage_info.grid_info[h][w]) == R_ROT) {
				player_angle_rest[i] += -(360_deg + 90_deg);
				players[i].dir = (players[i].dir + 3) % 4;
			}
		}
	}

};

const Array<Array<StageData> > StageInfo = {
	{// Stage1
		StageData( // 1-1
			6,
			5,
			7,
			{
			U".S...",
			U"....G",
			U"....#",
			U"S...G",
			U".....",
			U"....."
			})
		,

		StageData( // 1-2
		6,
		5,
		7,
		{
		U".S...",
		U".L..G",
		U"....#",
		U"S...G",
		U"..R..",
		U"....."
		})

		,

		StageData( // 1-3
		10,
		10,
		7,
		{
		U".S........",
		U".L..G.....",
		U"....#.....",
		U"S...G.....",
		U"..RLLRR...",
		U"..LLRRL...",
		U"....#.....",
		U"S...G.....",
		U"..RLLRR...",
		U"..LLRRL..."
		})
	} ,
	{ // Stage2
		StageData( // 2-1
			6,
			5,
			7,
			{
			U"..G..",
			U".....",
			U"#...#",
			U"RLLRL",
			U"LLRLL",
			U"RLSLL"
			}),
		StageData( // 2-2
		10,
		10,
		7,
		{
		U"RSLLRLRLLR",
		U"RLLLGRRLRL",
		U"LLRL#RRLLR",
		U"SRRRGRRLLR",
		U"LRRLLRRRLL",
		U"RRLLRRLRRL",
		U"RRLL#RLLRR",
		U"SRLRGLRRLR",
		U"RLRLLRRLLR",
		U"RRLLRRLRLL"
		})
	}
};
