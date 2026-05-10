#pragma once
#include"DirectWriteCustomFont.h"

//DWriteクラスの利用手順
//①読み込むファイル情報を入力（DirectWriteCustomFont.hの36行目のファイルパスを変更する）
//②その下の列挙型(enum)にフォント名を入力（プログラム実行中にフォントを変更する場合のみに使用。）
//③KdDirect3D.cppの52行目（第四引数）　"createFlg" → "D3D11_CREATE_DEVICE_BGRA_SUPPORT"　に変更
//④Init()で書いているフォントデータの改変を自分好みにカスタマイズする。
//⑤プログラム中でInit()を呼び出す。（SwapChainが作られた後に呼び出す）
//⑥Draw()で描画を行う。

//文字列描画用の関数
class DWriteCustom
{
public:
	//初期化
	void Init()
	{
		// DirectWrite用コンポーネントを作成
		m_pWrite = new DirectWriteCustomFont(&m_data);

		// 初期化
		m_pWrite->Init(D3D.GetSwapChain());

		// フォントデータを改変
		m_data.fontSize = 60;
		m_data.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
		m_data.Color = D2D1::ColorF(D2D1::ColorF::White);
		m_data.font = m_pWrite->GetFontName(0);
		/*	m_data.shadowColor = D2D1::ColorF(D2D1::ColorF::Gray);
			m_data.shadowOffset = D2D1::Point2F(5.0f, -5.0f);*/
		m_data.shadowColor = D2D1::ColorF(D2D1::ColorF::Gray);
		m_data.shadowOffset = D2D1::Point2F(0, 0);

		// フォントをセット
		m_pWrite->SetFont(m_data);
	}

	//描画1（登録されているフォントデータのまま描画する）
	void Draw(const std::string str, Math::Vector2 pos);

	//描画2（文字サイズを変更して描画する）
	void Draw(const std::string str, Math::Vector2 pos, const int size);

	//描画3（文字色を変更して描画する）
	void Draw(const std::string str, Math::Vector2 pos, const Math::Color& color);

	//描画4（文字色と文字サイズを変更して描画する）
	void Draw(const std::string str, Math::Vector2 pos, const int size, const Math::Color& color);

	//フォントを変更する
	void ChangeFont(FontName name)
	{
		if (name >= 0 && name < FontName::Kind)
		{
			m_data.font = m_pWrite->GetFontName(name);
			m_pWrite->SetFont(m_data);
		}
	}


	float GetFontSize() { return m_data.fontSize; }
	D2D1_COLOR_F GetColor() { return m_data.Color; }
	D2D1_COLOR_F GetShadowColor() { return m_data.shadowColor; }

	//フォントサイズを変更する
	void SetFontSize(float set)
	{
		m_data.fontSize = set;
		m_pWrite->SetFont(m_data);
	}

	void SetColor(D2D1_COLOR_F set )
	{
		m_data.Color = set;
		m_pWrite->SetFont(m_data);
	}

	//フォントの影の色を変更する
	void SetShadow(D2D1_POINT_2F offset,D2D1_COLOR_F color)
	{
		m_data.shadowOffset = offset;
		m_data.shadowColor = color;
		m_pWrite->SetFont(m_data);
	}

private:


	DirectWriteCustomFont* m_pWrite;					//描画用のクラス
	FontData m_data;									//フォントデータ用の構造体

	//座標補正
	void CorrectionPos(Math::Vector2* pos)
	{
		//ウィンドウの中心を(0,0)に合わせた座標にする
		pos->x += 1280.0f / 2.0f;
		pos->y -= 720.0f / 2.0f;
		pos->y *= -1;
	}
private:
//シングルトン
	DWriteCustom() {}
public:
	static DWriteCustom& Instance()
	{
		static DWriteCustom instance;
		return instance;
	}
};
