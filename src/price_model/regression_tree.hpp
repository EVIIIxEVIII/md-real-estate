#include <Eigen/Dense>
#include <memory>

namespace price_model {

class RegressionTree {

public:
    struct TrainConfig {
        int max_depth;
        int min_dataset_size;
    };

    Eigen::VectorXd predict(const Eigen::MatrixXd& X) const;
    void train(const Eigen::MatrixXd& X, const Eigen::VectorXd& y, const TrainConfig& config);

private:
    struct Node {
        int feature_index;
        double threshold;
        double value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        bool is_leaf;
    };

    struct TreeCandidate {
        double ssr;
        int feature_index;
        double threshold;

        bool operator<(const TreeCandidate& other) const {
            return ssr < other.ssr;
        }
    };

    void fit(const Eigen::MatrixXd& X, const Eigen::VectorXd& y, std::unique_ptr<Node>& node_ptr, int depth, const TrainConfig& config);
    TreeCandidate find_min_ssr_split(const Eigen::VectorXd& feature, const Eigen::VectorXd& y);
    std::unique_ptr<Node> root;
};


}
