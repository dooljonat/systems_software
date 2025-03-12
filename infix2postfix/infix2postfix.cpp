#include <iostream>
#include <cmath>
#include <vector>

#include <stdio.h>

#include "antlr4-runtime.h"

#include "ArithmeticLexer.h"
#include "ArithmeticParser.h"
#include "ArithmeticBaseListener.h"

using namespace antlr4;
using namespace std;

const char *filename;
map<string, int> symtab;

class CodeGen : public ArithmeticBaseListener {
public:
    virtual void enterNum(ArithmeticParser::NumContext * ctx) override {
        cout << ctx->NUM()->getText();
    }

    virtual void exitOperation(ArithmeticParser::OperationContext * ctx) override {
        cout << ctx->op->getText();
    }
};

int main(int argc, const char* argv[])  {
    ANTLRInputStream *input;

    if (argc > 1) {
        ifstream fstream;
        filename = argv[1];
        fstream.open(filename);
        ANTLRInputStream *finput = new ANTLRInputStream(fstream);
        input = finput;
    }
    else {
        filename = "stdin";
        input = new ANTLRInputStream(cin);
    }

    ArithmeticLexer lexer(input);
    CommonTokenStream tokens(&lexer);
    ArithmeticParser parser(&tokens);

    tree::ParseTree *tree = parser.expression();
    CodeGen listener;
    tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

    cout << endl;

    return 0;
}