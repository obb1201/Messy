#pragma once

#include <memory>

#include "System/Container/Vector.h"
#include "System/Hash.h"

namespace fm
{
	enum class eFileType : uint8;

	enum class ePackageStatus
	{
		Unused,
		Queued,
		Loaded,
	};

	struct ResourceDesc
	{
		hashid Name;
		Handle Handle;
		eFileType Type;
	};

	struct PackageDesc
	{
	private:
		static const int MAX_RESOURCES_PER_PACKAGE = 256;

	public:
		ePackageStatus Status;
		Vector<ResourceDesc, MAX_RESOURCES_PER_PACKAGE> Resources;
	};
}