#include <cassert>
#include <iostream>
#include <fmt/core.h>
#include <csv.h>
#include <Eigen/Dense>
#include <cuda_runtime.h>

int main() {

    io::CSVReader<3> in("./data/datasets/final_data.csv");
    in.read_header(io::ignore_extra_column, "number_of_rooms", "total_area", "price");

    const int features = 2;
    size_t row_count = 0;
    std::vector<std::array<float, 3>> data;
    float price, total_area, number_of_rooms;

    while(in.read_row(number_of_rooms, total_area, price)) {
        row_count++;
        data.push_back(std::array<float, 3>({ number_of_rooms, total_area, price }));
    }


    Eigen::MatrixXd mat(row_count, features);
    Eigen::VectorXd y(row_count);

    for (int i = 0; i < row_count; i++) {
        Eigen::Vector2d row;
        row << data[i][0], data[i][1];

        mat.row(i) = row;
        y(i) = data[i][2];
    }

    for (int i = 0; i < 10; i++) {
        std::cout << mat.row(i);
        std::cout << " " << y(i) << "\n";
    }

    return 0;
}
