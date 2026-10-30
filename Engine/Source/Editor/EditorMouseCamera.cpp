#include "EditorMouseCamera.h"

void EditorMouseCamera::Reset()
{
	mouseX = 0;
	mouseY = 0;
	dx = 0;
	dy = 0;
	
	leftButton = false;
	rightButton = false;

	// wheelCaptured = false;
	// wheelMode = false;
	// wheel = 0;
}

void EditorMouseCamera::UpdateMouseXY(double x, double y, double* mdx, double* mdy)
{
	dx = x - mouseX;
	dy = y - mouseY;
	mouseX = x;
	mouseY = y;
	if(mdx) *mdx = dx;
	if(mdy) *mdy = dy;
}

double EditorMouseCamera::GetMouseDx()
{
	return dx;
}

double EditorMouseCamera::GetMouseDy()
{
	return dy;
}

void EditorMouseCamera::RickClick(double x, double y)
{
	RightButton(true);
	mouseX = x;
	mouseY = y;
}

bool EditorMouseCamera::RightButton()
{
	return rightButton;
}

void EditorMouseCamera::RightButton(bool value)
{
	rightButton = value;
}

void EditorMouseCamera::LeftClick(double x, double y)
{
	LeftButton(true);
	mouseX = x;
	mouseY = y;
}

bool EditorMouseCamera::LeftButton()
{
	return leftButton;
}

void EditorMouseCamera::LeftButton(bool value)
{
	leftButton = value;
}

void EditorMouseCamera::CaptureWheel(double value)
{
	wheelValue += value;
	dw = value;
	// std::cout << __FUNCTION__ << "wheel:" << wheelValue << " dw:" << dw << std::endl;
}

double EditorMouseCamera::GetWheel()
{
	return dw;
}

/*
bool EditorMouseCamera::WheelCaptured() const { return wheelCaptured; }

bool EditorMouseCamera::WheelMode() const
{
	return wheelMode;
}

int EditorMouseCamera::Wheel() const { return wheel; }

void EditorMouseCamera::Wheel(int w)
{
	wheel = w;
}

void EditorMouseCamera::CaptureWheel(int scrollWheelValue)
{
	wheel = scrollWheelValue;
	wheelCaptured = true;
}

void EditorMouseCamera::UpdateWheelMode(int scrollWheelValue, double x, double y)
{
	mouseX = x;
	mouseY = y;
	wheel = scrollWheelValue;
	wheelMode = true;
}
*/


