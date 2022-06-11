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
  bool findFirstValidCh;

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
    if ( !findFirstValidCh ) {
      return true;
    } // if

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

    findFirstValidCh = true;
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
    findFirstValidCh = false;
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
    mTokenString.clear();
    mScanner.Init();
    cout << "> ";
    if ( Definition() ) {

    } // if
    else if ( Statement() ) {
      cout << "Statement executed ...\n";
    } // if
    else {
      mScanner.GetToken( token );
      string errorMsg = "";
      errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // else

    bool keepRun = true;
    while ( keepRun ) {
      mTokenString.clear();
      mScanner.Init();
      cout << "> ";
      if ( Definition() ) {

      } // if
      else if ( Statement() ) {
        cout << "Statement executed ...\n";
      } // if
      else {
        keepRun = false;
      } // else
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
          if ( gIdTable.find( idToken.mValue ) == gIdTable.end() ) {
            cout << "Definition of ";
          } // if
          else {
            cout << "New definition of ";
          } // else

          cout << idToken.mValue << " entered ...\n";
          gIdTable[idToken.mValue] = 1;
          return true;
        } // if
      } // if
       
      mScanner.GetToken( token );
      errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // if
    else if ( Type_specifier() ) {
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        idToken = token;
        if ( Function_Definition_or_Declarators() ) {
          if ( gIdTable.find( idToken.mValue ) == gIdTable.end() ) {
            cout << "Definition of ";
          } // if
          else {
            cout << "New definition of ";
          } // else

          cout << idToken.mValue << " entered ...\n";
          gIdTable[idToken.mValue] = 1;
          return true;
        } // if
      } // if

      mScanner.GetToken( token );
      errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // else if

    return false;
  } // Definition()

  bool Type_specifier() {
    Token token;
    mScanner.PeekToken( token );
    if ( token.mType == INT || token.mType == FLOAT || token.mType == CHAR
         || token.mType == STRING || token.mType == BOOL ) {
      mScanner.GetToken( token );
      return true;
    } // if

    return false;
  } // Type_specifier()

  bool Function_Definition_or_Declarators() {
    if ( Function_Definition_Without_ID() || Rest_of_Declarators() ) {
      return true;
    } // if

    return false;
  } // Function_Definition_or_Declarators()

  bool Rest_of_Declarators() {
    Token token;
    bool error = false;
    string errorMsg = "";

    // [ '[' Constant ']' ]
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
          error = true;
        } // else
      } // if
      else {
        error = true;
      } // else
    } // if

    if ( error ) {
      mScanner.GetToken( token );
      errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // if

    // { ',' Identifier [ '[' Constant ']' ] }
    mScanner.PeekToken( token );
    while ( token.mValue == "," ) {
      mScanner.GetToken( token );
      mScanner.PeekToken( token );
      if ( token.mType != ID ) {
        mScanner.GetToken( token );
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      mScanner.GetToken( token );

      // [ '[' Constant ']' ]
      mScanner.PeekToken( token );
      if ( token.mValue == "[" ) {
        mScanner.GetToken( token );
        mScanner.PeekToken( token );
        if ( token.mType == CONSTANT ) {
          mScanner.GetToken( token );
          if ( token.mValue == "]" ) {
            mScanner.GetToken( token );
          } // if
          else {
            error = true;
          } // else
        } // if
        else {
          error = true;
        } // else
      } // if

      if ( error ) {
        mScanner.GetToken( token );
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
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
    // '(' [ VOID | formal_parameter_list ] ')' compound_statement
    Token token;
    string errorMsg = "";
    mScanner.PeekToken( token );
    if ( token.mValue == "(" ) {
      mScanner.GetToken( token );
      mScanner.PeekToken( token );
      if ( token.mType == VOID ) {
        mScanner.GetToken( token );
      } // if
      else if ( Formal_parameter_list() ) {

      } // if
      else {
        mScanner.GetToken( token );
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // eles

      mScanner.GetToken( token );
      if ( token.mValue != ")" ) {
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      if ( !Compound_statement() ) {
        mScanner.GetToken( token );
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      return true;
    } // if ( token.mValue == "(" )

    return false;
  } // Function_Definition_Without_ID()

  bool Formal_parameter_list() {
    Token token;
    string errorMsg = "";
    bool matchFirstToken = false, success = false;
    if ( Type_specifier() ) {
      mScanner.PeekToken( token );
      if ( token.mValue == "&" ) {
        mScanner.GetToken( token );
      } // if

      mScanner.GetToken( token );
      if ( token.mType != ID ) {
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
      if ( token.mValue == "[") {
        mScanner.GetToken( token );
        mScanner.GetToken( token );
        if ( token.mType != CONSTANT ) {
          errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
          throw errorMsg;
        } // if

        mScanner.GetToken( token );
        if ( token.mValue != "]" ) {
          errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
          throw errorMsg;
        } // if
      } // if

      mScanner.PeekToken( token );
      while ( token.mValue == "," ) {
        mScanner.GetToken( token );
        if ( !Type_specifier() ) {
          errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
          throw errorMsg;
        } // if

        mScanner.PeekToken( token );
        if ( token.mValue == "&" ) {
          mScanner.GetToken( token );
        } // if

        mScanner.GetToken( token );
        if ( token.mType != ID ) {
          errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
          throw errorMsg;
        } // if ( token.mType != ID ) 

        mScanner.PeekToken( token );
        if ( token.mValue == "[") {
          mScanner.GetToken( token );
          mScanner.GetToken( token );
          if ( token.mType != CONSTANT ) {
            errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
            throw errorMsg;
          } // if

          mScanner.GetToken( token );
          if ( token.mValue != "]" ) {
            errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
            throw errorMsg;
          } // if
        } // if

        mScanner.PeekToken( token );
      } // while

      return true;
    } // if

    return false;
  } // Formal_parameter_list()

  bool Compound_statement() {
    // '{' { declaration | statement } '}'
    Token token;
    mScanner.PeekToken( token );
    if ( token.mValue == "{" ) {
      mScanner.GetToken( token );
      while ( Declaration() || Statement() ) {

      } // while

      mScanner.PeekToken( token );
      if ( token.mValue == "}" ) {
        mScanner.GetToken( token );
        return true;
      } // if
    } // if

    return false;
  } // Compound_statement()

  bool Declaration() {
    Token token;
    string errorMsg = "";
    // type_specifier Identifier rest_of_declarators
    if ( Type_specifier() ) {
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        if ( Rest_of_Declarators() ) {
          return true;
        } // if
      } // if

      mScanner.GetToken( token );
      errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // if

    return false;
  } // Declaration()

  bool Statement() {
    Token token;
    bool success = false;
    bool firstTokenMatch = false;

    mScanner.PeekToken( token );
    if ( token.mValue == ";" ) {
      mScanner.GetToken( token );
      firstTokenMatch = true;
      success = true;
    } // if
    else if ( Expression() ) {
      firstTokenMatch = true;
      mScanner.PeekToken( token );
      if ( token.mValue == ";" ) {
        mScanner.GetToken( token );
        mTokenString.push_back( token );
        success = true;
      } // if
    } // else if
    else if ( token.mType == RETURN ) {
      // RETURN [ expression ] ';'
      mScanner.GetToken( token );
      firstTokenMatch = true;
      Expression();
      mScanner.PeekToken( token );
      if ( token.mValue == ";" ) {
        mScanner.GetToken( token );
        mTokenString.push_back( token );
        success = true;
      } // if
    } // if
    else if ( Compound_statement() ) {
      firstTokenMatch = true;
      success = true;
    } // if
    else if ( token.mType == IF ) {
      // IF '(' expression ')' statement [ ELSE statement ]
      mScanner.GetToken( token );
      firstTokenMatch = true;
      mScanner.PeekToken( token );
      if ( token.mValue == "(" ) {
        mScanner.GetToken( token );
        if ( Expression() ) {
          mScanner.PeekToken( token );
          if ( token.mValue == ")" ) {
            mScanner.GetToken( token );
            if ( Statement() ) {
              mScanner.PeekToken( token );
              if ( token.mType == ELSE ) {
                mScanner.GetToken( token );
                if ( Statement() ) {
                  success = true;
                } // if
              } // if
              else {
                success = true;
              } // else
            } // if
          } // if
        } // if
      } // if
    } // if
    else if ( token.mType == WHILE ) {
      // WHILE '(' expression ')' statement
      firstTokenMatch = true;
      mScanner.GetToken( token );
      mScanner.PeekToken( token );
      if ( token.mValue == "(" ) {
        mScanner.GetToken( token );
        if ( Expression() ) {
          mScanner.PeekToken( token );
          if ( token.mValue == ")" ) {
            mScanner.GetToken( token );
            if ( Statement() ) {
              success = true;
            } // if
          } // if
        } // if
      } // if
    } // if
    else if ( token.mType == DO ) {
      // DO statement WHILE '(' expression ')' ';'
      firstTokenMatch = true;
      mScanner.GetToken( token );
      if ( Statement() ) {
        mScanner.PeekToken( token );
        if ( token.mType == WHILE ) {
          mScanner.GetToken( token );
          mScanner.PeekToken( token );
          if ( token.mValue == "(" ) {
            mScanner.GetToken( token );
            if ( Expression() ) {
              mScanner.PeekToken( token );
              if ( token.mValue == ")" ) {
                mScanner.GetToken( token );
                mScanner.PeekToken( token );
                if ( token.mValue == ";" ) {
                  mScanner.GetToken( token );
                  success = true;
                } // if
              } // if
            } // if
          } // if
        } // if
      } // if
    } // if

    if ( mTokenString.size() == 4 && mTokenString[0].mValue == "Done"
         && mTokenString[1].mValue == "(" && mTokenString[2].mValue == ")"
         && mTokenString[3].mValue == ";" ) {
      string errorMsg = "quit";
      throw errorMsg;
    } // if
    else if ( firstTokenMatch && !success ) {
      mScanner.GetToken( token );
      string errorMsg = "";
      errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // else if

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
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if
    } // while

    return true;
  } // Expression()

  bool Basic_Expression() {
    Token token, idToken;
    string errorMsg = "";
    mScanner.PeekToken( token );
    if ( token.mType == ID ) {
      mScanner.GetToken( token );
      idToken = token;
      mTokenString.push_back( token );
      if ( Rest_of_Identifier_started_basic_exp() ) {
        if ( gIdTable.find( idToken.mValue ) == gIdTable.end() ) {
          errorMsg = errorMsg + "undefined identifier : \'" + idToken.mValue + "\'\n";
          throw errorMsg;
        } // if

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
    else if ( Sign() ) {
      while ( Sign() ) {

      } // while

      if ( !Signed_unary_exp() || !Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      return true;
    } // if
    else if ( token.mType == PP || token.mType == MM ) {
      mScanner.GetToken( token );
      mScanner.PeekToken( token );
      if ( token.mType == ID ) {
        mScanner.GetToken( token );
        if ( Rest_of_PPMM_Identifier_started_basic_exp() ) {
          return true;
        } // if
      } // if
      
      mScanner.GetToken( token );
      errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // if

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

  bool Rest_of_PPMM_Identifier_started_basic_exp() {
    // [ '[' expression ']' ] romce_and_romloe
    Token token;
    string errorMsg;
    mScanner.PeekToken( token );
    if ( token.mValue == "[") {
      mScanner.GetToken( token );
      if ( !Expression() ) {
        mScanner.GetToken( token );
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      mScanner.GetToken( token );
      if ( token.mValue != "]" ) {
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      if ( !Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
        mScanner.GetToken( token );
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      return true;
    } // if

    if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp() ) {
      return true;
    } // if

    return false;
  } // Rest_of_PPMM_Identifier_started_basic_exp()

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
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
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
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
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
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
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
      errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
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
        errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
        throw errorMsg;
      } // if

      mScanner.PeekToken( token );
    } // while

    return true;
  } // Rest_of_maybe_mult_exp()

  bool Unary_exp() {
    Token token, idToken;
    string errorMsg = "";
    bool error = false;
    mScanner.PeekToken( token );
    if ( Unsigned_unary_exp() ) {
      return true;
    } // if
    else if ( Sign() ) {
      while ( Sign() ) {

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
        idToken = token;
        if ( token.mValue == "[" ) {
          mScanner.GetToken( token );
          if ( gIdTable.find( idToken.mValue ) == gIdTable.end() ) {
            errorMsg = errorMsg + "undefined identifier : \'" + idToken.mValue + "\'\n";
            throw errorMsg;
          } // if

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
    } // if

    if ( error ) {
      mScanner.GetToken( token );
      errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // if
    
    return false;
  } // Unary_exp()

  bool Signed_unary_exp() {
    Token token, idToken;
    string errorMsg = "";
    bool error = false;
    bool success = false;
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
      idToken = token;
      mScanner.PeekToken( token );
      if ( token.mValue == "(" ) {
        mScanner.GetToken( token );
        Actual_parameter_list();
        mScanner.PeekToken( token );
        if ( token.mValue == ")" ) {
          mScanner.GetToken( token );
          success = true;
        } // if
      } // if
      else if ( token.mValue == "[" ) {
        mScanner.GetToken( token );
        if ( Expression() ) {
          mScanner.PeekToken( token );
          if ( token.mValue == "]" ) {
            mScanner.GetToken( token );
            success = true;
          } // if
        } // if
      } // if
      else {
        success = true;
      } // else

      if ( success ) {
        if ( gIdTable.find( idToken.mValue ) == gIdTable.end() ) {
          errorMsg = errorMsg + "undefined identifier : \'" + idToken.mValue + "\'\n";
          throw errorMsg;
        } // if
        return true;
      } // if

      error = true;
    } // else if

    if ( error ) {
      mScanner.GetToken( token );
      errorMsg = errorMsg + "unexpected token : '" + token.mValue + "'\n";
      throw errorMsg;
    } // if

    return false;
  } // Signed_unary_exp()

  bool Unsigned_unary_exp() {
    Token token;
    string errorMsg = "";
    mScanner.PeekToken( token );
    if ( token.mType == ID ) {
      mScanner.GetToken( token );
      if ( gIdTable.find( token.mValue ) == gIdTable.end() ) {
        errorMsg = errorMsg + "undefined identifier : \'" + token.mValue + "\'\n";
        throw errorMsg;
      } // if
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

void LoadPreDefID() {
  gIdTable["Done"] = 1;
  gIdTable["cin"] = 1;
  gIdTable["cout"] = 1;
} // LoadPreDefID()

int main() {
  int testNum = 0;
  cin >> testNum ;
  ReadLine();

  cout << "Our-C running ...\n";
  bool quit = false;
  Parser parser;
  LoadPreDefID();
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
      } // else
    } // catch
  } // while

  cout << "Our-C exited ...";
  return 0;
} // main()
