#ifndef INCLUDED_SGE_INPUT_H
#define INCLUDED_SGE_INPUT_H

//====================================================================================================
// Filename:	SGE_Input.h
// Description:	Interface to SGE Input.
//====================================================================================================

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// SGE Functions
//====================================================================================================

// Keyboard Functions
bool Input_IsKeyDown(int iKey);
bool Input_IsKeyPressed(int iKey);

// Mouse Functions
bool Input_IsMouseDown(int iMouse);
bool Input_IsMousePressed(int iMouse);
int Input_GetMouseScreenX();
int Input_GetMouseScreenY();
int Input_GetMouseMoveX();
int Input_GetMouseMoveY();
int Input_GetMouseMoveZ();

// Gamepad Functions
bool Input_IsGamePadButtonDown(int iButton);
bool Input_IsGamePadButtonPressed(int iButton);
bool Input_IsDPadUp();
bool Input_IsDPadDown();
bool Input_IsDPadLeft();
bool Input_IsDPadRight();
float Input_GetLeftAnalogX();
float Input_GetLeftAnalogY();
float Input_GetRightAnalogX();
float Input_GetRightAnalogY();

} // namespace SGE

#endif // #ifndef INCLUDED_SGE_INPUT_H