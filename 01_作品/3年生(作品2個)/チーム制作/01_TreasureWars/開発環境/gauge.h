//==============================================================================================
//
// ゲージ　　　gauge.h
// tutida ryousei
//
//==============================================================================================
#ifndef _GAUGE_H_
#define	_GAUGE_H_

#include"object2D.h"
#include"ui.h"

class CGauge : public CUi
{
public:
	CGauge();
	~CGauge() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CGauge *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void ZeroGauge();	// ゲージを0にする
	void GaugeUp();		// ゲージを増やす

	void SetBase_Size(D3DXVECTOR3 base_size) { m_Base_Size = base_size; }
	
private:
	
	CObject2D *m_pBar;			// 後ろの黒いバー
	CObject2D *m_pGauge;		// 前方の伸び縮みするゲージ
	D3DXVECTOR3 m_Base_Size;	// 元のサイズ
	D3DXVECTOR3 m_Size;			// 現在のサイズ
	float m_fGauge_Heal;		// ゲージの回復速度
	bool m_bZero;
	static bool m_bUse;			// 誰かがゲージを使用したかの判定
};

#endif // !_GAUGE_H_
