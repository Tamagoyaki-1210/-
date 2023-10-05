//==============================================================================================
//
// ギミックモデル　　　gimmick_model.h
// tutida ryousei
//
//==============================================================================================
#ifndef _GIMMICK_MODEL_H_
#define	_GIMMICK_MODEL_H_

#include"trolley.h"

class CGimmick_Model : public CTrolley
{
public:
	CGimmick_Model();
	~CGimmick_Model();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CGimmick_Model *Create(D3DXVECTOR3 pos, int index, const char *Xfilename);

private:
	D3DXVECTOR3 m_Size;
};

#endif // !_GIMMICK_MODEL_H_
