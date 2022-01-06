/*
 *    Copyright (c) 2021 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include <ctime>
#include <list>

#include <lib/support/IntrusiveList.h>
#include <lib/support/UnitTestRegistration.h>

#include <nlunit-test.h>

namespace {

using namespace chip;

class ListNode : public IntrusiveListNodeBase
{
};

void TestIntrusiveListRandom(nlTestSuite * inSuite, void * inContext)
{
    IntrusiveList<ListNode> l1;
    ListNode node[100];
    std::list<ListNode *> l2;

    auto op = [&](auto fun) {
        if (l2.empty())
            return;

        auto l1p = l1.begin();
        auto l2p = l2.begin();
        for (size_t pos = static_cast<size_t>(std::rand()) % l2.size(); pos > 0; --pos)
        {
            ++l1p;
            ++l2p;
        }

        fun(l1p, l2p);
    };

    for (int i = 0; i < 100; ++i)
    {
        switch (std::rand() % 5)
        {
        case 0: // PushFront
            l1.PushFront(&node[i]);
            l2.push_front(&node[i]);
            break;
        case 1: // PushBack
            l1.PushBack(&node[i]);
            l2.push_back(&node[i]);
            break;
        case 2: // InsertBefore
            op([&](auto & l1p, auto & l2p) {
                l1.InsertBefore(l1p, &node[i]);
                l2.insert(l2p, &node[i]);
            });
            break;
        case 3: // InsertAfter
            op([&](auto & l1p, auto & l2p) {
                l1.InsertAfter(l1p, &node[i]);
                l2.insert(++l2p, &node[i]);
            });
            break;
        case 4: // Remove
            op([&](auto & l1p, auto & l2p) {
                l1.Remove(&*l1p);
                l2.erase(l2p);
            });
            break;
        default:
            break;
        }

        NL_TEST_ASSERT(inSuite,
                       std::equal(l1.begin(), l1.end(), l2.begin(), l2.end(),
                                  [](const ListNode & p1, const ListNode * p2) { return &p1 == p2; }));
    }

    while (!l1.Empty())
    {
        l1.Remove(&*l1.begin());
    }
}

int Setup(void * inContext)
{
    return SUCCESS;
}

int Teardown(void * inContext)
{
    return SUCCESS;
}

} // namespace

#define NL_TEST_DEF_FN(fn) NL_TEST_DEF("Test " #fn, fn)
/**
 *   Test Suite. It lists all the test functions.
 */
static const nlTest sTests[] = { NL_TEST_DEF_FN(TestIntrusiveListRandom), NL_TEST_SENTINEL() };

int TestIntrusiveList()
{
    nlTestSuite theSuite = { "CHIP IntrusiveList tests", &sTests[0], Setup, Teardown };

    unsigned seed = static_cast<unsigned>(std::time(nullptr));
    printf("Running " __FILE__ " using seed %d", seed);
    std::srand(seed);

    // Run test suit againt one context.
    nlTestRunner(&theSuite, nullptr);
    return nlTestRunnerStats(&theSuite);
}

CHIP_REGISTER_TEST_SUITE(TestIntrusiveList);
