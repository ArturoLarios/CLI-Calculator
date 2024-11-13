#include <iostream>
#include <string>
#include <list>
#include <cctype>
#include <stdexcept>

using std::istream, std::cin, std::cerr, std::cout, std::endl;
using std::string, std::to_string, std::list;
using std::invalid_argument, std::out_of_range;

class Expression {
    private:
        string expression;
        list<string> terms;
        long double result;
        void parse();
        void resolve();
        list<string>::iterator process_operator(list<string>::iterator left_operand_term, 
                                                list<string>::iterator operator_symbol_term);
        long double convert_operand(list<string>::iterator term);
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
        list<string>::iterator term = terms.begin();
        list<string>::iterator left_operand_term;
        while (term != terms.end()) {
            if (*term == "*" || *term == "/") {
                term = process_operator(left_operand_term, term);
            }
            else {
                left_operand_term = term;
                ++term;
            }
        }
    }

    {
        list<string>::iterator term = terms.begin();
        list<string>::iterator left_operand_term;
        while (term != terms.end()) {
            if (*term == "+" || *term == "-") {
                term = process_operator(left_operand_term, term);
            }
            else {
                left_operand_term = term;
                ++term;
            }
        }
    }

    result = stold(terms.front());
}

list<string>::iterator Expression::process_operator(list<string>::iterator left_operand_term, 
                                                    list<string>::iterator operator_symbol_term)
{
    list<string>::iterator operator_symbol = operator_symbol_term;

    long double left_operand = convert_operand(left_operand_term);

    list<string>::iterator right_operand_term = ++operator_symbol_term;
    long double right_operand = convert_operand(right_operand_term);

    if (*operator_symbol == "*")
        *left_operand_term = to_string(left_operand * right_operand);
    else if (*operator_symbol == "/")
        *left_operand_term = to_string(left_operand / right_operand);
    else if (*operator_symbol == "+")
        *left_operand_term = to_string(left_operand + right_operand);
    else if (*operator_symbol == "-")
        *left_operand_term = to_string(left_operand - right_operand);

    return terms.erase(++left_operand_term, ++right_operand_term);
}

long double Expression::convert_operand(list<string>::iterator term)
{
    long double operand;
    size_t index;
    char conversion_end;
    try {
        if (*term == "")
            throw invalid_argument("missing operand");
        operand = stold(*term, &index);
        conversion_end = (*term)[index];
        if (!isspace(conversion_end) && conversion_end != '\0')
            throw invalid_argument("invalid operand");
    }
    catch (const invalid_argument &e) {
        throw invalid_argument("convert_operand: " 
                                + (*term == "" ? e.what():e.what() + string(": ") + *term));
    }
    catch (const out_of_range &e) {
        throw out_of_range("convert_operand: operand exceeds range of representable values: " + *term);
    }
    return operand;
}

void Expression::print_terms()
{
    for (const auto &term : terms) {
        cout << term;
    }
}

int main()
{
    try {
        Expression expression(cin);
            expression.print_terms();
        cout << endl;
        cout << expression.get_result() << endl;
    }
    catch (const invalid_argument &e)
    {
        cerr << e.what() << endl;
    }
    catch (const out_of_range &e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}
