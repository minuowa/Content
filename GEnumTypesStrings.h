#ifndef GEnumTypesStrings_h__
#define GEnumTypesStrings_h__

#include "GEnums.h"
#undef GEnum
#undef GEle
#undef GEleV


#define GEnum(type) \
	CXDefineOnce CXEnumStructHelper<type> GCatCount(__gEnumHelper);\
	template<> inline void CXEnumStructHelper<type>::regist() 

#define GEle(ele)		getStructList().push_back(new CXEnumStruct(ele,#ele));
#define GEleV(ele,n)	getStructList().push_back(new CXEnumStruct(n,#ele));

#include "GEnums.inl"
#endif // 
