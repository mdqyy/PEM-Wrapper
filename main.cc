#include "pem.h"
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace mp;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, char **argv) {

    const string& opt_help = "help";
    const string& opt_predict = "predict";
    const string& opt_train = "train";
    const string& opt_pem = "pem";
    const string& opt_score = "score";

    po::options_description desc("Allowed options");
    desc.add_options()
        ((opt_help + ",h").c_str(), "produce help messages")
        ((opt_predict + ",p").c_str(), po::value<std::vector<string> >(), 
         "predict the units (-p pair1-image1 pair1-image2 pair2-image1 pair2-image2 ...)")
        ((opt_train + ",t").c_str(), po::value<std::string>(), "train from the config (-t config_path)")
        ((opt_pem + ",m").c_str(), po::value<std::string>(), "specify pem path (-m pem_path)")
        ((opt_score + ",s").c_str(), po::value<std::string>(), "specify predicted scores path (-s predicted_score_path)")
    ;

    po::positional_options_description p;
    p.add(opt_predict.c_str(), -1);

    try {

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), 
                vm);
        po::notify(vm);

        if (vm.count(opt_pem) && vm.count(opt_score) && (vm.count(opt_train) || vm.count(opt_predict))) {
            string pem_path = vm[opt_pem].as<string>();
            string predicted_score_path = vm[opt_score].as<string>();

            if (vm.count(opt_train)) {
                if (fs::exists(pem_path)) {
                    std::cerr << "Err: " << pem_path << " existed: make sure you are not override it..." << std::endl;
                } else {
                    string config_path = vm[opt_train].as<string>();
                    pem_wrapper wrapper;
                    wrapper.train(config_path.c_str(), pem_path.c_str(), predicted_score_path.c_str());
                }
            } else if (vm.count(opt_predict)) {
                vector<string> testing_units = vm[opt_predict].as<vector<string> >();
                if (testing_units.size() > 1) {
                    pem_wrapper wrapper;
                    vector<std::pair<int, double> > labels = wrapper.test(pem_path.c_str(), testing_units, predicted_score_path.c_str());
                    cout << "Prediction: " << endl;
                    for (long n = 0; n < testing_units.size()/2; n++) {
                        cout << testing_units[2*n] << " "
                            << testing_units[2*n+1] << " : " 
                            << labels[n].second << " (" 
                            << (labels[n].first == 1 ? "same" : "different") 
                            << ")" << endl;
                    }
                } else {
                    cout << "not enough arguments for -p" << endl;
                }
            }
        } else {
            cout << desc << endl;
            cout << "Possible combinations: \n";
            cout << "./main -m pem-path -s score-path -t config-path" << endl;
            cout << "./main -m pem-path -s score-path -p image-path1 image-path2 ..." << endl;
            exit(-1);
        }
    } catch (std::exception& e) {
        cout << "Exception: " << e.what() << endl;
        return -1;
    }

    return 0;
}
