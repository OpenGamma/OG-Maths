/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <sstream>
#include <iostream>
#include <string.h>
#include <regex>

#ifdef __linux
#include <dlfcn.h>
#endif // __linux

#ifndef __MINGW32__
#include <execinfo.h>
#include <cxxabi.h>
#endif // __MINGW32__

#include "exceptions.hh"
#include "debug.h"

using namespace std;

namespace dogma_exceptions {

#ifdef __APPLE__
vector<string> tokenize_backtrace(const string &backtrace_string)
{
  istringstream s(backtrace_string);
  vector<string> trace{istream_iterator<string>{s}, istream_iterator<string>{}};
  if (trace.size() != 6)
  {
    // Parse fail. Return empty vector.
    return vector<string>();
  }
  else
  {
    // Parse is OK.
    return trace;
  }
}
#endif // __APPLE__

string get_file(const string& backtrace_string)
{
#ifdef __APPLE__
  vector<string> tokens = tokenize_backtrace(backtrace_string);
  if (tokens.size() != 0)
  {
    return tokens[1];
  }
  else
  {
    return string("");
  }
#else
  size_t bracket = backtrace_string.find("(");
  if (bracket == string::npos)
  {
    return "";
  }
  return backtrace_string.substr(0, bracket);
#endif // __APPLE__
}

string get_fname(const string& backtrace_string)
{
  string fname;

#ifdef __APPLE__
  vector<string> tokens = tokenize_backtrace(backtrace_string);
  if (tokens.size() != 0)
  {
    fname = tokens[3];
  }
  else
  {
    fname = "";
  }
#else
  size_t begin = backtrace_string.find("(");
  if (begin == string::npos)
  {
    return "";
  }

  // Step over the bracket
  begin += 1;
  size_t end = backtrace_string.find(")");
  if (end == string::npos)
  {
    return "";
  }

  fname = backtrace_string.substr(begin, end-begin);

  // Get rid of the offset if there is one
  size_t offset = fname.find("+");
  if (offset != string::npos)
  {
    fname = fname.substr(0, offset);
  }
#endif // __APPLE__

  return fname;
}

const string demangle_fname(const string& fname, int* status)
{
  string ret = fname;
#ifndef __MINGW32__
  char* demangled = abi::__cxa_demangle(fname.c_str(), nullptr, 0, status);
  switch (*status)
  {
  case 0:
    ret = string(demangled); // Success
    free(demangled);
    break;
  case 1:
    DEBUG_PRINT("DEMANGLE_MALLOC_FAILED");
    break;
  case 2:
    DEBUG_PRINT("DEMANGLE_INVALID_MANGLED_NAME");
    break;
  case 3:
    DEBUG_PRINT("DEMANGLE_INVALID_ARGUMENT");
    break;
  default:
    DEBUG_PRINT("DEMANGLE_UNKNOWN_ERROR");
    break;
  }
#else
  *status = 0;
#endif // __MINGW32__
  return ret;
}

/**
 * BacktraceElement
 */

BacktraceElement::BacktraceElement()
{
  _address = nullptr;
  _function = string("<function>");
  _object_file = string("<object file>");
}

BacktraceElement::BacktraceElement(const BacktraceElement& other)
{
  _address = other.getAddress();
  _function = other.getFunction();
  _object_file = other.getObjectFile();
}

BacktraceElement::BacktraceElement(const string& backtrace_symbol, const void* address)
{
#ifdef __linux
  Dl_info dli;
  dladdr(address, &dli);
  _address = (void*) ((long long unsigned) address - (long long unsigned) dli.dli_fbase);
#else // __linux
  _address = address;
#endif // __linux
  _object_file = asciiOnly(get_file(backtrace_symbol));
  int status = 0;
  _function = asciiOnly(demangle_fname(get_fname(backtrace_symbol), &status));
}

const void*
BacktraceElement::getAddress() const
{
  return _address;
}

const string
BacktraceElement::getObjectFile() const
{
  return _object_file;
}

const string
BacktraceElement::getFunction() const
{
  return _function;
}

string
BacktraceElement::asciiOnly(string s)
{
  char* c = (char*) malloc(sizeof(char) * s.size()+1);
  for (size_t i = 0; i < s.size(); i++)
  {
    c[i] = s[i] & 0x7F;
  }
  c[s.size()] = 0;
  string s1 = string(c);
  free(c);
  return s1;
}

/**
 * dogma_error
 */

dogma_error::dogma_error(const string& what): runtime_error(what)
{
#ifndef __MINGW32__
  void *frameBuf[FRAMEBUF_SIZE];
  char **strings;
  _traceSize = backtrace(frameBuf, FRAMEBUF_SIZE);
  strings = backtrace_symbols(frameBuf, _traceSize);

  _backtrace = new BacktraceElement[_traceSize];

  for (size_t i = 0; i < _traceSize; i++)
  {
    _backtrace[i] = BacktraceElement(strings[i], frameBuf[i]);
  }

  free(strings);
#endif // __MINGW32__
}

dogma_error::dogma_error(const dogma_error& other): runtime_error::runtime_error(other)
{
#ifndef __MINGW32__
  _traceSize = other.getTraceSize();
  _backtrace = new BacktraceElement[_traceSize];
  for (size_t i = 0; i < _traceSize; i++)
  {
    _backtrace[i] = BacktraceElement(other._backtrace[i]);
  }
#endif // __MINGW32__
}

dogma_error::~dogma_error()
{
#ifndef __MINGW32__
  delete[] _backtrace;
#endif // __MINGW32__
}

const BacktraceElement*
dogma_error::getBacktrace() const
{
  return _backtrace;
}

size_t
dogma_error::getTraceSize() const
{
  return _traceSize;
}

} // end namespace dogma_exceptions
