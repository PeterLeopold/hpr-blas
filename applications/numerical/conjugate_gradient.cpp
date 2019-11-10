// conjugate_gradient.cpp: example program showing the conjugate gradient algorithm using posit arithmetic
//
// Copyright (C) 2017-2019 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <hprblas>
#include <universal/posit/posit>
#include <boost/multiprecision/cpp_bin_float.hpp>

typedef boost::multiprecision::number<boost::multiprecision::backends::cpp_bin_float<11, boost::multiprecision::backends::digit_base_2, void, boost::int16_t, -32, 31>, boost::multiprecision::expression_template_option::et_off> cpp_bin_float_half;

/*
From Wikipedia, the free encyclopedia
https://en.wikipedia.org/wiki/Conjugate_gradient_method

In mathematics, the conjugate gradient method is an algorithm for the numerical solution of particular 
systems of linear equations, namely those whose matrix is symmetric and positive-definite. 
The conjugate gradient method is often implemented as an iterative algorithm, applicable to 
sparse systems that are too large to be handled by a direct implementation or other direct methods 
such as the Cholesky decomposition. Large sparse systems often arise when numerically solving 
partial differential equations or optimization problems.

The conjugate gradient method can also be used to solve unconstrained optimization problems 
such as energy minimization. It was mainly developed by Magnus Hestenes and Eduard Stiefel who programmed it on the Z4.

The biconjugate gradient method provides a generalization to non-symmetric matrices. 
Various nonlinear conjugate gradient methods seek minima of nonlinear equations.
 */

// Conjugate Gradient algorithm, returns the iteration number of convergence
template<typename Matrix, typename Vector, typename Real>
unsigned CG(const Matrix& A, const Vector& b, Vector& x, Real epsilon) {
	using namespace sw::hprblas;
	//using Real = Vector::value_type;
	// starting x is provided by calling context
	unsigned k = 0;
	Vector r = b;
	Real error = mtl::two_norm(r);
	Vector p = r;
	Vector Ap(size(p)); // need to create the vector to be the same structure as p as the expression (A * p) doesn't do it
	while (error > epsilon) {
		Ap = A * p;
		Real alpha = dot(r, r) / dot(p, Ap);
		x = x + alpha * p;
		Vector r_prev = r;
		r = r - alpha * A * p;
		error = mtl::two_norm(r);
		std::cout << "iteration: " << k << " error : " << error << std::endl;
		Real beta = dot(r, r) / dot(r_prev, r_prev);
		p = r + beta * p;
		++k;
		if (k > 1.5* size(b)) break;
	}
	return k;
}

template<typename Real>
void CGdriver(unsigned N, Real epsilon) {
	using namespace std;
	using namespace mtl;
	using Matrix = mtl::dense2D<Real>;
	using Vector = mtl::dense_vector<Real>;

	Matrix A(N, N);
	mat::laplacian_setup(A, N, 1);
	cout << A << endl;

	Vector b(N), x(N), ones(N);
	ones = Real(1);
	b = A * ones;
	x = Real(0.0);  // starting x
	cout << b << endl;
	unsigned k = CG(A, b, x, epsilon);
	cout << "solution: " << x << " at iteration: " << k << endl;

	Vector separation(N);
	separation = ones - x;
	cout << "exact error is: " << two_norm(separation) << endl;
}

// CG test program
int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::unum;

	constexpr unsigned N = 10;

	{
		constexpr size_t nbits = 16;
		constexpr size_t es = 1;
		using Real = posit<nbits,es>;
		cout << "type: " << typeid(Real).name() << endl;
		cout << "minpos : " << minpos<nbits, es>() << endl;
		Real epsilon = Real(0.001);
		CGdriver(N, epsilon);
	}
	
	{
		using Real = cpp_bin_float_half;
		cout << "type: " << typeid(Real).name() << endl;
		Real epsilon = Real(0.001);
		CGdriver(N, epsilon);
	}

	return EXIT_SUCCESS;
}
catch (char const* msg) {
	std::cerr << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const posit_arithmetic_exception& err) {
	std::cerr << "Uncaught posit arithmetic exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const quire_exception& err) {
	std::cerr << "Uncaught quire exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const posit_internal_exception& err) {
	std::cerr << "Uncaught posit internal exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (std::runtime_error& err) {
	std::cerr << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}
