#pragma once

/************************************************************************/
/* Reads data from a set of image sequence with label                   */
/************************************************************************/

#include "DepthSensor.h"
#include "GlobalAppState.h"
#include <string>

class LabelReaderSensor : public DepthSensor
{
public:
	LabelReaderSensor();
	~LabelReaderSensor();

	HRESULT createFirstConnected();

	HRESULT processDepth();

	HRESULT processColor();

	HRESULT processLabel();

	HRESULT processPreDepth();

	HRESULT processPreColor();

	HRESULT processPreLabel();

	HRESULT toggleAutoWhiteBalance()
	{
		HRESULT hr = S_OK;
		return hr;
	}

	void setBaseFilePath(const std::string& basePath) 
	{
		m_BaseFilename = basePath;
	}

	void setCurrentFrameNumber(unsigned int i)
	{
		m_CurrentFrameNumberDepth = i;
		m_CurrentFrameNumberColor = i;
		m_CurrentFrameNumberLabel = i;
	}

	void setNumFrames(unsigned int i) 
	{
		m_NumFrames = i;
	}

	unsigned int getNumFrames() 
	{
		return m_NumFrames;
	}

private:
	std::string m_BaseFilename;
	unsigned int m_CurrentFrameNumberDepth;
	unsigned int m_CurrentFrameNumberColor;
	unsigned int m_CurrentFrameNumberLabel;
	unsigned int m_NumFrames;
};