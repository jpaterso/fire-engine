/**
 *  $Id: Item.h 112 2007-09-25 23:37:26Z jpaterso $
 *  Author: Joseph Paterson
 *
 *  An Item can be displayed, if it's visible field is set to true.
**/

#ifndef	__ITEM_H
#define	__ITEM_H

#include "CompileConfig.h"

namespace fire_engine
{

class _FIRE_ENGINE_API_ Item
{
	public:
		Item(void) : m_visible(true)
		{
		}

		/**
		 *	Check whether the Item is visible
		 *	@return	true if the Item is visible, false otherwise
		**/
		bool visible(void) { return (m_visible); }

		/**
		 *	Set the visible flag of an Item
		 *	@param	val	Make this true for the Item to be visible
		**/
		void setVisible(bool val) { m_visible = val; }
	protected:
		//!	Flag to set for the Item to be visible (ie. whether to display it)
		bool m_visible;
};

} // namespace fe

#endif
