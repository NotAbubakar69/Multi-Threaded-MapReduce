#include <string>
#include <vector>
using namespace std;

struct TestCase {
    string input;
    string expected_output;
};

vector<TestCase> test_cases = {
    {
        "apple banana cherry",
        "(apple, 1)\n(banana, 1)\n(cherry, 1)\n"
    },
    {
        "apple apple banana apple banana orange banana apple banana orange orange",
        "(apple, 4)\n(banana, 4)\n(orange, 3)\n"
    },
    {
        "cat dog elephant fox giraffe cat dog elephant fox giraffe cat dog elephant fox giraffe cat dog elephant fox giraffe",
        "(cat, 4)\n(dog, 4)\n(elephant, 4)\n(fox, 4)\n(giraffe, 4)\n"
    },
    {
        "hello hello hello hello hello hello hello",
        "(hello, 7)\n"
    },
    {
        "",
        ""
    },
    {
        "Pizza pizza PIZZA burger burger Pasta pasta P@sta BURGER Burger",
        "(Pizza, 1)\n(pizza, 1)\n(PIZZA, 1)\n(burger, 2)\n(Pasta, 1)\n(pasta, 1)\n(P@sta, 1)\n(BURGER, 1)\n(Burger, 1)\n"
    },
    {
        "hello, hello. world! world? test-test",
        "(hello,, 1)\n(hello., 1)\n(world!, 1)\n(world?, 1)\n(test-test, 1)\n"
    },
    {
        "apple Apple APPLE banana Banana BANANA",
        "(apple, 1)\n(Apple, 1)\n(APPLE, 1)\n(banana, 1)\n(Banana, 1)\n(BANANA, 1)\n"
    },
    {
        "apple Apple APPLE banana Banana BANANA",
        "(apple, 1)\n(Apple, 1)\n(APPLE, 2)\n(banana, 1)\n(Banana, 1)\n(BANANA, 1)\n"
    },
    {
        "word1 word2 word1 word3 word2 word1",
        "(word1, 3)\n(word2, 2)\n(word3, 1)\n"
    },
    {
        "@ # $ % ^ & *",
        "(@, 1)\n(#, 1)\n($, 1)\n(%, 1)\n(^, 1)\n(&, 1)\n(*, 1)\n"
    }
};
