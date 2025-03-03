#include <gtest/gtest.h>
#include "sdformat_parser.h"
#include <cstdlib>
#include <cstring>
#include <limits.h>

/**
 * @file test_sdformat_parser.cpp
 * @brief Unit tests for the SDFormatParser class.
 */
 
 /// \brief Unit test class for testing the SDFormatParser class.
 class SDFormatParserTest : public ::testing::Test {
 protected:
 std::shared_ptr<SDFormatParserI> sdformatParser;
 };
 
 TEST_F(SDFormatParserTest, AbsolutePathInitialization) {
    bool sdformat_parser_initialization_successful = false;

    // Provide an example absolute filepath
    const char* relative_path = "../example_models/SuperSimpleModel/model.sdf";
    char absolute_path[PATH_MAX];
    ASSERT_TRUE(realpath(relative_path, absolute_path) != nullptr);

    sdformatParser = std::make_shared<SDFormatParser>();

    sdformatParser->Initialize(absolute_path, sdformat_parser_initialization_successful);

    EXPECT_TRUE(sdformat_parser_initialization_successful);

    sdformatParser.reset();
 }

 TEST_F(SDFormatParserTest, RelativePathInitialization) {
    bool sdformat_parser_initialization_successful = false;

    // Provide an example relative filepath
    const char* relative_path = "../example_models/SuperSimpleModel/model.sdf";

    sdformatParser = std::make_shared<SDFormatParser>();

    sdformatParser->Initialize(relative_path, sdformat_parser_initialization_successful);

    EXPECT_TRUE(sdformat_parser_initialization_successful);

    sdformatParser.reset();
 }
 