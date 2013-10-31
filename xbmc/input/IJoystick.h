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

#include <vector>
#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

/* Defines a helper struct that is only visible to this class */
typedef boost::function<void (IJoystick*)> InputCallback;

class InputState {
public:
	InputState(IJoystick* parent):m_joystick(parent) {}
	void SetCallbacks(InputCallback buttoncb = 0, InputCallback hatcb = 0, InputCallback axescb = 0);
	InputState& operator-=(const InputState& rhs);
	const InputState operator-(const InputState &other) const;

	// Public data members
	std::vector<bool>  buttons;
	std::vector<JOYSTICK::Hat>  hats;
	std::vector<float> axes;
private:
	IJoystick* m_joystick;

	InputCallback _buttoncb;
	InputCallback _hatcb;
	InputCallback _axescb;
};

/**
 * Interface IJoystick
 *
 * Joysticks are abstracted as devices that repeatedly refresh and report their
 * ineternal state. Update() is called by CJoystickManager once per FrameMove()
 * to poll for input and should sync the SJoystick struct returned by GetState()
 * to the joystick's current state.
 */
class IJoystick
{
public:
  /**
   * Implementers should provide the following factories to create IJoystick objects
   * (See CJoystickManager::Initialize()):
   *
   * static void Initialize(JoystickArray &joysticks);
   * static void DeInitialize(JoystickArray &joysticks);
   */
  IJoystick(): m_id(0) { ResetState(); }
  virtual ~IJoystick() { }

  virtual void Update() = 0;

  void ResetState(unsigned int buttonCount = GAMEPAD_BUTTON_COUNT,
                  unsigned int hatCount = GAMEPAD_HAT_COUNT,
                  unsigned int axisCount = GAMEPAD_AXIS_COUNT);

  /* Public interface to the inputstate to set the callbacks */
  void SetCallbacks(InputCallback buttoncb = 0, InputCallback hatcb = 0, InputCallback axescb = 0) {
	  m_state.SetCallbacks(buttoncb, hatcb, axescb);
  }

    /**
   * Helper function to normalize a value to maxAxisAmount.
   */
  void SetAxis(unsigned int axis, long value, long maxAxisAmount);

private:
  std::string        m_name;
  unsigned int       m_id;
  InputState m_state;
  InputState m_oldstate;
};

typedef std::vector<boost::shared_ptr<IJoystick> > JoystickArray;
