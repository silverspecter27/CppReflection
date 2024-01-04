#include <iostream>

#include "Class.hpp"

// Test class
class Test1 {
public:
    Test1(int i, std::string str) : test_int(i), test_str(str) {
    }

private:
    int& test_function() {
        return test_static_int;
    }

    int& test_function(int i) {
        std::cout << "test: " << i << '\n';
        return test_int;
    }

    int test_int;
    std::string test_str;

    static int test_static_int;
};

int Test1::test_static_int = 50;

// REFLECT macro to declare the reflection, begin and end fields / methods is  necessary even if none declared
REFLECT(Test1,
    DECLARE_FIELDS(
        FIELD(int, test_int)
        FIELD(std::string, test_str)

        STATIC_FIELD(int, test_static_int)
    )

    DECLARE_METHODS(
        METHOD(int& (), test_function)
        METHOD(int& (int), test_function)
    )
)

// Test class
class Test2 {
public:
    Test2(int i, std::string str) : test_int(i), test_str(str) {
    }

private:
    static int& test_function() {
        return test_static_int;
    }

    static int& test_function(int i) {
        std::cout << "test: " << i << '\n';
        return test_static_int;
    }

    int test_int;
    std::string test_str;

    static int test_static_int;
};

int Test2::test_static_int = 100;

// REFLECT macro to declare the reflection, begin and end fields / methods is  necessary even if none declared
REFLECT(Test2,
    DECLARE_FIELDS(
        FIELD(int, test_int)
        FIELD(std::string, test_str)

        STATIC_FIELD(int, test_static_int)
    )

    DECLARE_METHODS(
        STATIC_METHOD(int& (), test_function)
        STATIC_METHOD(int& (int), test_function)
    )
)

int main() {
    // namespace of all reflection tags
    using namespace refl::tag;

    Test1 test1{ 25,  "Hello World!" };
    Test2 test2{ 50, "Bye Bye World!" };

    // loop throught all fields
    cstd::for_each(refl::get_class<Test1>().get_fields(), [&](const auto& field) {
        std::cout << field.get(test1) << '\n';
        });

    std::cout << '\n';

    // loop throught all fields
    cstd::for_each(refl::get_class<Test2>().get_fields(), [&](const auto& field) {
        std::cout << field.get(test2) << '\n';
        });

    std::cout << '\n';

    // loop throught all methods
    cstd::for_each(refl::get_class<Test1>().get_methods(), [&](const auto& method) {
        std::cout << method.name() << '\n';
        });

    std::cout << '\n';

    // loop throught all methods
    cstd::for_each(refl::get_class<Test2>().get_methods(), [&](const auto& method) {
        std::cout << method.name() << '\n';
        });

    // if you are using c++ 20 or above this #if is not necessary
#if HAS_CONSTEXPR_STRING
    std::cout << '\n';

    // get field using a string c++ 20 or above
    std::cout << refl::get_class<Test1>().get_field<"test_int">().get(test1) << '\n';

    std::cout << '\n';

    // get field using a string c++ 20 or above
    std::cout << refl::get_class<Test2>().get_field<"test_static_int">().get(nullptr) << '\n';

    std::cout << '\n';

    // get method using a string c++ 20 or above
    std::cout << refl::get_class<Test1>().get_method<"test_function">()(test1) << '\n';

    std::cout << '\n';

    // get method using a string c++ 20 or above
    std::cout << refl::get_class<Test2>().get_method<"test_function">()(nullptr) << '\n';
#endif // HAS_CONSTEXPR_STRING

    std::cout << '\n';

    // changing field value using a tag
    refl::get_class<Test1>().get_field<test_int>().get(test1) = 50;

    std::cout << '\n';

    // get field using a tag
    refl::get_class<Test2>().get_field<test_static_int>().get(nullptr) = 75;

    std::cout << '\n';

    // get method using a tag and using parameters
    std::cout << refl::get_class<Test1>().get_method<test_function, int>()(test1, 25) << '\n';

    std::cout << '\n';

    // get method using a tag and using parameters
    std::cout << refl::get_class<Test2>().get_method<test_function, int>()(nullptr, 75) << '\n';
}