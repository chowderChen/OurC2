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

bool IsSpecialSymbol( char ch ) {
  if ( ch == ';' || ch == '=' || ch == '+'
    || ch == '-' || ch == '*' || ch == '/'
    || ch == '(' || ch == ')' || ch == ':'
    || ch == '<' || ch == '>' ) {
    return true;
  } // if

  return false;
} // IsSpecialSymbol()

enum TokenType {
  ID, NUM,
  END_OF_FILE, GE,
  LE, GT, LT, S_COLON,
  ASSIGN, EQ, NEQ,
  PLUS, MINUS, MUL,
  DIV, LPAREN, RPAREN
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
    mType = END_OF_FILE;
  } // Init()

  bool IsEmpty() {
    if ( mValue.compare( "" ) == 0 && mType == END_OF_FILE ) {
      return true;
    } // if
    else {
      return false;
    } // else
  } // IsEmpty()
}; // class Token

enum ErrorType {
  LEXICAL_ERROR, // unrecognized token with first char
  SYNTAXTICAL_ERROR, // unexpected token
  SEMANTIC_ERROR // undefined identifier
};

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
    if ( str[0] == ':' || str[0] == '>' ) {
      if ( PeekChar( tmpChar ) && tmpChar == '=' ) {
        GetChar( tmpChar );
        str += tmpChar;
      } // if
    } // if
    else if ( str[0] == '<' ) {
      if ( PeekChar( tmpChar ) ) {
        if ( tmpChar == '=' || tmpChar == '>' ) {
          GetChar( tmpChar );
          str += tmpChar;
        } // if
      } // if
    } // else if
  } // GetRestOfSpecial()

  bool GetToken( Token& token ) {
    if ( !mNextToken.IsEmpty() ) {
      token = mNextToken;
      mNextToken.Init();
      return true;
    } // if

    char tmpChar = '\0';
    string tokenValue = "";
    bool getTokenSuces = false;

    if ( GetNonWhitespaceChar( tmpChar ) ) {
      tokenValue += tmpChar;
      char tmpChar2 = '\0';

      if ( IsLetter( tmpChar ) ) {
        GetRestOfID( tokenValue );
        token.mType = ID;
        getTokenSuces = true;
      } // if
      else if ( IsDigit( tmpChar )
        || ( tmpChar == '.' && PeekChar( tmpChar2 )
          && IsDigit( tmpChar2 ) ) ) {
        GetRestOfNum( tokenValue );
        token.mType = NUM;
        getTokenSuces = true;
      } // else if tmpChar == '.'
      else if ( tmpChar == ':' && PeekChar( tmpChar2 ) && tmpChar2 != '=' ) {
        // error: unrecognized token with first char
      } // else if
      else if ( IsSpecialSymbol( tmpChar ) ) {
        GetRestOfSpecial( tokenValue );
        getTokenSuces = true;
        if ( tokenValue == ":=" ) {
          token.mType = ASSIGN;
        } // if
        else if ( tokenValue == ";" ) {
          token.mType = S_COLON;
        } // else if
        else if ( tokenValue == "+" ) {
          token.mType = PLUS;
        } // else if
        else if ( tokenValue == "-" ) {
          token.mType = MINUS;
        } // else if
        else if ( tokenValue == "*" ) {
          token.mType = MUL;
        } // else if
        else if ( tokenValue == "/" ) {
          token.mType = DIV;
        } // else if
        else if ( tokenValue == "=" ) {
          token.mType = EQ;
        } // else if
        else if ( tokenValue == "<>" ) {
          token.mType = NEQ;
        } // else if
        else if ( tokenValue == ">" ) {
          token.mType = GT;
        } // else if
        else if ( tokenValue == "<" ) {
          token.mType = LT;
        } // else if
        else if ( tokenValue == ">=" ) {
          token.mType = GE;
        } // else if
        else if ( tokenValue == "<=" ) {
          token.mType = LE;
        } // else if
        else if ( tokenValue == "(" ) {
          token.mType = LPAREN;
        } // else if
        else if ( tokenValue == ")" ) {
          token.mType = RPAREN;
        } // else if
      } // else if IsSpecial
      else {
        // error: unrecognized token with first char
      } // else

      token.mValue = tokenValue;
    } // if

    if ( !getTokenSuces ) {
      string errorMsg = "";
      errorMsg = errorMsg + "Unrecognized token with first char : \'" + tmpChar + "\'\n";
      throw errorMsg;
    } // if

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

  } // UserInput()

  bool Definition() {
    Token token;
    mScanner.PeekToken( token );

    return false;
  } // Definition()

  bool Statement() {
    return false;
  } // Statement()

}; // class Parser

int main() {
  int testNum = 0;
  cin >> testNum;

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
