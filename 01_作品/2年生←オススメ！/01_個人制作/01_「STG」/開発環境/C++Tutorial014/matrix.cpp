//=============================================================================
//
// åvéZópÉ}ÉgÉäÉNÉX
// Author : tanimoto kosuke
//
//=============================================================================
#include <stdio.h>
#include "matrix.h"

//=====================================
//ê≥ãKâªåvéZèàóù
//=====================================
float Normalize(float val)
{
	float valNormalize = val * (1 / sqrtf(val * val));
	return valNormalize;
}

//=====================================
//ê≥ãKâªåvéZèàóù
//=====================================
D3DXVECTOR3 NormalizeLength(float valX, float valY)
{
	float valNormalize = 1 / sqrtf((valX * valX) + (valY * valY));

	D3DXVECTOR3 Length(valX * valNormalize, valY * valNormalize, 0.0f);

	return Length;
}

//=====================================
////â~å^ìñÇΩÇËîªíËåvéZèàóù
//=====================================
bool CollisionCircle(D3DXVECTOR3 pos, float fLength, D3DXVECTOR3 posSub, float fLengthSub)
{
	float fSumLengthRad = (fLength + fLengthSub) / 2;				//â~ÇÃîºåaìØémÇÃòa

	float fSubX = pos.x - posSub.x;									//Xç¿ïWÇÃç∑ï™
	float fSubY = pos.y - posSub.y;									//Yç¿ïWÇÃç∑ï™
	float fSubLength = sqrtf((fSubX * fSubX) + (fSubY * fSubY));	//åªç›ÇÃ2ì_ÇÃãóó£

	return fSumLengthRad >= fSubLength;
}

//=====================================
//éläpå`ìñÇΩÇËîªíËåvéZèàóù
//=====================================
bool CollisionSquare(D3DXVECTOR3 pos, D3DXVECTOR3 posSub, float fWidth, float fHeight, float fWidthSub, float fHeightSub)
{
	float fLeft = pos.x - (fWidth / 2);		//éläpå`ç∂ë§
	float fRight = pos.x + (fWidth / 2);		//éläpå`âEë§
	float fUpp = pos.y - (fHeight / 2);		//éläpå`è„ë§
	float fLow = pos.y + (fHeight / 2);		//éläpå`â∫ë§

	float fLeftSub = posSub.x - (fWidthSub / 2);		//éläpå`ç∂ë§
	float fRightSub = posSub.x + (fWidthSub / 2);		//éläpå`âEë§
	float fUppSub = posSub.y - (fHeightSub / 2);		//éläpå`è„ë§
	float fLowSub = posSub.y + (fHeightSub / 2);		//éläpå`â∫ë§

	if (fLeft < fRightSub && fRight > fLeftSub
		&& fUpp < fLowSub && fLow > fUppSub)
	{
		return true;
	}
	else
	{
		return false;
	}
}
