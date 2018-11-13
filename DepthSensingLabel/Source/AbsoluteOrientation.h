//////////////////////////////////////////////////////////////////////////
// version:  1.0
// date: 09.08.31
// function: absolute orientation, deal with space point set
// author: Zheng Qian
//            SIAT
// 
//////////////////////////////////////////////////////////////////////////

#pragma  once
#include <string>
#include <Eigen/Dense>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

class CAbsoluteOrientation
{
public:
    CAbsoluteOrientation() 
    {}
    ~CAbsoluteOrientation()
    {}

    static std::string title() { return "[AbsoluteOrientation]: "; }

public:
	
	//////////////////////////////////////////////////////////////////////////
	// B - (cRA+t) minimize right multiplication
	//////////////////////////////////////////////////////////////////////////
	static double ApplyToPoint(const Eigen::MatrixXd& vecPosA, const Eigen::MatrixXd& vecPosB, 
		Eigen::Matrix3d& matR, float& fScale, Eigen::Vector3d& vTranslation);

	//////////////////////////////////////////////////////////////////////////
	// weighted average minimize right multiplication
	//////////////////////////////////////////////////////////////////////////
	static double ApplyToPoint(const Eigen::MatrixXd& vecPosA, const Eigen::MatrixXd& vecPosB, const Eigen::VectorXd& weights,
		Eigen::Matrix3d& matR, float& fScale, Eigen::Vector3d& vTranslation);

	//////////////////////////////////////////////////////////////////////////
	// B - (cRA+t) minimize right multiplication
	//////////////////////////////////////////////////////////////////////////
	static double ApplyToPoint(const std::vector<cv::Point3f>& vecPosA, const std::vector<cv::Point3f>& vecPosB, 
		Eigen::Matrix3d& matR, float& fScale, Eigen::Vector3d& vTranslation);

	//double ApplyToPoint(const std::vector<Vertex*>& vecPosA, const std::vector<Vertex*>&vecPosB,
	//	Eigen::Matrix3d& matR, float& fScale, Eigen::Vector3d& vTranslation);

	//////////////////////////////////////////////////////////////////////////
	// weighted average minimize right multiplication
	//////////////////////////////////////////////////////////////////////////
	static double ApplyToPoint(const std::vector<cv::Point3f>& vecPosA, const std::vector<cv::Point3f>& vecPosB, const std::vector<float>& weights,
		Eigen::Matrix3d& matR, float& fScale, Eigen::Vector3d& vTranslation);

private:

	//void vertices_2_eigen_matrix(Eigen::MatrixXd& vecPos_Matrix, const std::vector<Vertex*>& vecPos_Vertices);
};