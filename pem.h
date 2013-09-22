/**
 * @file pem.h
 * @brief Wrapper for PEM
 *
 * Code for the work below: 
 * @INPROCEEDINGS {Li:CVPR2013,
 *  author={Li, Haoxiang and Hua, Gang and Lin, Zhe and Brandt, Jonathan and Yang, Jianchao},
 *  booktitle=cvpr,
 *  title={Probabilistic Elastic Matching for Pose Variant Face Verification},
 *  year=2013
 * }
 *
 * Functionalities:
 *  1. Train a PEM face "verifier" with respect to a configuration file
 *  2. Given testing image pairs, predict labels and probabilities for image pairs 
 *
 * @author Haoxiang Li
 * @version 1.0
 * @date 2013-09-13
 *
 */

#include <vector>
#include <string>

namespace mp {

    class pem_wrapper {

        public:
            /* -----------------------------------------------------*/
            /**
             * @brief Train a full PEM-pipeline from a configuration file 
             * which specified necessary parameters including:
             *
             * a) paths to training image/feature pairs
             * b) feature extraction parameter if features are not extracted beforehand
             * c) UBM-GMM training parameter (number of gaussians)
             * d) paths for intermediate models/representations
             *
             * @param config_path   [in] path to configuration file 
             * @param pem_path      [out] path to the saved full PEM-pipeline
             * @param predict_scores_path [out] path to saved predicted probabilities for training pairs
             */
            /* -----------------------------------------------------*/
            void train(
                    const char *config_path,
                    const char *pem_path,
                    const char *predict_scores_path);

            /* -----------------------------------------------------*/
            /**
             * @brief Predict labels and assign scores for testing image pairs
             *
             * @param pem_path      [in] path to the saved full PEM-pipeline
             * @param testing_pairs [in] path to testing images, every two rows are regarded as an image pair
             * @param predict_scores_path [out] path to saved predicted probabilities for testing pairs
             *
             * @return testing-labels-probs predicted labels and probabilities
             */
            /* -----------------------------------------------------*/
            std::vector<std::pair<int,double> > test(
                    const char *pem_path,
                    const std::vector<std::string>& testing_pairs,
                    const char *predict_scores_path);

    };

} //< namespace mp
