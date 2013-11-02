/*
*      Copyright (C) 2007-2013 Team XBMC
*      http://www.xbmc.org
*
*  This Program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2, or (at your option)
*  any later version.
*
*  This Program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with XBMC; see the file COPYING.  If not, see
*  <http://www.gnu.org/licenses/>.
*
*/

#pragma once
#include "Joystick.h"
#include "threads/SystemClock.h"
#include <string>

class CAction;

namespace JOYSTICK {

	/**
	* Track key presses for deferred action repeats.
	*/
	struct ActionTracker
	{
		ActionTracker() { Reset(); }
		void Reset();
		void Track(const CAction &action);

		int                  actionID; // Action ID, or 0 if not tracking any action
		std::string          name; // Action name
		XbmcThreads::EndTime timeout; // Timeout until action is repeated
	};

	class InputState {
	public:
		InputState() : id(0),  m_bWakeupChecked(false) {ResetState();}
		InputState& operator-=(const InputState& rhs);
		const InputState operator-(const InputState &other) const;

		void ResetState(unsigned int buttonCount = GAMEPAD_BUTTON_COUNT,
			unsigned int hatCount = GAMEPAD_HAT_COUNT,
			unsigned int axisCount = GAMEPAD_AXIS_COUNT);
		/**
		* Helper function to normalize a value to maxAxisAmount.
		*/
		void SetAxis(unsigned int axis, long value, long maxAxisAmount);

		// Public data members
		std::string        name;
		unsigned int       id;
		std::vector<bool>  buttons;
		std::vector<JOYSTICK::Hat>  hats;
		std::vector<float> axes;
	private:
		/**
		* After updating, look for changes in state.
		* @param oldState - previous joystick state, set to newState as a post-condition
		* @param newState - the updated joystick state
		* @param joyID - the ID of the joystick being processed
		*/
		void ProcessButtonPresses(const InputState& rhs);
		void ProcessHatPresses(const InputState& rhs);
		void ProcessAxisMotion(const InputState& rhs);

		// Returns true if this wakes up from the screensaver
		bool Wakeup();
		// Allows Wakeup() to perform another wakeup check
		void ResetWakeup() { m_bWakeupChecked = false; }
		bool          m_bWakeupChecked; // true if WakeupCheck() has been called

		ActionTracker m_actionTracker;
	};
}