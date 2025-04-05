#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "../include/env_loader.h"
}

class EnvLoaderTest : public ::testing::Test {
 protected:
  const char *test_filename = "test.env";

  void writeToFile(const char *content) {
    FILE *file = fopen(test_filename, "w");
    fprintf(file, "%s", content);
    fclose(file);
  }

  void TearDown() override {
    remove(test_filename);
    unsetenv("KEY1");
    unsetenv("KEY2");
    unsetenv("KEY_WITH_EQUALS");
    unsetenv("ONLYKEY");
    unsetenv("ONLYVALUE");
    unsetenv("EXISTING_KEY");
  }
};

TEST_F(EnvLoaderTest, LoadsEnvironmentVariables) {
  writeToFile("KEY1=value1\nKEY2=value2\n");
  load_env(test_filename);
  EXPECT_STREQ(getenv("KEY1"), "value1");
  EXPECT_STREQ(getenv("KEY2"), "value2");
}

TEST_F(EnvLoaderTest, IgnoresMissingFile) {
  remove(test_filename);
  load_env(test_filename);
  SUCCEED();
}

TEST_F(EnvLoaderTest, IgnoresCommentsAndEmptyLines) {
  writeToFile("# comment\n\nKEY1=value1\n# another comment\nKEY2=value2\n");
  load_env(test_filename);
  EXPECT_STREQ(getenv("KEY1"), "value1");
  EXPECT_STREQ(getenv("KEY2"), "value2");
}

TEST_F(EnvLoaderTest, SkipsLinesWithoutEquals) {
  writeToFile("KEY1value1\nKEY2=value2\n");
  load_env(test_filename);
  EXPECT_EQ(getenv("KEY1"), nullptr);
  EXPECT_STREQ(getenv("KEY2"), "value2");
}

TEST_F(EnvLoaderTest, HandlesMissingKeyOrValue) {
  writeToFile("ONLYKEY=\n=ONLYVALUE\nKEY1=value1\n");
  load_env(test_filename);
  EXPECT_EQ(getenv("ONLYVALUE"), nullptr);
  EXPECT_STREQ(getenv("ONLYKEY"), "");
  EXPECT_STREQ(getenv("KEY1"), "value1");
}

TEST_F(EnvLoaderTest, HandlesEqualsInValue) {
  writeToFile("KEY_WITH_EQUALS=value=with=equals\n");
  load_env(test_filename);
  EXPECT_STREQ(getenv("KEY_WITH_EQUALS"), "value=with=equals");
}

TEST_F(EnvLoaderTest, OverridesExistingEnvVariable) {
  setenv("EXISTING_KEY", "old_value", 1);
  writeToFile("EXISTING_KEY=new_value\n");
  load_env(test_filename);
  EXPECT_STREQ(getenv("EXISTING_KEY"), "new_value");
}
