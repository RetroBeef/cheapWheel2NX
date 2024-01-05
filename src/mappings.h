#pragma once
const uint8_t tracerPedalThreshold = 20;
const float tracerXSensitivity = 2.0f;

const uint8_t speedforcePedalThreshold = 20;
const float speeedforceXSensitivity = 1.0f;

#define HATSWITCH_UP            0x00
#define HATSWITCH_UPRIGHT       0x01
#define HATSWITCH_RIGHT         0x02
#define HATSWITCH_DOWNRIGHT     0x03
#define HATSWITCH_DOWN          0x04
#define HATSWITCH_DOWNLEFT      0x05
#define HATSWITCH_LEFT          0x06
#define HATSWITCH_UPLEFT        0x07
#define HATSWITCH_NONE          0x0F

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
} tracer_wheel_report_t;

typedef struct{
  uint16_t xAxis : 10;
  uint8_t reserved01 : 1;
  uint8_t reserved02 : 1;
  uint8_t dpadLeft : 1;
  uint8_t dpadRight : 1;
  uint8_t dpadDown : 1;
  uint8_t dpadUp : 1;
  uint8_t buttonPlus : 1;
  uint8_t buttonTwo : 1;
  uint8_t buttonOne : 1;
  uint8_t buttonB : 1;
  uint8_t buttonA : 1;
  uint8_t buttonMinus : 1;
  uint8_t buttonHome : 1;
  uint8_t reserved03 : 1;
  uint8_t paddleRight;
  uint8_t paddleLeft;
} speedforce_wheel_report_t;

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
