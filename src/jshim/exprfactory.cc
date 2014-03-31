/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <stack>
#include "numeric.hh"
#include "exprfactory.hh"
#include "jvmmanager.hh"
#include "debug.h"

namespace convert {

using namespace librdag;

/**
 * Represents direction of traversal through a Java expression tree
 */
enum class Direction { UP, DOWN };

/**
 * Get the args of a Java expression node
 * @param env the JNI environment pointer
 * @param obj the Java expression node
 * @return the array of args
 */
jobjectArray getArgs(JNIEnv* env, jobject obj)
{
  jmethodID method = JVMManager::getOGExprClazz_getExprs();
  jobject dataobj = JVMManager::callObjectMethod(env, obj, method);
  checkEx(env);
  return static_cast<jobjectArray>(dataobj);
}

/**
 * Get the type of a Java expression node
 * @param env the JNI environment pointer
 * @param obj the Java expression node
 * @return the type ID, which corresponds to the value in the expression type enum
 */
jlong getType(JNIEnv* env, jobject obj)
{
  jobject typeobj = env->CallObjectMethod(obj, JVMManager::getOGNumericClazz_getType());
  checkEx(env);
  return env->GetLongField(typeobj, JVMManager::getOGExprEnumClazz__hashdefined());
}

/**
 * A jnode struct is for convenience - when traversing the Java tree, we get
 * the arguments and number of arguments in order to do the correct traversal.
 * The arguments are then required in stage 2, where the linearised list of
 * Java tree nodes are converted to RDAG objects. The jnode is used to cache
 * a Java expression node with its arguments, so they don't have to be
 * retrieved a second time in stage 2.
 */
struct jnode
{
  jobject obj;
  jobjectArray args;
  jsize nArgs;

  /**
   * Construct a jnode from an object for convenience.
   * @param env the JNI environment pointer (required to get the arguments)
   * @param o the object from which to construct the jnode
   */
  jnode(JNIEnv* env, jobject o): obj{o}
  {
    if (!(getType(env, obj) & IS_NODE_MASK))
    {
      args = nullptr;
      nArgs = 0;
    }
    else
    {
      args = getArgs(env, obj);
      nArgs = env->GetArrayLength((jarray) args);
    }
  }
};

/**
 * Generates an RDAG expression tree from a java object
 * @param obj a Java OGNumeric type
 * @return the equivalent RDAG expression
 */
OGNumeric* createExpression(jobject jexpr)
{
  JNIEnv* env = nullptr;
  JVMManager::getEnv((void **) &env);

  // Stage 1: linearise java tree

  // We want to be able to build the Rdag tree by starting with the last node visited
  // here. So we do a simple depth-first traversal to linearise the tree, then the
  // Rdag tree construction can work through the stack to get nodes in reverse order.

  // Exceptions: stage 1 uses all stack-allocated variables and only deals with
  // local references, so no explicit cleanup is required if an exception is thrown.

  // jexprs is the stack containing the Java objects in the order they need to be
  // translated. We store a jnode, which holds its arguments and number of arguments
  // as well.
  vector<jnode> jexprs;
  // Workjexprs is our working stack of Java expression objects.
  stack<jnode> workJexprs;
  // argPos is a temporary state for recording how far we got through getting the args of a
  // particular node.
  stack<jsize> argPos;

  // Start by going downwards
  Direction dir = Direction::DOWN;

  // Initialise stacks with the root node, we're starting at arg position 0

  jnode firstNode{ env, jexpr };
  workJexprs.push(firstNode);
  argPos.push(0);

  while (!workJexprs.empty())
  {
    // Get the next work item
    jnode current = workJexprs.top();

    if (!(getType(env, current.obj) & IS_NODE_MASK))
    {
      // We've got a terminal, This node is next to execute
      jexprs.push_back(current);
      // Go back up to where we came from, by removing this node from the work stacks
      workJexprs.pop();
      argPos.pop();
      dir = Direction::UP;
    }
    else
    {
      // We've got an expression
      if (dir == Direction::UP)
      {
        // We were on our way up, so we need to find out whether to go down again
        // through the next arg, or further back up.

        // Get the index of the next argument
        jint pos = argPos.top() + 1;
        // Save the new index position back in argPos
        argPos.pop();
        argPos.push(pos);
        // Has our last arg already been traversed?
        if (pos < current.nArgs)
        {
          // No, so go down that child now
          jobject nextObj = (jobject) env->GetObjectArrayElement(current.args, pos);
          checkEx(env);
          jnode nextNode{ env, nextObj };
          workJexprs.push(nextNode);
          argPos.push(0);
          dir = Direction::DOWN;
        }
        else
        {
          // Yes, so current node is next in execution list and we need to carry on upwards

          // Current node is next in the execution list
          jexprs.push_back(current);
          // Go back to the previous node by removing this one from the stack
          argPos.pop();
          workJexprs.pop();
        }
      }
      else
      {
        // We're on our way down, so we need to push the next node on to the stack

        // Get the first arg and go down to it
        jobject nextObj = (jobject) env->GetObjectArrayElement(current.args, 0);
        checkEx(env);
        jnode nextNode{ env, nextObj };
        workJexprs.push(nextNode);
        argPos.push(0);
      }
    }
  }

  // Stage 2: construct Rdag tree

  // We iterate over the list of jexprs and construct the corresponding C++ node. When a
  // node is constructed, we push it on a stack. When we have construct an expression,
  // we need to pop the N nodes from the stack where N is the number of args it has.
  //
  // At the end of the iteration, there should be a single node left on the stack.
  // This node is the root of the expression tree.

  // Holds partially-constructed expressions that form part of our arguments.
  stack<OGNumeric*> exprStack;

  for(auto node: jexprs)
  {
    OGNumeric* numeric;
    const OGNumeric* arg0;
    const OGNumeric* arg1;

    switch (node.nArgs)
    {
    case 0:
      // This is a terminal, and requires no args
      numeric = translateNode(env, node.obj);
      break;
    case 1:
      // This is a UnaryExpr
      numeric = translateNode(env, node.obj, exprStack.top());
      exprStack.pop();
      break;
    case 2:
      // This is a BinaryExpr
      arg1 = exprStack.top();
      exprStack.pop();
      arg0 = exprStack.top();
      exprStack.pop();
      numeric = translateNode(env, node.obj, arg0, arg1);
      break;
    default:
      throw convert_error("Unexpected number of args for expr in stage 2 of conversion");
    }

    exprStack.push(numeric);
  }

  if (exprStack.size() != 1)
  {
    throw convert_error("Translated expression has multiple roots - "
                        "something must have gone wrong in createExpression");
  }

  return exprStack.top();
}


} // namespace convert
