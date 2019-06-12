//*****************************************************************************
//
//     水の処理[water.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "water.h"
#include "manager.h"
#include "renderer.h"

#include "input.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define WATER_TEXTURE_NAME_0  "data/TEXTURE/COMMON/water001.png"  // テクスチャのファイル名(水)
#define WATER_TEXTURE_NAME_1  "data/TEXTURE/COMMON/water000.png"  // テクスチャのファイル名(水)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
int                CWater::m_nNumAll = 0;                      // 水の総数
LPDIRECT3DTEXTURE9 CWater::m_apTexture[MAX_WATER_TEX] = {};    // 水のテクスチャ画像

//=============================================================================
//    コンストラクタ
//=============================================================================
CWater::CWater(int nPriority, OBJTYPE objType) : CScene3DMesh(nPriority, objType)
{
	// 各種値の初期化
	m_fTexU = 0.0f;           // テクスチャU座標
	m_fTexV = 0.0f;           // テクスチャV座標
	m_fWidth = 0.0f;          // 1ブロック分の幅
	m_fDepth = 0.0f;          // 1ブロック分の奥行
	m_nXBlock = 0;            // 横の分割数
	m_nZBlock = 0;            // 奥行の分割数
	m_fRipples = 0.0f;	      // 波紋計算用
	m_fRipplesHeight = 0.0f;  // 波紋の高さ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CWater::~CWater()
{

}

//=============================================================================
//    水の生成処理
//=============================================================================
CWater *CWater::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName, int nTexIdx, int nPriority)
{
	CWater *pWater = NULL;                 // 水クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pWater == NULL)
		{// メモリが空になっている
			pWater = new CWater(nPriority);
			if (pWater != NULL)
			{// インスタンスを生成できた
				if (FAILED(pWater->Init(pos, rot, col, fWidth, fDepth, nXBlock, nZBlock, aVertexFileName, nTexIdx)))
				{// 初期化に失敗した
					return NULL;
				}
			}
			else
			{// インスタンスを生成できなかった
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pWater;   // インスタンスのアドレスを返す
}

//=============================================================================
//    水のテクスチャ読み込み処理
//=============================================================================
HRESULT CWater::Load(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			char aTexName[MAX_WATER_TEX][256] =
			{
				WATER_TEXTURE_NAME_0,
				WATER_TEXTURE_NAME_1,
			};
			for (int nCntTex = 0; nCntTex < MAX_WATER_TEX; nCntTex++)
			{
				D3DXCreateTextureFromFile(pDevice, aTexName[nCntTex], &m_apTexture[nCntTex]);
			}
		}
	}
	return S_OK;
}

//=============================================================================
//    地面のテクスチャ解放処理
//=============================================================================
void CWater::UnLoad(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_WATER_TEX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    地面の初期化処理
//=============================================================================
HRESULT CWater::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName, int nTexIdx)
{
	int nNumVertex;  // 頂点数設定用
	int nNumIndex;   // インデックス数計算用
	int nNumPolygon; // ポリゴン数計算用

	// 各種値の設定
	BindTexture(m_apTexture[nTexIdx]);           // テクスチャ
	SetPos(pos);                                 // 座標
	SetRot(rot);                                 // 向き
	SetCol(col);                                 // 色
	m_fWidth = fWidth;                           // 1ブロック分の幅
	m_fDepth = fDepth;                           // 1ブロック分の奥行
	m_nXBlock = nXBlock;                         // 横の分割数
	m_nZBlock = nZBlock;                         // 奥行の分割数
	strcpy(m_aVertexFileName, aVertexFileName);  // 頂点情報のスクリプトファイル名

	// メッシュの作成
	// 作成に必要な頂点数,インデックス数,ポリゴン数を計算
	nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);                                                               // 頂点数を計算
	nNumIndex = ((m_nXBlock + 1) * (m_nZBlock + 1)) + (2 * (m_nZBlock - 1)) + (m_nXBlock + 1) * (m_nZBlock - 1);  // インデックス数を設定
	nNumPolygon = ((m_nXBlock * m_nZBlock) * 2) + ((m_nZBlock - 1) * 4);                                          // 描画するポリゴン数を設定

	// 計算した値を設定する
	SetNumVertex(nNumVertex);    // 頂点数
	SetNumIndex(nNumIndex);      // インデックス数
	SetNumPolygon(nNumPolygon);  // ポリゴン数

	// 共通の初期化処理
	CScene3DMesh::Init();

	return S_OK;
}

//=============================================================================
//    地面の終了処理
//=============================================================================
void CWater::Uninit(void)
{
	// 共通の終了処理
	CScene3DMesh::Uninit();
}

//=============================================================================
//    地面の更新処理
//=============================================================================
void CWater::Update(void)
{
	// テクスチャ座標を進める
	m_fTexU += 0.0001f;
	m_fTexV += 0.0001f;

	// 頂点情報の取得
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが取得できた
		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexU = 0.0f + m_fTexU;
		float fTexV = 0.0f + m_fTexV;
		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// 縦の分割数 + 1分繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 横の分割数 + 1分繰り返し
				pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);
				fTexU += 2.0f / (m_nXBlock + 1);
			}
			fTexU = 0.0f;
			fTexV += 2.0f / (m_nZBlock + 1);
			pVtx += m_nXBlock + 1;  // ポインタを進める
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();

		// 頂点バッファを設定する
		SetVtxBuff(pVtxBuff);
	}

	if (m_State == STATE_RIPPLE)
	{// 波紋を起こす状態ならば
		SetRipples(m_RipplePos, 1000.0f, m_fRipplesHeight);
		m_fRipplesHeight -= 0.4f;
		if (m_fRipplesHeight <= 0.0f)
		{// 波紋が消えた
			m_fRipplesHeight = 0.0f;
			m_State = STATE_NONE;
		}
	}
}

//=============================================================================
//    地面の描画処理
//=============================================================================
void CWater::Draw(void)
{
	// 共通の描画処理
	CScene3DMesh::Draw();
}

//=============================================================================
//    地面の頂点バッファ生成処理
//=============================================================================
void CWater::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	FILE *pFile = NULL;               // ファイルポインタ
	int nNumVertex = GetNumVertex();  // 頂点数

	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;  // 頂点バッファ生成用
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(m_aVertexFileName, "rb");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 頂点情報の取得
			VERTEX_3D *pVtx;
			// 頂点バッファをロックし,頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// バイナリファイルからフィールドの情報を読み込む
			fread(pVtx, sizeof(VERTEX_3D), GetNumVertex(), pFile);

			float fTexU = 0.0f;
			float fTexV = 0.0f;
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
			for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
			{// 縦の分割数 + 1分繰り返し
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// 横の分割数 + 1分繰り返し
					pVtx[nCntH].col = col;

					pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);
					fTexU += 2.0f / (m_nXBlock + 1);
				}
				fTexU = 0.0f;
				fTexV += 2.0f / (m_nZBlock + 1);
				pVtx += m_nXBlock + 1;  // ポインタを進める
			}

			// 頂点バッファをアンロックする
			pVtxBuff->Unlock();

			// 頂点バッファを設定する
			SetVtxBuff(pVtxBuff);

			// ファイルを閉じておく
			fclose(pFile);

			// 法線計算処理
			MakeNormal();
		}
	}
}

//=============================================================================
//    地面のインデックスバッファ生成処理
//=============================================================================
void CWater::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	int nNumIndex = GetNumIndex();   // インデックス数

	// インデックスバッファの生成
	LPDIRECT3DINDEXBUFFER9 pIdxBuff = NULL;  // インデックスバッファ生成用
	pDevice->CreateIndexBuffer(sizeof(WORD) * nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pIdxBuff,
		NULL);

	WORD *pIdx;       // インデックスデータへのポインタ
	int nCntIdx = 0;  // インデックス番号

					  // インデックスバッファをロックし,インデックスデータへのポインタを取得
	pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_nZBlock; nCntIdxY++)
	{// 縦の分割数分繰り返し
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// 横の分割数+１繰り返し
		    // 上下の二つ分インデックス番号を設定
			pIdx[0] = nCntIdx + m_nXBlock + 1; // 下側
			pIdx[1] = nCntIdx;                 // 上側

			pIdx += 2;  // 2つ分進める
			if (nCntIdxY < m_nZBlock - 1 && nCntIdxX == m_nXBlock)
			{// 1 , 下側が縦の分割数の最下層ではない
			    // 2 , 横の分割数分設定が終わった
				pIdx[0] = nCntIdx;                       // 上側
				pIdx[1] = nCntIdx + (m_nXBlock + 1) + 1; // 次の下側

				pIdx += 2; // 2つ分進める
			}
		}
	}

	// インデックスバッファをアンロックする
	pIdxBuff->Unlock();

	// インデックスバッファを設定する
	SetIdxBuff(pIdxBuff);
}

//=============================================================================
//    地面の面の高さ取得処理
//=============================================================================
float CWater::GetPolyHeight(const D3DXVECTOR3 pos, D3DXVECTOR3 *pPolygonNormal)
{
	float fHeight = 0.0f;               // 高さ(最終的にこの値を返す)
	float fRivision = 0.0f;             // 高さの補正値(法線を割り出すのに0.0の高さにそろえるため)
	int nVertexIdxStart = 0;            // 左上の頂点番号(ループするたびにずれていく)
	D3DXMATRIX mtxRot, mtxTrans;         // 計算用ワールドマトリックス
	D3DXMATRIX VertexMtxWorld[4];       // 頂点のワールドマトリックス4つ分(回転,位置を含めて計算する)
	D3DXVECTOR3 VertexPos[4];           // 頂点の座標4つ分(この座標はローカル座標なので計算には使わない)
	D3DXVECTOR3 VertexWorldPos[4];      // 頂点のワールド座標4つ分(ワールドマトリックスから抜き出す)
	D3DXVECTOR3 VertexLeftWorldPos[3];  // 左側のポリゴンの頂点のワールド座標3つ分(ワールドマトリックスから抜き出す)
	D3DXVECTOR3 VertexRightWorldPos[3]; // 右側のポリゴンの頂点のワールド座標3つ分(ワールドマトリックスから抜き出す)
	D3DXVECTOR3 PolygonVerPos[3];       // ポリゴンの座標(判定用の法線を導くために必要)
	D3DXVECTOR3 PolygonNormal;          // ポリゴンの法線(判定用の法線なので見た目の法線とは異なる)
	D3DXVECTOR3 PolygonVector[2];       // ポリゴンの法線を割り出すためのベクトル
	D3DXVECTOR3 VecPosToPolygon;        // 引数の座標とポリゴンとのベクトル
	D3DXVECTOR3 VectorVerToVer;         // 外積ベクトル1本目(頂点から頂点へのベクトル)
	D3DXVECTOR3 VectorVerToPos;         // 外積ベクトル2本目(頂点から引数の座標へのベクトル)
	int nCntLeftPolygon = 0;            // 左側のポリゴンのベクトル判定を通った回数
	int nCntRightPolygon = 0;           // 右側のポリゴンのベクトル判定を通った回数

	// 頂点データへのポインタを宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
		{// 奥行の分割数分繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
			{// 横の分割数分繰り返し
				// 頂点のローカル座標を抜き出す
				VertexPos[0] = pVtx[nVertexIdxStart].pos;
				VertexPos[1] = pVtx[nVertexIdxStart + 1].pos;
				VertexPos[2] = pVtx[nVertexIdxStart + (m_nXBlock + 1)].pos;
				VertexPos[3] = pVtx[nVertexIdxStart + (m_nXBlock + 1) + 1].pos;

				// 頂点のワールドマトリックスを作り出す
				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// 1ブロック分の頂点数分繰り返し
					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&VertexMtxWorld[nCntVer]);

					// 回転を反映
					D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
					D3DXMatrixMultiply(&VertexMtxWorld[nCntVer], &VertexMtxWorld[nCntVer], &mtxRot);

					// 位置を反映
					D3DXMatrixTranslation(&mtxTrans, VertexPos[nCntVer].x, VertexPos[nCntVer].y, VertexPos[nCntVer].z);
					D3DXMatrixMultiply(&VertexMtxWorld[nCntVer], &VertexMtxWorld[nCntVer], &mtxTrans);

					// 親の情報を掛け合わせる
					D3DXMatrixMultiply(&VertexMtxWorld[nCntVer], &VertexMtxWorld[nCntVer], &GetMtxWorld());
				}

				// 頂点のワールド座標を抜き出す
				VertexWorldPos[0] = D3DXVECTOR3(VertexMtxWorld[0]._41, VertexMtxWorld[0]._42, VertexMtxWorld[0]._43);
				VertexWorldPos[1] = D3DXVECTOR3(VertexMtxWorld[1]._41, VertexMtxWorld[1]._42, VertexMtxWorld[1]._43);
				VertexWorldPos[2] = D3DXVECTOR3(VertexMtxWorld[2]._41, VertexMtxWorld[2]._42, VertexMtxWorld[2]._43);
				VertexWorldPos[3] = D3DXVECTOR3(VertexMtxWorld[3]._41, VertexMtxWorld[3]._42, VertexMtxWorld[3]._43);

				// 左側のポリゴンの判定開始
				// 左側のポリゴンの座標を設定
				VertexLeftWorldPos[0] = VertexWorldPos[0];
				VertexLeftWorldPos[1] = VertexWorldPos[3];
				VertexLeftWorldPos[2] = VertexWorldPos[2];
				for (int nCntLeft = 0; nCntLeft < 3; nCntLeft++)
				{// ポリゴンの頂点数分繰り返し
					// 外積のベクトルを作り出す
					VectorVerToVer = VertexLeftWorldPos[(nCntLeft + 1) % 3] - VertexLeftWorldPos[nCntLeft];
					VectorVerToPos = pos - VertexLeftWorldPos[nCntLeft];

					if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
					{// 引数の座標が頂点と頂点のベクトルの右側にいる
						nCntLeftPolygon++;  // 判定を通った回数を進める
					}
				}
				if (nCntLeftPolygon == 3)
				{// 外積の判定を全て通った
					// 判定用の頂点座標を作る
					PolygonVerPos[0] = D3DXVECTOR3(VertexLeftWorldPos[2].x, VertexLeftWorldPos[2].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[2].z);
					PolygonVerPos[1] = D3DXVECTOR3(VertexLeftWorldPos[1].x, VertexLeftWorldPos[1].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[1].z);
					PolygonVerPos[2] = D3DXVECTOR3(VertexLeftWorldPos[0].x, VertexLeftWorldPos[0].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[0].z);

					// 外積ベクトルを作る
					PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
					PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

					// 外積ベクトルから法線を割り出す
					D3DXVec3Cross(&PolygonNormal, &PolygonVector[0], &PolygonVector[1]);
					D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

					// 判定の仕方が0に戻したうえで計算しているので補正をするために値を保持
					fRivision = VertexLeftWorldPos[2].y;

					// 引数の座標と頂点座標のベクトルを計算
					VecPosToPolygon = pos - PolygonVerPos[0];

					// 内積の概念を利用して高さを割り出す
					float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X成分の内積を計算
					float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z成分の内積を計算
					float Dot = -DotX - DotZ;                               // X成分とZ成分の内積同士を引く
					fHeight = (Dot / PolygonNormal.y) + fRivision;          // 引いたものをY成分の法線で割って補正値を足してあげる
					*pPolygonNormal = PolygonNormal;                        // 法線を代入しておく

																			// 高さから角度を求める
					float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
					float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
					float DotY = (PolygonNormal.y * fHeight);
					float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
					float fAngle = acosf(cosSita);
				}

				// 右側のポリゴンの判定開始
				// 右側のポリゴンの座標を設定
				VertexRightWorldPos[0] = VertexWorldPos[0];
				VertexRightWorldPos[1] = VertexWorldPos[1];
				VertexRightWorldPos[2] = VertexWorldPos[3];
				for (int nCntLeft = 0; nCntLeft < 3; nCntLeft++)
				{// ポリゴンの頂点数分繰り返し
				    // 外積のベクトルを作り出す
					VectorVerToVer = VertexRightWorldPos[(nCntLeft + 1) % 3] - VertexRightWorldPos[nCntLeft];
					VectorVerToPos = pos - VertexRightWorldPos[nCntLeft];

					if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
					{// 引数の座標が頂点と頂点のベクトルの右側にいる
						nCntRightPolygon++;  // 判定を通った回数を進める
					}
				}
				if (nCntRightPolygon == 3)
				{// 外積の判定を全て通った
				    // 判定用の頂点座標を作る
					PolygonVerPos[0] = D3DXVECTOR3(VertexRightWorldPos[1].x, VertexRightWorldPos[1].y - VertexRightWorldPos[1].y, VertexRightWorldPos[1].z);
					PolygonVerPos[1] = D3DXVECTOR3(VertexRightWorldPos[0].x, VertexRightWorldPos[0].y - VertexRightWorldPos[1].y, VertexRightWorldPos[0].z);
					PolygonVerPos[2] = D3DXVECTOR3(VertexRightWorldPos[2].x, VertexRightWorldPos[2].y - VertexRightWorldPos[1].y, VertexRightWorldPos[2].z);

					// 外積ベクトルを作る
					PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
					PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

					// 外積ベクトルから法線を割り出す
					D3DXVec3Cross(&PolygonNormal, &PolygonVector[1], &PolygonVector[0]);
					D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

					// 判定の仕方が0に戻したうえで計算しているので補正をするために値を保持
					fRivision = VertexRightWorldPos[1].y;

					// 引数の座標と頂点座標のベクトルを計算
					VecPosToPolygon = pos - PolygonVerPos[0];

					// 内積の概念を利用して高さを割り出す
					float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X成分の内積を計算
					float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z成分の内積を計算
					float Dot = -DotX - DotZ;                               // X成分とZ成分の内積同士を引く
					fHeight = (Dot / PolygonNormal.y) + fRivision;          // 引いたものをY成分の法線で割って補正値を足してあげる
					*pPolygonNormal = PolygonNormal;                        // 法線を代入しておく

																			// 高さから角度を求める
					float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
					float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
					float DotY = (PolygonNormal.y * fHeight);
					float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
					float fAngle = acosf(cosSita);
				}

				// 判定用の変数を初期化しておく
				if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
				{// ポリゴンに乗っていると判定されている
					break;
				}
				else
				{// まだポリゴンに乗っていないと判定されている
					nCntLeftPolygon = 0;
					nCntRightPolygon = 0;
					nVertexIdxStart++;
				}
			}
			if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
			{// ポリゴンに乗っていると判定されている
				break;
			}
			else
			{// まだポリゴンに乗っていないと判定されている
			    // 右端のブロックに判定するべきポリゴンは存在しないので番号をずらしておく
				nVertexIdxStart++;
			}
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}

	return fHeight;
}

//=============================================================================
//    地面の法線計算処理
//=============================================================================
void CWater::MakeNormal(void)
{
	VERTEX_3D *pVtx;	                              // 頂点データへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // 頂点バッファ

	D3DXVECTOR3 VecVer[3];	    // 法線計算用ベクトル
	D3DXVECTOR3 *pNor = NULL;   // 頂点の法線

	// 必要な数だけ動的に確保する
	pNor = new D3DXVECTOR3[(m_nXBlock * m_nZBlock) * 2];

	// 外積用にベクトルを作る
	if (pNor != NULL)
	{	// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		int nCntVec1 = 0;                // 番号計算用(左上の頂点番号)
		int nCntVec2 = (m_nXBlock + 1);  // 番号計算用(左下の頂点番号)
		int nCntVec3 = (m_nXBlock + 2);  // 番号計算用(右下の頂点番号)
		int nCntVec4 = 1;                // 番号計算用(右上の頂点番号)
		int nCntNor = 0;                 // 番号計算用(法線の番号)
		for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
		{// 垂直方向の分割数だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
			{// 水平方向の分割数だけ繰り返し
				VecVer[0] = pVtx[nCntVec2].pos - pVtx[nCntVec1].pos;
				VecVer[1] = pVtx[nCntVec3].pos - pVtx[nCntVec1].pos;
				VecVer[2] = pVtx[nCntVec4].pos - pVtx[nCntVec1].pos;

				// 外積を使ってベクトルにする
				D3DXVec3Cross(&pNor[nCntNor], &VecVer[1], &VecVer[0]);
				D3DXVec3Cross(&pNor[nCntNor + 1], &VecVer[2], &VecVer[1]);

				// 出てきたベクトルを正規化して法線にする
				D3DXVec3Normalize(&pNor[nCntNor], &pNor[nCntNor]);
				D3DXVec3Normalize(&pNor[nCntNor + 1], &pNor[nCntNor + 1]);

				// 番号をずらす
				nCntNor += 2;
				nCntVec1++;
				nCntVec2++;
				nCntVec3++;
				nCntVec4++;
			}
			// 右端はもう終わっているので左端になるように番号をそろえる
			nCntVec1++;
			nCntVec2++;
			nCntVec3++;
			nCntVec4++;
		}

		// 法線を代入
		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				if (nCntH == 0)
				{// 右端だった場合
					if (nCntV == 0)
					{// 一番左上の頂点番号の場合
						pVtx[nCntH].nor = (pNor[0] + pNor[1]) / 2;  // OK
					}
					else if (nCntV == m_nZBlock)
					{// 一番左下の頂点番号の場合
						pVtx[nCntH].nor = pNor[((m_nXBlock * 2) * (m_nZBlock - 1))];  // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2))] + pNor[nCntV * (m_nXBlock * 2)] + pNor[nCntV * (m_nXBlock * 2) + 1]) / 3;   // OK
					}
				}
				else if (nCntH == m_nXBlock)
				{// 右端だった場合
					if (nCntV == 0)
					{// 一番右上の頂点番号の場合
						pVtx[nCntH].nor = pNor[(m_nXBlock * 2) - 1];  // OK
					}
					else if (nCntV == m_nZBlock)
					{// 一番右下の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((m_nXBlock * 2) * m_nZBlock) - 1] + pNor[((m_nXBlock * 2) * m_nZBlock) - 2]) / 2;  // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2)] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2) + 1] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2) + 2]) / 3;  // OK
					}
				}
				else
				{// 真ん中の場合
					if (nCntV == 0)
					{// 一番上の頂点番号の場合
						pVtx[nCntH].nor = (pNor[(nCntH * 2) - 1] + pNor[(nCntH * 2)] + pNor[(nCntH * 2) + 1]) / 3;   // OK
					}
					else if (nCntV == m_nZBlock)
					{// 一番下の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nZBlock - 1))] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nZBlock - 1)) + 1] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nZBlock - 1)) + 2]) / 3; // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2)] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2) + 1] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2) + 2] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2)] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2) + 1] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2) + 2]) / 6;  // OK
					}
				}
			}
			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		if (pNor != NULL)
		{// メモリが確保されている
			delete[] pNor;
			pNor = NULL;
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    水の波紋処理
//=============================================================================
void CWater::SetRipples(D3DXVECTOR3 pos, float fRange, float fHeight)
{
	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene3DMesh::GetVtxBuff();

	// 頂点情報を設定
	VERTEX_3D *pVtx;	// 頂点情報のポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 波紋を動かす
	m_fRipples += 0.08f;

	for (int nCntVtx = 0; nCntVtx < CScene3DMesh::GetNumVertex(); nCntVtx++)
	{// 全頂点の距離を計算
		float fLength = powf(pVtx[nCntVtx].pos.x - pos.x, 2.0f) + powf(pVtx[nCntVtx].pos.z - pos.z, 2.0f);

		if (fLength <= powf(fRange, 2.0f))
		{// 範囲内にある頂点
			float fVtxHeight = sinf(D3DX_PI * ((sqrtf(fLength) / fRange) * -14.0f) + m_fRipples) * (fHeight * (1.0f - (sqrtf(fLength) / fRange)));

			pVtx[nCntVtx].pos.y = fVtxHeight;
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファの設定
	CScene3DMesh::SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    水の波紋を出す座標設定する処理
//=============================================================================
void CWater::SetRipplePos(const D3DXVECTOR3 RipplePos)
{
	m_RipplePos = RipplePos;
}

//=============================================================================
//    水の状態を設定する処理
//=============================================================================
void CWater::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
//    水の波紋の高さを設定する処理
//=============================================================================
void CWater::SetRipplesHeight(const float fRipplesHeight)
{
	m_fRipplesHeight = fRipplesHeight;
}

//=============================================================================
//    水の波紋の高さを取得する処理
//=============================================================================
float CWater::GetRipplesHeight(void)
{
	return m_fRipplesHeight;
}