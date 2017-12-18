//
// Created by Mihika Dave on 12/18/17.
//

#ifndef META_LET_H
#define META_LET_H

#include <functional>
#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <chrono>
#include <algorithm>

#include "meta/learn/loss/all.h"
#include "meta/learn/loss/hinge.h"
#include "meta/learn/loss/loss_function.h"
#include "meta/learn/loss/loss_function_factory.h"
#include "meta/learn/sgd.h"
#include "meta/learn/instance.h"
#include "meta/learn/dataset.h"
#include "meta/classify/classifier/svm_wrapper.h"
#include "meta/classify/classifier/classifier.h"


using namespace std;
//using namespace meta;
//using namespace learn;
using namespace meta::util;
using namespace meta::classify;

using namespace meta::util;
using namespace meta::classify;
namespace meta
{
namespace learn
{
namespace ranker
{

    class let {

    public:

        using tupl = std::tuple<feature_vector, int, string>;

        enum DATA_TYPE {
            TRAINING,
            VALIDATION,
            TESTING
        };

        enum CLASSIFY_TYPE {
            LIBSVM,
            SPD,
        };

        typedef struct forwardnode {
            operator int() const {
                return label;
            }

            operator feature_vector() const {
                return fv;
            }

            int label;
            feature_vector fv;
        } forward_node;

        //constructor
    public:

        void train(string data_dir, int feature_nums, sgd_model *model);

        svm_wrapper *train_svm(string data_dir, int feature_nums, string svm_path);

        void validate(string data_dir, int feature_nums, CLASSIFY_TYPE classify_type, svm_wrapper *wrapper,
                      sgd_model *model);

        void test(string data_dir, int feature_nums, CLASSIFY_TYPE classify_type, svm_wrapper *wrapper,
                  sgd_model *model);

    private:
        void read_data(DATA_TYPE data_type, string data_dir, vector<string> *qids,
                       unordered_map<string, unordered_map<int, vector<feature_vector>>> *dataset,
                       unordered_map<string, unordered_map<int, vector<string>>> *docids,
                       unordered_map<string, unordered_map<string, int>> *relevance_map, int feature_nums);

        std::pair<tupl, tupl> getRandomPair(vector<string> *training_qids,
                                            unordered_map<string, unordered_map<int, vector<feature_vector>>> *training_dataset,
                                            int random_seed);

        void
        build_dataset_nodes(unordered_map<string, unordered_map<int, vector<feature_vector>>> *training_dataset,
                            vector<forward_node> *dataset_nodes);

        static bool compare_docscore(const pair<string, double> &p1, const pair<string, double> &p2) {
            return p1.second > p2.second;
        }

        double compute_dcg(int limit, vector<int> &rankings);

        void
        evaluate(vector<string> *qids, unordered_map<string, unordered_map<int, vector<feature_vector>>> *dataset,
                 unordered_map<string, unordered_map<int, vector<string>>> *docids,
                 unordered_map<string, unordered_map<string, int>> *relevance_map,
                 int feature_nums,
                 CLASSIFY_TYPE classify_type,
                 svm_wrapper *wrapper,
                 sgd_model *model);

    };

}
}
}
#endif //META_LET_H
