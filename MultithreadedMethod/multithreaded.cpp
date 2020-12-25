#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <intrin.h>
#include <cmath>

const int THREADS = std::thread::hardware_concurrency();

void calculate(double** arr, unsigned int K, unsigned int N, const int Q, long double& prod, int t_id) {
	prod = 1.0;
	double* sum = new double[K];
	unsigned int begin = (K / THREADS) * t_id + 1;
	unsigned int end = begin + (K / THREADS);

	for (size_t i = begin; i < end; i++) {
		sum[i] = 0.0;
		arr[i] = new double[N];
		for (size_t j = 1; j < N; j++) {
			arr[i][j] = sqrt(Q * j / 100.0);
			sum[i] += arr[i][j];
		}
		prod *= sum[i];
		delete[] arr[i];
	}
	delete[] sum;
}

int main() {
	const int Q = 9;
	unsigned int K;
	unsigned int N;

	std::cout << "Enter matrix dimensions:\n";
	std::cout << "K = ";
	std::cin >> K;
	std::cout << "N = ";
	std::cin >> N;

	std::vector<std::thread*> threads(THREADS);
	long double result = 1.0;
	long double* prod = new long double[THREADS];
	double** arr = new double* [K];

	auto start_t = std::chrono::high_resolution_clock::now();
	auto start_c = __rdtsc();

	for (size_t i = 0; i < THREADS; i++) {
		threads[i] = new std::thread(calculate, arr, K, N, Q, std::ref(prod[i]), i);
	}
	std::for_each(threads.begin(), threads.end(), [](std::thread* thr) {
		thr->join();
		});

	auto end_c = __rdtsc();
	auto end_t = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < THREADS; i++) {
		result *= prod[i];
	}
	std::chrono::duration<float> time = end_t - start_t;

	std::cout << "\nExecution result: " << result << '\n';
	std::cout << "Execution clocks: " << end_c - start_c << '\n';
	std::cout << "Execution time: " << time.count() << " sec\n";

	delete[] prod;
	delete[] arr;
	system("pause");

	return 0;
}