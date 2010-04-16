/**
 * FILE:    INode.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: INode.cpp 115 2007-10-05 10:52:32Z jpaterso $
 * PURPOSE: Implementation of the INode class.
**/

#include "INode.h"
#include "List.h"
#include <stdio.h>

namespace fire_engine
{

INode::INode(INode * parent)
	: mParent(0)
{
	mChildren = new List<INode*>();
	this->setParent(parent);
}

INode::~INode()
{
	if (mParent)
	{
		mParent->mChildren->removeElement(this);
		mParent->drop();
	}
	this->removeAllChildren();
	delete mChildren;
}

void INode::setParent(INode * parent)
{
	if (mParent)
	{
		mParent->mChildren->removeElement(this);
		mParent->drop();
	}
	mParent = parent;
	if (mParent)
	{
		mParent->mChildren->push_back(this);
		mParent->grab();
	}
}

bool INode::removeChild(INode * child)
{
	if (mChildren->contains(child))
	{
		child->mParent = 0;
		child->drop();
		mChildren->removeElement(child);
		this->drop();
		return true;
	}
	return false;
}

void INode::addChild(INode * child)
{
	if (!mChildren->contains(child))
	{
		mChildren->push_back(child);
		child->grab();
	}
}

bool INode::containsChild(INode * child) const
{
	return mChildren->contains(child);
}

void INode::removeAllChildren()
{
	for (List<INode*>::iterator it = mChildren->begin(); it != mChildren->end(); it++)
	{
		it->mParent = 0;
		it->drop();
		this->drop();
	}
	mChildren->clear();
}

INode * INode::createAndAddChild()
{
	INode * child = new INode(this);
	this->addChild(child);
	return child;
}

void INode::releaseTree()
{
	for (List<INode*>::iterator it = mChildren->begin(); it != mChildren->end(); it++)
		it->releaseTree();
	this->removeAllChildren();
}

}
