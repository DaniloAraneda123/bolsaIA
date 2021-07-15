#include <iostream>
#include <fstream>
#include "pca.h"
#include <Eigen/Dense>


int main(int argc, char* argv[])
{
	Eigen::Matrix<float, 10, 5> pca_data_matrix;
	Eigen::Matrix<float, 10, 5> pca_center_matrix;
	pca_data_matrix <<
		1.5, 2.3, 1, 1, 1,
		3, 1.7, 2, 1, 1,
		1.2, 2.9, 3, 1, 9,
		2.1, 2.2, 4, 1, 1,
		3.1, 3.1, 5, 1, 1,
		1.3, 2.7, 6, 1, 1,
		2.0, 1.7, 7, 1, 7,
		1.0, 2, 9, 1, 1,
		0.5, 0.6, 8, 1, 3,
		1.0, 0.9, 9, 2, 2;


	pca_t<float> pca;
	pca.set_input(pca_data_matrix);
	pca.compute();
	

	std::cout
		<< "input matriz:		\n" << pca.get_input_matrix() << std::endl << std::endl
		<< "input matriz:		\n" << pca.get_input_matrix() << std::endl << std::endl
		<< "matriz centrada:	\n" << pca.get_centered_matrix() << std::endl << std::endl
		<< "covarianza matriz:	\n" << pca.get_covariance_matrix() << std::endl << std::endl
		<< "matriz proectada:	\n" << pca.get_projection_matrix() << std::endl << std::endl
		<< "auto valores:		\n" << pca.get_eigen_values() << std::endl << std::endl
		<< "auto vectores:		\n" << pca.get_eigen_vectors() << std::endl << std::endl;

	
	return EXIT_SUCCESS;
}