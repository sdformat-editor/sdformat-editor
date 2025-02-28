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
        bool model_viewer_initialization_successful = false;
       
        void SetUp() override {
            model_viewer = new ModelViewer("Test Window", model_viewer_initialization_successful);
        }
    
        void TearDown() override {
            delete model_viewer;
        }
};

TEST_F(ModelViewerTest, Initialization) {
    ASSERT_TRUE(model_viewer_initialization_successful);
}