#include <gtest/gtest.h>
#include "sdformat_parser.h"

/**
 * @file test_gui.cpp
 * @brief Unit tests for the GUI class.
 */
 
 /// \brief Unit test class for testing the GUI class.
 class SDFormatParserTest : public ::testing::Test {
 protected:
     SDFormatParserI* sdformat_parser;
     bool sdformat_parser_initialization_successful = false;
    
     void SetUp() override {
        sdformat_parser = new SDFormatParser("", sdformat_parser_initialization_successful);
     }
 
     void TearDown() override {
         delete sdformat_parser;
     }
 };
 
 TEST_F(SDFormatParserTest, Initialization) {
    EXPECT_TRUE(sdformat_parser_initialization_successful);
 }
 