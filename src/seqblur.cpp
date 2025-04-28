#include <iostream>
#include <vector>
#include <chrono>

struct Color {
	int red;
	int green;
	int blue;

	Color(int redd, int greenn, int bluee) : red(redd), green(greenn), blue(bluee) {}
	Color() : red(0), green(0), blue(0) {}
};

std::vector<std::vector<Color>> sequentialBlur(std::vector<std::vector<Color>>& imag) {
	if (imag.empty()) return {};
	int row = imag.size();
	int col = imag[0].size();
	std::vector<std::vector<Color>> new_imag(row, std::vector<Color>(col));
	int step = 5;
	for (int i = 0; i < row;i++) {
		for (int j = 0; j < col;j++) {
			long int sum_red = 0;
			long int sum_green = 0;
			long int sum_blue = 0;
			int count = 0;
			for (int minus_i = -step / 2; minus_i <= step / 2;minus_i++) {
				for (int minus_j = -step / 2; minus_j <= step / 2;minus_j++) {
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
	return new_imag;
}


int main() {
	std::vector<std::vector<Color>> large_image(100, std::vector<Color>(100));
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
	auto start_large_im = std::chrono::high_resolution_clock::now();
	auto blur_large = sequentialBlur(large_image);
	auto end_large_im = std::chrono::high_resolution_clock::now();
	std::chrono::duration <double> larg_im = end_large_im - start_large_im;
	std::cout << "large image time: " << larg_im.count() << "\n";
	return 0;
}