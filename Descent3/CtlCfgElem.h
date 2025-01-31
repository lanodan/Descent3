/*
 * $Logfile: /DescentIII/Main/CtlCfgElem.h $
 * $Revision: 15 $
 * $Date: 7/16/99 11:15a $
 * $Author: Samir $
 *
 * Header file
 *
 * $Log: /DescentIII/Main/CtlCfgElem.h $
 * 
 * 15    7/16/99 11:15a Samir
 * multiple hat support
 * 
 * 14    4/14/99 3:56a Jeff
 * fixed case mismatch in #includes
 * 
 * 13    2/26/99 2:09a Samir
 * added '?' button.
 * 
 * 12    2/21/99 6:36p Samir
 * focusing changes and key input changes to ui.,
 * 
 * 11    2/16/99 12:07p Samir
 * redid controller config with new ui.
 * 
 * 10    11/30/98 4:54p Samir
 * added rear view config item.
 * 
 * 9     10/22/98 4:11p Samir
 * put inventory keys back in.
 * 
 * 8     10/22/98 2:40p Samir
 * took out inventory keys for demo.
 * 
 * 7     10/21/98 12:32p Samir
 * fixed problem with size of gadget.
 * 
 * 6     10/18/98 1:07p Samir
 * tweaked user interface for controller config.
 * 
 * 5     10/17/98 7:31p Samir
 * added invertible axes
 * 
 * 4     10/08/98 10:52a Samir
 * added countermeasure and weapon cycling.
 * 
 * 3     10/02/98 4:15p Samir
 * added fancy artwork for config screens.
 * 
 * 2     9/30/98 4:37p Samir
 * 'incremental checkin'
 * 
 * 1     9/28/98 3:48p Samir
 * initial revision
 *
 * $NoKeywords: $
 */

#ifndef CTLCFGELEM_H
#define CTLCFGELEM_H

#include "newui.h"
#include "Controller.h"
#include "controls.h"
#include "stringtable.h"


//////////////////////////////////////////////////////////////////////////////
// Data types
#define N_CFGELEM_SLOTS			CTLBINDS_PER_FUNC		// number of controller slots per function
#define CFGELEM_SLOT_CLEAR		2


//////////////////////////////////////////////////////////////////////////////
//
class cfg_element: public UIGadget 
{
	static UIBitmapItem *m_btn_bmp_lit, *m_btn_bmp, *m_xbtn_bmp_lit, *m_xbtn_bmp;
	static short m_count;
	const char *m_title;
	sbyte m_fnid, m_flags;						// fnflags : 1 if invert btn visible 
	sbyte m_slot, m_curslot;					// what slow is in focus and number of slots.
	ubyte m_slot_alpha;
	sbyte m_blink_state;

public:
	void Create(UIWindow *wnd, int str_i, int x, int y, int fnid, int id);
	sbyte GetActiveSlot() const { return m_slot; };
	bool Configure(ct_type *elem_type, ubyte *controller, ubyte *new_elem, sbyte *slot);	// calls configuration routines (returns true if approved)

protected:
	virtual void OnDraw();
	virtual void OnKeyDown(int key);
	virtual void OnMouseBtnDown(int btn);
	virtual void OnMouseBtnUp(int btn);
	virtual void OnLostFocus();
	virtual void OnGainFocus();
	virtual void OnDestroy();
	virtual void OnFormat();
};


typedef struct tCfgDataParts
{
	ubyte bind_0, bind_1, ctrl_0, ctrl_1;
}
tCfgDataParts;

inline void parse_config_data(tCfgDataParts *parts, ct_type type0, ct_type type1, ct_config_data cfgdata)
{
	switch (type0)
	{
	case ctKey:	 parts->bind_0 = CONTROLLER_KEY1_VALUE(CONTROLLER_VALUE(cfgdata)); break;
	case ctAxis:
	case ctMouseAxis:
	case ctButton:
	case ctMouseButton:
	case ctPOV: 
	case ctPOV2:
	case ctPOV3:
	case ctPOV4: parts->bind_0 = CONTROLLER_CTL1_VALUE(CONTROLLER_VALUE(cfgdata)); break;
	default: parts->bind_0 = 0;
	}

	switch (type1)
	{
	case ctKey: parts->bind_1 = CONTROLLER_KEY2_VALUE(CONTROLLER_VALUE(cfgdata)); break;
	case ctAxis:
	case ctMouseAxis:
	case ctButton:
	case ctMouseButton:
	case ctPOV: 
	case ctPOV2:
	case ctPOV3:
	case ctPOV4: parts->bind_1 = CONTROLLER_CTL2_VALUE(CONTROLLER_VALUE(cfgdata)); break;
	default: parts->bind_1 = 0;
	}

	parts->ctrl_0 = CONTROLLER_CTL1_INFO(CONTROLLER_INFO(cfgdata));
	parts->ctrl_1 = CONTROLLER_CTL2_INFO(CONTROLLER_INFO(cfgdata));
}


inline ct_config_data unify_config_data(tCfgDataParts *parts)
{
	return MAKE_CONFIG_DATA(CONTROLLER_CTL_INFO(parts->ctrl_0, parts->ctrl_1), CONTROLLER_CTL_VALUE(parts->bind_0, parts->bind_1));
}

#endif