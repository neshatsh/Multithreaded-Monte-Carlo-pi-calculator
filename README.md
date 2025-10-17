# Multithreaded Monte Carlo π Calculator

A high-performance C++ implementation that estimates the value of π (pi) using the Monte Carlo method with multithreading support.

## Overview

This program uses a probabilistic approach to estimate π by randomly sampling points in a unit square and determining what fraction falls within a quarter circle. The calculation is parallelized across multiple threads for improved performance.

### How It Works

The Monte Carlo method for estimating π works as follows:

1. Generate random points (x, y) where both coordinates are between 0 and 1
2. Check if each point falls inside a quarter circle of radius 1 centered at the origin
3. A point is inside if: x² + y² ≤ 1
4. The ratio of points inside the circle to total points approximates π/4
5. Therefore: **π ≈ 4 × (points inside circle) / (total points)**

## Features

- **Multithreaded execution** for faster computation
- Automatic CPU core detection (uses `std::thread::hardware_concurrency()`)
- Command-line argument support for customization
- Atomic operations for thread-safe counting
- Detailed output including accuracy metrics
- Clean, object-oriented design


## Example Output

```
Calculating π using Monte Carlo method...

Monte Carlo π Estimation
========================
Total points:     10000000
Points inside:    7853833
Threads used:     4
Estimated π:      3.141533200000000
Actual π:         3.141592653589793
Error:            0.000059453589793
Elapsed time:     0.440503 seconds
```

## Algorithm Complexity

- **Time Complexity**: O(n/t) where n is the number of points and t is the number of threads
- **Space Complexity**: O(t) for thread storage
- **Convergence Rate**: Error decreases as O(1/√n)


