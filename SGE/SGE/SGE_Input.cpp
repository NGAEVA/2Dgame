//====================================================================================================
// Filename:	SGE_Input.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "SGE_Input.h"

#include "Input/InputSystem.h"

//====================================================================================================
// Namespace
//====================================================================================================

namespace SGE
{

//====================================================================================================
// SGE Functions
//====================================================================================================

bool Input_IsKeyDown(int iKey)
{
	return DXInput::Get()->IsKeyDown(iKey);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsKeyPressed(int iKey)
{
	return DXInput::Get()->IsKeyPressed(iKey);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsMouseDown(int iMouse)
{
	return DXInput::Get()->IsMouseDown(iMouse);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsMousePressed(int iMouse)
{
	return DXInput::Get()->IsMousePressed(iMouse);
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseScreenX()
{
	return DXInput::Get()->GetMouseScreenX();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseScreenY()
{
	return DXInput::Get()->GetMouseScreenY();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseMoveX()
{
	return DXInput::Get()->GetMouseMoveX();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseMoveY()
{
	return DXInput::Get()->GetMouseMoveY();
}

//----------------------------------------------------------------------------------------------------

int Input_GetMouseMoveZ()
{
	return DXInput::Get()->GetMouseMoveZ();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsGamePadButtonDown(int iButton)
{
	return DXInput::Get()->IsGamePadButtonDown(iButton);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsGamePadButtonPressed(int iButton)
{
	return DXInput::Get()->IsGamePadButtonPressed(iButton);
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadUp()
{
	return DXInput::Get()->IsDPadUp();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadDown()
{
	return DXInput::Get()->IsDPadDown();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadLeft()
{
	return DXInput::Get()->IsDPadLeft();
}

//----------------------------------------------------------------------------------------------------

bool Input_IsDPadRight()
{
	return DXInput::Get()->IsDPadRight();
}

//----------------------------------------------------------------------------------------------------

float Input_GetLeftAnalogX()
{
	return DXInput::Get()->GetLeftAnalogX();
}

//----------------------------------------------------------------------------------------------------

float Input_GetLeftAnalogY()
{
	return DXInput::Get()->GetLeftAnalogY();
}

//----------------------------------------------------------------------------------------------------

float Input_GetRightAnalogX()
{
	return DXInput::Get()->GetRightAnalogX();
}

//----------------------------------------------------------------------------------------------------

float Input_GetRightAnalogY()
{
	return DXInput::Get()->GetRightAnalogY();
}


} // namespace SGE