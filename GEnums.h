#ifndef GEnumTypes_h__
#define GEnumTypes_h__

#undef GEnum
#undef GEle
#undef GEleV
#define GEnum(type) enum type
#define GEle(ele) ele,
#define GEleV(ele,n) ele=n,
#include "GEnums.inl"
#endif // GEnumTypes_h__