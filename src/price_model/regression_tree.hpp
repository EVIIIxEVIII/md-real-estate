#include <Eigen/Dense>
#include <memory>

namespace price_model {

class RegressionTree {

public:
    void train(const Eigen::MatrixXd& X, const Eigen::VectorXd& y);
    Eigen::VectorXd predict(const Eigen::MatrixXd& X) const;

    struct TreeCandidate {
        double ssr;
        int feature_index;
        double threshold;

        bool operator<(const TreeCandidate& other) const {
            return ssr < other.ssr;
        }
    };

private:
    struct Node {
        int feature_index;
        double threshold;
        double value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        bool is_leaf;
    };

    struct TrainConfig {
        int max_depth;
        int min_dataset_size;
    };

    void train(const Eigen::MatrixXd& X, const Eigen::VectorXd& y, const TrainConfig& config);
    void fit(const Eigen::MatrixXd& X, const Eigen::VectorXd& y, std::unique_ptr<Node>& node_ptr, int depth, const TrainConfig& config);
    TreeCandidate find_min_ssr_split(const Eigen::VectorXd& feature, const Eigen::VectorXd& y);
    std::unique_ptr<Node> root;
};


}
