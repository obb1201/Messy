#include <CppUnitTest.h>

#include "system/container/Array.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fm;

namespace FMUnitTest
{		
	TEST_CLASS(SystemContainerArray)
	{
	public:
		class Dummy
		{
		public:
			int Number;
		};

		TEST_METHOD(Array_Value)
		{
			const auto arraySize = 16U;

			Array<int, arraySize> array;
			for (auto i = 0U; i < arraySize; ++i)
			{
				array[i] = i * 2;
			}

			for (auto i = 0; i < arraySize; ++i)
			{
				Assert::AreEqual(array.FindIndex(i * 2), i, L"array index is wrong", LINE_INFO());
			}

			Assert::AreEqual(array.GetLength(), arraySize, L"array size is wrong", LINE_INFO());
			Assert::AreEqual(array.FindIndex(-2030), INVALID_INDEX, L"array index is wrong", LINE_INFO());
		}
	};
}