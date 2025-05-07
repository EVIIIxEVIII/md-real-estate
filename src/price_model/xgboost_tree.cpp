#include <Eigen/Dense>
#include <limits>
#include <memory>
#include <numeric>

#include "xgboost_tree.hpp"

namespace price_model {

Eigen::VectorXd XGBoostTree:: predict(const Eigen::MatrixXd& X) const
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

void XGBoostTree::train(
    const Eigen::MatrixXd& X,
    const Eigen::VectorXd& y
) {
    fit(X, y, this->root, 0);
    prune(this->root);
}

bool XGBoostTree::prune(std::unique_ptr<Node>& node)
{
    if (!node || node->is_leaf) return true;   // already a leaf

    bool left_is_leaf  = prune(node->left);
    bool right_is_leaf = prune(node->right);

    if (left_is_leaf && right_is_leaf && node->gain <= config.gamma) {
        int    nL = node->left->count;
        int    nR = node->right->count;
        double wL = node->left->value;
        double wR = node->right->value;

        double sumL = wL * (nL + config.lambda);
        double sumR = wR * (nR + config.lambda);

        int    n    = nL + nR;
        double w    = (sumL + sumR) / (n + config.lambda);

        node = std::make_unique<Node>(Node{
            .value   = w,
            .gain    = 0.0,
            .count   = n,
            .is_leaf = true
        });
        return true;
    }
    return false;
}

double XGBoostTree::compute_value(const Eigen::VectorXd& y) {
    return y.sum() / (y.size() + config.lambda);
}

std::unique_ptr<XGBoostTree::SplitData> XGBoostTree::split_data(
    const Eigen::MatrixXd& X,
    const Eigen::VectorXd& y,
    double t,
    int f
) {
    std::vector<int> left_idx, right_idx;
    left_idx.reserve(X.rows());
    right_idx.reserve(X.rows());

    for (int i = 0; i < X.rows(); i++) {
        (X(i, f) < t ? left_idx : right_idx).push_back(i);
    }

    if (left_idx.empty() || right_idx.empty()) {
        return nullptr;
    }

    Eigen::MatrixXd X_left(left_idx.size(), X.cols());
    Eigen::VectorXd y_left(left_idx.size());

    Eigen::MatrixXd X_right(right_idx.size(), X.cols());
    Eigen::VectorXd y_right(right_idx.size());

    for (int i = 0; i < (int)left_idx.size(); i++) {
        X_left.row(i) = X.row(left_idx[i]);
        y_left(i)     = y(left_idx[i]);
    }

    for (int i = 0; i < (int)right_idx.size(); i++) {
        X_right.row(i) = X.row(right_idx[i]);
        y_right(i)     = y(right_idx[i]);
    }

    return std::make_unique<SplitData>(SplitData{
        .X_right = X_right,
        .X_left  = X_left,
        .y_right = y_right,
        .y_left  = y_left
    });
}

void XGBoostTree::fit(
    const Eigen::MatrixXd& X,
    const Eigen::VectorXd& y,
    std::unique_ptr<Node>& node_ptr,
    int depth
) {
    if (depth >= config.max_depth) {
        node_ptr = std::make_unique<Node>(Node{
            .value = compute_value(y),
            .count = static_cast<int>(y.size()),
            .is_leaf = true,
        });
        return;
    };

    const int F = X.cols();
    TreeCandidate best_candidate = {
        .gain            = -std::numeric_limits<double>::infinity(),
        .feature_index  = -1,
        .threshold      = std::numeric_limits<double>::infinity(),
    };

    for (int i = 0; i < F; ++i) {
        auto candidate = find_max_gain_split(X.col(i), y);
        candidate.feature_index = i;

        if (candidate.gain > best_candidate.gain) best_candidate = std::move(candidate);
    }

    const double leaf_gain = (y.sum() * y.sum()) / (y.size() + config.lambda);
    if (best_candidate.feature_index == -1
        || best_candidate.gain <= config.gamma
    ) {
        node_ptr = std::make_unique<Node>(Node{
            .value   = compute_value(y),
            .gain    = leaf_gain,
            .count   = static_cast<int>(y.size()),
            .is_leaf = true
        });
        return;
    }

    node_ptr = std::make_unique<Node>(Node{
        .feature_index = best_candidate.feature_index,
        .threshold     = best_candidate.threshold,
        .gain          = best_candidate.gain,
        .is_leaf       = false
    });

    double t = best_candidate.threshold;
    int f = best_candidate.feature_index;

    auto data = split_data(X, y, t, f);

    if (data == nullptr) {
        node_ptr = std::make_unique<Node>(Node{
            .value   = compute_value(y),
            .gain    = leaf_gain,
            .count   = static_cast<int>(y.size()),
            .is_leaf = true
        });
        return;
    }

    fit(data->X_left, data->y_left, node_ptr->left, depth+1);
    fit(data->X_right, data->y_right, node_ptr->right, depth+1);
}


XGBoostTree::TreeCandidate XGBoostTree::find_max_gain_split(
    const Eigen::VectorXd& feature,
    const Eigen::VectorXd& y
) {
    int n = feature.size();
    std::vector<int> indices(n);
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(indices.begin(), indices.end(),
              [&](int i, int j) { return feature[i] < feature[j]; });

    double total_sum = y.sum();
    double left_sum = 0.0;
    int left_count = 0;

    double best_gain = -std::numeric_limits<double>::infinity();
    double best_threshold = 0.0;

    double parent_gain = (y.sum() * y.sum()) / (y.size() + config.lambda);

    for (int i = 1; i < n; i++) {
        int index = indices[i - 1];
        double y_val = y[index];

        left_sum += y_val;
        ++left_count;

        if (feature[indices[i]] == feature[indices[i - 1]]) continue;

        double right_sum = total_sum - left_sum;
        int right_count = n - left_count;

        double gain_left = (left_sum * left_sum) / (left_count + config.lambda);
        double gain_right = (right_sum * right_sum) / (right_count + config.lambda);

        double total_gain = gain_left + gain_right - parent_gain;

        if (total_gain > best_gain) {
            best_gain = total_gain;
            best_threshold = 0.5 * (feature[indices[i]] + feature[indices[i - 1]]);
        }
    }

    if (best_gain == -std::numeric_limits<double>::infinity())
	    return { best_gain, -1, std::numeric_limits<double>::quiet_NaN() };

    return TreeCandidate {
        .gain = best_gain,
        .feature_index = -1,
        .threshold = best_threshold,
    };
}


}
