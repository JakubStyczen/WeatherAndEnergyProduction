#include "gmock/gmock.h"
#include "gtest/gtest.h"

extern "C" {
#include "../include/sections.h"
#include "../include/weather_condition.h"
}

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class WeatherMock {
 public:
  static void mock_fetch(const char *url, WeatherData *data) {
    data->temperature = 20.5;
    data->wind_speed = 5.3;
    data->cloudiness = 50;
  }

  static char *mock_build_url(const char *base_url, double lat, double lon,
                              const char *units, const char *api_key) {
    return strdup("http://mocked-url");
  }
};

TEST(WeatherConditionTest, DownloadsWeatherDataSuccessfully) {
  GeoLoc geoArray[1] = {{"Warsaw", 52.23, 21.01}};
  WeatherData weatherArray[1];
  UrlData url_data = {
      .base_url = "http://example.com?", .api_key = "abc", .units = "metric"};

  WeatherProvider mock_provider = {
      .fetch_weather_data = WeatherMock::mock_fetch,
      .build_url = WeatherMock::mock_build_url};

  int result = download_temperature_data_sections_mockable(
      geoArray, weatherArray, url_data, 0, 1, &mock_provider);

  EXPECT_EQ(result, 0);
  EXPECT_EQ(weatherArray[0].temperature, 20.5);
  EXPECT_EQ(weatherArray[0].wind_speed, 5.3);
  EXPECT_EQ(weatherArray[0].cloudiness, 50);
  EXPECT_STREQ(weatherArray[0].cities, "Warsaw");
}