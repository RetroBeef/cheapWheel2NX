#pragma once
const uint8_t pedalThreshold = 10;
const float xSensitivity = 2.0f;

const uint8_t wheelModeOff = 0xc0;
const uint8_t wheelModeOn = 0x40;

const uint8_t dpadIdle =      0x1f;
const uint8_t dpadUp =        0x00;
const uint8_t dpadUpRight =   0x01;
const uint8_t dpadRight =     0x02;
const uint8_t dpadDownRight = 0x03;
const uint8_t dpadDown =      0x04;
const uint8_t dpadDownLeft =  0x05;
const uint8_t dpadLeft =      0x06;
const uint8_t dpadUpLeft =    0x07;

#pragma pack(push,1)
typedef struct{
  uint8_t xAxis;
  uint8_t yAxis;
  uint8_t xAxisTrimmed;
  uint8_t centerX;
  uint8_t centerY;
  uint8_t dpad: 4;
  uint8_t button01 : 1;
  uint8_t button02 : 1;
  uint8_t button03 : 1;
  uint8_t button04 : 1;
  uint8_t button05 : 1;
  uint8_t button06 : 1;
  uint8_t button07 : 1;
  uint8_t button08 : 1;
  uint8_t button09 : 1;
  uint8_t button10 : 1;
  uint8_t paddleLeft : 1;
  uint8_t paddleRight : 1;
  uint8_t mode;
} hid_wheel_report_t;

typedef struct{
  uint8_t up: 1;
  uint8_t down: 1;
  uint8_t left: 1;
  uint8_t right: 1;
  uint8_t padding: 4;
} dpad_t;

typedef struct{
	uint8_t y : 1;
	uint8_t b : 1;
	uint8_t a : 1;
	uint8_t x : 1;
	uint8_t l : 1;
	uint8_t r : 1;
	uint8_t zl : 1;
	uint8_t zr : 1;
	uint8_t minus : 1;
	uint8_t plus : 1;
	uint8_t leftClick : 1;
	uint8_t rightClick : 1;
	uint8_t home : 1;
	uint8_t capture : 1;
	uint8_t reserved1: 2;
	uint8_t hat;
    uint8_t lx;
    uint8_t ly;
    uint8_t rx;
    uint8_t ry;
    uint8_t reserved3;
} switch_report_s;

typedef union{
  switch_report_s obj;
  uint8_t bytes[sizeof(switch_report_s)];
} switch_report_t;

#pragma pack(pop)
