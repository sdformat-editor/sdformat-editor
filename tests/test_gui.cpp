#include <gtest/gtest.h>
#include "gui.h"

/**
 * @file test_gui.cpp
 * @brief Unit tests for the GUI class.
 */
 
 /// \brief Unit test class for testing the GUI class.
 class GUITest : public ::testing::Test {
 protected:
     GUII* gui;
     bool gui_initialization_successful;
    
     void SetUp() override {
         gui = new GUI("Test Window", gui_initialization_successful);
     }
 
     void TearDown() override {
         delete gui;
     }
 };
 
 TEST_F(GUITest, Initialization) {
     ASSERT_TRUE(gui_initialization_successful);
     EXPECT_FALSE(gui->ShouldClose());
 }
 