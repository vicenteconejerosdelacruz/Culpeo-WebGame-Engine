#include "EditorMouseCamera.h"

void EditorMouseCamera::Reset()
{
	mouseX = 0;
	mouseY = 0;
	dx = 0;
	dy = 0;
	sx = 0;
	sy = 0;
	
	leftButton = false;
	rightButton = false;

	wheelValue = 0.0;
	dw = 0.0;
}

void EditorMouseCamera::UpdateMouseXY(double x, double y, double* mdx, double* mdy)
{
	// std::cout << "UpdateMouseXY: x=" << x << ", y=" << y << std::endl;
	dx = x - mouseX;
	dy = y - mouseY;
	sx+= dx;
	sy+= dy;
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

double EditorMouseCamera::GetMouseSx()
{
	return sx;
}

double EditorMouseCamera::GetMouseSy()
{
	return sy;
}

void EditorMouseCamera::ResetSxSy()
{
	sx = 0;
	sy = 0;
}

void EditorMouseCamera::RightClick(double x, double y)
{
	RightButton(true);
	mouseX = x;
	mouseY = y;
	dx = 0;
	dy = 0;
	sx = 0;
	sy = 0;
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
	dx = 0;
	dy = 0;
	sx = 0;
	sy = 0;
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
}

double EditorMouseCamera::GetWheel()
{
	return dw;
}
