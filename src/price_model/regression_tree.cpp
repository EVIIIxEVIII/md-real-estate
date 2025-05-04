#include <Eigen/Dense>
#include <limits>
#include <memory>
#include <numeric>

#include "regression_tree.hpp"

namespace price_model {

Eigen::VectorXd RegressionTree:: predict(const Eigen::MatrixXd& X) const
{
    Eigen::VectorXd prediction(X.rows());

    for (int i = 0; i < X.rows(); i++) {
        Node* current_node = this->root.get();
        while (!current_node->is_leaf) {
            if (X(i, current_node->feature_index) < current_node->threshold) {
                current_node = current_node->left.get();
            } else {
                current_node = current_node->right.get();
            }
        }

        prediction(i) = current_node->value;
    }

    return prediction;
}


void RegressionTree::train(
    const Eigen::MatrixXd& X,
    const Eigen::VectorXd& y,
    const TrainConfig& config
) {
    fit(X, y, this->root, 0, config);
}

void RegressionTree::fit(
    const Eigen::MatrixXd& X,
    const Eigen::VectorXd& y,
    std::unique_ptr<Node>& node_ptr,
    int depth,
    const TrainConfig& config
) {
    if (depth >= config.max_depth || X.rows() < config.min_dataset_size) {
        node_ptr = std::make_unique<Node>(Node{
            .value = y.mean(),
            .is_leaf = true,
        });

        return;
    };

    const int F = X.cols();

    TreeCandidate best_candidate = {
        .ssr            = std::numeric_limits<double>::infinity(),
        .feature_index  = -1,
        .threshold      = std::numeric_limits<double>::infinity(),
    };

    for (int i = 0; i < F; ++i) {
        auto candidate = find_min_ssr_split(X.col(i), y);
        candidate.feature_index = i;

        if (candidate.ssr < best_candidate.ssr) best_candidate = std::move(candidate);
    }

    if (best_candidate.feature_index == -1) {
        node_ptr = std::make_unique<Node>(Node{
            .value   = y.mean(),
            .is_leaf = true
        });
        return;
    }

    node_ptr = std::make_unique<Node>(Node{
        .feature_index = best_candidate.feature_index,
        .threshold     = best_candidate.threshold,
        .is_leaf       = false
    });

    double t = best_candidate.threshold;
    int f = best_candidate.feature_index;

    std::vector<int> left_idx, right_idx;
    left_idx.reserve(X.rows());
    right_idx.reserve(X.rows());

    for (int i = 0; i < X.rows(); i++)
        (X(i, f) < t ? left_idx : right_idx).push_back(i);

    if (left_idx.empty() || right_idx.empty()) {
        node_ptr = std::make_unique<Node>(Node{
            .value   = y.mean(),
            .is_leaf = true
        });
        return;
    }

    Eigen::MatrixXd X_left(left_idx.size(), F);
    Eigen::VectorXd y_left(left_idx.size());

    Eigen::MatrixXd X_right(right_idx.size(), F);
    Eigen::VectorXd y_right(right_idx.size());

    for (int i = 0; i < (int)left_idx.size(); i++) {
        X_left.row(i) = X.row(left_idx[i]);
        y_left(i)     = y(left_idx[i]);
    }

    for (int i = 0; i < (int)right_idx.size(); i++) {
        X_right.row(i) = X.row(right_idx[i]);
        y_right(i)     = y(right_idx[i]);
    }

    fit(X_left, y_left, node_ptr->left, depth+1, config);
    fit(X_right, y_right, node_ptr->right, depth+1, config);
}


RegressionTree::TreeCandidate RegressionTree::find_min_ssr_split(
    const Eigen::VectorXd& feature,
    const Eigen::VectorXd& y
) {
    int n = feature.size();
    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(indices.begin(), indices.end(),
              [&](int i, int j) { return feature[i] < feature[j]; });

    double total_sum = y.sum();
    double total_sq_sum = y.squaredNorm();

    double left_sum = 0.0;
    double left_sq_sum = 0.0;

    int left_count = 0;

    double best_ssr = std::numeric_limits<double>::infinity();
    double best_threshold = 0.0;

    for (int i = 1; i < n; i++) {
        int index = indices[i - 1];
        double y_val = y[index];

        left_sum += y_val;
        left_sq_sum += y_val * y_val;
        ++left_count;

        if (feature[indices[i]] == feature[indices[i - 1]]) continue;

        double right_sum = total_sum - left_sum;
        double right_sq_sum = total_sq_sum - left_sq_sum;
        int right_count = n - left_count;

        double left_mean = left_sum / left_count;
        double right_mean = right_sum / right_count;

        double ssr_left = left_sq_sum - 2 * left_mean * left_sum + left_count * left_mean * left_mean;
        double ssr_right = right_sq_sum - 2 * right_mean * right_sum + right_count * right_mean * right_mean;
        double total_ssr = ssr_left + ssr_right;

        if (total_ssr < best_ssr) {
            best_ssr = total_ssr;
            best_threshold = 0.5 * (feature[indices[i]] + feature[indices[i - 1]]);
        }
    }

    if (best_ssr == std::numeric_limits<double>::infinity())
	    return { best_ssr, -1, std::numeric_limits<double>::quiet_NaN() };

    return TreeCandidate {
        .ssr = best_ssr,
        .feature_index = -1,
        .threshold = best_threshold
    };
}



}
