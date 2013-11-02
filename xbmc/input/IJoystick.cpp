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