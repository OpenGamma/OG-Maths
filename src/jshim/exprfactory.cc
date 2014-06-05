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
#include "exceptions.hh"

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
OGNumeric::Ptr createExpression(jobject jexpr)
{
  JNIEnv* env = nullptr;
  JVMManager::getEnv((void **) &env);

  // This function implements a procedural depth-first traversal of the expression tree,
  // building the RDAG expression tree as it travels. Since it visits nodes in reverse
  // Polish order, we can use a stack to place tree components whilst we're constructing
  // them, and any time we need operands, they're always on the top of the stack.

  // Holds partially-constructed expressions whilst we are constructing the tree. At the
  // end of the traversal, it has a single entry, which is the root of the tree.
  stack<OGNumeric::Ptr> exprStack;
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
      // We've got a terminal, This node is next to execute, so translate and put on
      // the expression stack so it is ready to be picked up by its operator
      OGNumeric::Ptr n = translateNode(env, current.obj);
      exprStack.push(n);
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
         
          // Translate the current node - its arguments are on the top of the expression stack
          OGNumeric::Ptr n, arg0, arg1;
          switch (current.nArgs)
          {
          case 1:
            arg0 = exprStack.top();
            exprStack.pop();
            n = translateNode(env, current.obj, arg0);
            break;
          case 2:
            arg1 = exprStack.top();
            exprStack.pop();
            arg0 = exprStack.top();
            exprStack.pop();
            n = translateNode(env, current.obj, arg0, arg1);
            break;
          }
          exprStack.push(n);
          
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
  
  if (exprStack.size() != 1)
  {
    throw convert_error("Translated expression has multiple roots - "
                        "something must have gone wrong in createExpression");
  }

  return exprStack.top();
}


} // namespace convert
