// #include <gtest/gtest.h>
// #include "gui.h"
// #include "sdformat_parser.h"

// /**
//  * @file test_gui.cpp
//  * @brief Unit tests for the GUI class.
//  */
 
//  /// \brief Unit test class for testing the GUI class.
//  class GUITest : public ::testing::Test {
//  protected:
//      std::shared_ptr<GUII> gui;
//      std::shared_ptr<SDFormatParserI> sdformatParser;
//      bool gui_initialization_successful = false;
    
//      void SetUp() override {

//         sdformatParser = std::make_shared<SDFormatParser>();

//         gui = std::make_shared<GUI>("SDFormat Editor", sdformatParser, gui_initialization_successful);
//      }
 
//  };
 
//  TEST_F(GUITest, Initialization) {
//      ASSERT_TRUE(gui_initialization_successful);
//      EXPECT_FALSE(gui->ShouldClose());
//  }
 