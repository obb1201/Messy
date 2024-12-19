#pragma once

// 
#define SHADERTYPE_LIST				\
	SHADERTYPE_LISTENTRY(Basic),	\
	SHADERTYPE_LISTENTRY(Player),	\
//	SHADERTYPE_LISTENTRY(Stadium),	\

namespace fm
{
#define SHADERTYPE_LISTENTRY(name) name
	enum class eShaderType
	{
		SHADERTYPE_LIST

		Count,
	};
#undef SHADERTYPE_LISTENTRY
}