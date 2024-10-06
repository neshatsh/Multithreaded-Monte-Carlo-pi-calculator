#include <thread>
#include <atomic>
#include <random>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>

int main()
{
   const int total_points_arg = 10000000;
   constexpr int threads_num = 4;

   std::atomic<size_t> total_points_in(0);
   const auto run = [&total_points_in](const int points){
      int points_in = 0;
      std::random_device random_device;
      std::mt19937 gen(random_device());
      std::uniform_real_distribution<> dis(0.0, 1.0);

      for (int i = 0; i < points; i++)
      {
         const double x_value = dis(gen);
         const double y_value = dis(gen);
         const double distance = std::sqrt((x_value * x_value) + (y_value * y_value));

         if (distance <= 1) {
            points_in++;
         }
      }
      total_points_in.fetch_add(points_in, std::memory_order_relaxed);
   };

   const auto start = std::chrono::high_resolution_clock::now();

   std::thread threads[threads_num];
   for (auto &thread : threads) {
      thread = std::thread(run, total_points_arg / threads_num);
   }

   for (auto &thread : threads) {
      thread.join();
   }

   const auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> duration = end - start;

   const auto pi = 4. * total_points_in.load(std::memory_order_acquire) / total_points_arg;
   std::cout << "The value of Pi is: " << std::setprecision(15) << pi << std::endl;;
   std::cout << "Total elapsed time: " << std::setprecision(10) << duration.count() << " s" << std::endl;;

   return 0;
}