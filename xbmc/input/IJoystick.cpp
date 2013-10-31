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

#include "IJoystick.h"
#include "settings/AdvancedSettings.h"
#include "utils/log.h"

#ifndef ABS
#define ABS(X) ((X) >= 0 ? (X) : (-(X)))
#endif

void InputState::SetCallbacks(InputCallback buttoncb, InputCallback hatcb, InputCallback axescb) {
	_buttoncb = buttoncb;
	_hatcb = hatcb;
	_axescb = axescb;
}

/* Modify the current state and create the actions. Pass the action back if there is an event */
InputState& InputState::operator-=(const InputState& rhs) {
	//Iterate through the buttons
	for (unsigned int i = 0; i < buttons.size(); i++) {
		//Execute the button callback
		_buttoncb(m_joystick);
    }

	//Iterate through the hats
	for (unsigned int i = 0; i < rhs.hats.size(); i++) {
		JOYSTICK::Hat &oldHat = hats[i];
		const JOYSTICK::Hat &newHat = rhs.hats[i];
		if (oldHat == newHat)
			continue;
		//CLog::Log(LOGDEBUG, "Joystick %d hat %d new direction: %s", joyID, i + 1, newHat.GetDirection());

		// Up, right, down, left
		for (unsigned int j = 0; j < 4; j++) {
			if (oldHat[j] == newHat[j])
				continue;
			oldHat[j] = newHat[j];
			//Execute the hat callback
			_hatcb(m_joystick);
		}
	}

	// Iterate throught the axes
	for (unsigned int i = 0; i < rhs.axes.size(); i++)
	{
		// Absolute magnitude
		float absAxis = ABS(rhs.axes[i]);

		// Only send one "centered" message
		if (absAxis < 0.01f)
		{
			if (ABS(axes[i]) < 0.01f)
			{
				// The values might not have been exactly equal, so make them
			   axes[i] = rhs.axes[i];
				continue;
			}
			CLog::Log(LOGDEBUG, "Joystick %d axis %d centered", joyID, i + 1);
		}

		//Execute the axes callback
		_axescb(m_joystick);
	}
	*/
	return *this;
}

// Subtract this instance's value with the other, and return the new instance 
// with the result
const InputState InputState::operator-(const InputState &other) const {
	return InputState(*this) -= other;
}

void IJoystick::ResetState(unsigned int buttonCount /* = GAMEPAD_BUTTON_COUNT */,
                          unsigned int hatCount /* = GAMEPAD_HAT_COUNT */,
                          unsigned int axisCount /* = GAMEPAD_AXIS_COUNT */)
{
  m_state.buttons.clear();
  m_state.hats.clear();
  m_state.axes.clear();

  m_state.buttons.resize(buttonCount);
  m_state.hats.resize(hatCount);
  m_state.axes.resize(axisCount);

  m_oldstate = m_state;
}

void IJoystick::SetAxis(unsigned int axis, long value, long maxAxisAmount)
{
  if (axis >= m_state.axes.size())
    return;
  if (value > maxAxisAmount)
    value = maxAxisAmount;
  else if (value < -maxAxisAmount)
    value = -maxAxisAmount;

  long deadzoneRange = (long)(g_advancedSettings.m_controllerDeadzone * maxAxisAmount);

  if (value > deadzoneRange)
    m_state.axes[axis] = (float)(value - deadzoneRange) / (float)(maxAxisAmount - deadzoneRange);
  else if (value < -deadzoneRange)
    m_state.axes[axis] = (float)(value + deadzoneRange) / (float)(maxAxisAmount - deadzoneRange);
  else
    m_state.axes[axis] = 0.0f;
}