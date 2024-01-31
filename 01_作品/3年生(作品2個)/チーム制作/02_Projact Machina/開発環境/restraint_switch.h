//==============================================================================================
//
// 拘束スイッチ　　　restraint_switch.h
// tutida ryousei
//
//==============================================================================================
#ifndef _RESTRAINT_SWITCH_H_
#define	_RESTRAINT_SWITCH_H_

#include"move_object.h"

class CObject3D;

class CRestraint_Switch : public CMove_Object
{
public:
	// スイッチの数
	static const int SWITHC_NUM = 4;

	CRestraint_Switch(const PRIORITY priority);
	~CRestraint_Switch();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Hit(CMove_Object* pHit) override;

	static CRestraint_Switch *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, int index, const PRIORITY priority = PRIORITY_MODEL);

	bool GetHit() { return m_bHit; }
	bool GetPush() { return m_bPush; }
	int GetCountSwitch() { return m_nCount_Switch; }

	void SetHit(bool hit) { m_bHit = hit; }
	void AddCountSwitch() { m_nCount_Switch++; m_bPush = true; }

private:
	static int m_nCount_Switch;		// 押されたスイッチの数
	int m_nIndex;
	bool m_bPush;
	bool m_bHit;
	bool m_bBoss;
	CObject3D *m_Display_Key;		// キーの表示
};

#endif // !_RESTRAINT_SWITCH_H_
