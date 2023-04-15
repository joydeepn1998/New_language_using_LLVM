#include<string>


//-------Lexer-----------------------
/*
    Tokens in the range [0-255] are for unknown characters.
    These are known characters
*/
enum token{
    tok_eof =-1,

    //commands
    tok_def = -2,
    tok_extern = -3,

    //primary
    tok_identifier = -4,
    tok_number = -5,
};

static std::string IdentifierStr;
static double NumVal;

//Return the next token from Standard Input
static int gettok(){
    static int LastChar = ' ';

    //Skip whitespaces
    while(isspace(LastChar))
        LastChar = getchar();

    if(isalpha(LastChar)){ //identifiers
        IdentifierStr = LastChar;
        while(isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;

        if(IdentifierStr == "def")
            return tok_def;
            
        if(IdentifierStr == "extern")
            return tok_extern;

        return tok_identifier;
    }

    if(isdigit(LastChar) || LastChar == '.'){
        std::string NumStr;

        do{
            NumStr += LastChar;
            LastChar = getchar();
        }while(isdigit(LastChar) || LastChar == '.');

        //parse the string as a double
        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if(LastChar == '#'){
        //comment until end of line
        do LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if(LastChar != EOF)
            return gettok();
    }

    if(LastChar == EOF)
        return tok_eof;

    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

//---------Abstract Syntax Tree----------

//Base class for all expression nodes
class ExprAST{
    public:
        //virtual for runtime polymorphism - https://www.geeksforgeeks.org/virtual-function-cpp/
        //default - https://stackoverflow.com/questions/6502828/what-does-default-mean-after-a-class-function-declaration
        virtual ~ExprAST() = default;
};

class NumberExprAST : public ExprAST{
    double Val;
    public :
        NumberExprAST(double Val) : Val(Val){}
};

//class for referencing variables
class VariableExprAST : public ExprAST{
    std::string Name;

    public:
        VariableExprAST(const std::string &Name) : Name(Name){}
};

//class for a binary Operator
class BinaryExprAST : public ExprAST{
    char Op;

    //unique_ptr - https://www.geeksforgeeks.org/auto_ptr-unique_ptr-shared_ptr-weak_ptr-in-cpp/#
    std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};




