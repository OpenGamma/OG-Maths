/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _EXCEPTIONS_HH
#define _EXCEPTIONS_HH

#include <exception>
#include <stdexcept>

/**
 * Maximum number of frames to retrieve for a stack trace. A somewhat arbitrary
 * choice, but probably more than 50 stack frames implies you have worse problems
 * than needing to know what was going on in the 51st frame.
 */
#define FRAMEBUF_SIZE 50

using namespace std;

namespace dogma_exceptions {

/**
 * Details of one stack frame in a backtrace.
 */
class BacktraceElement
{
  public:
    /**
     * Default constructor required for new[]
     */
    BacktraceElement();
    /**
     * Copy constructor
     *
     * @param other the BacktraceElement to copy
     */
    BacktraceElement(const BacktraceElement& other);
    /**
     * Constructor that is usually used for constructing a new BacktraceElement.
     *
     * @param backtrace_symbol a single description of a frame from backtrace_symbols()
     * @param address the address of the instruction pointer for the frame
     */
    BacktraceElement(const string& backtrace_symbol, const void* address);
    /**
     * Get the address of the instruction pointer for the frame.
     */
    const void* getAddress() const;
    /**
     * Get the name and path of the object file that this frame is inside.
     */
    const string getObjectFile() const;
    /**
     * Get the name of the function that this frame is inside.
     */
    const string getFunction() const;
  private:
    const void* _address;
    string _object_file;
    string _function;
};

/**
 * Any exception thrown during the execution of the DOGMA libraries
 */
class dogma_error: public runtime_error
{
  public:
    /**
     * The usual constructor for building a dogma_error.
     *
     * @param what Description of the exception, in a form suitable for printing to the user as the
     *             stack trace in Java.
     */
    dogma_error(const string& what);
    /**
     * Copy constructor
     *
     * @param other the dogma_error to copy
     */
    dogma_error(const dogma_error& other);
    /**
     * Destructor. Deletes all the BacktraceElements that this exception holds.
     */
    virtual ~dogma_error();
    /**
     * Get an array of all frames in the backtrace. The array is owned (and deleted) by the dogma_error.
     */
    const BacktraceElement* getBacktrace() const;
    /**
     * Get the number of frames in the backtrace.
     */
    size_t getTraceSize() const;
  private:
    BacktraceElement* _backtrace;
    size_t _traceSize;
};

} // end namespace dogma_exceptions

namespace librdag {

/*
 * Exceptions thrown during DOGMA execution
 */
class rdag_error: public ::dogma_exceptions::dogma_error
{
  using ::dogma_exceptions::dogma_error::dogma_error;
};

} // namespace librdag

namespace convert {

/**
 * Exceptions thrown during conversion from an OG-Maths AST (Java) to a DOGMA AST (C++)
 */
class convert_error: public ::dogma_exceptions::dogma_error
{
  using ::dogma_exceptions::dogma_error::dogma_error;
};

} // namespace convert

#endif
