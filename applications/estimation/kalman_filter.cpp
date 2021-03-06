// kalman_filter.cpp example program comparing float vs posit kalman filtering
//
// Copyright (C) 2017-2020 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

// enable the mathematical constants in cmath: old-style preprocessor magic which isn't best practice anymore
#include "common.hpp"

#include "kalman.hpp"

// Turn it off for now
#define USE_POSIT

int main(int argc, char** argv)
try {
	const size_t nbits = 16;
	const size_t es = 1;
	const size_t vecSize = 32;

#ifdef USE_POSIT
	using Ty     = sw::unum::posit<8, 0>;
	using Matrix = mtl::dense2D< Ty >;
	using Vector = mtl::dense_vector< Ty >;
#else
	using Ty     = float;
	using Matrix = mtl::dense2D<float>;
	using Vector = mtl::dense_vector<float>;
#endif

  int n = 3; // Number of states
  int m = 1; // Number of measurements

  double dt = 1.0/30; // Time step

  mtl::dense2D<Ty> A(n, n); // System dynamics matrix
  mtl::dense2D<Ty> C(m, n); // Output matrix
  mtl::dense2D<Ty> Q(n, n); // Process noise covariance
  mtl::dense2D<Ty> R(m, m); // Measurement noise covariance
  mtl::dense2D<Ty> P(n, n); // Estimate error covariance

  // Discrete LTI projectile motion, measuring position only
  A = 1, dt, 0, 0, 1, dt, 0, 0, 1;
  C = 1, 0, 0;

  // Reasonable covariance matrices
  Q = .05, .05, .0, .05, .05, .0, .0, .0, .0;
  R = 5;
  P = .1, .1, .1, .1, 10000, 10, .1, 10, 100;

  std::cout << "A: \n" << A << std::endl;
  std::cout << "C: \n" << C << std::endl;
  std::cout << "Q: \n" << Q << std::endl;
  std::cout << "R: \n" << R << std::endl;
  std::cout << "P: \n" << P << std::endl;

  // Construct the filter
  KalmanFilter<Ty> kf(dt, A, C, Q, R, P);

  // List of noisy position measurements (y)
  std::vector<double> measurements = {
      1.04202710058, 1.10726790452, 1.2913511148, 1.48485250951, 1.72825901034,
      1.74216489744, 2.11672039768, 2.14529225112, 2.16029641405, 2.21269371128,
      2.57709350237, 2.6682215744, 2.51641839428, 2.76034056782, 2.88131780617,
      2.88373786518, 2.9448468727, 2.82866600131, 3.0006601946, 3.12920591669,
      2.858361783, 2.83808170354, 2.68975330958, 2.66533185589, 2.81613499531,
      2.81003612051, 2.88321849354, 2.69789264832, 2.4342229249, 2.23464791825,
      2.30278776224, 2.02069770395, 1.94393985809, 1.82498398739, 1.52526230354,
      1.86967808173, 1.18073207847, 1.10729605087, 0.916168349913, 0.678547664519,
      0.562381751596, 0.355468474885, -0.155607486619, -0.287198661013, -0.602973173813
  };

  // Best guess of initial states
  mtl::dense_vector< Ty > x0(n);
  x0 = measurements[0], 0, -9.81;
  double t0 = 0.0;
  kf.init(t0, x0);

  // Feed measurements into filter, output estimated states
  double t = 0;
  Vector y(m);
  std::cout << "t = " << t << ", " << "x_hat[0]: " << trans(kf.state()) << std::endl;
  for(int i = 0; i < measurements.size(); i++) {
    t += dt;
    y = measurements[i];
    kf.update(y);
    std::cout << "t = " << t << ", " << "y[" << i << "] = " << trans(y)
        << ", x_hat[" << i << "] = " << trans(kf.state()) << std::endl;
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