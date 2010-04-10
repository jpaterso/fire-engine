/**
 * FILE:    INode.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: INode.h 115 2007-10-05 10:52:32Z jpaterso $
**/

#ifndef INODE_H_INCLUDED
#define INODE_H_INCLUDED

#include "Types.h"
#include "CompileConfig.h"
#include "list.h"
#include "Object.h"

namespace fire_engine
{

/** An interface for some sort of node in a tree, with a parent and possibly a
 bunch of children. 
 Implements the Object interface and handles proper grab()ing and drop()ing of 
 children and parents. */
class _FIRE_ENGINE_API_ INode : public virtual Object
{
public:
	//! Destructor
	virtual ~INode();

	/** Set the parent for the INode. */
	void setParent(INode * parent);

	//! Add a child to the list of children
	void addChild(INode * child);

	//! Returns true if the INode contains the given child
	bool containsChild(INode * child) const;

	//! Remove all the children from the list
	void removeAllChildren();

	/** Create and add a new child to the current INode.
	 \return A pointer to the newly created child */
	virtual INode * createAndAddChild();

	/** Attempts to release the entire tree structure below this node. */
	void releaseTree();

	/** Remove a child from the list.
	 \param child        A pointer to the child to remove.
	 \return true if the child was correctly removed. */
	virtual bool removeChild(INode * child);

protected:
	INode *         mParent;
	list<INode*> *  mChildren;

	//! Constructor made private to ensure it stays an interface
	INode(INode * parent = 0);
};

}

#endif // INODE_H_INCLUDED
