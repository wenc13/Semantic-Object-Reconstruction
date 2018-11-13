#include "stdafx.h"

#include "LabelReaderSensor.h"


LabelReaderSensor::LabelReaderSensor()
{
	init(640, 480, 640, 480);

	//default path should be actually overwritten
	m_BaseFilename = "";
	//m_BaseFilename = "../stanfordData/copyroom_png/";
	m_NumFrames = 0;
}

LabelReaderSensor::~LabelReaderSensor()
{
}

HRESULT LabelReaderSensor::createFirstConnected()
{
	HRESULT hr = S_OK;

	//what Qian-Yi / Vladlen tell us
	float focalLengthX = 525.0f;
	float focalLengthY = 525.0f;
	//float cx = 319.5f;
	//float cy = 239.5f;

	//what the oni framework gives us
	//float focalLengthX = 570.34f;
	//float focalLengthY = 570.34f;
	float cx = 319.5f;
	float cy = 239.5f;
	initializeIntrinsics(focalLengthX, focalLengthY, cx, cy);

	m_CurrentFrameNumberColor = 0;
	m_CurrentFrameNumberDepth = 0;
	m_CurrentFrameNumberLabel = 0;

	return hr;
}

HRESULT LabelReaderSensor::processDepth()
{
	HRESULT hr = S_OK;
	if (m_CurrentFrameNumberDepth >= m_NumFrames) 
	{
		return S_FALSE;
	}

	std::cout << "Processing Depth Frame " << m_CurrentFrameNumberDepth << std::endl;
	char frameNumber_c[10];
	sprintf_s(frameNumber_c,"%07d", m_CurrentFrameNumberDepth+1);
	std::string frameNumber(frameNumber_c);
	std::string currFileName = m_BaseFilename;
	currFileName.append("depth/").append(frameNumber).append(".png");
	DepthImage image;
	FreeImageWrapper::loadImage(currFileName, image);
	//image.flipY();
	for (UINT i = 0; i < getDepthWidth() * getDepthHeight(); i++) 
	{
		m_depthD16[i] = (USHORT)(image.getDataPointer()[i] * 1000);
	}
	m_CurrentFrameNumberDepth++;
	return hr;
}

HRESULT LabelReaderSensor::processColor()
{
	HRESULT hr = S_OK;
	if (m_CurrentFrameNumberColor >= m_NumFrames) 
	{
		return S_FALSE;
	}

	bool readColor = true;

	if (readColor) 
	{
		char frameNumber_c[10];
		sprintf_s(frameNumber_c,"%07d", m_CurrentFrameNumberColor+1);
		std::string frameNumber(frameNumber_c);
		std::string currFileName = m_BaseFilename;
		currFileName.append("rgb/").append(frameNumber).append(".jpg");
		ColorImageRGB image;
		FreeImageWrapper::loadImage(currFileName, image);
		//image.flipY();
		for (UINT i = 0; i < getDepthWidth() * getDepthHeight(); i++) 
		{
			vec3f c = image.getDataPointer()[i];
			c = 255.0f*c;

			m_colorRGBX[4*i+0] = (BYTE)c.x;
			m_colorRGBX[4*i+1] = (BYTE)c.y; 
			m_colorRGBX[4*i+2] = (BYTE)c.z;
			m_colorRGBX[4*i+3] = 255; 
		}
	}
	m_CurrentFrameNumberColor++;
	return hr;
}

HRESULT LabelReaderSensor::processLabel()
{
	HRESULT hr = S_OK;
	if (m_CurrentFrameNumberLabel >= m_NumFrames) 
	{
		return S_FALSE;
	}

	bool readLabel = true;

	if (readLabel) 
	{
		char frameNumber_c[10];
		BaseImage<float> image;
		sprintf_s(frameNumber_c,"%07d", m_CurrentFrameNumberLabel+1);
		std::string frameNumber(frameNumber_c);
		for (int i = 0; i < GlobalAppState::getInstance().s_labelNum; i++)
		{
			std::string currFileName = m_BaseFilename;
			char partNumber_c[10];
			sprintf_s(partNumber_c,"%d", i);
			std::string partNumber(partNumber_c);

			currFileName.append("label/").append(frameNumber).append("_part").append(partNumber).append(".png");
			
			FreeImageWrapper::loadImage(currFileName, image);
			for (UINT j = 0; j < getDepthWidth() * getDepthHeight(); j++)
			{
				m_label[(i-i%4)/4][i%4+j*4] = (BYTE)(image.getDataPointer()[j]*1000/65535*255);
			}
		}
	}
	m_CurrentFrameNumberLabel++;
	return hr;
}

HRESULT LabelReaderSensor::processPreDepth()
{
	HRESULT hr = S_OK;
	if (m_CurrentFrameNumberDepth <= 1 || m_CurrentFrameNumberDepth > m_NumFrames)
	{
		return S_FALSE;
	}

	bool readPreDepth = true;

	if (readPreDepth)
	{
		char frameNumber_c[10];
		sprintf_s(frameNumber_c, "%07d", m_CurrentFrameNumberDepth-1);
		std::string frameNumber(frameNumber_c);
		std::string preFileName = m_BaseFilename;
		preFileName.append("depth/").append(frameNumber).append(".png");
		DepthImage image;
		FreeImageWrapper::loadImage(preFileName, image);
		//image.flipY();
		for (UINT i = 0; i < getDepthWidth() * getDepthHeight(); i++)
		{
			m_preDepthD16[i] = (USHORT)(image.getDataPointer()[i] * 1000);
		}
	}

	return hr;
}

HRESULT LabelReaderSensor::processPreColor()
{
	HRESULT hr = S_OK;
	if (m_CurrentFrameNumberColor <= 1 || m_CurrentFrameNumberColor > m_NumFrames)
	{
		return S_FALSE;
	}

	bool readPreColor = true;

	if (readPreColor) 
	{
		char frameNumber_c[10];
		sprintf_s(frameNumber_c, "%07d", m_CurrentFrameNumberColor-1);
		std::string frameNumber(frameNumber_c);
		std::string preFileName = m_BaseFilename;
		preFileName.append("rgb/").append(frameNumber).append(".jpg");
		ColorImageRGB image;
		FreeImageWrapper::loadImage(preFileName, image);
		//image.flipY();
		for (UINT i = 0; i < getDepthWidth() * getDepthHeight(); i++) 
		{
			vec3f c = image.getDataPointer()[i];
			c = 255.0f*c;

			m_preColorRGBX[4*i+0] = (BYTE)c.x;
			m_preColorRGBX[4*i+1] = (BYTE)c.y;
			m_preColorRGBX[4*i+2] = (BYTE)c.z;
			m_preColorRGBX[4*i+3] = 255;
		}
	}

	return hr;
}

HRESULT LabelReaderSensor::processPreLabel()
{
	HRESULT hr = S_OK;
	if (m_CurrentFrameNumberLabel <= 1 || m_CurrentFrameNumberLabel > m_NumFrames)
	{
		return S_FALSE;
	}

	bool readPreLabel = true;

	if (readPreLabel)
	{
		char frameNumber_c[10];
		BaseImage<float> image;
		sprintf_s(frameNumber_c, "%07d", m_CurrentFrameNumberLabel-1);
		std::string frameNumber(frameNumber_c);
		for (int i = 0; i < GlobalAppState::getInstance().s_labelNum; i++)
		{
			std::string preFileName = m_BaseFilename;
			char partNumber_c[10];
			sprintf_s(partNumber_c, "%d", i);
			std::string partNumber(partNumber_c);

			preFileName.append("label/").append(frameNumber).append("_part").append(partNumber).append(".png");

			FreeImageWrapper::loadImage(preFileName, image);
			for (UINT j = 0; j < getDepthWidth() * getDepthHeight(); j++)
			{
				m_preLabel[(i-i%4)/4][i%4+j*4] = (BYTE)(image.getDataPointer()[j]*1000/65535*255);
			}
		}
	}

	return hr;
}