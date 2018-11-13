#pragma once

/************************************************************************/
/* Customized active stereo sensor with different intrinsics and API    */
/************************************************************************/

#include "DepthSensor.h"

#include "stdafx.h"


class ChristophSensor : public DepthSensor
{
public:

	//! Constructor
	ChristophSensor();

	//! Destructor; releases allocated ressources
	~ChristophSensor();

	//! Initializes the sensor
	HRESULT createFirstConnected();

	//! Reads the next depth (and color) frame
	HRESULT processDepth();

	HRESULT processColor() 
	{
		//everything done in process depth since order is relevant (color must be read first)
		return S_OK;
	}

	HRESULT processLabel() 
	{
		//no Label 
		return S_OK;
	}

	HRESULT processPreDepth()
	{
		return S_OK;
	}

	HRESULT processPreColor()
	{
		return S_OK;
	}

	HRESULT processPreLabel()
	{
		//no Label
		return S_OK;
	}

	HRESULT ChristophSensor::toggleNearMode() {
		return S_OK;
	}

	//! Toggle enable auto white balance
	HRESULT toggleAutoWhiteBalance() {
		return S_OK;
	}

	void setCurrentFrameNumber(unsigned int i)
	{
		// TODO
	}

	bool isKinect4Windows() {
		return true;
	}

private:
	StringCounter* m_StringCounter;
};
