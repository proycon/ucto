/*
  Copyright (c) 2019
  CLST - Radboud University

  This file is part of Ucto

  Ucto is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Ucto is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  For questions and suggestions, see:
      https://github.com/LanguageMachines/ucto/issues
  or send mail to:
      lamasoftware (at ) science.ru.nl

*/
#include <string>
#include <vector>
#include "ticcutils/StringOps.h"
#include "ticcutils/PrettyPrint.h"
#include "ticcutils/LogStream.h"
#include "config.h"
#include "ucto/my_textcat.h"

using namespace std;

using TiCC::operator<<;

#define DBG *TiCC::Log(dbg)

#ifdef HAVE_TEXTCAT
TextCat::~TextCat() { textcat_Done( TC ); }

TextCat::TextCat( const std::string& cf, TiCC::LogStream *log ) {
  debug = false;
  dbg = log;
  TC = textcat_Init( cf.c_str() );
  textcat_SetProperty( TC, TCPROP_MINIMUM_DOCUMENT_SIZE, 2 );
  if ( TC == 0 ){
    throw runtime_error( "TextCat init failed: " + cf );
  }
  cfName = cf;

}

TextCat::TextCat( const TextCat& in ) {
  debug = false;
  dbg = in.dbg;
  TC = textcat_Init( in.cfName.c_str() );
  cfName = in.cfName;
}

bool TextCat::set_debug( bool b ){
  bool result = debug;
  debug = b;
  return result;
}

vector<string> TextCat::get_languages( const string& in ) const {
  if ( debug ){
    DBG << "textcat.get_languages( " << in << " )" << endl;
  }
  vector<string> vals;
  char *res = textcat_Classify( TC, in.c_str(), in.size() );
  if ( debug ){
    if ( res ){
      DBG << "textcat.get_languages, res= '" << res << "'" << endl;
    }
    else {
      DBG << "textcat.get_languages, res= NULL" << endl;
    }
  }
  if ( res && strlen(res) > 0 && strcmp( res, "SHORT" ) != 0 ){
    string val = res;
    TiCC::split_at_first_of( val, vals, "[]" );
  }
  if ( debug ){
    DBG << "textcat.get_languages found: " << vals << endl;
  }
  return vals;
}

string TextCat::get_language( const string& in ) const {
  vector<string> vals = get_languages( in );
  if ( vals.size() > 0 ){
    return vals[0];
  }
  else {
    return "";
  }
}

#else
TextCat::~TextCat() {}

TextCat::TextCat( const std::string& cf ): TC(0) {
  throw runtime_error( "TextCat::TextCat(" + cf + "): TextCat Support not available" );
}

TextCat::TextCat( const TextCat& in ): TC(0) {
  throw runtime_error( "TextCat::TextCat(): TextCat Support not available" );
}

vector<string> TextCat::get_languages( const string& in ) const {
  throw runtime_error( "TextCat::get_languages(): TextCat Support not available" );
}

string TextCat::get_language( const string& in ) const {
  throw runtime_error( "TextCat::get_language(): TextCat Support not available" );
}

#endif
