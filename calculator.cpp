#include <iostream>
#include <string>
#include <list>

using std::istream, std::cin, std::cout, std::endl, std::string, std::to_string, std::list;

/* 
    using a deque to hold the operands and operators as strings
        push_back and pop_front
    using string operations to parse
        substr(pos, len)
        erase(pos, len)
        find_first_of()
        to_string()
        stold()
*/

class Expression {
    private:
        string expression;
        string operators{"*/+-"};
        list<string> terms;
        long double result;
        void parse();
    public:
        Expression(istream &is) { getline(is, expression); parse(); }
        Expression(const string &s) : expression(s) { parse(); }
        long double resolve();
        void print_terms();
};

void Expression::parse()
{
    string::size_type pos = 0, old_pos = 0;
    while ((pos = expression.find_first_of("*/+-")) != string::npos) {
        terms.push_back(expression.substr(0, pos)); // pos should be > 0 or error
        terms.push_back(expression.substr(pos, 1));
        expression = expression.substr(pos + 1);
        old_pos = pos;
    }
    terms.push_back(expression);
}

long double Expression::resolve()
{
    string left_operand, right_operand;
    auto it = terms.cbegin();
    while (it != terms.cend()) {
        if (*it == "*") {
            auto b = it;
            right_operand = *(++it);
            auto e = ++it;
            left_operand = to_string(stold(left_operand) * stold(right_operand));
            it = terms.erase(--b, e);
            terms.insert(it, left_operand);
        }
        else if (*it == "/") {
            auto b = it;
            right_operand = *(++it);
            auto e = ++it;
            left_operand = to_string(stold(left_operand) / stold(right_operand));
            it = terms.erase(--b, e);
            terms.insert(it, left_operand);
        }
        else {
            left_operand = *it;
            ++it;
        }
    }

    print_terms();
    cout << endl;

    for (auto it = terms.cbegin(); it != terms.cend(); ++it) {
        if (*it == "+") {
            right_operand = *(++it);
            left_operand = to_string(stold(left_operand) + stold(right_operand));
        }
        else if (*it == "-") {
            right_operand = *(++it);
            left_operand = to_string(stold(left_operand) + stold(right_operand));
        }
        else {
            left_operand = *it;
        }
    }

    result = stold(left_operand);

    return result;
}

void Expression::print_terms()
{
    for (const auto &term : terms) {
        cout << term;
    }
}

int main()
{
    Expression expression(cin);
    expression.print_terms();
    cout << endl;
    cout << expression.resolve() << endl;

    return 0;
}