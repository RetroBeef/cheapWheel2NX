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

typedef struct{
    uint16_t vid;
    uint16_t pid;
}supported_device_t;

typedef struct{
    supported_device_t devId;
    uint8_t devReady;
    uint8_t devAddress;
    uint8_t devInstance;
    uint8_t activated;
} connected_device_t;

supported_device_t tracer = {0x0079, 0x0006};
supported_device_t speedForce = {0x046d, 0xc29c};

const size_t supportedDevicesCount = 2;
supported_device_t* supportedDevices[supportedDevicesCount] = {&tracer, &speedForce};

tracer_wheel_report_t lastTracerReport = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
speedforce_wheel_report_t lastSpeedforceReport = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
switch_report_t lastSwitchReport = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

connected_device_t* connectedDevice = 0;

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t getAdjustedTracerAxisValue(uint8_t axisValue, uint8_t axisCenter, float multiplier) {
    if(tracerXSensitivity==1.0f)return axisValue;
    int16_t delta = (int16_t)axisValue - (int16_t)axisCenter;
    float adjustedDelta = delta * multiplier;

    int16_t adjustedValue = axisCenter + (int16_t)adjustedDelta;

    if(adjustedValue<0)adjustedValue=0;
    if(adjustedValue>0xff)adjustedValue=0xff;

    return (uint8_t)adjustedValue;
}

uint8_t getAdjustedSpeedforceAxisValue(uint16_t axisValue, uint16_t axisCenter, float multiplier) {
    int32_t adjustedValue = axisValue;
    if(speeedforceXSensitivity!=1.0f){
        int32_t delta = (int32_t)axisValue - (int32_t)axisCenter;
        float adjustedDelta = delta * multiplier;

        int32_t adjustedValue = axisCenter + (int32_t)adjustedDelta;

        if(adjustedValue<0)adjustedValue=0;
        if(adjustedValue>0x400)adjustedValue=0x400;
    }

    return (uint8_t)map(adjustedValue,0,1024,0,255);
}     

void translateTracerToNx(){
	lastSwitchReport.obj.hat = lastTracerReport.dpad;
    lastSwitchReport.obj.y = lastTracerReport.button01;
    lastSwitchReport.obj.b = lastTracerReport.button03;
    lastSwitchReport.obj.a = lastTracerReport.button04;
    lastSwitchReport.obj.x = lastTracerReport.button02;
    lastSwitchReport.obj.l = lastTracerReport.paddleLeft;
    lastSwitchReport.obj.r = lastTracerReport.paddleRight;
    lastSwitchReport.obj.home = lastTracerReport.button09;
    lastSwitchReport.obj.plus = lastTracerReport.button10;

    lastSwitchReport.obj.lx = getAdjustedTracerAxisValue(lastTracerReport.xAxisTrimmed, lastTracerReport.centerX, tracerXSensitivity);
    if(!lastTracerReport.button03 && lastTracerReport.yAxis>lastTracerReport.centerY+tracerPedalThreshold){
        lastSwitchReport.obj.b = 1;//mk break
    }
    if(!lastTracerReport.button04 && lastTracerReport.yAxis < lastTracerReport.centerY-tracerPedalThreshold){
        lastSwitchReport.obj.a = 1;//mk gas
    }

    lastSwitchReport.obj.ly = 127;//center?
    lastSwitchReport.obj.rx = 127;//center?
    lastSwitchReport.obj.ry = 127;//center?
}

void translateSpeedforceToNx(){
	uint8_t up = lastSpeedforceReport.dpadUp;
	uint8_t down = lastSpeedforceReport.dpadDown;
    uint8_t left = lastSpeedforceReport.dpadLeft;
    uint8_t right = lastSpeedforceReport.dpadRight;
    lastSwitchReport.obj.hat = HATSWITCH_NONE;
    if(up){
    	if(right) lastSwitchReport.obj.hat = HATSWITCH_UPRIGHT;
    	else if(left) lastSwitchReport.obj.hat = HATSWITCH_UPLEFT;
    	else lastSwitchReport.obj.hat = HATSWITCH_UP;
    }else if(down){
    	if(right) lastSwitchReport.obj.hat = HATSWITCH_DOWNRIGHT;
    	else if(left) lastSwitchReport.obj.hat = HATSWITCH_DOWNLEFT;
    	else lastSwitchReport.obj.hat = HATSWITCH_DOWN;
    }else if(left){
    	lastSwitchReport.obj.hat = HATSWITCH_LEFT;
    }else if(right){
    	lastSwitchReport.obj.hat = HATSWITCH_RIGHT;
    }
	
    lastSwitchReport.obj.y = lastSpeedforceReport.buttonOne;
    lastSwitchReport.obj.b = lastSpeedforceReport.buttonB;
    lastSwitchReport.obj.a = lastSpeedforceReport.buttonA;
    lastSwitchReport.obj.x = lastSpeedforceReport.buttonTwo;

    lastSwitchReport.obj.l = lastSpeedforceReport.paddleLeft;
    lastSwitchReport.obj.r = lastSpeedforceReport.paddleRight;

    if(lastSpeedforceReport.paddleLeft < 0xff - speedforcePedalThreshold){
        lastSwitchReport.obj.l = 1;
    }else{
        lastSwitchReport.obj.l = 0;
    }

    if(lastSpeedforceReport.paddleRight < 0xff - speedforcePedalThreshold){
        lastSwitchReport.obj.r = 1;
    }else{
        lastSwitchReport.obj.r = 0;
    }

    lastSwitchReport.obj.home = lastSpeedforceReport.buttonHome;
    lastSwitchReport.obj.plus = lastSpeedforceReport.buttonPlus;

    lastSwitchReport.obj.lx = getAdjustedSpeedforceAxisValue(lastSpeedforceReport.xAxis, 512, speeedforceXSensitivity);

    lastSwitchReport.obj.ly = 127;//center?
    lastSwitchReport.obj.rx = 127;//center?
    lastSwitchReport.obj.ry = 127;//center?
}

void hid_task(void){
    const uint32_t interval_ms = 1;
    static uint32_t start_ms = 0;

    if (millis() - start_ms < interval_ms) return;
    start_ms += interval_ms;

    if(connectedDevice){
        if(connectedDevice->devId.vid == tracer.vid && connectedDevice->devId.pid == tracer.pid){
            translateTracerToNx();
        }else if(connectedDevice->devId.vid == speedForce.vid && connectedDevice->devId.pid == speedForce.pid){
            translateSpeedforceToNx();
        }
    }

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

  uint64_t lastCheckMs = 0;
  uint64_t secondCheckMs = 0;

  while (true) {
    tuh_task();
    if(connectedDevice){
        if(connectedDevice->devId.vid == speedForce.vid && connectedDevice->devId.pid == speedForce.pid){
            if(connectedDevice->devReady>=9 && !connectedDevice->activated){
                if(!lastCheckMs)lastCheckMs = millis();
                if(millis()-lastCheckMs>200){
                    uint8_t pairReport[] = {0xAF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
                    tuh_hid_set_report(connectedDevice->devAddress, connectedDevice->devInstance, 0, HID_REPORT_TYPE_FEATURE, pairReport, sizeof(pairReport));
                    if(!secondCheckMs)secondCheckMs = millis();
                    if(millis()-secondCheckMs>200){
                        pairReport[0] = 0xb2;
                        pairReport[1] = 0x04;
                        pairReport[2] = 0x0b;
                        tuh_hid_set_report(connectedDevice->devAddress, connectedDevice->devInstance, 0, HID_REPORT_TYPE_FEATURE, pairReport, sizeof(pairReport));
                        connectedDevice->activated = 1;
                        secondCheckMs = 0;
                        lastCheckMs = 0;
                    }
                }
            }
        }
    }
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
  
  for(uint8_t i=0;i<supportedDevicesCount;++i){
    if(vid == supportedDevices[i]->vid && pid == supportedDevices[i]->pid){
        connectedDevice = (connected_device_t*)malloc(sizeof(connected_device_t));
        connectedDevice->devId.vid = vid;
        connectedDevice->devId.pid = pid;
        connectedDevice->devReady = 0;
        connectedDevice->devAddress = dev_addr;
        connectedDevice->devInstance = instance;
        connectedDevice->activated = 0;

        if(vid == tracer.vid && pid == tracer.pid){
            connectedDevice->devReady = 1;
            connectedDevice->activated = 1;
        }

        if (itf_protocol == HID_ITF_PROTOCOL_NONE){
          if (!tuh_hid_receive_report(dev_addr, instance)){
            //todo
          }
        }
        break;
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

static void process_tracer_report(uint8_t dev_addr, tracer_wheel_report_t const * report){
    (void)dev_addr;
    memcpy(&lastTracerReport, report, sizeof(tracer_wheel_report_t));
}

static void process_speedforce_report(uint8_t dev_addr, speedforce_wheel_report_t const * report){
    (void)dev_addr;
    memcpy(&lastSpeedforceReport, report, sizeof(speedforce_wheel_report_t));
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len){
  (void)len;
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  switch(itf_protocol){
    case HID_ITF_PROTOCOL_NONE:{
        if(connectedDevice){
            if(connectedDevice->devId.vid == tracer.vid && connectedDevice->devId.pid == tracer.pid){
                process_tracer_report(dev_addr, (tracer_wheel_report_t const*) report);
            }else if(connectedDevice->devId.vid == speedForce.vid && connectedDevice->devId.pid == speedForce.pid){
                if(connectedDevice->devReady>=9 && connectedDevice->activated){
                    process_speedforce_report(dev_addr, (speedforce_wheel_report_t const*) report);
                }else if(len==5){
                    if(report[0]==0x02 && report[1]==0x0a && report[2]==0x00 && report[3]==0xff && report[4]==0xff){
                        connectedDevice->devReady++;
                    }
                }
            }
        }
    }break;

    default: break;
  }

  if ( !tuh_hid_receive_report(dev_addr, instance) ){
    //todo
  }
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance){
    (void)dev_addr;
    (void)instance;
    if(connectedDevice){
        free(connectedDevice);
        connectedDevice = 0;
    }
}
