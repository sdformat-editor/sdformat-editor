#include <gtest/gtest.h>
#include "model_viewer.h"

/**
 * @file test_model_viewer.cpp
 * @brief Unit tests for the Model Viewer class.
 */
 
/// \brief Unit test class for testing the Model Viewer class.
class ModelViewerTest : public ::testing::Test {
    protected:
        ModelViewerI* model_viewer;

        void SetUp() override {
            model_viewer = new ModelViewer;
        }
       
        void TearDown() override {
            delete model_viewer;
        }
};

TEST_F(ModelViewerTest, Initialization) {
    bool model_viewer_initialization_successful = false;

    this->model_viewer->Initialize(model_viewer_initialization_successful);

    EXPECT_TRUE(model_viewer_initialization_successful);
}

TEST_F(ModelViewerTest, Updating) {

    ASSERT_NO_THROW(this->model_viewer->Update());

}
