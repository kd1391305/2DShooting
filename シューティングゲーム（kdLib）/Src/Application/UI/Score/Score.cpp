#include"Score.h"
#include"../../TextureCache/TextureCache.h"

//デストラクタ
Score::~Score()
{
	while (!m_waitScore.empty())
	{
		m_score += m_waitScore.front();
		m_waitScore.pop();
	}
	//ハイスコアを書き換える
	if (m_score > m_highScore)
	{
		m_highScore = m_score;
		SaveHighScore();
	}
}

void Score::Init()
{
	m_score = 0;
	m_highScore = 0;
	LoadHighScore();
}

void Score::Update()
{
	//スコア加算の待機配列の先頭の値を取り出す
	if (!m_waitScore.empty())
	{
		m_score += m_waitScore.front();
		m_waitScore.pop();
	}
}

void Score::Draw()
{
	Math::Matrix scaleMat, transMat;
	{
		scaleMat = Math::Matrix::CreateScale(0.1f);
		transMat = Math::Matrix::CreateTranslation(-362, 340, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/Score_White.png"));
		transMat = Math::Matrix::CreateTranslation(-380, 310, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/HighScore_White.png"));
	}

	{
		KdTexture* tex = TextureCache::Instance().Get("Texture/UI/colon.png").get();
		scaleMat = Math::Matrix::CreateScale(0.1f);
		transMat = Math::Matrix::CreateTranslation(-338, 340, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(tex);
		transMat = Math::Matrix::CreateTranslation(-338, 310, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(tex);
	}

	//現在のスコアを描画
	DrawScore(m_score, { -310,340 });
	DrawScore(m_highScore, { -310,310 });
}

void Score::Add(int addScore)
{
	//スコアをs_waitCnt分に分割し、分割したスコアを毎フレーム足していく
	int temp[s_waitCnt];		//分割したスコアの一時的な入れ物

	int sum = 0;
	for (int i = 0; i < s_waitCnt; i++)
	{
		temp[i] = addScore / s_waitCnt;
		sum += temp[i];
	}
	//切り捨てられた値があったら
	if (sum < addScore)
	{
		//切り捨てられた分を最後の分割したスコアに加算
		temp[s_waitCnt - 1] += addScore - sum;
	}

	//スコア加算の待機配列に値を追加
	for (int i = 0; i < s_waitCnt; i++)
	{
		m_waitScore.push(temp[i]);
	}
}

void Score::LoadHighScore()
{
	FILE* fp;
	if ((fp = fopen("Data/HighScore.csv", "r")) != nullptr)
	{
		//ハイスコアを読み込む
		fscanf_s(fp, "%ld", &m_highScore);
		fclose(fp);
	}
}

bool Score::SaveHighScore()
{
	FILE* fp;

	//ハイスコアを新たに書き込む
	if ((fp = fopen("Data/HighScore.csv", "w")) != nullptr)
	{
		fprintf_s(fp, "%ld", m_highScore);
		fclose(fp);
		//セーブが出来た
		return true;
	}

	//セーブができなかった
	return false;
}

void Score::DrawScore(int score, Math::Vector2 pos)
{
	bool drawFlg = false;
	char scoreStr[9];
	char path[50];
	Math::Matrix scaleMat, transMat;
	scaleMat = Math::Matrix::CreateScale(0.17f);
	sprintf_s(scoreStr, sizeof(scoreStr), "%.8d", score);
	float scorePosY = -160;
	//一桁ずつ描画
	for (int i = 0; i < 8; i++)
	{
		transMat = Math::Matrix::CreateTranslation(pos.x + 15 * i, pos.y, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		sprintf_s(path, sizeof(path), "Texture/UI/Digit/%c.png", scoreStr[i]);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(path));
	}
}