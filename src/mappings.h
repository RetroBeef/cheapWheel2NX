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

typedef struct{
	uint8_t up : 1;
	uint8_t down : 1;
	uint8_t left : 1;
	uint8_t right : 1;
	uint8_t a : 1;
	uint8_t b : 1;
	uint8_t start : 1;
	uint8_t c : 1;
} md_report_s;

typedef union{
  md_report_s obj;
  uint8_t bytes[sizeof(switch_report_s)];
} md_report_t;

#pragma pack(pop)

#define B01  2
#define B02  3
#define B03  4
#define B04  5
#define B05  6
#define B06  7
#define B07  8
#define B08  9
#define B09 10

#define EA_CTRL B08
#define TP_CTRL B09

#define MD_P1 B01
#define MD_P2 B02
#define MD_P3 B03
#define MD_P4 B04
#define MD_P6 B05
#define MD_P7 B06
#define MD_P9 B07

#define MD_X_UP MD_P1
#define MD_X_DOWN MD_P2
#define MD_L_LEFT MD_P3
#define MD_L_RIGHT MD_P4
#define MD_A_B MD_P6
#define MD_START_C MD_P9
#define MD_SELECT MD_P7
