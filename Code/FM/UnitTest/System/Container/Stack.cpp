#include <CppUnitTest.h>

#include "system/container/Stack.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fm;

namespace FMUnitTest
{		
	TEST_CLASS(SystemContainerStack)
	{
	public:
		class Dummy
		{
		public:
			int Number;
		};

		TEST_METHOD(Stack_Value)
		{
			const auto stackSize = 16U;

			Stack<int, stackSize> stack;

			Assert::AreEqual(stack.GetCapacity(), stackSize, L"stack capacity is wrong", LINE_INFO());
			Assert::AreEqual(stack.GetSize(), 0U, L"stack size is wrong", LINE_INFO());

			stack.Push(3);
			stack.Push(12);
			Assert::AreEqual(stack.GetSize(), 2U, L"stack size is wrong", LINE_INFO());
			Assert::AreEqual(stack.Peek(), 12, L"stack head value is wrong", LINE_INFO());
			Assert::AreEqual(stack.Pop(), 12, L"stack pop doesn't work", LINE_INFO());
			Assert::AreEqual(stack.Pop(), 3, L"stack pope doesn't work", LINE_INFO());
			Assert::AreEqual(stack.GetSize(), 0U, L"stack size is wrong", LINE_INFO());
		}

		TEST_METHOD(Stack_Object)
		{
			const auto stackSize = 16U;

			Stack<Dummy, stackSize> stack;

			Dummy dummy3{ 3 };
			Dummy dummy12{ 12 };
			stack.Push(dummy3);
			stack.Push(dummy12);
			Assert::AreEqual(stack.GetSize(), 2U, L"stack size is wrong", LINE_INFO());
			Assert::AreEqual(stack.Peek().Number, 12, L"stack head value is wrong", LINE_INFO());
			Assert::AreEqual(stack.Pop().Number, 12, L"stack pop doesn't work", LINE_INFO());
			Assert::AreEqual(stack.Pop().Number, 3, L"stack pop doesn't work", LINE_INFO());
			Assert::AreEqual(stack.GetSize(), 0U, L"stack size is wrong", LINE_INFO());
		}

		TEST_METHOD(Stack_Pointer)
		{
			const auto stackSize = 16U;

			Stack<Dummy*, stackSize> stack;

			Dummy dummy3{ 3 };
			Dummy dummy12{ 12 };
			stack.Push(&dummy3);
			stack.Push(&dummy12);
			Assert::AreEqual(stack.GetSize(), 2U, L"stack size is wrong", LINE_INFO());
			Assert::AreEqual((void*)stack.Peek(), (void*)&dummy12, L"stack head value is wrong", LINE_INFO());
			Assert::AreEqual((void*)stack.Pop(), (void*)&dummy12, L"stack pop doesn't work", LINE_INFO());
			Assert::AreEqual((void*)stack.Pop(), (void*)&dummy3, L"stack pop doesn't work", LINE_INFO());
			Assert::AreEqual(stack.GetSize(), 0U, L"stack size is wrong", LINE_INFO());
		}
	};
}