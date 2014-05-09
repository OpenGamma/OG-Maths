/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <stack>
#include "expression.hh"
#include "terminal.hh"
#include "execution.hh"

namespace librdag {

enum class Direction { UP, DOWN };

ExecutionList::ExecutionList(pOGNumeric tree)
{
  // Procedural construction of an execution list. The algorithm performs
  // a depth-first traversal of the tree, pushing nodes on to the execution
  // list during the traversal. This is slightly more complicated than a
  // DFT of a binary tree, since each expression has a variable number of
  // children. In order to keep track of which child we got to when we
  // revisit an expression node, we keep a stack of argument positions
  // as well as a stack of nodes that we are visiting. 
  
  _execList = new _ExpressionList();

  // treePos contains the list of nodes we've visited but not finished with
  stack<pOGNumeric> treePos;
  // argPos records how far down the arg of the node in a given position we've got
  stack<size_t> argPos;

  // Start by going downwards
  Direction dir = Direction::DOWN;
  // Initialise work stacks with the root node, and arg position 0
  treePos.push(tree);
  argPos.push(0);

  // When there's nothing left on the stack, the execution list is complete.
  while (!treePos.empty())
  {
    // Get the next work item
    pOGNumeric current = treePos.top();

    ExprType_t type = current->getType();
    if (!(type & IS_NODE_MASK))
    {
      // We've got a terminal, This node is next to execute
      _execList->push_back(current);
      // Go back up to where we came from, by removing this node from the work stacks
      treePos.pop();
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
        size_t pos = argPos.top() + 1;
        // Save the new index position back in argPos
        argPos.pop();
        argPos.push(pos);
        // Get our args
        const ArgContainer& currentArgs = current->asOGExpr()->getArgs();
        // Has our last arg already been traversed?
        if (pos < currentArgs.size())
        {
          // No, so go down that child now
          treePos.push(currentArgs[pos]);
          argPos.push(0);
          dir = Direction::DOWN;
        }
        else
        {
          // Yes, so current node is next in execution list and we need to carry on upwards
          
          // Current node is next in the execution list
          _execList->push_back(current);
          // Go back to the previous node by removing this one from the stack
          argPos.pop();
          treePos.pop();
        }
      }
      else
      {
        // We're on our way down, so we need to push the next node on to the stack
        
        // Get the first arg and go down to it
        const ArgContainer& args = current->asOGExpr()->getArgs();
        treePos.push(args[0]);
        argPos.push(0);
      }
    }
  }
}

ExecutionList::~ExecutionList()
{
  delete _execList;
}

size_t
ExecutionList::size()
{
  return _execList->size();
}

ExecutionList::citerator
ExecutionList::begin()
{
  return _execList->begin();
}

ExecutionList::citerator
ExecutionList::end()
{
  return _execList->end();
}

pOGNumeric
ExecutionList::operator[](size_t n)
{
  return _execList->operator[](n);
}

} // namespace librdag
