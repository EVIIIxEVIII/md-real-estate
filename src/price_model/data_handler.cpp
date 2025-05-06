#include "data_handler.hpp"

namespace price_model {

SplitData DataHandler::get_split_data(float training_ratio)
{
	const std::size_t total_size = loaded_data.X_rows.size();
	const std::size_t train_size = std::round(total_size * training_ratio);
	const std::size_t test_size  = total_size - train_size;

	assert(train_size && "empty or too small CSV?");

	const std::size_t num_features = F;

	Eigen::MatrixXd X_train(train_size, num_features);
	Eigen::VectorXd Y_train(train_size);
	for (std::size_t i = 0; i < train_size; ++i) {
		for (std::size_t j = 0; j < F; ++j)
			X_train(i, j) = loaded_data.X_rows[i][j];

		Y_train(i) = loaded_data.y_vals[i];
	}

	Eigen::MatrixXd X_test(test_size, num_features);
	Eigen::VectorXd Y_test(test_size);
	for (std::size_t i = 0; i < test_size; ++i) {
		const std::size_t data_idx = train_size + i;

		for (std::size_t j = 0; j < F; ++j)
			X_test(i, j) = loaded_data.X_rows[data_idx][j];

		Y_test(i) = loaded_data.y_vals[data_idx];
	}

	return SplitData{
		X_train,
		Y_train,
		X_test,
		Y_test
	};
}

template<std::size_t... Is>
bool DataHandler::read_row_array(io::CSVReader<F + 1>& in,
                    std::array<double, F>&  x,
                    double&                 y,
                    std::index_sequence<Is...>)
{
    return in.read_row(x[Is]..., y);
}

void DataHandler::load_data(std::string data_path) {
    io::CSVReader<F + 1> in(data_path);

    [&]<std::size_t... Is>(std::index_sequence<Is...>)
    {
        in.read_header(io::ignore_extra_column, feature[Is]..., "price");
    }(std::make_index_sequence<F>{});

    using Row = std::array<double, F>;
    std::vector<Row> X_rows;
    std::vector<double>  y_vals;

    Row    x{};
    double y = 0.;

    while (read_row_array(in, x, y, std::make_index_sequence<F>{})) {
        X_rows.push_back(x);
        y_vals.push_back(y);
    }

    loaded_data = LoadedData<F> {
        X_rows,
        y_vals
    };
}

}
