#include"DWriteCustom.h"

void DWriteCustom::Init()
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
void DWriteCustom::Draw(const std::string str, Math::Vector2 pos)
{
	//座標補正
	CorrectionPos(&pos);

	//描画
	m_pWrite->DrawString(str, pos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT, true);
}

//描画2（文字サイズを変更して描画する）
void DWriteCustom::Draw(const std::string str, Math::Vector2 pos, const int size)
{
	//座標補正
	CorrectionPos(&pos);

	//変更するデータを一時的に保存しておく
	float tempFontSize = m_data.fontSize;

	//データを変更する
	m_data.fontSize = size;
	m_pWrite->SetFont(m_data);

	//描画
	m_pWrite->DrawString(str, pos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT, true);

	//元のフォントデータに戻す
	m_data.fontSize = tempFontSize;
	m_pWrite->SetFont(m_data);
}

//描画3（文字色を変更して描画する）
void DWriteCustom::Draw(const std::string str, Math::Vector2 pos, const Math::Color& color)
{
	//座標補正
	CorrectionPos(&pos);

	//変更するデータを一時的に保存しておく
	D2D_COLOR_F tempColor = m_data.Color;

	//データを変更する
	m_data.Color = { color.R(),color.G(),color.B(),color.A() };
	m_pWrite->SetFont(m_data);

	//描画
	m_pWrite->DrawString(str, pos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT, true);

	//元のフォントデータに戻す
	m_data.Color = tempColor;
	m_pWrite->SetFont(m_data);
}

//描画4（文字色と文字サイズを変更して描画する）
void DWriteCustom::Draw(const std::string str, Math::Vector2 pos, const int size, const Math::Color& color)
{
	D3D.SetBackBuffer();

	//座標補正
	CorrectionPos(&pos);

	//変更するデータを一時的に保存しておく
	float tempFontSize = m_data.fontSize;
	D2D_COLOR_F tempColor = m_data.Color;

	//データを変更する
	m_data.fontSize = size;
	m_data.Color = { color.R(),color.G(),color.B(),color.A() };
	m_pWrite->SetFont(m_data);

	//描画
	m_pWrite->DrawString(str, pos, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT, true);

	//元のフォントデータに戻す
	m_data.fontSize = tempFontSize;
	m_data.Color = tempColor;
	m_pWrite->SetFont(m_data);
}