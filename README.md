This repository contains a Battery component for [ESPHome](https://esphome.io/)
to support the ESP32 LILYGO T5 4.7" E-paper display.

(Do not confuse it with the plus version based on ESP32-s3.)

This is updated component originaly created by [vbaksa](https://github.com/vbaksa/esphome/tree/dev/esphome/components/lilygo_t5_47_battery), so it definitely needs improvement.  

## Usage

To use the board with [ESPHome](https://esphome.io/) **you have to put quite a
number of options in your esphome config**, see the examples at https://github.com/nickolay/esphome-lilygo-t547plus

```yaml
# ... required esp32, platformio_options configuration omitted for brevity ...

external_components:
  - source: github://arcao/esphome-lilygo-t547
    components: ["lilygo_t5_47_battery"]

sensor:
  - platform: lilygo_t5_47_battery
    id: battery_voltage
    voltage:
      name: "Battery Voltage"

  - platform: template
    name: "Battery Percentage"
    id: battery_percentage
    lambda: |-
      // tweak values in mapping for calibration
      // 4.1 = max voltage
      // 3.3 = min voltage
      float y = (id(battery_voltage).voltage->state - 3.3) * 100.0 / (4.1 - 3.3);
      if (y < 100.0) { return y; } else { return 100.0; };
```

## Discussion

https://github.com/esphome/feature-requests/issues/1960
