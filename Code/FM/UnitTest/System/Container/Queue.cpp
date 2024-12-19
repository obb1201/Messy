#include <CppUnitTest.h>

#include "System/container/Queue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fm;

namespace FMUnitTest
{		
	TEST_CLASS(SystemContainerQueue)
	{
	public:
		class Dummy
		{
		public:
			int Number;
		};

		TEST_METHOD(Queue_Value)
		{
			const auto queueSize = 4U;

			Queue<int, queueSize> queue;

			Assert::AreEqual(queue.GetCapacity(), queueSize, L"queue capacity is wrong", LINE_INFO());
			Assert::AreEqual(queue.GetSize(), 0U, L"queue size is wrong", LINE_INFO());
			Assert::AreEqual(queue.IsEmpty(), true, L"queue size is wrong", LINE_INFO());

			queue.Enqueue(3);
			queue.Enqueue(4);
			queue.Enqueue(2);
			queue.Enqueue(1);

			Assert::AreEqual(queue.GetSize(), 4U, L"stack size is wrong", LINE_INFO());
			Assert::AreEqual(queue.Peek(), 3, L"queue head value is wrong", LINE_INFO());
			Assert::AreEqual(queue.Dequeue(), 3, L"stack dequeue doesn't work", LINE_INFO());
			Assert::AreEqual(queue.Dequeue(), 4, L"stack dequeue doesn't work", LINE_INFO());
			Assert::AreEqual(queue.GetSize(), 2U, L"stack size is wrong", LINE_INFO());
		}

		TEST_METHOD(Queue_Object)
		{
			const auto queueSize = 4U;

			Queue<Dummy, queueSize> queue;

			Assert::AreEqual(queue.GetCapacity(), queueSize, L"queue capacity is wrong", LINE_INFO());
			Assert::AreEqual(queue.GetSize(), 0U, L"queue size is wrong", LINE_INFO());
			Assert::AreEqual(queue.IsEmpty(), true, L"queue size is wrong", LINE_INFO());

			Dummy dummy3{ 3 };
			Dummy dummy4{ 4 };
			Dummy dummy2{ 2 };
			Dummy dummy1{ 1 };

			queue.Enqueue(dummy3);
			queue.Enqueue(dummy4);
			queue.Enqueue(dummy2);
			queue.Enqueue(dummy1);

			Assert::AreEqual(queue.GetSize(), 4U, L"stack size is wrong", LINE_INFO());
			Assert::AreEqual(queue.Peek().Number, 3, L"queue head value is wrong", LINE_INFO());
			Assert::AreEqual(queue.Dequeue().Number, 3, L"stack dequeue doesn't work", LINE_INFO());
			Assert::AreEqual(queue.Dequeue().Number, 4, L"stack dequeue doesn't work", LINE_INFO());
			Assert::AreEqual(queue.GetSize(), 2U, L"stack size is wrong", LINE_INFO());
		}

		TEST_METHOD(Queue_Pointer)
		{
			const auto queueSize = 4U;

			Queue<Dummy*, queueSize> queue;

			Assert::AreEqual(queue.GetCapacity(), queueSize, L"queue capacity is wrong", LINE_INFO());
			Assert::AreEqual(queue.GetSize(), 0U, L"queue size is wrong", LINE_INFO());
			Assert::AreEqual(queue.IsEmpty(), true, L"queue size is wrong", LINE_INFO());

			Dummy dummy3{ 3 };
			Dummy dummy4{ 4 };
			Dummy dummy2{ 2 };
			Dummy dummy1{ 1 };

			queue.Enqueue(&dummy3);
			queue.Enqueue(&dummy4);
			queue.Enqueue(&dummy2);
			queue.Enqueue(&dummy1);

			Assert::AreEqual(queue.GetSize(), 4U, L"stack size is wrong", LINE_INFO());
			Assert::AreEqual((void*)queue.Peek(), (void*)&dummy3, L"queue head value is wrong", LINE_INFO());
			Assert::AreEqual((void*)queue.Dequeue(), (void*)&dummy3, L"stack dequeue doesn't work", LINE_INFO());
			Assert::AreEqual((void*)queue.Dequeue(), (void*)&dummy4, L"stack dequeue doesn't work", LINE_INFO());
			Assert::AreEqual(queue.GetSize(), 2U, L"stack size is wrong", LINE_INFO());
		}
	};
}