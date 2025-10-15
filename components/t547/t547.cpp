#include "t547.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/core/helpers.h"

#include <esp32-hal-gpio.h>

namespace esphome {
namespace t547 {

static const char *const TAG = "t574";

void T547::setup() {
  ESP_LOGV(TAG, "Initialize called");
  epd_init();
  uint32_t buffer_size = this->get_buffer_length_();

  if (this->buffer_ != nullptr) {
    free(this->buffer_);  // NOLINT
  }

  this->buffer_ = (uint8_t *) ps_malloc(buffer_size);

  if (this->buffer_ == nullptr) {
    ESP_LOGE(TAG, "Could not allocate buffer for display!");
    this->mark_failed();
    return;
  }

  memset(this->buffer_, 0xFF, buffer_size);
  ESP_LOGV(TAG, "Initialize complete");
}

float T547::get_setup_priority() const { return setup_priority::PROCESSOR; }
size_t T547::get_buffer_length_() {
    return this->get_width_internal() * this->get_height_internal() / 2;
}

void T547::update() {
  this->do_update_();
  this->display();
}

void HOT T547::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (x >= this->get_width_internal() || y >= this->get_height_internal() || x < 0 || y < 0)
    return;
  uint8_t gs = 255 - ((color.red * 2126 / 10000) + (color.green * 7152 / 10000) + (color.blue * 722 / 10000));
  epd_draw_pixel(x, y, gs, this->buffer_);
}

void T547::dump_config() {
  LOG_DISPLAY("", "T547", this);
  LOG_UPDATE_INTERVAL(this);
}

void T547::eink_off_() {
  //if (panel_on_ == 0)
  //  return;
  epd_poweroff();
  ESP_LOGV(TAG, "OFF");
  panel_on_ = 0;
}

void T547::eink_off_all() {
  //if (panel_on_ == 0)
  //  return;
  epd_poweroff_all();
  ESP_LOGV(TAG, "OFF ALL");
  panel_on_ = 0;
}  
  
void T547::scan_off() {
  //if (panel_on_ == 0)
  //  return;
  epd_scanoff();
  ESP_LOGV(TAG, "Scan OFF");
  panel_on_ = 0;
}
  
void T547::scan_on() {
  //if (panel_on_ == 0)
  //  return;
  epd_scanon();
  ESP_LOGV(TAG, "Scan ON");
  panel_on_ = 0;
}   

void T547::eink_on_() {
  //if (panel_on_ == 1)
  //  return;
  epd_poweron();
  ESP_LOGV(TAG, "ON");
  panel_on_ = 1;
}

void T547::display() {
  uint32_t start_time = millis();
  //epd_poweron();
  //eink_on_();
  ESP_LOGV(TAG, "Clear");
  epd_clear();
  //epd_draw_image(epd_full_screen(), this->buffer_, WHITE_ON_BLACK);
  ESP_LOGV(TAG, "Updating...");
  epd_draw_grayscale_image(epd_full_screen(), this->buffer_);
  //epd_poweroff();
  //epd_poweroff_all();
  //eink_off_();

  ESP_LOGV(TAG, "Display finished (full) (%ums)", millis() - start_time);
}

}  // namespace T547
}  // namespace esphome

