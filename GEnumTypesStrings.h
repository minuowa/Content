#ifndef GEnumTypesStrings_h__
#define GEnumTypesStrings_h__

#include "GEnums.h"
#undef GEnum
#undef GEle
#undef GEleV


#define GEnum(type) \
	CXDefineOnce CXEnumStructHelper<type> GCatCount(__gEnumHelper);\
	template<> inline void CXEnumStructHelper<type>::regist() 

#define GEle(ele)		mStructList.push_back(CXEnumStruct(ele,#ele));
#define GEleV(ele,n)	mStructList.push_back(CXEnumStruct(n,#ele));

#include "GEnums.inl"
#endif // 
