#include <CppUnitTest.h>

#include "System\String.h"
#include "System\UnicodeString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fm;

namespace FMUnitTest
{
	TEST_CLASS(SystemString)
	{
	public:
		class Dummy
		{
		public:
			int Number;
		};

		TEST_METHOD(String_Value)
		{
			const auto arraySize = 16U;

			String<arraySize> s;
			for (auto i = 0U; i < arraySize; ++i)
			{
				s[i] = 'a' + i;
			}

			for (auto i = 0; i < arraySize; ++i)
			{
				Assert::AreEqual(s[i], static_cast<const char>('a' + i), L"string index is wrong", LINE_INFO());
			}

			Assert::AreEqual(s.GetCapacity(), arraySize, L"string capacity is wrong", LINE_INFO());

			const char* test = "test";
			String<arraySize> s2(test);
			for (int i = 0; i < 4; ++i)
			{
				Assert::AreEqual(s2[i], test[i], L"String(const char*) is wrong", LINE_INFO());
			}

			// Append test
			s2.Append(test);	// string without format
			for (int i = 0; i < 8; ++i)
			{
				Assert::AreEqual(s2[i], test[i % 4], L"String.Append(normal string) is wrong", LINE_INFO());
			}

			const char format[] = { "%d%d%1.2f" };
			s2.Append(format, 1, 3, 2.52f);
			char testStr[10];
			int size = snprintf(testStr, 10, format, 1, 3, 2.52f);

			for (int i = 0; i < 8; ++i)
			{
				Assert::AreEqual(s2[i], test[i % 4], L"String.Append() changed existing data", LINE_INFO());
			}
			for (int i = 0; i < size; ++i)
			{
				Assert::AreEqual(s2[i + 8], testStr[i], L"String.Append(format) is wrong", LINE_INFO());
			}

			// SubString test
			const char* sub = s2.SubString(8);
			for (int i = 0; i < size; ++i)
			{
				Assert::AreEqual(sub[i], testStr[i], L"SubString is wrong", LINE_INFO());
			}

			const char* str = s2.GetCString();
			for (int i = 0; i < 8; ++i)
			{
				Assert::AreEqual(str[i], test[i % 4], L"String.Append() changed existing data", LINE_INFO());
			}
			for (int i = 0; i < size; ++i)
			{
				Assert::AreEqual(str[i + 8], testStr[i], L"String.Append(format) is wrong", LINE_INFO());
			}
		}

		TEST_METHOD(String_Unicode)
		{
			const uint32 SIZE = 16;
			UnicodeString<SIZE> s("test");

			Assert::AreEqual(s.GetLength(), 4U, L"ascii length is wrong", LINE_INFO());

			UnicodeString<5> s2(u8"\uBDC1");	// 뷁
			Assert::AreEqual(s2.GetLength(), 1U, L"unicode length is wrong", LINE_INFO());
			Assert::AreEqual(s2.GetByteLength(), 3U, L"unicode byte length is wrong", LINE_INFO());

			s.Append(s2);

			String<2> a("A");
			s.Append(a);
		}
	};
}
