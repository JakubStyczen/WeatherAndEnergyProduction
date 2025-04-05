#include "gtest/gtest.h"
extern "C" {
#include "../include/sections.h"
}

#define MAX_SIZE 5

TEST(GeoSectionsTest, AllocateGeoArrayInitializesCorrectly) {
  GeoLoc array[MAX_SIZE];
  allocate_geo_array(array, MAX_SIZE);

  for (int i = 0; i < MAX_SIZE; ++i) {
    EXPECT_STREQ(array[i].cities, "");
    EXPECT_EQ(array[i].lat, 0);
    EXPECT_EQ(array[i].lon, 0);
  }
}

TEST(GeoSectionsTest, AddGeoRecordAddsCorrectData) {
  GeoLoc array[MAX_SIZE];
  allocate_geo_array(array, MAX_SIZE);

  int result = add_geo_record(array, MAX_SIZE, "Warsaw", 52.23, 21.01);
  EXPECT_EQ(result, 0);
  EXPECT_STREQ(array[0].cities, "Warsaw");
  EXPECT_DOUBLE_EQ(array[0].lat, 52.23);
  EXPECT_DOUBLE_EQ(array[0].lon, 21.01);
}

TEST(GeoSectionsTest, AddGeoRecordFailsWhenFull) {
  GeoLoc array[MAX_SIZE];
  allocate_geo_array(array, MAX_SIZE);

  for (int i = 0; i < MAX_SIZE; ++i) {
    char name[10];
    snprintf(name, sizeof(name), "City%d", i);
    add_geo_record(array, MAX_SIZE, name, 0.1 * i, 0.2 * i);
  }

  int result = add_geo_record(array, MAX_SIZE, "Overflow", 1.0, 1.0);
  EXPECT_EQ(result, -1);
}

TEST(GeoSectionsTest, LoadGeoDataFromCSVLoadsCorrectly) {
  const char *test_file = "test_geo.csv";
  FILE *file = fopen(test_file, "w");
  fprintf(file, "Berlin,52.52,13.40\nParis,48.85,2.35\n");
  fclose(file);

  GeoLoc array[MAX_SIZE];
  allocate_geo_array(array, MAX_SIZE);
  int count = load_geo_data_from_csv(test_file, array, MAX_SIZE);

  EXPECT_EQ(count, 2);
  EXPECT_STREQ(array[0].cities, "Berlin");
  EXPECT_DOUBLE_EQ(array[0].lat, 52.52);
  EXPECT_DOUBLE_EQ(array[0].lon, 13.40);
  EXPECT_STREQ(array[1].cities, "Paris");
  EXPECT_DOUBLE_EQ(array[1].lat, 48.85);
  EXPECT_DOUBLE_EQ(array[1].lon, 2.35);

  remove(test_file);  // clean up
}
