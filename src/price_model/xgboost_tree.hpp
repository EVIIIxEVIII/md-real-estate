#include <Eigen/Dense>
#include <memory>

namespace price_model {

struct XGBoostConfig {
    int n_estimators = 100;
    double eta = 0.3;
    struct XGBoostTreeConfig {
        double lambda = 1.0;
        double gamma = 0.0;
        int max_depth = 6;
    } tree_config;
};

class XGBoostTree {

public:
    XGBoostTree(const XGBoostConfig::XGBoostTreeConfig& config): _config(config) {};
    Eigen::VectorXd predict(const Eigen::MatrixXd& X) const;
    void train( const Eigen::MatrixXd& X, const Eigen::VectorXd& y);

    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);

private:
    struct Node {
        int feature_index;
        double threshold;
        double value;
        double gain;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        int count;
        bool is_leaf;
    };

    struct TreeCandidate {
        double gain;
        int feature_index;
        double threshold;

        bool operator<(const TreeCandidate& other) const {
            return gain < other.gain;
        }
    };

    struct SplitData {
        Eigen::MatrixXd X_right;
        Eigen::MatrixXd X_left;

        Eigen::VectorXd y_right;
        Eigen::VectorXd y_left;
    };

    const XGBoostConfig::XGBoostTreeConfig& _config;
    std::unique_ptr<Node> _root;

    void fit(
        const Eigen::MatrixXd& X,
        const Eigen::VectorXd& y,
        std::unique_ptr<Node>& node_ptr,
        int depth
    );

    void serialize_node(std::ostream& out, const Node* node) const;
    std::unique_ptr<Node> deserialize_node(std::istream& in) const;

    bool prune(std::unique_ptr<Node>& node);
    std::unique_ptr<XGBoostTree::SplitData> split_data(const Eigen::MatrixXd& X, const Eigen::VectorXd& y, double t, int f);
    double compute_value(const Eigen::VectorXd& y);
    TreeCandidate find_max_gain_split(const Eigen::VectorXd& feature, const Eigen::VectorXd& y);
};

}
