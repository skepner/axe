#include <iostream>
#include <string>

#include "axe.h"

// ----------------------------------------------------------------------

int print_tel_number(const char* n);

template <typename I> int print_tel_number(I begin, I end)
{
    using namespace axe;
    int result = 0;

    auto tel1 = '(' & r_numstr(3) & ')' & r_numstr(3) & '-' & r_numstr(4);
    auto tel2 = r_numstr(3) & '-' & r_numstr(3) & '-' & r_numstr(4);
    auto tel3 = "1-" & r_numstr(3) & '-' & r_numstr(3) & '-' & r_numstr(4);
    auto tel4 = r_numstr(3) & '.' & r_numstr(3) & '.' & r_numstr(4);
    auto tel = tel1 | tel2 | tel3 | tel4;

    auto report = [](I i1, I i2) {
        std::cout << "parsed number is: " << std::string(i1, i2) << std::endl;
    };
    auto error = [&result](I i1, I i2) {
        std::cerr << "Error: telephone number was not matched: " << std::string(i1, i2) << std::endl;
        result = 1;
    };

    auto tel_rule = tel >> e_ref(report) | r_fail(error);

    tel_rule(begin, end);
    return result;
}

// ----------------------------------------------------------------------

int print_tel_number(const char* n)
{
    std::string nn(n);
    return print_tel_number(nn.begin(), nn.end());
}

// ----------------------------------------------------------------------

int main()
{
    int exit_code = 0;
    exit_code += print_tel_number("(123)456-7890");
    exit_code += print_tel_number("123-456-7890");
    exit_code += print_tel_number("1-123-456-7890");
    exit_code += print_tel_number("123.456.7890");
    return exit_code;
}

// ----------------------------------------------------------------------
