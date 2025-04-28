#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cstdint>

struct Color {
	int red;
	int green;
	int blue;

	Color() : red(0), green(0), blue(0) {}
	Color(int redd, int greenn, int bluee) : red(redd), green(greenn), blue(bluee) {}
};


void one_Blur(const std::vector<std::vector<Color>>& imag, std::vector<std::vector<Color>>& new_imag, int first, int last) {
    int row = imag.size();
    int col = imag[0].size();
    int step = 5;
    int half = step / 2;
    for (int i = first; i < last; i++) {
        for (int j = 0; j < col; j++) {
            long int sum_red = 0;
            long int sum_green = 0;
            long int sum_blue = 0;
            int count = 0;
            for (int minus_i = -half; minus_i <= half; minus_i++) {
                for (int minus_j = -half; minus_j <= half; minus_j++) {
                    int new_row = i + minus_i;
                    int new_col = j + minus_j;
                    if (new_row >= 0 && new_col >= 0 && new_row < row && new_col < col) {
                        sum_red += imag[new_row][new_col].red;
                        sum_green += imag[new_row][new_col].green;
                        sum_blue += imag[new_row][new_col].blue;
                        count++;
                    }
                }
            }
            if (count != 0) {
                new_imag[i][j].red = (sum_red + count / 2) / count;
                new_imag[i][j].green = (sum_green + count / 2) / count;
                new_imag[i][j].blue = (sum_blue + count / 2) / count;
            }
        }
    }
}

std::vector<std::vector<Color>> parallellBlurThreads(const std::vector<std::vector<Color>>& imag, int threads_count) {
    int row = imag.size();
    int col = imag[0].size();
    std::vector<std::vector<Color>> new_imag(row, std::vector<Color>(col));
    std::vector<std::thread> threads;
    int rows_thread = (row + threads_count - 1) / threads_count;
    for (int i = 0; i < threads_count; i++) {
        int first_row = i * rows_thread;
        int last_row = std::min(first_row + rows_thread, row);
        if (i == threads_count - 1) {
            last_row = row;
        }
        else {
            last_row = first_row + rows_thread;
        }
        threads.push_back(std::thread(one_Blur, std::cref(imag), std::ref(new_imag), first_row, last_row));
    }
    for (auto& thr : threads) {
        thr.join();
    }
    return new_imag;
}


int main() {
    std::vector<std::vector<Color>> large_image(100, std::vector <Color>(100)); //100*100
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 100; j++) {
            large_image[i][j] = Color(255, 255, 0);
        }
    }
    for (int i = 25; i < 50; i++) {
        for (int j = 0; j < 100; j++) {
            large_image[i][j] = Color(0, 255, 255);
        }
    }
    for (int i = 50; i < 75; i++) {
        for (int j = 0; j < 100; j++) {
            large_image[i][j] = Color(255, 0, 255);
        }
    }
    for (int i = 75; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            large_image[i][j] = Color(255, 255, 255);
        }
    }
    auto starting = std::chrono::high_resolution_clock::now();
    auto bluring = parallellBlurThreads(large_image, 5);
    auto ending = std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> timin = ending - starting;
    std::cout << "time for 5 threads: " << timin.count() << "\n";

    auto starting2 = std::chrono::high_resolution_clock::now();
    auto bluring2 = parallellBlurThreads(large_image, 10);
    auto ending2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> timin2 = ending2 - starting2;
    std::cout << "time for 10 threads: " << timin2.count() << "\n";
    return 0;
}