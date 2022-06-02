# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <cmath>
# include <cstdlib>
# include <iomanip>
# include <exception>

using namespace std;

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
    if ( mNextChar == '\0' && !( cin.get( mNextChar ) ) ) {
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

  bool ReadLine() {
    char tmpChar = '\0';
    bool getCharSuccess = false;
    getCharSuccess = GetChar( tmpChar );
    while ( getCharSuccess && tmpChar != '\n' ) {
      getCharSuccess = GetChar( tmpChar );
    } // while

    return getCharSuccess;
  } // ReadLine()

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
    // >= <= <> :=
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
        || ( ch == '.' && PeekChar( tmpChar2 )
          && IsDigit( tmpChar2 ) ) ) {
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
      else {
        // terminate = true;
      } // else
    } // else

    return peakSuces;
  } // PeekToken()

}; // TokenScanner

class Parser {
public:
  TokenScanner mScanner;
  
  void User_Input() {
    Token token;
    if ( Definition() ) {
      
    } // if
    else if ( Statement() ) {

    } // else if
    else {
      mScanner.GetToken( token );
      string errorMsg = "";
      errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // else

    while ( Definition() || Statement() ) {

    } // while

  } // UserInput()

  bool Definition() {
    Token token;
    mScanner.PeekToken( token );
    if ( token.mType == VOID ) {
      mScanner.GetToken( token );
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        if ( Function_Definition_Without_ID() ) {
          return true;
        } // if
      } // if
       
      mScanner.GetToken( token );
      string errorMsg = "";
      errorMsg = errorMsg + "Unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // if
    else if ( Type_Specifier() ) {
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        if ( Function_Definition_or_Declarators() ) {
          return true;
        } // if
      } // if

      mScanner.GetToken( token );
      string errorMsg = "";
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
  } // Type_Specifier

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
    } // if

    return false;
  } // Rest_of_Declarators()

  bool Function_Definition_Without_ID() {
    return false;
  } // Function_Definition_Without_ID

  bool Statement() {
    return false;
  } // Statement()

}; // class Parser

int main() {
  int testNum = 0;
  char ch = '\0';
  cin >> testNum ;
  cin.get( ch );

  cout << "Our-C running ...\n";
  bool quit = false;
  Parser parser;
  while ( !quit ) {
    cout << "> ";
    try {
      parser.User_Input();
    }
    catch ( string& errorMsg ) {
      cout << "Line " << parser.mScanner.mLine << " : " << errorMsg;
      parser.mScanner.ReadLine();
      parser.mScanner.Init();
    } 
  } // while

  cout << "Our-C exited ...";
  return 0;
} // main()
