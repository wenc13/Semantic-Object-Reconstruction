#include <fstream>
#include <iostream>
#include "AbsoluteOrientation.h"

double CAbsoluteOrientation::ApplyToPoint(const std::vector<cv::Point3f>& vecPosA, const std::vector<cv::Point3f>& vecPosB, 
	Eigen::Matrix3d& matR, float& fScale, Eigen::Vector3d& vTranslation)
{
	if (vecPosA.size() != vecPosB.size())
		return -1;
	unsigned int num = vecPosB.size();
	if (num < 3)
		return -1;

	// compute the center of pos_vec_a, pos_vec_b
	Eigen::Vector3d center_ap(0,0,0);
	Eigen::Vector3d center_bp(0,0,0);
	for (unsigned int i = 0 ;i != num; ++i)
	{
		Eigen::Vector3d a( vecPosA[i].x, vecPosA[i].y, vecPosA[i].z );
		Eigen::Vector3d b( vecPosB[i].x, vecPosB[i].y, vecPosB[i].z );
		center_ap += a;
		center_bp += b;
	}
	center_ap /= num;
	center_bp /= num;

	// compute the variance of pos_vec_a, pos_vec_b
	double var_a = 0, var_b = 0;
	for( unsigned int i = 0; i < num; i++ )
	{
		Eigen::Vector3d a( vecPosA[i].x, vecPosA[i].y, vecPosA[i].z );
		Eigen::Vector3d b( vecPosB[i].x, vecPosB[i].y, vecPosB[i].z );

		var_a += ( a - center_ap ).squaredNorm();
		var_b += ( b - center_bp ).squaredNorm();
	}
	var_a /= num;
	var_b /= num;

	// compute the matrix
	Eigen::Matrix3d  mat = Eigen::Matrix3d::Zero();
	Eigen::MatrixXd  am(num,3);
	Eigen::MatrixXd  bm(num,3);
	for( unsigned int i = 0; i<num; i++ )
	{	
		Eigen::Vector3d a( vecPosA[i].x, vecPosA[i].y, vecPosA[i].z );
		Eigen::Vector3d b( vecPosB[i].x, vecPosB[i].y, vecPosB[i].z );

		Eigen::Vector3d ap = a- center_ap;
		Eigen::Vector3d bp = b - center_bp;
		mat += bp*ap.transpose();
	}
	mat /= num;

	// compute the SVD of mat
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV );
	double det = mat.determinant();
	Eigen::Matrix3d d_mat = Eigen::Matrix3d::Zero();
	Eigen::Matrix3d u_mat  = Eigen::Matrix3d::Zero();
	Eigen::Matrix3d v_mat = Eigen::Matrix3d::Zero();
	d_mat.block(0,0,3,3) = svd.singularValues().asDiagonal();
	u_mat.block(0,0,3,3) = svd.matrixU();
	v_mat.block(0,0,3,3) = svd.matrixV();

	// compute the transform
	Eigen::Matrix3d s_mat = Eigen::Matrix3d::Identity();
	if( det < 0)
		s_mat(2,2) = -1;
	Eigen::Matrix3d r_mat = u_mat * s_mat * v_mat.transpose();

	double tr= ( d_mat*s_mat ).trace() ;
	double c = tr/ var_a;
	Eigen::Vector3d tv = center_bp - c*r_mat*center_ap;
	matR = r_mat;
	fScale = c;
	vTranslation = tv;

	double err = var_b - tr*tr/var_b;
	return err;
}

/*
double CAbsoluteOrientation::ApplyToPoint(const std::vector<Vertex*>& vecPosA, const std::vector<Vertex*>& vecPosB, 
	Eigen::Matrix3d& matR, float& fScale, Eigen::Vector3d& vTranslation)
{
	if( vecPosA.size() != vecPosB.size())
		return -1;
	unsigned int num = vecPosB.size();
	if( num < 3)
		return -1;

	// compute the center of pos_vec_a, pos_vec_b
	Eigen::Vector3d center_ap(0,0,0);
	Eigen::Vector3d center_bp(0,0,0);
	for( unsigned int i = 0 ;i!= num; ++i )
	{
		Eigen::Vector3d a( vecPosA[i]->point().x(), vecPosA[i]->point().y(), vecPosA[i]->point().z() );
		Eigen::Vector3d b( vecPosB[i]->point().x(), vecPosB[i]->point().y(), vecPosB[i]->point().z() );
		center_ap += a;
		center_bp += b;
	}
	center_ap /= num;
	center_bp /= num;

	// compute the variance of pos_vec_a, pos_vec_b
	double var_a = 0, var_b = 0;
	for( unsigned int i = 0; i<num; i++ )
	{
		Eigen::Vector3d a( vecPosA[i]->point().x(), vecPosA[i]->point().y(), vecPosA[i]->point().z() );
		Eigen::Vector3d b( vecPosB[i]->point().x(), vecPosB[i]->point().y(), vecPosB[i]->point().z() );

		var_a += ( a - center_ap ).squaredNorm();
		var_b += ( b - center_bp ).squaredNorm();
	}
	var_a /= num;
	var_b /= num;

	// compute the matrix
	Eigen::Matrix3d  mat = Eigen::Matrix3d::Zero();
	Eigen::MatrixXd  am(num,3);
	Eigen::MatrixXd  bm(num,3);
	for( unsigned int i = 0; i<num; i++ )
	{	
		Eigen::Vector3d a( vecPosA[i]->point().x(), vecPosA[i]->point().y(), vecPosA[i]->point().z() );
		Eigen::Vector3d b( vecPosB[i]->point().x(), vecPosB[i]->point().y(), vecPosB[i]->point().z() );

		Eigen::Vector3d ap = a- center_ap;
		Eigen::Vector3d bp = b - center_bp;
		mat += bp*ap.transpose();
	}
	mat /= num;

	// compute the SVD of mat
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV );
	double det = mat.determinant();
	Eigen::Matrix3d d_mat = Eigen::Matrix3d::Zero();
	Eigen::Matrix3d u_mat  = Eigen::Matrix3d::Zero();
	Eigen::Matrix3d v_mat = Eigen::Matrix3d::Zero();
	d_mat.block(0,0,3,3) = svd.singularValues().asDiagonal();
	u_mat.block(0,0,3,3) = svd.matrixU();
	v_mat.block(0,0,3,3) = svd.matrixV();

	// compute the transform
	Eigen::Matrix3d s_mat = Eigen::Matrix3d::Identity();
	if( det < 0)
		s_mat(2,2) = -1;
	Eigen::Matrix3d r_mat = u_mat * s_mat * v_mat.transpose();

	double tr= ( d_mat*s_mat ).trace() ;
	double c = tr/ var_a;
	Eigen::Vector3d tv = center_bp - c*r_mat*center_ap;
	matR = r_mat;
	fScale = c;
	vTranslation = tv;

	double err = var_b - tr*tr/var_b;
	return err;
}
*/

double CAbsoluteOrientation::ApplyToPoint(const std::vector<cv::Point3f>& vecPosA, const std::vector<cv::Point3f>& vecPosB, const std::vector<float>& weights,
	Eigen::Matrix3d& matR, float& fScale, Eigen::Vector3d& vTranslation)
{
	if( vecPosA.size() != vecPosB.size()|| weights.size() != vecPosA.size() )
		return -1;
	unsigned int num = vecPosB.size();
	if( num < 3)
		return -1;

	// compute the center of pos_vec_a, pos_vec_b
	Eigen::Vector3d center_ap(0,0,0);
	Eigen::Vector3d center_bp(0,0,0);
	double dbSum = 0;
	for( unsigned int i = 0 ;i!= num; ++i )
	{
		Eigen::Vector3d a( vecPosA[i].x, vecPosA[i].y, vecPosA[i].z );
		Eigen::Vector3d b( vecPosB[i].x, vecPosB[i].y, vecPosB[i].z );

		center_ap += a* weights[i];
		center_bp += b* weights[i];
		dbSum += weights[i];
	}
	center_ap /= dbSum;
	center_bp /= dbSum;

	// compute the variance of pos_vec_a, pos_vec_b
	double var_a = 0, var_b = 0;
	for( unsigned int i = 0; i<num; i++ )
	{
		Eigen::Vector3d a( vecPosA[i].x, vecPosA[i].y, vecPosA[i].z );
		Eigen::Vector3d b( vecPosB[i].x, vecPosB[i].y, vecPosB[i].z );

		var_a +=  weights[i]*( ( a- center_ap ).squaredNorm() );
		var_b +=  weights[i]*( ( b - center_bp).squaredNorm() );
	}
	var_a /= dbSum;
	var_b /= dbSum;

	// compute the matrix
	Eigen::Matrix3d  mat = Eigen::Matrix3d::Zero();
	Eigen::MatrixXd  am(num,3);
	Eigen::MatrixXd  bm(num,3);
	for( unsigned int i = 0; i<num; i++ )
	{	
		Eigen::Vector3d a( vecPosA[i].x, vecPosA[i].y, vecPosA[i].z );
		Eigen::Vector3d b( vecPosB[i].x, vecPosB[i].y, vecPosB[i].z );

		Eigen::Vector3d ap = a- center_ap;
		Eigen::Vector3d bp = b- center_bp;
		mat += bp*ap.transpose()*weights[i];
	}
	mat /= dbSum;

	// compute the SVD of mat
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV );
	double det = mat.determinant();
	Eigen::Matrix3d d_mat = Eigen::Matrix3d::Zero();
	Eigen::Matrix3d u_mat  = Eigen::Matrix3d::Zero();
	Eigen::Matrix3d v_mat = Eigen::Matrix3d::Zero();
	d_mat.block(0,0,3,3) = svd.singularValues().asDiagonal();
	u_mat.block(0,0,3,3) = svd.matrixU();
	v_mat.block(0,0,3,3) = svd.matrixV();

	// compute the transform
	Eigen::Matrix3d s_mat = Eigen::Matrix3d::Identity();
	if( det < 0)
		s_mat(2,2) = -1;
	Eigen::Matrix3d r_mat = u_mat * s_mat * v_mat.transpose();

	double tr= ( d_mat*s_mat ).trace() ;
	double c = tr/ var_a;
	Eigen::Vector3d tv = center_bp - c*r_mat*center_ap;
	matR = r_mat;
	fScale = c;
	vTranslation = tv;

	double err = var_b - tr*tr/var_b;
	return err;
}

/*
void CAbsoluteOrientation::vertices_2_eigen_matrix(Eigen::MatrixXd& vecPos_Matrix, const std::vector<Vertex*>& vecPos_Vertices)
{
	unsigned int num_vertices = vecPos_Vertices.size();
	vecPos_Matrix = Eigen::MatrixXd(num_vertices, 3);

	for (unsigned int i = 0; i < num_vertices; i++)
	{
		vecPos_Matrix(i,0) = vecPos_Vertices[i]->point().x();
		vecPos_Matrix(i,1) = vecPos_Vertices[i]->point().y();
		vecPos_Matrix(i,2) = vecPos_Vertices[i]->point().z();
	}
}
*/

double CAbsoluteOrientation::ApplyToPoint(const Eigen::MatrixXd& vecPosA, const Eigen::MatrixXd& vecPosB, 
	Eigen::Matrix3d& matR, float& fScale, Eigen::Vector3d& vTranslation)
{
	if( vecPosA.rows() != vecPosB.rows() || vecPosA.cols() != 3 || vecPosA.cols() != 3)
		return -1;
	unsigned int num = vecPosB.rows();
	if( num < 3)
		return -1;

	// compute the center of pos_vec_a, pos_vec_b
	Eigen::Vector3d center_ap(0,0,0);
	Eigen::Vector3d center_bp(0,0,0);
	for( unsigned int i = 0 ;i!= num; ++i )
	{
		Eigen::Vector3d a( vecPosA(i,0), vecPosA(i,1), vecPosA(i,2) );
		Eigen::Vector3d b( vecPosB(i,0), vecPosB(i,1), vecPosB(i,2) );
		center_ap += a;
		center_bp += b;
	}
	center_ap /= num;
	center_bp /= num;

	// compute the variance of pos_vec_a, pos_vec_b
	double var_a = 0, var_b = 0;
	for( unsigned int i = 0; i<num; i++ )
	{
		Eigen::Vector3d a( vecPosA(i,0), vecPosA(i,1), vecPosA(i,2) );
		Eigen::Vector3d b( vecPosB(i,0), vecPosB(i,1), vecPosB(i,2) );

		var_a += ( a - center_ap ).squaredNorm();
		var_b += ( b - center_bp ).squaredNorm();
	}
	var_a /= num;
	var_b /= num;

	// compute the matrix
	Eigen::Matrix3d  mat = Eigen::Matrix3d::Zero();
	Eigen::MatrixXd  am(num,3);
	Eigen::MatrixXd  bm(num,3);
	for( unsigned int i = 0; i<num; i++ )
	{	
		Eigen::Vector3d a( vecPosA(i,0), vecPosA(i,1), vecPosA(i,2) );
		Eigen::Vector3d b( vecPosB(i,0), vecPosB(i,1), vecPosB(i,2) );

		Eigen::Vector3d ap = a- center_ap;
		Eigen::Vector3d bp = b - center_bp;
		mat += bp*ap.transpose();
	}
	mat /= num;

	// compute the SVD of mat
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV );
	double det = mat.determinant();
	Eigen::Matrix3d d_mat = Eigen::Matrix3d::Zero();
	Eigen::Matrix3d u_mat  = Eigen::Matrix3d::Zero();
	Eigen::Matrix3d v_mat = Eigen::Matrix3d::Zero();
	d_mat.block(0,0,3,3) = svd.singularValues().asDiagonal();
	u_mat.block(0,0,3,3) = svd.matrixU();
	v_mat.block(0,0,3,3) = svd.matrixV();

	// compute the transform
	Eigen::Matrix3d s_mat = Eigen::Matrix3d::Identity();
	if( det < 0)
		s_mat(2,2) = -1;
	Eigen::Matrix3d r_mat = u_mat * s_mat * v_mat.transpose();

	double tr= ( d_mat*s_mat ).trace() ;
	double c = tr/ var_a;
	Eigen::Vector3d tv = center_bp - c*r_mat*center_ap;
	matR = r_mat;
	fScale = c;
	vTranslation = tv;

	double err = var_b - tr*tr/var_b;
	return err;
}

double CAbsoluteOrientation::ApplyToPoint(const Eigen::MatrixXd& vecPosA, const Eigen::MatrixXd& vecPosB, const Eigen::VectorXd& weights,
	Eigen::Matrix3d& matR, float& fScale, Eigen::Vector3d& vTranslation)
{
	if( vecPosA.rows() != vecPosB.rows() || vecPosA.cols() != 3 || vecPosA.cols() != 3 || weights.size() != vecPosA.rows() )
		return -1;
	unsigned int num = vecPosB.rows();
	if( num < 3)
		return -1;

	// compute the center of pos_vec_a, pos_vec_b
	Eigen::Vector3d center_ap(0,0,0);
	Eigen::Vector3d center_bp(0,0,0);
	double dbSum = 0;
	for( unsigned int i = 0 ;i!= num; ++i )
	{
		Eigen::Vector3d a( vecPosA(i,0), vecPosA(i,1), vecPosA(i,2) );
		Eigen::Vector3d b( vecPosB(i,0), vecPosB(i,1), vecPosB(i,2) );

		center_ap += a* weights(i);
		center_bp += b* weights(i);
		dbSum += weights(i);
	}
	center_ap /= dbSum;
	center_bp /= dbSum;

	// compute the variance of pos_vec_a, pos_vec_b
	double var_a = 0, var_b = 0;
	for( unsigned int i = 0; i<num; i++ )
	{
		Eigen::Vector3d a( vecPosA(i,0), vecPosA(i,1), vecPosA(i,2) );
		Eigen::Vector3d b( vecPosB(i,0), vecPosB(i,1), vecPosB(i,2) );

		var_a +=  weights(i)*( ( a- center_ap ).squaredNorm() );
		var_b +=  weights(i)*( ( b - center_bp).squaredNorm() );
	}
	var_a /= dbSum;
	var_b /= dbSum;

	// compute the matrix
	Eigen::Matrix3d  mat = Eigen::Matrix3d::Zero();
	Eigen::MatrixXd  am(num,3);
	Eigen::MatrixXd  bm(num,3);
	for( unsigned int i = 0; i<num; i++ )
	{	
		Eigen::Vector3d a( vecPosA(i,0), vecPosA(i,1), vecPosA(i,2) );
		Eigen::Vector3d b( vecPosB(i,0), vecPosB(i,1), vecPosB(i,2) );

		Eigen::Vector3d ap = a- center_ap;
		Eigen::Vector3d bp = b- center_bp;
		mat += bp*ap.transpose()*weights(i);
	}
	mat /= dbSum;

	// compute the SVD of mat
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV );
	double det = mat.determinant();
	Eigen::Matrix3d d_mat = Eigen::Matrix3d::Zero();
	Eigen::Matrix3d u_mat  = Eigen::Matrix3d::Zero();
	Eigen::Matrix3d v_mat = Eigen::Matrix3d::Zero();
	d_mat.block(0,0,3,3) = svd.singularValues().asDiagonal();
	u_mat.block(0,0,3,3) = svd.matrixU();
	v_mat.block(0,0,3,3) = svd.matrixV();

	// compute the transform
	Eigen::Matrix3d s_mat = Eigen::Matrix3d::Identity();
	if( det < 0)
		s_mat(2,2) = -1;
	Eigen::Matrix3d r_mat = u_mat * s_mat * v_mat.transpose();

	double tr= ( d_mat*s_mat ).trace() ;
	double c = tr/ var_a;
	Eigen::Vector3d tv = center_bp - c*r_mat*center_ap;
	matR = r_mat;
	fScale = c;
	vTranslation = tv;

	double err = var_b - tr*tr/var_b;
	return err;
}