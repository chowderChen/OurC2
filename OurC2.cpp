# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <cmath>
# include <cstdlib>
# include <iomanip>
# include <exception>

using namespace std;

map<string, int> gIdTable;

bool IsInt( float num ) {
  return floor( num ) == num;
} // IsInt()

bool IsInt( string str ) {
  return str.find_first_not_of( "0123456789" ) == string::npos;
} // IsInt()

/*
bool IsChar( string str ) {
  if ( str.length() == 1 ) {
    if ( IsLetter( str[0] ) ) {
      return true;
    } // if
  } // if

  return false;
} // IsChar()
*/
bool IsWhitespace( char ch ) {
  if ( ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' ) {
    return true;
  } // if
  else {
    return false;
  } // else
} // IsWhitespace()

bool IsLetter( char ch ) {
  if ( ( ch >= 'a' && ch <= 'z' ) || ( ch >= 'A' && ch <= 'Z' ) ) {
    return true;
  } // if

  return false;
} // IsLetter()

bool IsDigit( char ch ) {
  if ( ch >= '0' && ch <= '9' ) {
    return true;
  } // if

  return false;
} // IsDigit()

void ReadLine() {
  char ch = '\0';
  while ( cin.get( ch ) && ch != '\n' ) {
    
  } // while
} // ReadLine()

enum TokenType {
  ID, CONSTANT, UNDEFINED_TYPE, 
  GE, LE, // >= <=
  EQ, NEQ, // == !=
  INT, FLOAT, CHAR,
  STRING, BOOL, VOID,
  IF, ELSE, WHILE,
  DO, RETURN,
  AND, OR, // && ||
  PE, ME, // += -=
  TE, DE, // *= /=
  RE, // %=
  PP, MM, // ++ --
  RS, LS // >> <<
};

class Token {
public:
  string mValue;
  TokenType mType;

  Token() {
    Init();
  } // Token()

  void Init() {
    mValue = "";
    mType = UNDEFINED_TYPE;
  } // Init()

  bool IsEmpty() {
    if ( mValue.compare( "" ) == 0 ) {
      return true;
    } // if
    else {
      return false;
    } // else
  } // IsEmpty()
}; // class Token

class TokenScanner {
  bool mIsNewLine;

  bool GetChar( char& ch ) {
    if ( mNextChar != '\0' ) {
      ch = mNextChar;
      mNextChar = '\0';
    } // if nextChar != '\0'
    else if ( cin.get( ch ) ) {
      ;
    } // if cin >> ch
    else {
      return false;
    } // else

    // get char success
    if ( mIsNewLine ) {
      mLine++;
      mColumn = 0;
      mIsNewLine = false;
    } // if

    if ( ch == '\n' ) {
      mIsNewLine = true;
    } // if

    mColumn++;
    return true;
  } // GetChar()

  bool PeekChar( char& ch ) {
    if ( mNextChar == '\0' && ! ( cin.get( mNextChar ) ) ) {
      return false;
    } // if
    else {
      ch = mNextChar;
      return true;
    } // else
  } // PeekChar()

  bool GetNonWhitespaceChar( char& ch ) {
    bool getCharSuccess = GetChar( ch );
    while ( getCharSuccess && IsWhitespace( ch ) ) {
      getCharSuccess = GetChar( ch );
    } // while

    if ( getCharSuccess && ch == '/' ) {
      // reminder: divide symbol "/" will into this section
      char tmpChar = '\0';
      if ( PeekChar( tmpChar ) && tmpChar == '/' ) {
        ReadLine();
        return GetNonWhitespaceChar( ch );
      } // if
    } // if

    return getCharSuccess;
  } // GetNonWhitespaceChar()

  void GetRestOfID( string& str ) {
    // start with letter
    char tmpChar = '\0';
    while ( PeekChar( tmpChar ) &&
            ( IsLetter( tmpChar ) || IsDigit( tmpChar ) || tmpChar == '_' ) ) {
      GetChar( tmpChar );
      str += tmpChar;
    } // while
  } // GetRestOfID()

  bool GetRestOfNum( string& str ) {
    // start with digit or '.' or '-'
    char tmpChar = '\0';
    bool dotOnce = false;
    bool gotDigit = false;
    if ( str[0] == '.' ) {
      dotOnce = true;
    } // if

    while ( PeekChar( tmpChar )
            && ( IsDigit( tmpChar ) || ( !dotOnce && tmpChar == '.' ) ) ) {
      GetChar( tmpChar );
      str += tmpChar;
      if ( tmpChar == '.' ) {
        dotOnce = true;
      } // if

      if ( IsDigit( tmpChar ) ) {
        gotDigit = true;
      } // if
    } // while

    return gotDigit;
  } // GetRestOfNum()

  void GetRestOfSpecial( string& str ) {
    // special symbol
    char tmpChar = '\0';
    if ( str[0] == '!' || str[0] == '*' 
         || str[0] == '/' || str[0] == '%' ) {
      if ( PeekChar( tmpChar ) && tmpChar == '=' ) {
        GetChar( tmpChar );
        str += tmpChar;
      } // if
    } // if
    else if ( str[0] == '&' || str[0] == '|' ) {
      if ( PeekChar( tmpChar ) && tmpChar == str[0] ) {
        GetChar( tmpChar );
        str += tmpChar;
      } // if
    } // else if
    else if ( str[0] == '>' || str[0] == '<' || str[0] == '='
              || str[0] == '+' || str[0] == '-' ) {
      if ( PeekChar( tmpChar ) && ( tmpChar == str[0] || tmpChar == '=' ) ) {
        GetChar( tmpChar );
        str += tmpChar;
      } // if
    } // else if
  } // GetRestOfSpecial()

  void DetermineTokenType( Token& token ) {
    if ( token.mValue == "true" || token.mValue == "false" ) {
      token.mType = CONSTANT;
    } // if
    else if ( token.mValue == "int" ) {
      token.mType = INT;
    } // else if
    else if ( token.mValue == "float" ) {
      token.mType = FLOAT;
    } // else if
    else if ( token.mValue == "char" ) {
      token.mType = CHAR;
    } // else if
    else if ( token.mValue == "string" ) {
      token.mType = STRING;
    } // else if
    else if ( token.mValue == "bool" ) {
      token.mType = BOOL;
    } // else if
    else if ( token.mValue == "void" ) {
      token.mType = VOID;
    } // else if
    else if ( token.mValue == "if" ) {
      token.mType = IF;
    } // else if
    else if ( token.mValue == "else" ) {
      token.mType = ELSE;
    } // else if
    else if ( token.mValue == "while" ) {
      token.mType = WHILE;
    } // else if
    else if ( token.mValue == "do" ) {
      token.mType = DO;
    } // else if
    else if ( token.mValue == "return" ) {
      token.mType = RETURN;
    } // else if
    else if ( token.mValue == "==" ) {
      token.mType = EQ;
    } // else if
    else if ( token.mValue == "!=" ) {
      token.mType = NEQ;
    } // else if
    else if ( token.mValue == ">=" ) {
      token.mType = GE;
    } // else if
    else if ( token.mValue == "<=" ) {
      token.mType = LE;
    } // else if
    else if ( token.mValue == "&&" ) {
      token.mType = AND;
    } // else if
    else if ( token.mValue == "||" ) {
      token.mType = OR;
    } // else if
    else if ( token.mValue == "+=" ) {
      token.mType = PE;
    } // else if
    else if ( token.mValue == "-=" ) {
      token.mType = ME;
    } // else if
    else if ( token.mValue == "*=" ) {
      token.mType = TE;
    } // else if
    else if ( token.mValue == "/=" ) {
      token.mType = DE;
    } // else if
    else if ( token.mValue == "%=" ) {
      token.mType = RE;
    } // else if
    else if ( token.mValue == "++" ) {
      token.mType = PP;
    } // else if
    else if ( token.mValue == "--" ) {
      token.mType = MM;
    } // else if
    else if ( token.mValue == ">>" ) {
      token.mType = RS;
    } // else if
    else if ( token.mValue == "<<" ) {
      token.mType = LS;
    } // else if
  } // DetermineTokenType()

public:
  int mLine;
  int mColumn;
  char mNextChar;
  Token mNextToken;

  TokenScanner() {
    Init();
  } // TokenScanner()

  void Init() {
    mLine = 0;
    mColumn = 0;
    mIsNewLine = true;
    mNextChar = '\0';
    mNextToken.Init();
  } // Init()

  bool ReadLine() {
    char tmpChar = '\0';
    bool getCharSuccess = false;
    getCharSuccess = GetChar( tmpChar );
    while ( getCharSuccess && tmpChar != '\n' ) {
      getCharSuccess = GetChar( tmpChar );
    } // while

    return getCharSuccess;
  } // ReadLine()

  bool GetToken( Token& token ) {
    if ( !mNextToken.IsEmpty() ) {
      token = mNextToken;
      mNextToken.Init();
      return true;
    } // if

    char ch = '\0';
    string tokenValue = "";
    bool getTokenSuces = false;

    if ( GetNonWhitespaceChar( ch ) ) {
      tokenValue += ch;
      char tmpChar2 = '\0';

      if ( IsLetter( ch ) ) {
        GetRestOfID( tokenValue );
        token.mType = ID;
        getTokenSuces = true;
      } // if
      else if ( IsDigit( ch ) 
                || ( ch == '.' && PeekChar( tmpChar2 ) && IsDigit( tmpChar2 ) ) ) {
        GetRestOfNum( tokenValue );
        token.mType = CONSTANT;
        getTokenSuces = true;
      } // else if
      else if ( ch == '\'' ) {
        GetChar( ch );
        tokenValue += ch;
        GetChar( ch );
        tokenValue += ch;
        token.mType = CONSTANT;
        getTokenSuces = true;
      } // else if
      else if ( ch == '\"' ) {
        while ( GetChar( ch ) && ch != '\"' ) {
          tokenValue += ch;
        } // while

        tokenValue += ch;
        token.mType = CONSTANT;
        getTokenSuces = true;
      } // else if
      else if ( ch == '(' || ch == ')' 
                || ch == '[' || ch == ']'
                || ch == '{' || ch == '}'
                || ch == '+' || ch == '-'
                || ch == '*' || ch == '/'
                || ch == '%' || ch == '^'
                || ch == '>' || ch == '<'
                || ch == '&' || ch == '|'
                || ch == '=' || ch == '!'
                || ch == ';' || ch == ':'
                || ch == '?' || ch == ',' ) {
        GetRestOfSpecial( tokenValue );
        token.mType = UNDEFINED_TYPE;
        getTokenSuces = true;
      } // else if

      token.mValue = tokenValue;
    } // if
    
    if ( !getTokenSuces ) {
      string errorMsg = "";
      errorMsg = errorMsg + "Unrecognized token with first char : \'" + ch + "\'\n";
      throw errorMsg;
    } // if
    else {
      DetermineTokenType( token );
    } // else

    return getTokenSuces;
  } // GetToken()

  bool PeekToken( Token& token ) {
    bool peakSuces = false;
    if ( !mNextToken.IsEmpty() ) {
      token = mNextToken;
      peakSuces = true;
    } // if
    else {
      peakSuces = GetToken( token );
      if ( peakSuces ) {
        mNextToken = token;
      } // if
    } // else

    return peakSuces;
  } // PeekToken()

}; // TokenScanner

class Parser {
public:
  TokenScanner mScanner;
  vector<Token> mTokenString;
  
  void User_Input() {
    Token token;
    cout << "> ";
    if ( Definition() || Statement() ) {
      
    } // if
    else {
      mScanner.GetToken( token );
      string errorMsg = "";
      errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // else

    mTokenString.clear();
    cout << "> ";
    while ( Definition() || Statement() ) {
      mTokenString.clear();
      cout << "> ";
    } // while
  } // User_Input()

  bool Definition() {
    Token token, idToken;
    bool success = false;
    string errorMsg = "";
    

    mScanner.PeekToken( token );
    if ( token.mType == VOID ) {
      mScanner.GetToken( token );
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        idToken = token;
        if ( Function_Definition_Without_ID() ) {
          cout << "Definition of " << idToken.mValue << " entered ...\n";
          return true;
        } // if
      } // if
       
      mScanner.GetToken( token );
      errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // if
    else if ( Type_Specifier() ) {
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        idToken = token;
        if ( Function_Definition_or_Declarators() ) {
          cout << "Definition of " << idToken.mValue << " entered ...\n";
          return true;
        } // if
      } // if

      mScanner.GetToken( token );
      errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // else if

    return false;
  } // Definition()

  bool Type_Specifier() {
    Token token;
    mScanner.PeekToken( token );
    if ( token.mType == INT || token.mType == FLOAT || token.mType == CHAR
         || token.mType == STRING || token.mType == BOOL ) {
      mScanner.GetToken( token );
      return true;
    } // if

    return false;
  } // Type_Specifier()

  bool Function_Definition_or_Declarators() {
    if ( Function_Definition_Without_ID() || Rest_of_Declarators() ) {
      return true;
    } // if

    return false;
  } // Function_Definition_or_Declarators()

  bool Rest_of_Declarators() {
    Token token;
    mScanner.PeekToken( token );
    if ( token.mValue == "[" ) {
      mScanner.GetToken( token );
      mScanner.PeekToken( token );
      if ( token.mType == CONSTANT ) {
        mScanner.GetToken( token );
        mScanner.PeekToken( token );
        if ( token.mValue == "]" ) {
          mScanner.GetToken( token );
        } // if
        else {
          mScanner.GetToken( token );
          string errorMsg = "";
          errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
          throw errorMsg;
        } // else
      } // if
      else {
        mScanner.GetToken( token );
        string errorMsg = "";
        errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // else
    } // if

    mScanner.PeekToken( token );
    while ( token.mValue == "," ) {
      mScanner.GetToken( token );
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
      } // if
    } // while

    mScanner.PeekToken( token );
    if ( token.mValue == ";" ) {
      mScanner.GetToken( token );
      return true;
    } // if

    return false;
  } // Rest_of_Declarators()

  bool Function_Definition_Without_ID() {
    return false;
  } // Function_Definition_Without_ID()

  bool Statement() {
    Token token;
    bool success = false;
    mScanner.PeekToken( token );
    if ( token.mValue == ";" ) {
      mScanner.GetToken( token );
      success = true;
    } // if
    else if ( Expression() ) {
      mScanner.PeekToken( token );
      if ( token.mValue == ";" ) {
        mScanner.GetToken( token );
        mTokenString.push_back( token );
        success = true;
      } // if
    } // else if

    if ( mTokenString.size() == 4 && mTokenString[0].mValue == "Done"
         && mTokenString[1].mValue == "(" && mTokenString[2].mValue == ")"
         && mTokenString[3].mValue == ";" ) {
      string errorMsg = "quit";
      throw errorMsg;
    } // if
    else if ( success ) {
      cout << "Statement executed ...\n";
    } // if

    return success;
  } // Statement()

  bool Expression() {
    if ( !Basic_Expression() ) {
      return false;  
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "," ) {
      mScanner.GetToken( token );
      if ( !Basic_Expression() ) {
        mScanner.GetToken( token );
        string errorMsg = "";
        errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if
    } // while

    return true;
  } // Expression()

  bool Basic_Expression() {
    Token token;
    mScanner.PeekToken( token );
    if ( token.mType == ID ) {
      mScanner.GetToken( token );
      mTokenString.push_back( token );
      if ( Rest_of_Identifier_started_basic_exp() ) {
        return true;
      } // if
    } // if
    else if ( token.mType == CONSTANT || token.mValue == "(" ) {
      mScanner.GetToken( token );
      mTokenString.push_back( token );
      if ( token.mValue == "(" ) {
        if ( Expression() ) {
          mScanner.PeekToken( token );
          if ( token.mValue == ")" ) {
            mScanner.GetToken( token );
            mTokenString.push_back( token );
          } // if
        } // if
      } // if

      if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
        return true;
      } // if
    } // else if

    return false;
  } // Basic_Expression()

  bool Rest_of_Identifier_started_basic_exp() {
    Token token;

    mScanner.PeekToken( token );
    if ( token.mValue == "(" ) {
      mScanner.GetToken( token );
      mTokenString.push_back( token );
      Actual_parameter_list();
      mScanner.PeekToken( token );
      if ( token.mValue == ")" ) {
        mScanner.GetToken( token );
        mTokenString.push_back( token );
        if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
          return true;
        } // if
      } // if
    } // if
    else {
      mScanner.PeekToken( token );
      if ( token.mValue == "[" ) {
        mScanner.GetToken( token );
        if ( Expression() ) {
          mScanner.PeekToken( token );
          if ( token.mValue == "]" ) {
            mScanner.GetToken( token );
          } // if
        } // if 
      } // if

      if ( Assignment_Operator() ) {
        if ( Basic_Expression() ) {
          return true;
        } // if
      } // if
      else {
        mScanner.PeekToken( token );
        if ( token.mType == PP || token.mType == MM ) {
          mScanner.GetToken( token );
        } // if

        if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
          return true;
        } // if
      } // else
    } // else

    return false;
  } // Rest_of_Identifier_started_basic_exp()

  bool Sign() {
    Token token;
    mScanner.PeekToken( token );
    if ( token.mValue == "+" || token.mValue == "-" || token.mValue == "!" ) {
      mScanner.GetToken( token );
      return true;
    } // if

    return false;
  } // Sign()

  bool Actual_parameter_list() {
    if ( !Basic_Expression() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "," ) {
      mScanner.GetToken( token );
      mTokenString.push_back( token );
      if ( !Basic_Expression() ) {
        // throw error
        mScanner.GetToken( token );
        string errorMsg = "";
        errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Actual_parameter_list()

  bool Assignment_Operator() {
    Token token;
    mScanner.PeekToken( token );
    if ( token.mValue == "=" || token.mType == TE || token.mType == RE
         || token.mType == PE || token.mType == ME ) {
      mScanner.GetToken( token );
      return true;
    } // if

    return false;
  } // Assignment_Operator()

  bool Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() {
    if ( Rest_of_maybe_logical_OR_exp() ) {
      return true;
    } // if

    return false;
  } // Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp()

  bool Rest_of_maybe_logical_OR_exp() {
    if ( Rest_of_maybe_logical_AND_exp() ) {
      return true;
    } // if

    return false;
  } // Rest_of_maybe_logical_OR_exp()

  bool Rest_of_maybe_logical_AND_exp() {
    if ( Rest_of_maybe_bit_OR_exp() ) {
      return true;
    } // if

    return false;
  } // Rest_of_maybe_logical_AND_exp()

  bool Rest_of_maybe_bit_OR_exp() {
    if ( Rest_of_maybe_bit_ex_OR_exp() ) {
      return true;
    } // if

    return false;
  } // Rest_of_maybe_bit_OR_exp()

  bool Rest_of_maybe_bit_ex_OR_exp() {
    if ( Rest_of_maybe_bit_AND_exp() ) {
      return true;
    } // if

    return false;
  } // Rest_of_maybe_bit_ex_OR_exp()

  bool Rest_of_maybe_bit_AND_exp() {
    if ( Rest_of_maybe_equality_exp() ) {
      return true;
    } // if

    return false;
  } // Rest_of_maybe_bit_AND_exp()

  bool Rest_of_maybe_equality_exp() {
    if ( Rest_of_maybe_relational_exp() ) {
      return true;
    } // if

    return false;
  } // Rest_of_maybe_equality_exp()

  bool Rest_of_maybe_relational_exp() {
    if ( Rest_of_maybe_shift_exp() ) {
      return true;
    } // if

    return false;
  } // Rest_of_maybe_relational_exp()

  bool Rest_of_maybe_shift_exp() {
    if ( !Rest_of_maybe_additive_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mType == LS || token.mType == RS ) {
      mScanner.GetToken( token );
      if ( !Maybe_additive_exp() ) {
        mScanner.GetToken( token );
        string errorMsg = "";
        errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_shift_exp()

  bool Maybe_additive_exp() {
    if ( !Maybe_mult_exp() ) {
      return false;
    } // if

    return true;
  } // Maybe_additive_exp()

  bool Rest_of_maybe_additive_exp() {
    if ( !Rest_of_maybe_mult_exp() ) {
      return false;
    } // if

    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "+" || token.mValue == "-" ) {
      mScanner.GetToken( token );
      if ( !Maybe_mult_exp() ) {
        mScanner.GetToken( token );
        string errorMsg = "";
        errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_additive_exp()

  bool Maybe_mult_exp() {
    if ( Unary_exp() ) {
      if ( Rest_of_maybe_mult_exp() ) {
        return true;
      } // if

      Token token;
      mScanner.GetToken( token );
      string errorMsg = "";
      errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // if

    return false;
  } // Maybe_mult_exp()

  bool Rest_of_maybe_mult_exp() {
    Token token;
    mScanner.PeekToken( token );
    while ( token.mValue == "*" || token.mValue == "/"
           || token.mValue == "%" ) {
      mScanner.GetToken( token );
      if ( !Unary_exp() ) {
        mScanner.GetToken( token );
        string errorMsg = "";
        errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_mult_exp()

  bool Unary_exp() {
    Token token;
    bool error = false;
    mScanner.PeekToken( token );
    if ( Unsigned_unary_exp() ) {
      return true;
    } // if
    else if ( Sign() ) {
      while ( Sign() ){

      } // while

      if ( Signed_unary_exp() ) {
        return true;
      } // if
      else {
        error = true;
      } // else
    } // else if
    else if ( token.mType == PP || token.mType == MM ) {
      mScanner.GetToken( token );
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        if ( token.mValue == "[" ) {
          mScanner.GetToken( token );
          if ( Expression() ) {
            mScanner.PeekToken( token );
            if ( token.mValue == "]" ) {
              mScanner.GetToken( token );
              return true;
            } // if
          } // if

          error = true;
        } // if
        else {
          return true;
        } // else
      } // if

      error = true;
    } // else

    if ( error ) {
      mScanner.GetToken( token );
      string errorMsg = "";
      errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // if
    
    return false;
  } // Unary_exp()

  bool Signed_unary_exp() {
    Token token;
    bool error = false;
    mScanner.PeekToken( token );
    if ( token.mType == CONSTANT ) {
      mScanner.GetToken( token );
      return true;
    } // if
    else if ( token.mValue == "(" ) {
      mScanner.GetToken( token );
      if ( Expression() ) {
        mScanner.PeekToken( token );
        if ( token.mValue == ")" ) {
          mScanner.GetToken( token );
          return true;
        } // if
      } // if

      error = true;
    } // else if
    else if ( token.mType == ID ) {
      mScanner.GetToken( token );
      mScanner.PeekToken( token );
      if ( token.mValue == "(" ) {
        mScanner.GetToken( token );
        Actual_parameter_list();
        mScanner.PeekToken( token );
        if ( token.mValue == ")" ) {
          mScanner.GetToken( token );
          return true;
        } // if
      } // if
      else if ( token.mValue == "[") {
        mScanner.GetToken( token );
        if ( Expression() ) {
          mScanner.PeekToken( token );
          if ( token.mValue == "]" ) {
            mScanner.GetToken( token );
            return true;
          } // if
        } // if
      } // if
      else {
        return true;
      } // else

      error = true;
    } // else if

    if ( error ) {
      mScanner.GetToken( token );
      string errorMsg = "";
      errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // if

    return false;
  } // Signed_unary_exp()

  bool Unsigned_unary_exp() {
    Token token;
    mScanner.PeekToken( token );
    if ( token.mType == ID ) {
      mScanner.GetToken( token );
      return true;
    } // if
    else if ( token.mType == CONSTANT ) {
      mScanner.GetToken( token );
      return true;
    } // else if
    else if ( token.mValue == "(" ) {
      mScanner.GetToken( token );
      if ( Expression() ) {
        mScanner.PeekToken( token );
        if ( token.mValue == ")" ) {
          mScanner.GetToken( token );
          return true;
        } // if
      } // if 
    } // else if

    return false;
  } // Unsigned_unary_exp()
  
}; // class Parser

int main() {
  int testNum = 0;
  cin >> testNum ;
  ReadLine();

  cout << "Our-C running ...\n";
  bool quit = false;
  Parser parser;
  while ( !quit ) {
    try {
      parser.User_Input();
    } 
    catch ( string& errorMsg ) {
      if ( errorMsg == "quit" ) {
        quit = true;
      } // if
      else {
        cout << "Line " << parser.mScanner.mLine << " : " << errorMsg;
        parser.mScanner.ReadLine();
        parser.mScanner.Init();
        parser.mTokenString.clear();
      } // else
    } // catch
  } // while

  cout << "Our-C exited ...";
  return 0;
} // main()
