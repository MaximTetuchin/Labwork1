# Lab Report #1

**Topic:** Multithreaded Image Processing
**Author:** Maxim Tetuchin
**Year:** 2025
**Version:** 1.0

## Objective

The goal of this lab work is to develop a multithreaded version of an image processing program, which performs rotation and Gaussian blur on multiple images. The main aim is to accelerate processing by performing operations concurrently on separate threads.

## Implementation Details

1. **Multithreading**

   * Each image file is processed in a separate thread.

2. **Image Processing Steps**

   * Read the image from disk (`readImage()`).
   * Convert to a matrix for processing (`vecToMat()`).
   * Rotate the image (`matRotate()`).
   * Convert back to vector format (`matToVec()`).
   * Save the rotated image (`saveToRaw()`).
   * Apply Gaussian blur (`gauss(9)`), convert back, and save the result.

3. **Program Architecture**

   * A vector of threads `std::vector<std::thread>` is used to execute tasks concurrently.
   * A vector of `std::unique_ptr<image>` stores image objects with unique ownership.
   * Each thread receives an image object using `std::move()`, avoiding copying of large data.

## Testing Results

* 8 images of the same size were used for testing.
* Performance was compared between single-threaded and multithreaded versions on an 4-core CPU.

**Execution Time:**

| Program Version           | Average Time for 8 Images |
| ------------------------- | ------------------------- |
| Single-threaded           | ~12.0 seconds              |
| Multithreaded (4 threads) | ~3.0 seconds               |

**Conclusion:**
The multithreaded version achieved an average **4× speedup** compared to the single-threaded version. This is because image processing for each file runs independently on separate CPU cores.

## Conclusion

Multithreaded image processing significantly speeds up tasks that can be executed independently. Using an atomic counter and unique pointers ensures proper ID management and safe object ownership in a multithreaded environment.
