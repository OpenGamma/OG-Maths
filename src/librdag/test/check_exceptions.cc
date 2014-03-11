/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

/**
 * We check both copy and assign in these tests since they may be called implicitly
 * when exceptions are propagated. The assignment operator is not defined for these
 * classes, but should use the copy constructor - if it does, then these tests
 * should pass. If it does not, some tests will fail.
 */

#include "gtest/gtest.h"
#include "exceptions.hh"

using namespace dogma_exceptions;

#ifdef __APPLE__
const string backtrace_symbol = "2   librdag_dbg.0.dylib                 0x0000000102edcc48 _ZNK7librdag10OGTerminal21toReal16ArrayOfArraysEv + 150";
const void* address = (void*) 0x0000000102edcc48;
#else
const string backtrace_symbol = "/tmp/og-maths-720755781067700325/libjshim_sse41.so.0(_ZN7convert16createExpressionEP8_jobject+0x37) [0x7f35c0b765b7]";
const void* address = (void*) 0x7f35c0b765b7;
#endif // __APPLE__

class BacktraceElementTest: public ::testing::Test
{
  protected:
    virtual void SetUp()
    {
      _element = new BacktraceElement(backtrace_symbol, address);
    }

    virtual void TearDown()
    {
      delete _element;
    }

    BacktraceElement* _element;
};

TEST_F(BacktraceElementTest, Constructor)
{
  EXPECT_EQ(_element->getAddress(), address);
#ifdef __APPLE__
  EXPECT_EQ("librdag_dbg.0.dylib", _element->getObjectFile());
  EXPECT_EQ("librdag::OGTerminal::toReal16ArrayOfArrays() const", _element->getFunction());
#else
  EXPECT_EQ("/tmp/og-maths-720755781067700325/libjshim_sse41.so.0", _element->getObjectFile());
  EXPECT_EQ("convert::createExpression(_jobject*)", _element->getFunction());
#endif // __APPLE__
}

TEST_F(BacktraceElementTest, Copy)
{
  BacktraceElement e2(*_element);
  EXPECT_EQ(e2.getAddress(), _element->getAddress());
  EXPECT_EQ(e2.getObjectFile(), _element->getObjectFile());
  EXPECT_EQ(e2.getFunction(), _element->getFunction());
}

TEST_F(BacktraceElementTest, Assign)
{
  BacktraceElement e2 = *_element;
  EXPECT_EQ(e2.getAddress(), _element->getAddress());
  EXPECT_EQ(e2.getObjectFile(), _element->getObjectFile());
  EXPECT_EQ(e2.getFunction(), _element->getFunction());
}

class DogmaErrorTest: public ::testing::Test
{
  protected:
    virtual void SetUp()
    {
      _error = new dogma_error("An error");
    }

    virtual void TearDown()
    {
      delete _error;
    }

    dogma_error* _error;
};

TEST_F(DogmaErrorTest, Constructor)
{
  // The number of stack frames varies often depending on platform/versions,
  // so we just check the what() method here.
  EXPECT_STREQ(_error->what(), "An error");
}

TEST_F(DogmaErrorTest, Frames)
{
  // We just look at the function names of the first two frames. Other things (addresses, filename) we
  // can't rely on the value of, and further frames are inside GTest.
  // On apple only the first frame is non-gtest, so ignore it in that case.
  const BacktraceElement* trace = _error->getBacktrace();
  EXPECT_STREQ("dogma_exceptions::dogma_error::dogma_error(std::string const&)", trace[0].getFunction().c_str());
#ifndef __APPLE__
  EXPECT_STREQ("DogmaErrorTest::SetUp()",                                        trace[1].getFunction().c_str());
#endif // __APPLE__
}

TEST_F(DogmaErrorTest, Copy)
{
  dogma_error e2(*_error);

  // Check what is the same
  EXPECT_STREQ(_error->what(), e2.what());

  // Check for distinct but equal backtraces
  ASSERT_EQ(_error->getTraceSize(), e2.getTraceSize());
  const BacktraceElement* trace1 = _error->getBacktrace();
  const BacktraceElement* trace2 = e2.getBacktrace();

  for (size_t i = 0; i < e2.getTraceSize(); i++)
  {
    EXPECT_EQ(trace1[i].getAddress(),    trace2[i].getAddress());
    EXPECT_EQ(trace1[i].getObjectFile(), trace2[i].getObjectFile());
    EXPECT_EQ(trace1[i].getFunction(),   trace2[i].getFunction());
  }
}

TEST_F(DogmaErrorTest, Assign)
{
  dogma_error e2 = *_error;

  // Check what is the same
  EXPECT_STREQ(_error->what(), e2.what());

  // Check for distinct but equal backtraces
  ASSERT_EQ(_error->getTraceSize(), e2.getTraceSize());
  const BacktraceElement* trace1 = _error->getBacktrace();
  const BacktraceElement* trace2 = e2.getBacktrace();

  for (size_t i = 0; i < e2.getTraceSize(); i++)
  {
    EXPECT_EQ(trace1[i].getAddress(),    trace2[i].getAddress());
    EXPECT_EQ(trace1[i].getObjectFile(), trace2[i].getObjectFile());
    EXPECT_EQ(trace1[i].getFunction(),   trace2[i].getFunction());
  }
}
