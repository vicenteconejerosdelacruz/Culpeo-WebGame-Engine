#pragma once

struct EditorMouseCamera
{
	//cursor
	double mouseX = 0.0;
	double mouseY = 0.0;
	double dx = 0.0;
	double dy = 0.0;
	//buttons
	bool leftButton = false;
	bool rightButton = false;
	//wheel
	double wheelValue;
	double dw = 0.0;
	// bool wheelCaptured = false;
	// bool wheelMode = false;
	// int wheel = 0;

	void Reset();
	void UpdateMouseXY(double x, double y, double* mdx=nullptr, double *mdy=nullptr);
	double GetMouseDx();
	double GetMouseDy();

	void RickClick(double x, double y);
	void LeftClick(double x, double y);
	bool RightButton();
	bool LeftButton();
	void RightButton(bool value);
	void LeftButton(bool value);

	void CaptureWheel(double value);
	double GetWheel();

	// bool WheelCaptured() const;
	// bool WheelMode() const;
	// int Wheel() const;
	// void Wheel(int);
	// void CaptureWheel(int scrollWheelValue);
	// void UpdateWheelMode(int scrollWheelValue, double x, double y);
};