#include <iostream>
#include <string>
#include <list>
#include <cctype>
#include <stdexcept>

using std::istream, std::cin, std::cout, std::endl, std::string, std::to_string, std::list, std::invalid_argument;

class Expression {
    private:
        string expression;
        list<string> terms;
        long double result;
        void parse();
        void resolve();
    public:
        Expression(istream &is) { getline(is, expression); parse(); resolve(); }
        Expression(const string &s) : expression(s) { parse(); resolve(); }
        long double get_result() { return result; }
        void print_terms();
};

void Expression::parse()
{
    string expression_copy = expression;
    string::size_type pos;
    while ((pos = expression_copy.find_first_of("*/+-")) != string::npos) {
        terms.push_back(expression_copy.substr(0, pos));
        terms.push_back(expression_copy.substr(pos, 1));
        expression_copy = expression_copy.substr(pos + 1);
    }
    terms.push_back(expression_copy);
}

void Expression::resolve()
{
    {
        auto it = terms.begin();
        decltype(it) left_operand_it;
        while (it != terms.end()) {
            if (*it == "*") {
                size_t index;
                char conversion_end;

                long double left_operand = stold(*left_operand_it, &index);
                conversion_end = (*left_operand_it)[index];
                if (!isdigit(conversion_end) && conversion_end != '\0')
                    throw invalid_argument("Invalid operand: " + *left_operand_it);

                auto right_operand_it = ++it;
                long double right_operand = stold(*right_operand_it, &index);
                conversion_end = (*right_operand_it)[index];
                if (!isdigit(conversion_end) && conversion_end != '\0')
                    throw invalid_argument("Invalid operand: " + *right_operand_it);

                *left_operand_it = to_string(left_operand * right_operand);
                it = terms.erase(++left_operand_it, ++right_operand_it);
            }
            else if (*it == "/") {
                size_t index;
                char conversion_end;

                long double left_operand = stold(*left_operand_it, &index);
                conversion_end = (*left_operand_it)[index];
                if (!isdigit(conversion_end) && conversion_end != '\0')
                    throw invalid_argument("Invalid operand: " + *left_operand_it);

                auto right_operand_it = ++it;
                long double right_operand = stold(*right_operand_it, &index);
                conversion_end = (*right_operand_it)[index];
                if (!isdigit(conversion_end) && conversion_end != '\0')
                    throw invalid_argument("Invalid operand: " + *right_operand_it);

                *left_operand_it = to_string(left_operand / right_operand);
                it = terms.erase(++left_operand_it, ++right_operand_it);
            }
            else {
                left_operand_it = it;
                ++it;
            }
        }
    }

    {
        auto it = terms.begin();
        decltype(it) left_operand, right_operand;
        while (it != terms.end()) {
            if (*it == "+") {
                right_operand = ++it;
                *left_operand = to_string(stold(*left_operand) + stold(*right_operand));
                it = terms.erase(++left_operand, ++right_operand);
            }
            else if (*it == "-") {
                right_operand = ++it;
                *left_operand = to_string(stold(*left_operand) / stold(*right_operand));
                it = terms.erase(++left_operand, ++right_operand);
            }
            else {
                left_operand = it;
                ++it;
            }
        }
    }

    result = stold(terms.front());
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
    cout << expression.get_result() << endl;

    return 0;
}
