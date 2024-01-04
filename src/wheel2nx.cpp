#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/bootrom.h"
#include "time.h"

#include "pio_usb.h"
#include "tusb.h"

#include "mappings.h"

#define millis() to_ms_since_boot(get_absolute_time())

hid_wheel_report_t lastWheelReport = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
switch_report_t lastSwitchReport = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t getAdjustedAxisValue(uint8_t axisValue, uint8_t axisCenter, float multiplier) {
    int16_t delta = (int16_t)axisValue - (int16_t)axisCenter;
    float adjustedDelta = delta * multiplier;

    int16_t adjustedValue = axisCenter + (int16_t)adjustedDelta;

    if(adjustedValue<0)adjustedValue=0;
    if(adjustedValue>0xff)adjustedValue=0xff;

    return (uint8_t)adjustedValue;
}    

void translateToNx(){
	lastSwitchReport.obj.hat = lastWheelReport.dpad;
    lastSwitchReport.obj.y = lastWheelReport.button01;
    lastSwitchReport.obj.b = lastWheelReport.button03;
    lastSwitchReport.obj.a = lastWheelReport.button04;
    lastSwitchReport.obj.x = lastWheelReport.button02;
    lastSwitchReport.obj.l = lastWheelReport.paddleLeft;
    lastSwitchReport.obj.r = lastWheelReport.paddleRight;
    lastSwitchReport.obj.home = lastWheelReport.button09;
    lastSwitchReport.obj.plus = lastWheelReport.button10;

    lastSwitchReport.obj.lx = getAdjustedAxisValue(lastWheelReport.xAxisTrimmed, lastWheelReport.centerX, xSensitivity);
    if(!lastWheelReport.button03 && lastWheelReport.yAxis>lastWheelReport.centerY+pedalThreshold){
        lastSwitchReport.obj.b = 1;//mk break
    }
    if(!lastWheelReport.button04 && lastWheelReport.yAxis < lastWheelReport.centerY-pedalThreshold){
        lastSwitchReport.obj.a = 1;//mk gas
    }

    lastSwitchReport.obj.ly = 127;//center?
    lastSwitchReport.obj.rx = 127;//center?
    lastSwitchReport.obj.ry = 127;//center?
}

void hid_task(void){
    const uint32_t interval_ms = 1;
    static uint32_t start_ms = 0;

    if (millis() - start_ms < interval_ms) return;
    start_ms += interval_ms;

    translateToNx();

    if (tud_suspended()){
        tud_remote_wakeup();
    }

    if (tud_hid_ready()){
        tud_hid_report(0, lastSwitchReport.bytes, sizeof(lastSwitchReport.bytes));
    }
}

void core1_main() {
  sleep_ms(10);

  pio_usb_configuration_t pio_cfg = PIO_USB_DEFAULT_CONFIG;
  tuh_configure(1, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &pio_cfg);

  tuh_init(1);

  while (true) {
    tuh_task();
  }
}

int main(void) {
  set_sys_clock_khz(120000, true);//multiple of 12MHz

  sleep_ms(10);

  multicore_reset_core1();
  multicore_launch_core1(core1_main);

  tud_init(0);

  while (1){
    hid_task();
    tud_task();
  }

  return 0;
}

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len){
  (void)desc_report;
  (void)desc_len;

  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);

  if (itf_protocol == HID_ITF_PROTOCOL_NONE){
    if ( !tuh_hid_receive_report(dev_addr, instance) ){
      //todo
    }
  }
}

void tud_mount_cb(void){
}

void tud_umount_cb(void){
}

void tud_suspend_cb(bool remote_wakeup_en){
    (void)remote_wakeup_en;
}

void tud_resume_cb(void){
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen){
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;

    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize){
    (void)instance;
    (void)report_id;
    (void)report_type;
    tud_hid_report(0, buffer, bufsize);
}

static void process_wheel_report(uint8_t dev_addr, hid_wheel_report_t const * report){
    (void)dev_addr;
    memcpy(&lastWheelReport, report, sizeof(hid_wheel_report_t));
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len){
  (void)len;
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  switch(itf_protocol){
    case HID_ITF_PROTOCOL_NONE:{
        process_wheel_report(dev_addr, (hid_wheel_report_t const*) report );
    }break;

    default: break;
  }

  if ( !tuh_hid_receive_report(dev_addr, instance) ){
    //todo
  }
}
