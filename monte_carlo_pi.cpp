#include <thread>
#include <atomic>
#include <random>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

class MonteCarloPI {
private:
    std::atomic<size_t> points_inside_circle;
    const size_t total_points;
    const unsigned int num_threads;

    void calculate_partial(size_t points_per_thread) {
        size_t local_points_inside = 0;
        
        // Each thread gets its own random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (size_t i = 0; i < points_per_thread; ++i) {
            double x = dist(gen);
            double y = dist(gen);
            double distance_squared = x * x + y * y;

            // Point is inside the quarter circle if distance <= 1
            if (distance_squared <= 1.0) {
                ++local_points_inside;
            }
        }

        // Atomic update of shared counter
        points_inside_circle.fetch_add(local_points_inside, std::memory_order_relaxed);
    }

public:
    MonteCarloPI(size_t total_points, unsigned int threads = std::thread::hardware_concurrency())
        : points_inside_circle(0), total_points(total_points), 
          num_threads(threads > 0 ? threads : 1) {}

    double estimate_pi() {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        threads.reserve(num_threads);

        size_t points_per_thread = total_points / num_threads;
        size_t remainder = total_points % num_threads;

        // Launch worker threads
        for (unsigned int i = 0; i < num_threads; ++i) {
            // Distribute remainder points to first few threads
            size_t points = points_per_thread + (i < remainder ? 1 : 0);
            threads.emplace_back(&MonteCarloPI::calculate_partial, this, points);
        }

        // Wait for all threads to complete
        for (auto& thread : threads) {
            thread.join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        // Calculate π estimate
        double pi_estimate = 4.0 * points_inside_circle.load(std::memory_order_acquire) / total_points;

        // Print results
        std::cout << std::fixed << std::setprecision(15);
        std::cout << "Monte Carlo π Estimation\n";
        std::cout << "========================\n";
        std::cout << "Total points:     " << total_points << "\n";
        std::cout << "Points inside:    " << points_inside_circle.load() << "\n";
        std::cout << "Threads used:     " << num_threads << "\n";
        std::cout << "Estimated π:      " << pi_estimate << "\n";
        std::cout << "Actual π:         " << M_PI << "\n";
        std::cout << "Error:            " << std::abs(pi_estimate - M_PI) << "\n";
        std::cout << std::setprecision(6);
        std::cout << "Elapsed time:     " << elapsed.count() << " seconds\n";

        return pi_estimate;
    }
};

int main(int argc, char* argv[]) {
    // Default values
    size_t total_points = 10'000'000;
    unsigned int num_threads = std::thread::hardware_concurrency();

    // Parse command line arguments
    if (argc > 1) {
        total_points = std::stoull(argv[1]);
    }
    if (argc > 2) {
        num_threads = std::stoul(argv[2]);
    }

    std::cout << "Calculating π using Monte Carlo method...\n\n";

    MonteCarloPI calculator(total_points, num_threads);
    calculator.estimate_pi();

    return 0;
}