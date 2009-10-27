/**
 * FILE:    ISpaceNode.h
 * AUTHOR:  Joseph Paterson ( joseph dot paterson at gmail dot com )
 * RCS ID:  $Id: ISpaceNode.h 112 2007-09-25 23:37:26Z jpaterso $
 * PURPOSE: A node in 3-Dimensional space.
**/

#ifndef ISPACENODE_H_INCLUDED
#define ISPACENODE_H_INCLUDED

#include "Types.h"
#include "INode.h"
#include "matrix4.h"
#include "vector3.h"
#include "IRenderable.h"

namespace fire_engine
{

class ISpaceNodeAnimator;

/** A class representing a Node in 3-dimensional space, and in a hierarchy.
 With this class, Nodes can be represented hierarchically, with parents and
 children (a single parent is allowed). Each ISpaceNode has a relative transform,
 which is represented relative to the transformation of the parent. This ensures
 that if a parent moves, all its children will move with it.
 All objects that appear in the scene should derive from this class, and override
 the preRender() and render() methods. Derived classes should still call the original
 preRender() and render() methods, as they take care of calling the same methods
 on all children in the space tree. */
class _FIRE_ENGINE_API_ ISpaceNode : public INode, public virtual IRenderable
{
public:
	/** Construct an ISpaceNode from a parent */
	ISpaceNode(INode * parent = 0);

	/** Destructor. */
	virtual ~ISpaceNode();

	/** Attach an ISpaceNodeAnimator to this node, that can perform some animation
	 when preRender() is called. */
	void attachAnimator(ISpaceNodeAnimator * animator);

	/** Detach the ISpaceNodeAnimator, if it exists. */
	void detachAnimator(void);

	/** Returns whether the ISpaceNode has an ISpaceNodeAnimator attached to it or
	 not. */
	bool hasAnimatorAttached(void) const;

	/** Returns a pointer to the ISpaceNodeAnimator animating this node. */
	ISpaceNodeAnimator * getAnimator(void);

	/** Inherited from IRenderable. These methods should be overwritten,
	 *BUT* they should still be called, as they call their respective methods on
	 the children in the hierarchy. */
	virtual void preRender(f64 time);
	virtual s32 render(IRenderer * rd);

	/** Set the debug information flag. If it is set to true, debugging information
	 will be shown on the node. */
	void setShowDebugInformation(bool sdi);

	/** Returns whether debugging information should be shown or not. */
	bool showDebugInformation() const;

	/** Set the visible flag. If it is true, the ISpaceNode will be rendered. */
	void setVisible(bool v);

	/** Returns whether the ISpaceNode is visible or not. */
	bool isVisible() const;

	/** Returns the full transform that affects the ISpaceNode. */
	const matrix4f& getWorldTransform() const;

	/** Sets the relative position of the ISpaceNode, with it's parent as reference. */
	void setRelativePosition(const vector3f& translation);

	/** Returns the relative position of the ISpaceNode. */
	const vector3f& getRelativePosition() const;

	/** Translates the ISpaceNode by a given vector. */
	void translate(const vector3f& translation);

	/** Sets the relative scale of the ISpaceNode, with it's parent as reference. */
	void setRelativeScale(const vector3f& factor);

	/** Returns the relative scale of the ISpaceNode. */
	const vector3f& getRelativeScale() const;

	/** Scales the ISpaceNode. */
	void scale(const vector3f& factor);

	/** Sets the relative orientation of the ISpaceNode. */
	void setRelativeOrientation(const matrix4f& orientation);

	/** Returns the relative orientation of the ISpaceNode. */
	const matrix4f& getRelativeOrientation() const;

	/** Rotates the ISpaceNode, multiplying the current rotation matrix by
	 a new one. */
	void rotate(const matrix4f& rotation);

protected:
	/** The full transform that the ISpaceNode must undertake. */
	matrix4f mWorldTransform;

	/** The full transform with respect to the ISpaceNodes parent. */
	matrix4f mRelativeTransform;

	/** The scale of the ISpaceNode, with respect to its parent. */
	vector3f mRelativeScale;

	/** The relative position of the ISpaceNode, with regards to its parent. */
	vector3f mRelativePosition;

	/** The orientation of the ISpaceNode, in relation to the coordinate axes of
	 the parent, or (x, y, z) if no parent. */
	matrix4f mRelativeOrientation;

	/** The animator responsible for this ISpaceNode. It can animate the ISpaceNode
	 by moving it around in space. */
	ISpaceNodeAnimator * m_animator;

	/** A flag to set for debugging information to be shown. The information shown
	 will depend entirely on the type of ISpaceNode. */
	bool mShowDebugInformation;

	/** A flag to set if the ISpaceNode is to be rendered. */
	bool mVisible;

	/** Sets the correct local transform (mLocalTransform), according to the
	 local scale, the relative position, and the orientation of the ISpaceNode. */
	virtual void updateTransforms();
};

}

#endif // ISPACENODE_H_INCLUDED
