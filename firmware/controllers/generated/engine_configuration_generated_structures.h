#pragma once
#include "rusefi_types.h"
struct stft_cell_cfg_s {
	// %
	// offset 0
	int8_t maxAdd;
	// %
	// offset 1
	int8_t maxRemove;
	// Time constant for correction while in this cell: this sets responsiveness of the closed loop correction. A value of 5.0 means it will try to make most of the correction within 5 seconds, and a value of 1.0 will try to correct within 1 second.
	// sec
	// offset 2
	scaled_channel<uint16_t, 10, 1> timeConstant;
};
static_assert(sizeof(stft_cell_cfg_s) == 4);

struct stft_s {
	// Below this RPM, the idle region is active
	// RPM
	// offset 0
	scaled_channel<uint8_t, 1, 50> maxIdleRegionRpm;
	// Below this engine load, the overrun region is active
	// load
	// offset 1
	uint8_t maxOverrunLoad;
	// Above this engine load, the power region is active
	// load
	// offset 2
	uint8_t minPowerLoad;
	// When close to correct AFR, pause correction. This can improve stability by not changing the adjustment if the error is extremely small, but is not required.
	// %
	// offset 3
	scaled_channel<uint8_t, 10, 1> deadband;
	// Below this temperature, correction is disabled.
	// C
	// offset 4
	int8_t minClt;
	// Below this AFR, correction is paused
	// afr
	// offset 5
	scaled_channel<uint8_t, 10, 1> minAfr;
	// Above this AFR, correction is paused
	// afr
	// offset 6
	scaled_channel<uint8_t, 10, 1> maxAfr;
	// Delay after starting the engine before beginning closed loop correction.
	// seconds
	// offset 7
	uint8_t startupDelay;
	// offset 8
	stft_cell_cfg_s cellCfgs[STFT_CELL_COUNT];
};
static_assert(sizeof(stft_s) == 24);

struct pid_s {
	// offset 0
	float pFactor;
	// offset 4
	float iFactor;
	// offset 8
	float dFactor;
	// Linear addition to PID logic
	// offset 12
	int16_t offset;
	// PID dTime
	// ms
	// offset 14
	int16_t periodMs;
	// Output Min Duty Cycle
	// offset 16
	int16_t minValue;
	// Output Max Duty Cycle
	// offset 18
	int16_t maxValue;
};
static_assert(sizeof(pid_s) == 20);

struct cranking_parameters_s {
	// Base mass of the per-cylinder fuel injected during cranking. This is then modified by the multipliers for CLT, IAT, TPS ect, to give the final cranking pulse width.
	// A reasonable starting point is 60mg per liter per cylinder.
	// ex: 2 liter 4 cyl = 500cc/cyl, so 30mg cranking fuel.
	// mg
	// offset 0
	float baseFuel;
	// This sets the RPM limit below which the ECU will use cranking fuel and ignition logic, typically this is around 350-450rpm.
	// RPM
	// offset 4
	int16_t rpm;
	// offset 6
	uint8_t alignmentFill_at_6[2];
};
static_assert(sizeof(cranking_parameters_s) == 8);

struct gppwm_channel {
	// Select a pin to use for PWM or on-off output.
	// offset 0
	output_pin_e pin;
	// If an error (with a sensor, etc) is detected, this value is used instead of reading from the table.
	// This should be a safe value for whatever hardware is connected to prevent damage.
	// %
	// offset 2
	uint8_t dutyIfError;
	// offset 3
	uint8_t alignmentFill_at_3[1];
	// Select a frequency to run PWM at.
	// Set this to 0hz to enable on-off mode.
	// hz
	// offset 4
	uint16_t pwmFrequency;
	// Hysteresis: in on-off mode, turn the output on when the table value is above this duty.
	// %
	// offset 6
	uint8_t onAboveDuty;
	// Hysteresis: in on-off mode, turn the output off when the table value is below this duty.
	// %
	// offset 7
	uint8_t offBelowDuty;
	// Selects the Y axis to use for the table.
	// offset 8
	gppwm_channel_e loadAxis;
	// Selects the X axis to use for the table.
	// offset 9
	gppwm_channel_e rpmAxis;
	// load
	// offset 10
	scaled_channel<int16_t, 10, 1> loadBins[GPPWM_LOAD_COUNT];
	// RPM
	// offset 26
	int16_t rpmBins[GPPWM_RPM_COUNT];
	// duty
	// offset 42
	scaled_channel<uint8_t, 2, 1> table[GPPWM_RPM_COUNT][GPPWM_LOAD_COUNT];
	// offset 106
	uint8_t alignmentFill_at_106[2];
};
static_assert(sizeof(gppwm_channel) == 108);

struct air_pressure_sensor_config_s {
	// kPa value at low volts
	// kpa
	// offset 0
	float lowValue;
	// kPa value at high volts
	// kpa
	// offset 4
	float highValue;
	// offset 8
	air_pressure_sensor_type_e type;
	// offset 9
	adc_channel_e hwChannel;
	// offset 10
	uint8_t alignmentFill_at_10[2];
};
static_assert(sizeof(air_pressure_sensor_config_s) == 12);

struct MAP_sensor_config_s {
	// offset 0
	float samplingAngleBins[MAP_ANGLE_SIZE];
	// MAP averaging sampling start crank degree angle
	// deg
	// offset 32
	float samplingAngle[MAP_ANGLE_SIZE];
	// offset 64
	float samplingWindowBins[MAP_WINDOW_SIZE];
	// MAP averaging angle crank degree duration
	// deg
	// offset 96
	float samplingWindow[MAP_WINDOW_SIZE];
	// offset 128
	air_pressure_sensor_config_s sensor;
};
static_assert(sizeof(MAP_sensor_config_s) == 140);

struct thermistor_conf_s {
	// these values are in Celcius
	// *C
	// offset 0
	scaled_channel<int, 100, 1> tempC_1;
	// *C
	// offset 4
	scaled_channel<int, 100, 1> tempC_2;
	// *C
	// offset 8
	scaled_channel<int, 100, 1> tempC_3;
	// Ohm
	// offset 12
	scaled_channel<uint32_t, 100, 1> resistance_1;
	// Ohm
	// offset 16
	scaled_channel<uint32_t, 100, 1> resistance_2;
	// Ohm
	// offset 20
	scaled_channel<uint32_t, 100, 1> resistance_3;
	// Pull-up resistor value on your board
	// Ohm
	// offset 24
	scaled_channel<uint32_t, 10, 1> bias_resistor;
};
static_assert(sizeof(thermistor_conf_s) == 28);

struct linear_sensor_s {
	// offset 0
	adc_channel_e hwChannel;
	// offset 1
	uint8_t alignmentFill_at_1[3];
	// volts
	// offset 4
	float v1;
	// kPa
	// offset 8
	float value1;
	// volts
	// offset 12
	float v2;
	// kPa
	// offset 16
	float value2;
};
static_assert(sizeof(linear_sensor_s) == 20);

struct ThermistorConf {
	// offset 0
	thermistor_conf_s config;
	// offset 28
	adc_channel_e adcChannel;
	// offset 29
	uint8_t alignmentFill_at_29[3];
};
static_assert(sizeof(ThermistorConf) == 32);

struct injector_s {
	// This is your injector flow at the fuel pressure used in the vehicle. cc/min, cubic centimetre per minute
	// By the way, g/s = 0.125997881 * (lb/hr)
	// g/s = 0.125997881 * (cc/min)/10.5
	// g/s = 0.0119997981 * cc/min
	// cm3/min
	// offset 0
	float flow;
	// volts
	// offset 4
	scaled_channel<uint16_t, 100, 1> battLagCorrBins[VBAT_INJECTOR_CURVE_SIZE];
	// ms delay between injector open and close dead times
	// ms
	// offset 20
	scaled_channel<uint16_t, 100, 1> battLagCorr[VBAT_INJECTOR_CURVE_SIZE];
};
static_assert(sizeof(injector_s) == 36);

struct trigger_config_s {
	// https://github.com/rusefi/rusefi/wiki/All-Supported-Triggers
	// offset 0
	trigger_type_e type;
	// number
	// offset 4
	int customTotalToothCount;
	// number
	// offset 8
	int customSkippedToothCount;
};
static_assert(sizeof(trigger_config_s) == 12);

struct afr_sensor_s {
	// offset 0
	adc_channel_e hwChannel;
	// offset 1
	adc_channel_e hwChannel2;
	// volts
	// offset 2
	scaled_channel<uint16_t, 1000, 1> v1;
	// AFR
	// offset 4
	scaled_channel<uint16_t, 1000, 1> value1;
	// volts
	// offset 6
	scaled_channel<uint16_t, 1000, 1> v2;
	// AFR
	// offset 8
	scaled_channel<uint16_t, 1000, 1> value2;
	// offset 10
	uint8_t alignmentFill_at_10[2];
};
static_assert(sizeof(afr_sensor_s) == 12);

struct idle_hardware_s {
	// Hz
	// offset 0
	int solenoidFrequency;
	// offset 4
	output_pin_e solenoidPin;
	// offset 6
	Gpio stepperDirectionPin;
	// offset 8
	Gpio stepperStepPin;
	// offset 10
	pin_output_mode_e solenoidPinMode;
	// offset 11
	uint8_t alignmentFill_at_11[1];
};
static_assert(sizeof(idle_hardware_s) == 12);

struct dc_io {
	// offset 0
	Gpio directionPin1;
	// offset 2
	Gpio directionPin2;
	// Acts as EN pin in two-wire mode
	// offset 4
	Gpio controlPin;
	// offset 6
	Gpio disablePin;
};
static_assert(sizeof(dc_io) == 8);

struct vr_threshold_s {
	// rpm
	// offset 0
	scaled_channel<uint8_t, 1, 50> rpmBins[6];
	// volts
	// offset 6
	scaled_channel<uint8_t, 100, 1> values[6];
	// offset 12
	Gpio pin;
	// offset 14
	uint8_t alignmentFill_at_14[2];
};
static_assert(sizeof(vr_threshold_s) == 16);

struct engine_configuration_s {
	// http://rusefi.com/wiki/index.php?title=Manual:Engine_Type
	// offset 0
	engine_type_e engineType;
	// Disable sensor sniffer above this rpm
	// RPM
	// offset 4
	uint16_t sensorSnifferRpmThreshold;
	// A secondary Rev limit engaged by the driver to help launch the vehicle faster
	// rpm
	// offset 6
	uint16_t launchRpm;
	// rpm
	// offset 8
	uint16_t rpmHardLimit;
	// Engine sniffer would be disabled above this rpm
	// RPM
	// offset 10
	uint16_t engineSnifferRpmThreshold;
	// Disable multispark above this engine speed.
	// rpm
	// offset 12
	scaled_channel<uint8_t, 1, 50> multisparkMaxRpm;
	// Above this RPM, disable AC. Set to 0 to disable check.
	// rpm
	// offset 13
	scaled_channel<uint8_t, 1, 50> maxAcRpm;
	// Above this TPS, disable AC. Set to 0 to disable check.
	// %
	// offset 14
	uint8_t maxAcTps;
	// Above this CLT, disable AC to prevent overheating the engine. Set to 0 to disable check.
	// deg C
	// offset 15
	uint8_t maxAcClt;
	// RPM
	// offset 16
	uint16_t knockNoiseRpmBins[ENGINE_NOISE_CURVE_SIZE];
	// This parameter sets the latest that the last multispark can occur after the main ignition event. For example, if the ignition timing is 30 degrees BTDC, and this parameter is set to 45, no multispark will ever be fired after 15 degrees ATDC.
	// deg
	// offset 48
	uint8_t multisparkMaxSparkingAngle;
	// Configures the maximum number of extra sparks to fire (does not include main spark)
	// count
	// offset 49
	uint8_t multisparkMaxExtraSparkCount;
	// Single coil = distributor
	// Individual coils = one coil per cylinder (COP, coil-near-plug), requires sequential mode
	// Wasted spark = Fires pairs of cylinders together, either one coil per pair of cylinders or one coil per cylinder
	// Two distributors = A pair of distributors, found on some BMW, Toyota and other engines
	// offset 50
	ignition_mode_e ignitionMode;
	// offset 51
	can_nbc_e canNbcType;
	// offset 52
	injector_s injector;
	// Does the vehicle have a turbo or supercharger?
	// offset 88 bit 0
	bool isForcedInduction : 1 {};
	// On some Ford and Toyota vehicles one of the throttle sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	// offset 88 bit 1
	bool useFordRedundantTps : 1 {};
	// offset 88 bit 2
	bool lambdaProtectionEnable : 1 {};
	// offset 88 bit 3
	bool overrideTriggerGaps : 1 {};
	// Turn on this fan when AC is on.
	// offset 88 bit 4
	bool enableFan1WithAc : 1 {};
	// Turn on this fan when AC is on.
	// offset 88 bit 5
	bool enableFan2WithAc : 1 {};
	// Inhibit operation of this fan while the engine is not running.
	// offset 88 bit 6
	bool disableFan1WhenStopped : 1 {};
	// Inhibit operation of this fan while the engine is not running.
	// offset 88 bit 7
	bool disableFan2WhenStopped : 1 {};
	// Enable secondary spark outputs that fire after the primary (rotaries, twin plug engines).
	// offset 88 bit 8
	bool enableTrailingSparks : 1 {};
	// TLE7209 uses two-wire mode. TLE9201 and VNH2SP30 do NOT use two wire mode.
	// offset 88 bit 9
	bool etb_use_two_wires : 1 {};
	// Subaru/BMW style where default valve position is somewhere in the middle. First solenoid opens it more while second can close it more than default position.
	// offset 88 bit 10
	bool isDoubleSolenoidIdle : 1 {};
	// offset 88 bit 11
	bool enableStagedInjection : 1 {};
	// offset 88 bit 12
	bool useTLE8888_cranking_hack : 1 {};
	// offset 88 bit 13
	bool kickStartCranking : 1 {};
	// This uses separate ignition timing and VE tables not only for idle conditions, also during the postcranking-to-idle taper transition (See also afterCrankingIACtaperDuration).
	// offset 88 bit 14
	bool useSeparateIdleTablesForCrankingTaper : 1 {};
	// offset 88 bit 15
	bool launchControlEnabled : 1 {};
	// "Detect double trigger edges"
	// offset 88 bit 16
	bool doNotFilterTriggerEdgeNoise : 1 {};
	// offset 88 bit 17
	bool antiLagEnabled : 1 {};
	// For cranking either use the specified fixed base fuel mass, or use the normal running math (VE table).
	// offset 88 bit 18
	bool useRunningMathForCranking : 1 {};
	// offset 88 bit 19
	bool useTLE8888_stepper : 1 {};
	// offset 88 bit 20
	bool usescriptTableForCanSniffingFiltering : 1 {};
	// Print incoming and outgoing first bus CAN messages in FOME console
	// offset 88 bit 21
	bool verboseCan : 1 {};
	// Experimental setting that will cause a misfire
	// DO NOT ENABLE.
	// offset 88 bit 22
	bool artificialTestMisfire : 1 {};
	// On some Ford and Toyota vehicles one of the pedal sensors is not linear on the full range, i.e. in the specific range of the positions we effectively have only one sensor.
	// offset 88 bit 23
	bool useFordRedundantPps : 1 {};
	// offset 88 bit 24
	bool cltSensorPulldown : 1 {};
	// offset 88 bit 25
	bool iatSensorPulldown : 1 {};
	// offset 88 bit 26
	bool allowIdenticalPps : 1 {};
	// offset 88 bit 27
	bool unusedBit_42_27 : 1 {};
	// offset 88 bit 28
	bool unusedBit_42_28 : 1 {};
	// offset 88 bit 29
	bool unusedBit_42_29 : 1 {};
	// offset 88 bit 30
	bool unusedBit_42_30 : 1 {};
	// offset 88 bit 31
	bool unusedBit_42_31 : 1 {};
	// Closed throttle, 1 volt = 200 units
	// ADC
	// offset 92
	int16_t tpsMin;
	// Full throttle, 1 volt = 200 units
	// ADC
	// offset 94
	int16_t tpsMax;
	// TPS error detection: what throttle % is unrealistically low?
	// Also used for accelerator pedal error detection if so equiped.
	// %
	// offset 96
	int16_t tpsErrorDetectionTooLow;
	// TPS error detection: what throttle % is unrealistically high?
	// Also used for accelerator pedal error detection if so equiped.
	// %
	// offset 98
	int16_t tpsErrorDetectionTooHigh;
	// offset 100
	cranking_parameters_s cranking;
	// Dwell duration while cranking
	// ms
	// offset 108
	float ignitionDwellForCrankingMs;
	// Once engine speed passes this value, start reducing ETB angle.
	// rpm
	// offset 112
	uint16_t etbRevLimitStart;
	// This far above 'Soft limiter start', fully close the throttle. At the bottom of the range, throttle control is normal. At the top of the range, the throttle is fully closed.
	// rpm
	// offset 114
	uint16_t etbRevLimitRange;
	// offset 116
	MAP_sensor_config_s map;
	// todo: merge with channel settings, use full-scale Thermistor here!
	// offset 256
	ThermistorConf clt;
	// offset 288
	ThermistorConf iat;
	// deg
	// offset 320
	int launchTimingRetard;
	// We calculate knock band based of cylinderBore
	//  Use this to override - kHz knock band override
	// Requires power cycling to effect
	// kHz
	// offset 324
	float knockBandCustom;
	// Engine displacement in litres
	// L
	// offset 328
	scaled_channel<uint16_t, 1000, 1> displacement;
	// RPM
	// offset 330
	uint16_t triggerSimulatorRpm;
	// Number of cylinder the engine has.
	// offset 332
	uint32_t cylindersCount;
	// offset 336
	firing_order_e firingOrder;
	// deg
	// offset 337
	uint8_t vvtBumpAmount;
	// Duration of each test pulse
	// ms
	// offset 338
	scaled_channel<uint16_t, 100, 1> benchTestOnTime;
	// Cylinder diameter in mm.
	// mm
	// offset 340
	float cylinderBore;
	// This setting controls which fuel quantity control algorithm is used.
	// Alpha-N means drive by TPS commonly only used for NA engines
	// Speed Density requires MAP sensor and is the default choice for may installs
	// MAF air charge is a cylinder filling based method that uses a mass air flow sensor.
	// offset 344
	engine_load_mode_e fuelAlgorithm;
	// %
	// offset 345
	uint8_t ALSMaxTPS;
	// offset 346
	Gpio binarySerialTxPin;
	// offset 348
	Gpio binarySerialRxPin;
	// This is the injection strategy during engine start. See Fuel/Injection settings for more detail. It is suggested to use "Simultaneous".
	// offset 350
	injection_mode_e crankingInjectionMode;
	// This is where the fuel injection type is defined: "Simultaneous" means all injectors will fire together at once. "Sequential" fires the injectors on a per cylinder basis, which requires individually wired injectors. "Batched" will fire the injectors in groups. If your injectors are individually wired you will also need to enable "Two wire batch emulation".
	// offset 351
	injection_mode_e injectionMode;
	// Minimum RPM to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	// offset 352
	uint16_t boostControlMinRpm;
	// Minimum TPS to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	// offset 354
	uint8_t boostControlMinTps;
	// Minimum MAP to enable boost control. Use this to avoid solenoid noise at idle, and help spool in some cases.
	// offset 355
	uint8_t boostControlMinMap;
	// Dynamic uses the timing map to decide the ignition timing, Static timing fixes the timing to the value set below (only use for checking static timing with a timing light).
	// offset 356
	timing_mode_e timingMode;
	// offset 357
	adc_channel_e mafAdcChannel;
	// How many test bench pulses do you want
	// offset 358
	uint16_t benchTestCount;
	// Ignition advance angle used during engine cranking, 5-10 degrees will work as a base setting for most engines.
	// There is tapering towards running timing advance
	// deg
	// offset 360
	angle_t crankingTimingAngle;
	// How many consecutive gap rations have to match expected ranges for sync to happen
	// count
	// offset 364
	int8_t gapTrackingLengthOverride;
	// Above this speed, disable closed loop idle control. Set to 0 to disable (allow closed loop idle at any speed).
	// kph
	// offset 365
	uint8_t maxIdleVss;
	// Expected oil pressure after starting the engine. If oil pressure does not reach this level within 5 seconds of engine start, fuel will be cut. Set to 0 to disable and always allow starting.
	// kPa
	// offset 366
	uint16_t minOilPressureAfterStart;
	// This value is the ignition timing used when in 'fixed timing' mode, i.e. constant timing
	// This mode is useful when adjusting distributor location.
	// RPM
	// offset 368
	angle_t fixedModeTiming;
	// Angle between Top Dead Center (TDC) and the first trigger event.
	// Positive value in case of synchnization point before TDC and negative in case of synchnization point after TDC
	// .Knowing this angle allows us to control timing and other angles in reference to TDC.
	// deg btdc
	// offset 372
	angle_t globalTriggerAngleOffset;
	// Ratio/coefficient of input voltage dividers on your PCB. For example, use '2' if your board divides 5v into 2.5v. Use '1.66' if your board divides 5v into 3v.
	// coef
	// offset 376
	float analogInputDividerCoefficient;
	// This is the ratio of the resistors for the battery voltage, measure the voltage at the battery and then adjust this number until the gauge matches the reading.
	// coef
	// offset 380
	float vbattDividerCoeff;
	// Cooling fan turn-on temperature threshold, in Celsius
	// deg C
	// offset 384
	uint8_t fanOnTemperature;
	// Cooling fan turn-off temperature threshold, in Celsius
	// deg C
	// offset 385
	uint8_t fanOffTemperature;
	// offset 386
	output_pin_e acrPin;
	// Number of revolutions per kilometer for the wheels your vehicle speed sensor is connected to. Use an online calculator to determine this based on your tire size.
	// revs/km
	// offset 388
	float driveWheelRevPerKm;
	// CANbus thread period in ms
	// ms
	// offset 392
	int canSleepPeriodMs;
	// index
	// offset 396
	int byFirmwareVersion;
	// First throttle body, first sensor
	// offset 400
	adc_channel_e tps1_1AdcChannel;
	// This is the processor input pin that the battery voltage circuit is connected to, if you are unsure of what pin to use, check the schematic that corresponds to your PCB.
	// offset 401
	adc_channel_e vbattAdcChannel;
	// This is the processor pin that your fuel level sensor in connected to. This is a non standard input so will need to be user defined.
	// offset 402
	adc_channel_e fuelLevelSensor;
	// Second throttle body, first sensor
	// offset 403
	adc_channel_e tps2_1AdcChannel;
	// 0.1 is a good default value
	// x
	// offset 404
	float idle_derivativeFilterLoss;
	// just a temporary solution
	// angle
	// offset 408
	int trailingSparkAngle;
	// offset 412
	trigger_config_s trigger;
	// Extra air taper amount
	// %
	// offset 424
	float airByRpmTaper;
	// Duty cycle to use in case of a sensor failure. This duty cycle should produce the minimum possible amount of boost. This duty is also used in case any of the minimum RPM/TPS/MAP conditions are not met.
	// %
	// offset 428
	uint8_t boostControlSafeDutyCycle;
	// Throttle pedal position first channel
	// offset 429
	adc_channel_e throttlePedalPositionAdcChannel;
	// offset 430
	pin_output_mode_e tle6240_csPinMode;
	// offset 431
	uint8_t acrRevolutions;
	// coef
	// offset 432
	float globalFuelCorrection;
	// volts
	// offset 436
	float adcVcc;
	// Deg
	// offset 440
	float mapCamDetectionAnglePosition;
	// Camshaft input could be used either just for engine phase detection if your trigger shape does not include cam sensor as 'primary' channel, or it could be used for Variable Valve timing on one of the camshafts.
	// offset 444
	brain_input_pin_e camInputs[CAM_INPUTS_COUNT];
	// offset 452
	afr_sensor_s afr;
	// offset 464
	Gpio tle6240_cs;
	// Throttle Pedal not pressed switch - used on some older vehicles like early Mazda Miata
	// offset 466
	switch_input_pin_e throttlePedalUpPin;
	// offset 468
	air_pressure_sensor_config_s baroSensor;
	// offset 480
	idle_hardware_s idle;
	// Value between 0 and 100 used in Manual mode
	// %
	// offset 492
	float manIdlePosition;
	// Ignition timing to remove when a knock event occurs.
	// %
	// offset 496
	scaled_channel<uint8_t, 10, 1> knockRetardAggression;
	// After a knock event, reapply timing at this rate.
	// deg/s
	// offset 497
	scaled_channel<uint8_t, 10, 1> knockRetardReapplyRate;
	// Select which cam is used for engine sync. Other cams will be used only for VVT measurement, but not engine sync.
	// offset 498
	engineSyncCam_e engineSyncCam;
	// Set this so your vehicle speed signal is responsive, but not noisy. Larger value give smoother but slower response.
	// offset 499
	uint8_t vssFilterReciprocal;
	// Number of turns of your vehicle speed sensor per turn of the wheels. For example if your sensor is on the transmission output, enter your axle/differential ratio. If you are using a hub-mounted sensor, enter a value of 1.0.
	// ratio
	// offset 500
	scaled_channel<uint16_t, 1000, 1> vssGearRatio;
	// Number of pulses output per revolution of the shaft where your VSS is mounted. For example, GM applications of the T56 output 17 pulses per revolution of the transmission output shaft.
	// count
	// offset 502
	uint8_t vssToothCount;
	// Override the Y axis (load) value used for only the Idle VE table.
	// Advanced users only: If you aren't sure you need this, you probably don't need this.
	// offset 503
	ve_override_e idleVeOverrideMode;
	// offset 504
	Gpio l9779_cs;
	// On-off O2 sensor heater control. 'ON' if engine is running, 'OFF' if stopped or cranking.
	// offset 506
	output_pin_e o2heaterPin;
	// offset 508
	output_pin_e injectionPins[MAX_CYLINDER_COUNT];
	// offset 532
	output_pin_e injectionPinsStage2[MAX_CYLINDER_COUNT];
	// offset 556
	output_pin_e ignitionPins[MAX_CYLINDER_COUNT];
	// offset 580
	pin_output_mode_e injectionPinMode;
	// offset 581
	pin_output_mode_e ignitionPinMode;
	// offset 582
	output_pin_e fuelPumpPin;
	// offset 584
	pin_output_mode_e fuelPumpPinMode;
	// Throttle pedal, secondary channel.
	// offset 585
	adc_channel_e throttlePedalPositionSecondAdcChannel;
	// Check engine light, also malfunction indicator light. Always blinks once on boot.
	// offset 586
	output_pin_e malfunctionIndicatorPin;
	// offset 588
	pin_output_mode_e malfunctionIndicatorPinMode;
	// offset 589
	pin_output_mode_e fanPinMode;
	// offset 590
	output_pin_e fanPin;
	// Some cars have a switch to indicate that clutch pedal is all the way down
	// offset 592
	switch_input_pin_e clutchDownPin;
	// offset 594
	output_pin_e alternatorControlPin;
	// offset 596
	pin_output_mode_e alternatorControlPinMode;
	// offset 597
	pin_input_mode_e clutchDownPinMode;
	// offset 598
	pin_output_mode_e electronicThrottlePin1Mode;
	// offset 599
	spi_device_e max31855spiDevice;
	// offset 600
	Gpio debugTriggerSync;
	// offset 602
	Gpio mc33972_cs;
	// offset 604
	pin_output_mode_e mc33972_csPinMode;
	// Useful in Research&Development phase
	// offset 605
	adc_channel_e auxFastSensor1_adcChannel;
	// First throttle body, second sensor.
	// offset 606
	adc_channel_e tps1_2AdcChannel;
	// Second throttle body, second sensor.
	// offset 607
	adc_channel_e tps2_2AdcChannel;
	// %
	// offset 608
	uint8_t fuelLevelValues[FUEL_LEVEL_TABLE_COUNT];
	// 0.1 is a good default value
	// x
	// offset 616
	float idle_antiwindupFreq;
	// offset 620
	brain_input_pin_e triggerInputPins[TRIGGER_INPUT_PIN_COUNT];
	// Minimum allowed time for the boost phase. If the boost target current is reached before this time elapses, it is assumed that the injector has failed short circuit.
	// us
	// offset 624
	uint16_t mc33_t_min_boost;
	// offset 626
	output_pin_e tachOutputPin;
	// offset 628
	pin_output_mode_e tachOutputPinMode;
	// offset 629
	adc_channel_e maf2AdcChannel;
	// offset 630
	output_pin_e mainRelayPin;
	// offset 632
	Gpio sdCardCsPin;
	// set_can_tx_pin X
	// offset 634
	Gpio canTxPin;
	// set_can_rx_pin X
	// offset 636
	Gpio canRxPin;
	// offset 638
	pin_input_mode_e throttlePedalUpPinMode;
	// Additional idle % while A/C is active
	// %
	// offset 639
	uint8_t acIdleExtraOffset;
	// Ratio between the wheels and your transmission output.
	// ratio
	// offset 640
	scaled_channel<uint16_t, 100, 1> finalGearRatio;
	// offset 642
	brain_input_pin_e tcuInputSpeedSensorPin;
	// offset 644
	uint8_t tcuInputSpeedSensorTeeth;
	// offset 645
	pin_output_mode_e mainRelayPinMode;
	// Voltage when the wastegate is closed.
	// You probably don't have one of these!
	// mv
	// offset 646
	uint16_t wastegatePositionMin;
	// Voltage when the wastegate is fully open.
	// You probably don't have one of these!
	// 1 volt = 1000 units
	// mv
	// offset 648
	uint16_t wastegatePositionMax;
	// Some Subaru and some Mazda use double-solenoid idle air valve
	// offset 650
	output_pin_e secondSolenoidPin;
	// Secondary TTL channel baud rate
	// BPs
	// offset 652
	uint32_t tunerStudioSerialSpeed;
	// Just for reference really, not taken into account by any logic at this point
	// CR
	// offset 656
	float compressionRatio;
	// Generate a synthetic trigger signal for an external ECU
	// offset 660
	Gpio triggerSimulatorPins[TRIGGER_SIMULATOR_PIN_COUNT];
	// g/s
	// offset 664
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseSlope;
	// RPM
	// offset 666
	scaled_channel<uint8_t, 1, 100> lambdaProtectionMinRpm;
	// %
	// offset 667
	scaled_channel<uint8_t, 1, 10> lambdaProtectionMinLoad;
	// offset 668 bit 0
	bool is_enabled_spi_1 : 1 {};
	// offset 668 bit 1
	bool is_enabled_spi_2 : 1 {};
	// offset 668 bit 2
	bool is_enabled_spi_3 : 1 {};
	// enable sd/disable sd
	// offset 668 bit 3
	bool isSdCardEnabled : 1 {};
	// Use 11 bit (standard) or 29 bit (extended) IDs for FOME verbose CAN format.
	// offset 668 bit 4
	bool rusefiVerbose29b : 1 {};
	// offset 668 bit 5
	bool unused644b5 : 1 {};
	// This setting should only be used if you have a stepper motor idle valve and a stepper motor driver installed.
	// offset 668 bit 6
	bool useStepperIdle : 1 {};
	// offset 668 bit 7
	bool enabledStep1Limiter : 1 {};
	// offset 668 bit 8
	bool verboseTLE8888 : 1 {};
	// CAN broadcast using custom FOME protocol
	// enable can_broadcast/disable can_broadcast
	// offset 668 bit 9
	bool enableVerboseCanTx : 1 {};
	// offset 668 bit 10
	bool etb1configured : 1 {};
	// offset 668 bit 11
	bool etb2configured : 1 {};
	// Useful for individual intakes
	// offset 668 bit 12
	bool measureMapOnlyInOneCylinder : 1 {};
	// offset 668 bit 13
	bool stepperForceParkingEveryRestart : 1 {};
	// If enabled, try to fire the engine before a full engine cycle has been completed using RPM estimated from the last 90 degrees of engine rotation. As soon as the trigger syncs plus 90 degrees rotation, fuel and ignition events will occur. If disabled, worst case may require up to 4 full crank rotations before any events are scheduled.
	// offset 668 bit 14
	bool isFasterEngineSpinUpEnabled : 1 {};
	// This setting disables fuel injection while the engine is in overrun, this is useful as a fuel saving measure and to prevent back firing.
	// offset 668 bit 15
	bool coastingFuelCutEnabled : 1 {};
	// Override the IAC position during overrun conditions to help reduce engine breaking, this can be helpful for large engines in light weight cars or engines that have trouble returning to idle.
	// offset 668 bit 16
	bool useIacTableForCoasting : 1 {};
	// offset 668 bit 17
	bool useIdleTimingPidControl : 1 {};
	// Allows disabling the ETB when the engine is stopped. You may not like the power draw or PWM noise from the motor, so this lets you turn it off until it's necessary.
	// offset 668 bit 18
	bool disableEtbWhenEngineStopped : 1 {};
	// offset 668 bit 19
	bool is_enabled_spi_4 : 1 {};
	// Disable the electronic throttle motor and DC idle motor for testing.
	// This mode is for testing ETB/DC idle position sensors, etc without actually driving the throttle.
	// offset 668 bit 20
	bool pauseEtbControl : 1 {};
	// offset 668 bit 21
	bool alignEngineSnifferAtTDC : 1 {};
	// AEM X-Series or rusEFI Wideband
	// offset 668 bit 22
	bool enableAemXSeries : 1 {};
	// offset 668 bit 23
	bool unusedBit_197_23 : 1 {};
	// offset 668 bit 24
	bool unusedBit_197_24 : 1 {};
	// offset 668 bit 25
	bool unusedBit_197_25 : 1 {};
	// offset 668 bit 26
	bool unusedBit_197_26 : 1 {};
	// offset 668 bit 27
	bool unusedBit_197_27 : 1 {};
	// offset 668 bit 28
	bool unusedBit_197_28 : 1 {};
	// offset 668 bit 29
	bool unusedBit_197_29 : 1 {};
	// offset 668 bit 30
	bool unusedBit_197_30 : 1 {};
	// offset 668 bit 31
	bool unusedBit_197_31 : 1 {};
	// offset 672
	brain_input_pin_e logicAnalyzerPins[LOGIC_ANALYZER_CHANNEL_COUNT];
	// offset 680
	uint32_t verboseCanBaseAddress;
	// Boost Voltage
	// v
	// offset 684
	uint8_t mc33_hvolt;
	// Minimum MAP before closed loop boost is enabled. Use to prevent misbehavior upon entering boost.
	// kPa
	// offset 685
	uint8_t minimumBoostClosedLoopMap;
	// Optional Radiator Fan used with A/C
	// offset 686
	output_pin_e acFanPin;
	// offset 688
	pin_output_mode_e acFanPinMode;
	// offset 689
	spi_device_e l9779spiDevice;
	// volts
	// offset 690
	scaled_channel<uint8_t, 10, 1> dwellVoltageCorrVoltBins[DWELL_CURVE_SIZE];
	// multiplier
	// offset 698
	scaled_channel<uint8_t, 50, 1> dwellVoltageCorrValues[DWELL_CURVE_SIZE];
	// kg
	// offset 706
	uint16_t vehicleWeight;
	// How far above idle speed do we consider idling?
	// For example, if target = 800, this param = 200, then anything below 1000 RPM is considered idle.
	// RPM
	// offset 708
	int16_t idlePidRpmUpperLimit;
	// Apply nonlinearity correction below a pulse of this duration. Pulses longer than this duration will receive no adjustment.
	// ms
	// offset 710
	scaled_channel<uint16_t, 1000, 1> applyNonlinearBelowPulse;
	// offset 712
	Gpio lps25BaroSensorScl;
	// offset 714
	Gpio lps25BaroSensorSda;
	// offset 716
	brain_input_pin_e vehicleSpeedSensorInputPin;
	// Some vehicles have a switch to indicate that clutch pedal is all the way up
	// offset 718
	switch_input_pin_e clutchUpPin;
	// offset 720
	InjectorNonlinearMode injectorNonlinearMode;
	// offset 721
	pin_input_mode_e clutchUpPinMode;
	// offset 722
	Gpio max31855_cs[EGT_CHANNEL_COUNT];
	// Continental/GM flex fuel sensor, 50-150hz type
	// offset 738
	brain_input_pin_e flexSensorPin;
	// offset 740
	Gpio test557pin;
	// offset 742
	pin_output_mode_e stepperDirectionPinMode;
	// offset 743
	spi_device_e mc33972spiDevice;
	// Stoichiometric ratio for your secondary fuel. This value is used when the Flex Fuel sensor indicates E100, typically 9.0
	// :1
	// offset 744
	scaled_channel<uint8_t, 10, 1> stoichRatioSecondary;
	// Maximum allowed ETB position. Some throttles go past fully open, so this allows you to limit it to fully open.
	// %
	// offset 745
	uint8_t etbMaximumPosition;
	// Rate the ECU will log to the SD card, in hz (log lines per second).
	// hz
	// offset 746
	uint16_t sdCardLogFrequency;
	// offset 748
	Gpio debugMapAveraging;
	// offset 750
	output_pin_e starterRelayDisablePin;
	// On some vehicles we can disable starter once engine is already running
	// offset 752
	pin_output_mode_e starterRelayDisablePinMode;
	// offset 753
	imu_type_e imuType;
	// offset 754
	switch_input_pin_e startStopButtonPin;
	// This many MAP samples are used to estimate the current MAP. This many samples are considered, and the minimum taken. Recommended value is 1 for single-throttle engines, and your number of cylinders for individual throttle bodies.
	// count
	// offset 756
	int mapMinBufferLength;
	// Below this throttle position, the engine is considered idling. If you have an electronic throttle, this checks accelerator pedal position instead of throttle position, and should be set to 1-2%.
	// %
	// offset 760
	int16_t idlePidDeactivationTpsThreshold;
	// %
	// offset 762
	int16_t stepperParkingExtraSteps;
	// ADC
	// offset 764
	uint16_t tps1SecondaryMin;
	// ADC
	// offset 766
	uint16_t tps1SecondaryMax;
	// rpm
	// offset 768
	int16_t antiLagRpmTreshold;
	// Maximum time to crank starter when start/stop button is pressed
	// Seconds
	// offset 770
	uint16_t startCrankingDuration;
	// %
	// offset 772
	uint8_t lambdaProtectionMinTps;
	// Only respond once lambda is out of range for this period of time. Use to avoid transients triggering lambda protection when not needed
	// s
	// offset 773
	scaled_channel<uint8_t, 10, 1> lambdaProtectionTimeout;
	// RPM
	// offset 774
	scaled_channel<uint8_t, 1, 100> lambdaProtectionRestoreRpm;
	// offset 775
	pin_output_mode_e acRelayPinMode;
	// offset 776
	output_pin_e acRelayPin;
	// offset 778
	uint8_t unused754;
	// offset 779
	spi_device_e drv8860spiDevice;
	// offset 780
	script_setting_t scriptSetting[SCRIPT_SETTING_COUNT];
	// offset 812
	Gpio spi1mosiPin;
	// offset 814
	Gpio spi1misoPin;
	// offset 816
	Gpio spi1sckPin;
	// offset 818
	Gpio spi2mosiPin;
	// offset 820
	Gpio spi2misoPin;
	// offset 822
	Gpio spi2sckPin;
	// offset 824
	Gpio spi3mosiPin;
	// offset 826
	Gpio spi3misoPin;
	// offset 828
	Gpio spi3sckPin;
	// offset 830
	uart_device_e consoleUartDevice;
	// rusEFI console Sensor Sniffer mode
	// offset 831
	sensor_chart_e sensorChartMode;
	// offset 832 bit 0
	bool clutchUpPinInverted : 1 {};
	// offset 832 bit 1
	bool clutchDownPinInverted : 1 {};
	// If enabled we use two H-bridges to drive stepper idle air valve
	// offset 832 bit 2
	bool useHbridgesToDriveIdleStepper : 1 {};
	// offset 832 bit 3
	bool multisparkEnable : 1 {};
	// offset 832 bit 4
	bool enableLaunchRetard : 1 {};
	// Read VSS from OEM CAN bus according to selected CAN vehicle configuration.
	// offset 832 bit 5
	bool enableCanVss : 1 {};
	// offset 832 bit 6
	bool enableInnovateLC2 : 1 {};
	// offset 832 bit 7
	bool unused808b7 : 1 {};
	// If enabled, adjust at a constant rate instead of a rate proportional to the current lambda error. This mode may be easier to tune, and more tolerant of sensor noise.
	// offset 832 bit 8
	bool stftIgnoreErrorMagnitude : 1 {};
	// offset 832 bit 9
	bool enableSoftwareKnock : 1 {};
	// Verbose info in console below engineSnifferRpmThreshold
	// enable vvt_details
	// offset 832 bit 10
	bool verboseVVTDecoding : 1 {};
	// get invertCamVVTSignal
	// offset 832 bit 11
	bool invertCamVVTSignal : 1 {};
	// In Alpha-N mode, compensate for air temperature.
	// offset 832 bit 12
	bool alphaNUseIat : 1 {};
	// offset 832 bit 13
	bool knockBankCyl1 : 1 {};
	// offset 832 bit 14
	bool knockBankCyl2 : 1 {};
	// offset 832 bit 15
	bool knockBankCyl3 : 1 {};
	// offset 832 bit 16
	bool knockBankCyl4 : 1 {};
	// offset 832 bit 17
	bool knockBankCyl5 : 1 {};
	// offset 832 bit 18
	bool knockBankCyl6 : 1 {};
	// offset 832 bit 19
	bool knockBankCyl7 : 1 {};
	// offset 832 bit 20
	bool knockBankCyl8 : 1 {};
	// offset 832 bit 21
	bool knockBankCyl9 : 1 {};
	// offset 832 bit 22
	bool knockBankCyl10 : 1 {};
	// offset 832 bit 23
	bool knockBankCyl11 : 1 {};
	// offset 832 bit 24
	bool knockBankCyl12 : 1 {};
	// offset 832 bit 25
	bool tcuEnabled : 1 {};
	// offset 832 bit 26
	bool canBroadcastUseChannelTwo : 1 {};
	// If enabled we use four Push-Pull outputs to directly drive stepper idle air valve coilss
	// offset 832 bit 27
	bool useRawOutputToDriveIdleStepper : 1 {};
	// Print incoming and outgoing second bus CAN messages in FOME console
	// offset 832 bit 28
	bool verboseCan2 : 1 {};
	// offset 832 bit 29
	bool unusedBit_292_29 : 1 {};
	// offset 832 bit 30
	bool unusedBit_292_30 : 1 {};
	// offset 832 bit 31
	bool unusedBit_292_31 : 1 {};
	// offset 836
	dc_io etbIo[ETB_COUNT];
	// Wastegate control Solenoid
	// offset 852
	output_pin_e boostControlPin;
	// offset 854
	pin_output_mode_e boostControlPinMode;
	// offset 855
	boostType_e boostType;
	// offset 856
	switch_input_pin_e ALSActivatePin;
	// offset 858
	switch_input_pin_e launchActivatePin;
	// offset 860
	pid_s boostPid;
	// Hz
	// offset 880
	int boostPwmFrequency;
	// offset 884
	launchActivationMode_e launchActivationMode;
	// offset 885
	antiLagActivationMode_e antiLagActivationMode;
	// offset 886
	Gpio mc33816_flag0;
	// Disabled above this speed
	// Kph
	// offset 888
	int launchSpeedThreshold;
	// Range from Launch RPM for Timing Retard to activate
	// RPM
	// offset 892
	int launchTimingRpmRange;
	// Extra Fuel Added
	// %
	// offset 896
	int launchFuelAdded;
	// Duty Cycle for the Boost Solenoid
	// %
	// offset 900
	int launchBoostDuty;
	// Range from Launch RPM to activate Hard Cut
	// RPM
	// offset 904
	int hardCutRpmRange;
	// offset 908
	float turbochargerFilter;
	// offset 912
	int launchTpsThreshold;
	// offset 916
	float launchActivateDelay;
	// offset 920
	stft_s stft;
	// offset 944
	dc_io stepperDcIo[DC_PER_STEPPER];
	// For example, BMW, GM or Chevrolet
	// offset 960
	vehicle_info_t engineMake;
	// For example, LS1 or NB2
	// offset 992
	vehicle_info_t engineCode;
	// For example, Hunchback or Orange Miata
	// Vehicle name has to be unique between your vehicles.
	// offset 1024
	vehicle_info_t vehicleName;
	// offset 1056
	output_pin_e tcu_solenoid[TCU_SOLENOID_COUNT];
	// offset 1068
	dc_function_e etbFunctions[ETB_COUNT];
	// offset 1070
	Gpio drv8860_cs;
	// offset 1072
	pin_output_mode_e drv8860_csPinMode;
	// offset 1073
	idle_mode_e idleMode;
	// offset 1074
	Gpio drv8860_miso;
	// volt
	// offset 1076
	scaled_channel<uint16_t, 1000, 1> fuelLevelBins[FUEL_LEVEL_TABLE_COUNT];
	// offset 1092
	output_pin_e luaOutputPins[LUA_PWM_COUNT];
	// Angle between cam sensor and VVT zero position
	// value
	// offset 1108
	scaled_channel<int16_t, 10, 1> vvtOffsets[CAM_INPUTS_COUNT];
	// offset 1116
	vr_threshold_s vrThreshold[VR_THRESHOLD_COUNT];
	// offset 1148
	gppwm_note_t gpPwmNote[GPPWM_CHANNELS];
	// ADC
	// offset 1212
	uint16_t tps2SecondaryMin;
	// ADC
	// offset 1214
	uint16_t tps2SecondaryMax;
	// Select which bus the wideband controller is attached to.
	// offset 1216 bit 0
	bool widebandOnSecondBus : 1 {};
	// Enables lambda sensor closed loop feedback for fuelling.
	// offset 1216 bit 1
	bool fuelClosedLoopCorrectionEnabled : 1 {};
	// offset 1216 bit 2
	bool unused1192b2 : 1 {};
	// offset 1216 bit 3
	bool boardUseTachPullUp : 1 {};
	// offset 1216 bit 4
	bool boardUseTempPullUp : 1 {};
	// offset 1216 bit 5
	bool yesUnderstandLocking : 1 {};
	// Sometimes we have a performance issue while printing error
	// offset 1216 bit 6
	bool silentTriggerError : 1 {};
	// offset 1216 bit 7
	bool useLinearCltSensor : 1 {};
	// enable can_read/disable can_read
	// offset 1216 bit 8
	bool canReadEnabled : 1 {};
	// enable can_write/disable can_write
	// offset 1216 bit 9
	bool canWriteEnabled : 1 {};
	// offset 1216 bit 10
	bool useLinearIatSensor : 1 {};
	// offset 1216 bit 11
	bool boardUse2stepPullDown : 1 {};
	// Treat milliseconds value as duty cycle value, i.e. 0.5ms would become 50%
	// offset 1216 bit 12
	bool tachPulseDurationAsDutyCycle : 1 {};
	// This enables smart alternator control and activates the extra alternator settings.
	// offset 1216 bit 13
	bool isAlternatorControlEnabled : 1 {};
	// Invert the signal from the primary trigger sensor.
	// offset 1216 bit 14
	bool invertPrimaryTriggerSignal : 1 {};
	// Invert the signal from the secondary trigger sensor.
	// offset 1216 bit 15
	bool invertSecondaryTriggerSignal : 1 {};
	// offset 1216 bit 16
	bool cutFuelOnHardLimit : 1 {};
	// Be careful enabling this: some engines are known to self-disassemble their valvetrain with a spark cut. Fuel cut is much safer.
	// offset 1216 bit 17
	bool cutSparkOnHardLimit : 1 {};
	// offset 1216 bit 18
	bool launchFuelCutEnable : 1 {};
	// This is the Cut Mode normally used
	// offset 1216 bit 19
	bool launchSparkCutEnable : 1 {};
	// offset 1216 bit 20
	bool boardUseCrankPullUp : 1 {};
	// offset 1216 bit 21
	bool boardUseCamPullDown : 1 {};
	// offset 1216 bit 22
	bool boardUseCamVrPullUp : 1 {};
	// offset 1216 bit 23
	bool boardUseD2PullDown : 1 {};
	// offset 1216 bit 24
	bool boardUseD3PullDown : 1 {};
	// offset 1216 bit 25
	bool boardUseD4PullDown : 1 {};
	// offset 1216 bit 26
	bool boardUseD5PullDown : 1 {};
	// offset 1216 bit 27
	bool verboseIsoTp : 1 {};
	// offset 1216 bit 28
	bool engineSnifferFocusOnInputs : 1 {};
	// offset 1216 bit 29
	bool launchActivateInverted : 1 {};
	// offset 1216 bit 30
	bool twoStroke : 1 {};
	// Where is your primary skipped wheel located?
	// offset 1216 bit 31
	bool skippedWheelOnCam : 1 {};
	// A/C button input
	// offset 1220
	switch_input_pin_e acSwitch;
	// offset 1222
	adc_channel_e vRefAdcChannel;
	// Expected neutral position
	// %
	// offset 1223
	uint8_t etbNeutralPosition;
	// offset 1224 bit 0
	bool isInjectionEnabled : 1 {};
	// offset 1224 bit 1
	bool isIgnitionEnabled : 1 {};
	// When enabled if TPS is held above 95% no fuel is injected while cranking to clear excess fuel from the cylinders.
	// offset 1224 bit 2
	bool isCylinderCleanupEnabled : 1 {};
	// Should we use tables to vary tau/beta based on CLT/MAP, or just with fixed values?
	// offset 1224 bit 3
	bool complexWallModel : 1 {};
	// offset 1224 bit 4
	bool alwaysInstantRpm : 1 {};
	// offset 1224 bit 5
	bool isMapAveragingEnabled : 1 {};
	// If enabled, use separate temperature multiplier table for cranking idle position.
	// If disabled, use normal running multiplier table applied to the cranking base position.
	// offset 1224 bit 6
	bool overrideCrankingIacSetting : 1 {};
	// This activates a separate ignition timing table for idle conditions, this can help idle stability by using ignition retard and advance either side of the desired idle speed. Extra retard at low idle speeds will prevent stalling and extra advance at high idle speeds can help reduce engine power and slow the idle speed.
	// offset 1224 bit 7
	bool useSeparateAdvanceForIdle : 1 {};
	// offset 1224 bit 8
	bool isWaveAnalyzerEnabled : 1 {};
	// This activates a separate fuel table for Idle, this allows fine tuning of the idle fuelling.
	// offset 1224 bit 9
	bool useSeparateVeForIdle : 1 {};
	// Verbose info in console below engineSnifferRpmThreshold
	// enable trigger_details
	// offset 1224 bit 10
	bool verboseTriggerSynchDetails : 1 {};
	// Usually if we have no trigger events that means engine is stopped
	// Unless we are troubleshooting and spinning the engine by hand - this case a longer
	// delay is needed
	// offset 1224 bit 11
	bool isManualSpinningMode : 1 {};
	// offset 1224 bit 12
	bool unused1200b12 : 1 {};
	// offset 1224 bit 13
	bool neverInstantRpm : 1 {};
	// offset 1224 bit 14
	bool unused1200b14 : 1 {};
	// offset 1224 bit 15
	bool useFixedBaroCorrFromMap : 1 {};
	// In Constant mode, timing is automatically tapered to running as RPM increases.
	// In Table mode, the "Cranking ignition advance" table is used directly.
	// offset 1224 bit 16
	bool useSeparateAdvanceForCranking : 1 {};
	// This enables the various ignition corrections during cranking (IAT, CLT, FSIO and PID idle).
	// You probably don't need this.
	// offset 1224 bit 17
	bool useAdvanceCorrectionsForCranking : 1 {};
	// Enable a second cranking table to use for E100 flex fuel, interpolating between the two based on flex fuel sensor.
	// offset 1224 bit 18
	bool flexCranking : 1 {};
	// This flag allows to use a special 'PID Multiplier' table (0.0-1.0) to compensate for nonlinear nature of IAC-RPM controller
	// offset 1224 bit 19
	bool useIacPidMultTable : 1 {};
	// offset 1224 bit 20
	bool isBoostControlEnabled : 1 {};
	// Interpolates the Ignition Retard from 0 to 100% within the RPM Range
	// offset 1224 bit 21
	bool launchSmoothRetard : 1 {};
	// Some engines are OK running semi-random sequential while other engine require phase synchronization
	// offset 1224 bit 22
	bool isPhaseSyncRequiredForIgnition : 1 {};
	// If enabled, use a curve for RPM limit (based on coolant temperature) instead of a constant value.
	// offset 1224 bit 23
	bool useCltBasedRpmLimit : 1 {};
	// If enabled, don't wait for engine start to heat O2 sensors. WARNING: this will reduce the life of your sensor, as condensation in the exhaust from a cold start can crack the sensing element.
	// offset 1224 bit 24
	bool forceO2Heating : 1 {};
	// If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	// offset 1224 bit 25
	bool invertVvtControlIntake : 1 {};
	// If increased VVT duty cycle increases the indicated VVT angle, set this to 'advance'. If it decreases, set this to 'retard'. Most intake cams use 'advance', and most exhaust cams use 'retard'.
	// offset 1224 bit 26
	bool invertVvtControlExhaust : 1 {};
	// offset 1224 bit 27
	bool useBiQuadOnAuxSpeedSensors : 1 {};
	// 'Trigger' mode will write a high speed log of trigger events (warning: uses lots of space!). 'Normal' mode will write a standard MLG of sensors, engine function, etc. similar to the one captured in TunerStudio.
	// offset 1224 bit 28
	bool sdTriggerLog : 1 {};
	// offset 1224 bit 29
	bool ALSActivateInverted : 1 {};
	// offset 1224 bit 30
	bool unusedBit_397_30 : 1 {};
	// offset 1224 bit 31
	bool unusedBit_397_31 : 1 {};
	// count
	// offset 1228
	uint32_t engineChartSize;
	// mult
	// offset 1232
	float turboSpeedSensorMultiplier;
	// offset 1236
	Gpio camInputsDebug[CAM_INPUTS_COUNT];
	// Extra idle target speed when A/C is enabled. Some cars need the extra speed to keep the AC efficient while idling.
	// RPM
	// offset 1244
	int16_t acIdleRpmBump;
	// seconds
	// offset 1246
	int16_t warningPeriod;
	// angle
	// offset 1248
	float knockDetectionWindowStart;
	// angle
	// offset 1252
	float knockDetectionWindowEnd;
	// ms
	// offset 1256
	float idleStepperReactionTime;
	// count
	// offset 1260
	int idleStepperTotalSteps;
	// TODO: finish this #413
	// sec
	// offset 1264
	float noAccelAfterHardLimitPeriodSecs;
	// At what trigger index should some MAP-related math be executed? This is a performance trick to reduce load on synchronization trigger callback.
	// index
	// offset 1268
	int mapAveragingSchedulingAtIndex;
	// Duration in ms or duty cycle depending on selected mode
	// offset 1272
	float tachPulseDuractionMs;
	// Length of time the deposited wall fuel takes to dissipate after the start of acceleration.
	// Seconds
	// offset 1276
	float wwaeTau;
	// offset 1280
	pid_s alternatorControl;
	// offset 1300
	pid_s etb;
	// offset 1320
	Gpio triggerInputDebugPins[TRIGGER_INPUT_PIN_COUNT];
	// RPM range above upper limit for extra air taper
	// RPM
	// offset 1324
	int16_t airTaperRpmRange;
	// offset 1326
	brain_input_pin_e turboSpeedSensorInputPin;
	// ADC
	// offset 1328
	int16_t tps2Min;
	// ADC
	// offset 1330
	int16_t tps2Max;
	// offset 1332
	output_pin_e starterControlPin;
	// offset 1334
	pin_input_mode_e startStopButtonMode;
	// Pulse
	// offset 1335
	uint8_t tachPulsePerRev;
	// kPa value which is too low to be true
	// kPa
	// offset 1336
	float mapErrorDetectionTooLow;
	// kPa value which is too high to be true
	// kPa
	// offset 1340
	float mapErrorDetectionTooHigh;
	// How long to wait for the spark to fire before recharging the coil for another spark.
	// ms
	// offset 1344
	scaled_channel<uint16_t, 1000, 1> multisparkSparkDuration;
	// This sets the dwell time for subsequent sparks. The main spark's dwell is set by the dwell table.
	// ms
	// offset 1346
	scaled_channel<uint16_t, 1000, 1> multisparkDwell;
	// offset 1348
	pid_s idleRpmPid;
	// 0 = No fuel settling on port walls 1 = All the fuel settling on port walls setting this to 0 disables the wall wetting enrichment.
	// Fraction
	// offset 1368
	float wwaeBeta;
	// offset 1372
	Gpio auxValves[AUX_DIGITAL_VALVE_COUNT];
	// offset 1376
	switch_input_pin_e tcuUpshiftButtonPin;
	// offset 1378
	switch_input_pin_e tcuDownshiftButtonPin;
	// voltage
	// offset 1380
	float throttlePedalUpVoltage;
	// Pedal in the floor
	// voltage
	// offset 1384
	float throttlePedalWOTVoltage;
	// on IGN voltage detection turn fuel pump on to build fuel pressure
	// seconds
	// offset 1388
	int16_t startUpFuelPumpDuration;
	// If the RPM closer to target than this value, disable closed loop idle correction to prevent oscillation
	// RPM
	// offset 1390
	int16_t idlePidRpmDeadZone;
	// This is the target battery voltage the alternator PID control will attempt to maintain
	// Volts
	// offset 1392
	float targetVBatt;
	// offset 1396
	uint32_t unused1372;
	// This is the duration in cycles that the IAC will take to reach its normal idle position, it can be used to hold the idle higher for a few seconds after cranking to improve startup.
	// cycles
	// offset 1400
	int16_t afterCrankingIACtaperDuration;
	// Extra IAC, in percent between 0 and 100, tapered between zero and idle deactivation TPS value
	// percent
	// offset 1402
	int16_t iacByTpsTaper;
	// Auxiliary sensor serial, not to be confused with secondary calibration serial
	// offset 1404
	Gpio auxSerialTxPin;
	// Auxiliary sensor serial, not to be confused with secondary calibration serial
	// offset 1406
	Gpio auxSerialRxPin;
	// offset 1408
	Gpio LIS302DLCsPin;
	// How long to look back for TPS-based acceleration enrichment. Increasing this time will trigger enrichment for longer when a throttle position change occurs.
	// sec
	// offset 1410
	scaled_channel<uint8_t, 20, 1> tpsAccelLookback;
	// Below this speed, disable DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	// kph
	// offset 1411
	uint8_t coastingFuelCutVssLow;
	// Above this speed, allow DFCO. Use this to prevent jerkiness from fuel enable/disable in low gears.
	// kph
	// offset 1412
	uint8_t coastingFuelCutVssHigh;
	// Pause closed loop fueling after deceleration fuel cut occurs. Set this to a little longer than however long is required for normal fueling behavior to resume after fuel cut.
	// sec
	// offset 1413
	scaled_channel<uint8_t, 10, 1> noFuelTrimAfterDfcoTime;
	// Duration of each test pulse
	// ms
	// offset 1414
	scaled_channel<uint8_t, 10, 1> ignTestOnTime;
	// offset 1415
	uint8_t alignmentFill_at_1415[1];
	// Maximum change delta of TPS percentage over the 'length'. Actual TPS change has to be above this value in order for TPS/TPS acceleration to kick in.
	// roc
	// offset 1416
	float tpsAccelEnrichmentThreshold;
	// offset 1420
	brain_input_pin_e auxSpeedSensorInputPin[2];
	// offset 1424
	uint8_t totalGearsCount;
	// Sets what part of injection's is controlled by the injection phase table.
	// offset 1425
	InjectionTimingMode injectionTimingMode;
	// See https://wiki.fome.tech/r/debugmode
	// offset 1426
	debug_mode_e debugMode;
	// Additional idle % when fan #1 is active
	// %
	// offset 1427
	uint8_t fan1ExtraIdle;
	// Band rate for primary TTL
	// BPs
	// offset 1428
	uint32_t uartConsoleSerialSpeed;
	// For decel we simply multiply delta of TPS and tFor decel we do not use table?!
	// roc
	// offset 1432
	float tpsDecelEnleanmentThreshold;
	// Magic multiplier, we multiply delta of TPS and get fuel squirt duration
	// coeff
	// offset 1436
	float tpsDecelEnleanmentMultiplier;
	// BPs
	// offset 1440
	uint32_t auxSerialSpeed;
	// voltage
	// offset 1444
	float throttlePedalSecondaryUpVoltage;
	// Pedal in the floor
	// voltage
	// offset 1448
	float throttlePedalSecondaryWOTVoltage;
	// set can_baudrate
	// offset 1452
	can_baudrate_e canBaudRate;
	// Override the Y axis (load) value used for the VE table.
	// Advanced users only: If you aren't sure you need this, you probably don't need this.
	// offset 1453
	ve_override_e veOverrideMode;
	// offset 1454
	can_baudrate_e can2BaudRate;
	// Override the Y axis (load) value used for the AFR table.
	// Advanced users only: If you aren't sure you need this, you probably don't need this.
	// offset 1455
	load_override_e afrOverrideMode;
	// A
	// offset 1456
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_peak;
	// A
	// offset 1457
	scaled_channel<uint8_t, 10, 1> mc33_hpfp_i_hold;
	// How long to deactivate power when hold current is reached before applying power again
	// us
	// offset 1458
	uint8_t mc33_hpfp_i_hold_off;
	// Maximum amount of time the solenoid can be active before assuming a programming error
	// ms
	// offset 1459
	uint8_t mc33_hpfp_max_hold;
	// Enable if DC-motor driver (H-bridge) inverts the signals (eg. RZ7899 on Hellen boards)
	// offset 1460 bit 0
	bool stepperDcInvertedPins : 1 {};
	// Allow OpenBLT on Primary CAN
	// offset 1460 bit 1
	bool canOpenBLT : 1 {};
	// Allow OpenBLT on Secondary CAN
	// offset 1460 bit 2
	bool can2OpenBLT : 1 {};
	// Select whether to configure injector flow in volumetric flow (defualt, cc/min) or mass flow (g/s).
	// offset 1460 bit 3
	bool injectorFlowAsMassFlow : 1 {};
	// offset 1460 bit 4
	bool unusedBit_472_4 : 1 {};
	// offset 1460 bit 5
	bool unusedBit_472_5 : 1 {};
	// offset 1460 bit 6
	bool unusedBit_472_6 : 1 {};
	// offset 1460 bit 7
	bool unusedBit_472_7 : 1 {};
	// offset 1460 bit 8
	bool unusedBit_472_8 : 1 {};
	// offset 1460 bit 9
	bool unusedBit_472_9 : 1 {};
	// offset 1460 bit 10
	bool unusedBit_472_10 : 1 {};
	// offset 1460 bit 11
	bool unusedBit_472_11 : 1 {};
	// offset 1460 bit 12
	bool unusedBit_472_12 : 1 {};
	// offset 1460 bit 13
	bool unusedBit_472_13 : 1 {};
	// offset 1460 bit 14
	bool unusedBit_472_14 : 1 {};
	// offset 1460 bit 15
	bool unusedBit_472_15 : 1 {};
	// offset 1460 bit 16
	bool unusedBit_472_16 : 1 {};
	// offset 1460 bit 17
	bool unusedBit_472_17 : 1 {};
	// offset 1460 bit 18
	bool unusedBit_472_18 : 1 {};
	// offset 1460 bit 19
	bool unusedBit_472_19 : 1 {};
	// offset 1460 bit 20
	bool unusedBit_472_20 : 1 {};
	// offset 1460 bit 21
	bool unusedBit_472_21 : 1 {};
	// offset 1460 bit 22
	bool unusedBit_472_22 : 1 {};
	// offset 1460 bit 23
	bool unusedBit_472_23 : 1 {};
	// offset 1460 bit 24
	bool unusedBit_472_24 : 1 {};
	// offset 1460 bit 25
	bool unusedBit_472_25 : 1 {};
	// offset 1460 bit 26
	bool unusedBit_472_26 : 1 {};
	// offset 1460 bit 27
	bool unusedBit_472_27 : 1 {};
	// offset 1460 bit 28
	bool unusedBit_472_28 : 1 {};
	// offset 1460 bit 29
	bool unusedBit_472_29 : 1 {};
	// offset 1460 bit 30
	bool unusedBit_472_30 : 1 {};
	// offset 1460 bit 31
	bool unusedBit_472_31 : 1 {};
	// Time between bench test pulses
	// ms
	// offset 1464
	scaled_channel<uint8_t, 1, 5> benchTestOffTime;
	// %
	// offset 1465
	uint8_t lambdaProtectionRestoreTps;
	// %
	// offset 1466
	scaled_channel<uint8_t, 1, 10> lambdaProtectionRestoreLoad;
	// offset 1467
	pin_input_mode_e launchActivatePinMode;
	// set_can2_tx_pin X
	// offset 1468
	Gpio can2TxPin;
	// set_can2_rx_pin X
	// offset 1470
	Gpio can2RxPin;
	// offset 1472
	pin_output_mode_e starterControlPinMode;
	// offset 1473
	adc_channel_e wastegatePositionSensor;
	// Override the Y axis (load) value used for the ignition table.
	// Advanced users only: If you aren't sure you need this, you probably don't need this.
	// offset 1474
	load_override_e ignOverrideMode;
	// Select which fuel pressure sensor measures the pressure of the fuel at your injectors.
	// offset 1475
	injector_pressure_type_e injectorPressureType;
	// offset 1476
	output_pin_e hpfpValvePin;
	// offset 1478
	pin_output_mode_e hpfpValvePinMode;
	// offset 1479
	spi_device_e accelerometerSpiDevice;
	// MAP value above which fuel is cut in case of overboost.
	// Set to 0 to disable overboost cut.
	// kPa (absolute)
	// offset 1480
	float boostCutPressure;
	// kg/h
	// offset 1484
	scaled_channel<uint8_t, 1, 5> tchargeBins[16];
	// ratio
	// offset 1500
	scaled_channel<uint8_t, 100, 1> tchargeValues[16];
	// Fixed timing, useful for TDC testing
	// deg
	// offset 1516
	float fixedTiming;
	// MAP voltage for low point
	// v
	// offset 1520
	float mapLowValueVoltage;
	// MAP voltage for low point
	// v
	// offset 1524
	float mapHighValueVoltage;
	// EGO value correction
	// value
	// offset 1528
	float egoValueShift;
	// VVT output solenoid pin for this cam
	// offset 1532
	output_pin_e vvtPins[CAM_INPUTS_COUNT];
	// This is the IAC position during cranking, some engines start better if given more air during cranking to improve cylinder filling.
	// percent
	// offset 1540
	int crankingIACposition;
	// offset 1544
	float tChargeMinRpmMinTps;
	// offset 1548
	float tChargeMinRpmMaxTps;
	// offset 1552
	float tChargeMaxRpmMinTps;
	// offset 1556
	float tChargeMaxRpmMaxTps;
	// offset 1560
	pwm_freq_t vvtOutputFrequency;
	// Minimim timing advance allowed. No spark on any cylinder will ever fire after this angle BTDC. For example, setting -10 here means no spark ever fires later than 10 deg ATDC. Note that this only concerns the primary spark: any trailing sparks or multispark may violate this constraint.
	// deg BTDC
	// offset 1562
	int8_t minimumIgnitionTiming;
	// Maximum timing advance allowed. No spark on any cylinder will ever fire before this angle BTDC. For example, setting 45 here means no spark ever fires earlier than 45 deg BTDC
	// deg BTDC
	// offset 1563
	int8_t maximumIgnitionTiming;
	// Hz
	// offset 1564
	int alternatorPwmFrequency;
	// offset 1568
	vvt_mode_e vvtMode[CAMS_PER_BANK];
	// Additional idle % when fan #2 is active
	// %
	// offset 1570
	uint8_t fan2ExtraIdle;
	// Delay to allow fuel pressure to build before firing the priming pulse.
	// sec
	// offset 1571
	scaled_channel<uint8_t, 100, 1> primingDelay;
	// offset 1572
	adc_channel_e auxAnalogInputs[LUA_ANALOG_INPUT_COUNT];
	// offset 1580
	output_pin_e trailingCoilPins[MAX_CYLINDER_COUNT];
	// offset 1604
	tle8888_mode_e tle8888mode;
	// offset 1605
	pin_output_mode_e LIS302DLCsPinMode;
	// None = I have a MAP-referenced fuel pressure regulator
	// Fixed rail pressure = I have an atmosphere-referenced fuel pressure regulator (returnless, typically)
	// Sensed rail pressure = I have a fuel pressure sensor
	// offset 1606
	injector_compensation_mode_e injectorCompensationMode;
	// offset 1607
	pin_output_mode_e fan2PinMode;
	// This is the pressure at which your injector flow is known.
	// For example if your injectors flow 400cc/min at 3.5 bar, enter 350kpa here.
	// kPa
	// offset 1608
	float fuelReferencePressure;
	// Fuel multiplier (enrichment) immediately after engine start
	// mult
	// offset 1612
	float postCrankingFactor;
	// Time over which to taper out after start enrichment
	// seconds
	// offset 1616
	float postCrankingDurationSec;
	// offset 1620
	ThermistorConf auxTempSensor1;
	// offset 1652
	ThermistorConf auxTempSensor2;
	// Deg
	// offset 1684
	int16_t knockSamplingDuration;
	// Hz
	// offset 1686
	int16_t etbFreq;
	// offset 1688
	pid_s etbWastegatePid;
	// For micro-stepping, make sure that PWM frequency (etbFreq) is high enough
	// offset 1708
	stepper_num_micro_steps_e stepperNumMicroSteps;
	// Use to limit the current when the stepper motor is idle, not moving (100% = no limit)
	// %
	// offset 1709
	uint8_t stepperMinDutyCycle;
	// Use to limit the max.current through the stepper motor (100% = no limit)
	// %
	// offset 1710
	uint8_t stepperMaxDutyCycle;
	// offset 1711
	spi_device_e sdCardSpiDevice;
	// per-cylinder timing correction
	// deg
	// offset 1712
	angle_t timing_offset_cylinder[MAX_CYLINDER_COUNT];
	// seconds
	// offset 1760
	float idlePidActivationTime;
	// offset 1764
	pin_mode_e spi1SckMode;
	// Modes count be used for 3v<>5v integration using pull-ups/pull-downs etc.
	// offset 1765
	pin_mode_e spi1MosiMode;
	// offset 1766
	pin_mode_e spi1MisoMode;
	// offset 1767
	pin_mode_e spi2SckMode;
	// offset 1768
	pin_mode_e spi2MosiMode;
	// offset 1769
	pin_mode_e spi2MisoMode;
	// offset 1770
	pin_mode_e spi3SckMode;
	// offset 1771
	pin_mode_e spi3MosiMode;
	// offset 1772
	pin_mode_e spi3MisoMode;
	// offset 1773
	pin_output_mode_e stepperEnablePinMode;
	// ResetB
	// offset 1774
	Gpio mc33816_rstb;
	// offset 1776
	Gpio mc33816_driven;
	// Brake pedal switch
	// offset 1778
	switch_input_pin_e brakePedalPin;
	// offset 1780
	pin_input_mode_e brakePedalPinMode;
	// offset 1781
	tChargeMode_e tChargeMode;
	// offset 1782
	pin_input_mode_e tcuUpshiftButtonPinMode;
	// offset 1783
	pin_input_mode_e tcuDownshiftButtonPinMode;
	// VVT output PID
	// TODO: rename to vvtPid
	// offset 1784
	pid_s auxPid[CAMS_PER_BANK];
	// offset 1824
	float injectorCorrectionPolynomial[8];
	// C
	// offset 1856
	int8_t primeBins[8];
	// offset 1864
	linear_sensor_s oilPressure;
	// offset 1884
	output_pin_e fan2Pin;
	// Cooling fan turn-on temperature threshold, in Celsius
	// deg C
	// offset 1886
	uint8_t fan2OnTemperature;
	// Cooling fan turn-off temperature threshold, in Celsius
	// deg C
	// offset 1887
	uint8_t fan2OffTemperature;
	// offset 1888
	Gpio stepperEnablePin;
	// offset 1890
	Gpio tle8888_cs;
	// offset 1892
	pin_output_mode_e tle8888_csPinMode;
	// offset 1893
	uint8_t alignmentFill_at_1893[1];
	// offset 1894
	can_vss_nbc_e canVssNbcType;
	// offset 1896
	Gpio mc33816_cs;
	// offset 1898
	uint8_t alignmentFill_at_1898[2];
	// hz
	// offset 1900
	float auxFrequencyFilter;
	// RPM
	// offset 1904
	int16_t vvtControlMinRpm;
	// offset 1906
	brain_input_pin_e sentInputPins[SENT_INPUT_COUNT];
	// %
	// offset 1908
	int8_t launchFuelAdderPercent;
	// Time required to detect a stuck throttle.
	// sec
	// offset 1909
	scaled_channel<uint8_t, 50, 1> etbJamTimeout;
	// By the way ETB PID runs at 500hz, length in 1/500 of second here.
	// offset 1910
	uint16_t etbExpAverageLength;
	// This sets the RPM above which fuel cut is active.
	// rpm
	// offset 1912
	int16_t coastingFuelCutRpmHigh;
	// This sets the RPM below which fuel cut is deactivated, this prevents jerking or issues transitioning to idle
	// rpm
	// offset 1914
	int16_t coastingFuelCutRpmLow;
	// Throttle position below which fuel cut is active. With an electronic throttle enabled, this checks against pedal position.
	// %
	// offset 1916
	int16_t coastingFuelCutTps;
	// Fuel cutoff is disabled when the engine is cold.
	// C
	// offset 1918
	int16_t coastingFuelCutClt;
	// Increases PID reaction for RPM<target by adding extra percent to PID-error
	// %
	// offset 1920
	int16_t pidExtraForLowRpm;
	// MAP value above which fuel injection is re-enabled.
	// kPa
	// offset 1922
	int16_t coastingFuelCutMap;
	// offset 1924
	linear_sensor_s highPressureFuel;
	// offset 1944
	linear_sensor_s lowPressureFuel;
	// C
	// offset 1964
	int8_t cltRevLimitRpmBins[CLT_LIMITER_CURVE_SIZE];
	// RPM
	// offset 1968
	uint16_t cltRevLimitRpm[CLT_LIMITER_CURVE_SIZE];
	// offset 1976
	gppwm_note_t scriptCurveName[SCRIPT_CURVE_COUNT];
	// offset 2072
	gppwm_note_t scriptTableName[SCRIPT_TABLE_COUNT];
	// offset 2136
	gppwm_note_t scriptSettingName[SCRIPT_SETTING_COUNT];
	// Heat transfer coefficient at zero flow.
	// 0 means the air charge is fully heated to the same temperature as CLT.
	// 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	// offset 2264
	float tChargeAirCoefMin;
	// Heat transfer coefficient at high flow, as defined by "max air flow".
	// 0 means the air charge is fully heated to the same temperature as CLT.
	// 1 means the air charge gains no heat, and enters the cylinder at the temperature measured by IAT.
	// offset 2268
	float tChargeAirCoefMax;
	// High flow point for heat transfer estimation.
	// Set this to perhaps 50-75% of your maximum airflow at wide open throttle.
	// kg/h
	// offset 2272
	float tChargeAirFlowMax;
	// Maximum allowed rate of increase allowed for the estimated charge temperature
	// deg/sec
	// offset 2276
	float tChargeAirIncrLimit;
	// Maximum allowed rate of decrease allowed for the estimated charge temperature
	// deg/sec
	// offset 2280
	float tChargeAirDecrLimit;
	// iTerm min value
	// offset 2284
	int16_t etb_iTermMin;
	// iTerm max value
	// offset 2286
	int16_t etb_iTermMax;
	// offset 2288
	pid_s idleTimingPid;
	// By the way ETB PID runs at 500hz, length in 1/500 of second here.
	// offset 2308
	int16_t etbRocExpAverageLength;
	// A delay in cycles between fuel-enrich. portions
	// cycles
	// offset 2310
	int16_t tpsAccelFractionPeriod;
	// A fraction divisor: 1 or less = entire portion at once, or split into diminishing fractions
	// coef
	// offset 2312
	float tpsAccelFractionDivisor;
	// offset 2316
	spi_device_e tle8888spiDevice;
	// offset 2317
	spi_device_e mc33816spiDevice;
	// iTerm min value
	// offset 2318
	int16_t idlerpmpid_iTermMin;
	// offset 2320
	spi_device_e tle6240spiDevice;
	// Stoichiometric ratio for your primary fuel. When Flex Fuel is enabled, this value is used when the Flex Fuel sensor indicates E0.
	// E0 = 14.7
	// E10 = 14.1
	// E85 = 9.9
	// E100 = 9.0
	// :1
	// offset 2321
	scaled_channel<uint8_t, 10, 1> stoichRatioPrimary;
	// iTerm max value
	// offset 2322
	int16_t idlerpmpid_iTermMax;
	// This sets the range of the idle control on the ETB. At 100% idle position, the value specified here sets the base ETB position.
	// %
	// offset 2324
	float etbIdleThrottleRange;
	// Select which fuel correction bank this cylinder belongs to. Group cylinders that share the same O2 sensor
	// offset 2328
	uint8_t cylinderBankSelect[MAX_CYLINDER_COUNT];
	// mg
	// offset 2340
	scaled_channel<uint8_t, 1, 5> primeValues[8];
	// Trigger comparator center point voltage
	// V
	// offset 2348
	scaled_channel<uint8_t, 50, 1> triggerCompCenterVolt;
	// Trigger comparator hysteresis voltage (Min)
	// V
	// offset 2349
	scaled_channel<uint8_t, 50, 1> triggerCompHystMin;
	// Trigger comparator hysteresis voltage (Max)
	// V
	// offset 2350
	scaled_channel<uint8_t, 50, 1> triggerCompHystMax;
	// VR-sensor saturation RPM
	// RPM
	// offset 2351
	scaled_channel<uint8_t, 1, 50> triggerCompSensorSatRpm;
	// offset 2352
	gppwm_channel gppwm[GPPWM_CHANNELS];
	// Boost Current
	// mA
	// offset 2784
	uint16_t mc33_i_boost;
	// Peak Current
	// mA
	// offset 2786
	uint16_t mc33_i_peak;
	// Hold Current
	// mA
	// offset 2788
	uint16_t mc33_i_hold;
	// Maximum allowed boost phase time. If the injector current doesn't reach the threshold before this time elapses, it is assumed that the injector is missing or has failed open circuit.
	// us
	// offset 2790
	uint16_t mc33_t_max_boost;
	// us
	// offset 2792
	uint16_t mc33_t_peak_off;
	// Peak phase duration
	// us
	// offset 2794
	uint16_t mc33_t_peak_tot;
	// us
	// offset 2796
	uint16_t mc33_t_bypass;
	// us
	// offset 2798
	uint16_t mc33_t_hold_off;
	// Hold phase duration
	// us
	// offset 2800
	uint16_t mc33_t_hold_tot;
	// offset 2802
	pin_output_mode_e tcu_solenoid_mode[TCU_SOLENOID_COUNT];
	// Knock sensor output knock detection threshold depending on current RPM.
	// dB
	// offset 2808
	scaled_channel<int8_t, 2, 1> knockBaseNoise[ENGINE_NOISE_CURVE_SIZE];
	// ratio
	// offset 2824
	float triggerGapOverrideFrom[GAP_TRACKING_LENGTH];
	// ratio
	// offset 2896
	float triggerGapOverrideTo[GAP_TRACKING_LENGTH];
	// Below this RPM, use camshaft information to synchronize the crank's position for full sequential operation. Use this if your cam sensor does weird things at high RPM. Set to 0 to disable, and always use cam to help sync crank.
	// rpm
	// offset 2968
	scaled_channel<uint8_t, 1, 50> maxCamPhaseResolveRpm;
	// Delay before cutting fuel. Set to 0 to cut immediately with no delay. May cause rumbles and pops out of your exhaust...
	// sec
	// offset 2969
	scaled_channel<uint8_t, 10, 1> dfcoDelay;
	// Delay before engaging the AC compressor. Set to 0 to engage immediately with no delay. Use this to prevent bogging at idle when AC engages.
	// sec
	// offset 2970
	scaled_channel<uint8_t, 10, 1> acDelay;
	// offset 2971
	pin_input_mode_e acSwitchMode;
	// mg
	// offset 2972
	scaled_channel<uint16_t, 1000, 1> fordInjectorSmallPulseBreakPoint;
	// multiplier
	// offset 2974
	scaled_channel<uint8_t, 50, 1> tpsTspCorrValues[TPS_TPS_ACCEL_CLT_CORR_TABLE];
	// %
	// offset 2978
	uint8_t etbJamIntegratorLimit;
	// lobes/cam
	// offset 2979
	uint8_t hpfpCamLobes;
	// offset 2980
	hpfp_cam_e hpfpCam;
	// Crank angle ATDC of first lobe peak
	// deg
	// offset 2981
	uint8_t hpfpPeakPos;
	// If the requested activation time is below this angle, don't bother running the pump
	// deg
	// offset 2982
	uint8_t hpfpMinAngle;
	// offset 2983
	vin_number_t vinNumber;
	// Size of the pump chamber in cc. Typical Bosch HDP5 has a 9.0mm diameter, typical BMW N* stroke is 4.4mm.
	// cc
	// offset 3000
	scaled_channel<uint16_t, 1000, 1> hpfpPumpVolume;
	// How long to keep the valve activated (in order to allow the pump to build pressure and keep the valve open on its own)
	// deg
	// offset 3002
	uint8_t hpfpActivationAngle;
	// offset 3003
	uint8_t issFilterReciprocal;
	// %/kPa
	// offset 3004
	scaled_channel<uint16_t, 1000, 1> hpfpPidP;
	// %/kPa/lobe
	// offset 3006
	scaled_channel<uint16_t, 100000, 1> hpfpPidI;
	// The fastest rate the target pressure can be reduced by. This is because HPFP have no way to bleed off pressure other than injecting fuel.
	// kPa/s
	// offset 3008
	uint16_t hpfpTargetDecay;
	// %
	// offset 3010
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileQuantityBins[HPFP_LOBE_PROFILE_SIZE];
	// deg
	// offset 3026
	scaled_channel<uint8_t, 2, 1> hpfpLobeProfileAngle[HPFP_LOBE_PROFILE_SIZE];
	// volts
	// offset 3042
	uint8_t hpfpDeadtimeVoltsBins[HPFP_DEADTIME_SIZE];
	// ms
	// offset 3050
	scaled_channel<uint16_t, 1000, 1> hpfpDeadtimeMS[HPFP_DEADTIME_SIZE];
	// kPa
	// offset 3066
	uint16_t hpfpTarget[HPFP_TARGET_SIZE][HPFP_TARGET_SIZE];
	// load
	// offset 3266
	scaled_channel<uint16_t, 10, 1> hpfpTargetLoadBins[HPFP_TARGET_SIZE];
	// RPM
	// offset 3286
	scaled_channel<uint8_t, 1, 50> hpfpTargetRpmBins[HPFP_TARGET_SIZE];
	// %
	// offset 3296
	int8_t hpfpCompensation[HPFP_COMPENSATION_SIZE][HPFP_COMPENSATION_SIZE];
	// cc/lobe
	// offset 3396
	scaled_channel<uint16_t, 1000, 1> hpfpCompensationLoadBins[HPFP_COMPENSATION_SIZE];
	// RPM
	// offset 3416
	scaled_channel<uint8_t, 1, 50> hpfpCompensationRpmBins[HPFP_COMPENSATION_SIZE];
	// offset 3426
	output_pin_e stepper_raw_output[4];
	// ratio
	// offset 3434
	scaled_channel<uint16_t, 100, 1> gearRatio[GEARS_COUNT];
	// We need to give engine time to build oil pressure without diverting it to VVT
	// ms
	// offset 3450
	uint16_t vvtActivationDelayMs;
	// deg C
	// offset 3452
	int8_t wwCltBins[WWAE_TABLE_SIZE];
	// offset 3460
	scaled_channel<uint8_t, 100, 1> wwTauCltValues[WWAE_TABLE_SIZE];
	// offset 3468
	scaled_channel<uint8_t, 100, 1> wwBetaCltValues[WWAE_TABLE_SIZE];
	// kPa
	// offset 3476
	int8_t wwMapBins[WWAE_TABLE_SIZE];
	// offset 3484
	scaled_channel<uint8_t, 100, 1> wwTauMapValues[WWAE_TABLE_SIZE];
	// offset 3492
	scaled_channel<uint8_t, 100, 1> wwBetaMapValues[WWAE_TABLE_SIZE];
	// offset 3500
	GearControllerMode gearControllerMode;
	// offset 3501
	TransmissionControllerMode transmissionControllerMode;
	// deg
	// offset 3502
	uint16_t acrDisablePhase;
	// offset 3504
	linear_sensor_s auxLinear1;
	// offset 3524
	linear_sensor_s auxLinear2;
	// offset 3544
	output_pin_e tcu_tcc_onoff_solenoid;
	// offset 3546
	pin_output_mode_e tcu_tcc_onoff_solenoid_mode;
	// offset 3547
	pin_output_mode_e tcu_tcc_pwm_solenoid_mode;
	// offset 3548
	output_pin_e tcu_tcc_pwm_solenoid;
	// offset 3550
	pwm_freq_t tcu_tcc_pwm_solenoid_freq;
	// offset 3552
	output_pin_e tcu_pc_solenoid_pin;
	// offset 3554
	pin_output_mode_e tcu_pc_solenoid_pin_mode;
	// offset 3555
	pin_output_mode_e tcu_32_solenoid_pin_mode;
	// offset 3556
	pwm_freq_t tcu_pc_solenoid_freq;
	// offset 3558
	output_pin_e tcu_32_solenoid_pin;
	// offset 3560
	pwm_freq_t tcu_32_solenoid_freq;
	// offset 3562
	output_pin_e acrPin2;
	// %
	// offset 3564
	float etbMinimumPosition;
	// offset 3568
	uint16_t tuneHidingKey;
	// offset 3570
	SentEtbType sentEtbType;
	// offset 3571
	fuel_pressure_sensor_mode_e fuelPressureSensorMode;
	// offset 3572
	Gpio luaDigitalInputPins[LUA_DIGITAL_INPUT_COUNT];
	// RPM
	// offset 3588
	scaled_channel<uint8_t, 1, 50> tpsTspCorrValuesBins[TPS_TPS_ACCEL_CLT_CORR_TABLE];
	// rpm
	// offset 3592
	int16_t ALSMinRPM;
	// rpm
	// offset 3594
	int16_t ALSMaxRPM;
	// sec
	// offset 3596
	int16_t ALSMaxDuration;
	// C
	// offset 3598
	int8_t ALSMinCLT;
	// C
	// offset 3599
	int8_t ALSMaxCLT;
	// offset 3600
	uint8_t alsMinTimeBetween;
	// offset 3601
	uint8_t alsEtbPosition;
	// %
	// offset 3602
	uint8_t acRelayAlternatorDutyAdder;
	// deg
	// offset 3603
	uint8_t instantRpmRange;
	// %
	// offset 3604
	int ALSIdleAdd;
	// %
	// offset 3608
	int ALSEtbAdd;
	// offset 3612
	int ALSSkipRatio;
	// %
	// offset 3616
	uint8_t ALSMaxDriverThrottleIntent;
	// offset 3617
	pin_input_mode_e ALSActivatePinMode;
	// For Ford TPS, use 53%. For Toyota ETCS-i, use ~65%
	// %
	// offset 3618
	scaled_channel<uint8_t, 2, 1> tpsSecondaryMaximum;
	// For Toyota ETCS-i, use ~69%
	// %
	// offset 3619
	scaled_channel<uint8_t, 2, 1> ppsSecondaryMaximum;
	// offset 3620
	pin_input_mode_e luaDigitalInputPinModes[LUA_DIGITAL_INPUT_COUNT];
	// If the hard limit is 7200rpm and hysteresis is 200rpm, then when the ECU sees 7200rpm, fuel/ign will cut, and stay cut until 7000rpm (7200-200) is reached
	// RPM
	// offset 3628
	scaled_channel<uint8_t, 1, 10> rpmHardLimitHyst;
	// How many test bench pulses do you want
	// offset 3629
	uint8_t ignTestCount;
	// Time between bench test pulses
	// ms
	// offset 3630
	scaled_channel<uint8_t, 1, 5> ignTestOffTime;
	// offset 3631
	uint8_t alignmentFill_at_3631[1];
	// Scale the reported vehicle speed value from CAN. Example: Parameter set to 1.1, CAN VSS reports 50kph, ECU will report 55kph instead.
	// ratio
	// offset 3632
	scaled_channel<uint16_t, 10000, 1> canVssScaling;
	// offset 3634
	uint8_t alignmentFill_at_3634[2];
	// offset 3636
	ThermistorConf oilTempSensor;
	// offset 3668
	ThermistorConf fuelTempSensor;
	// offset 3700
	ThermistorConf ambientTempSensor;
	// offset 3732
	ThermistorConf compressorDischargeTemperature;
	// Place the sensor before the throttle, but after any turbocharger/supercharger and intercoolers if fitted. Uses the same calibration as the MAP sensor.
	// offset 3764
	adc_channel_e throttleInletPressureChannel;
	// Place the sensor after the turbocharger/supercharger, but before any intercoolers if fitted. Uses the same calibration as the MAP sensor.
	// offset 3765
	adc_channel_e compressorDischargePressureChannel;
	// If injector duty cycle hits this value, instantly cut fuel.
	// %
	// offset 3766
	uint8_t maxInjectorDutyInstant;
	// If injector duty cycle hits this value for the specified delay time, cut fuel.
	// %
	// offset 3767
	uint8_t maxInjectorDutySustained;
	// Timeout period for duty cycle over the sustained limit to trigger duty cycle protection.
	// sec
	// offset 3768
	scaled_channel<uint8_t, 10, 1> maxInjectorDutySustainedTimeout;
	// offset 3769
	uint8_t alignmentFill_at_3769[1];
	// offset 3770
	output_pin_e speedometerOutputPin;
	// Number of speedometer pulses per kilometer travelled.
	// offset 3772
	uint16_t speedometerPulsePerKm;
	// units
	// offset 3774
	uint8_t mainUnusedEnd[246];
};
static_assert(sizeof(engine_configuration_s) == 4020);

struct cyl_trim_s {
	// offset 0
	scaled_channel<int8_t, 5, 1> table[TRIM_SIZE][TRIM_SIZE];
};
static_assert(sizeof(cyl_trim_s) == 16);

struct blend_table_s {
	// offset 0
	scaled_channel<int16_t, 10, 1> table[8][8];
	// Load
	// offset 128
	uint16_t loadBins[8];
	// RPM
	// offset 144
	uint16_t rpmBins[8];
	// offset 160
	gppwm_channel_e blendParameter;
	// offset 161
	uint8_t alignmentFill_at_161[1];
	// offset 162
	scaled_channel<int16_t, 10, 1> blendBins[8];
	// %
	// offset 178
	scaled_channel<uint8_t, 2, 1> blendValues[8];
	// offset 186
	uint8_t alignmentFill_at_186[2];
};
static_assert(sizeof(blend_table_s) == 188);

struct persistent_config_s {
	// offset 0
	engine_configuration_s engineConfiguration;
	// target TPS value, 0 to 100%
	// TODO: use int8 data date once we template interpolation method
	// target TPS position
	// offset 4020
	float etbBiasBins[ETB_BIAS_CURVE_LENGTH];
	// PWM bias, 0 to 100%
	// ETB duty cycle bias
	// offset 4052
	float etbBiasValues[ETB_BIAS_CURVE_LENGTH];
	// %
	// offset 4084
	scaled_channel<uint8_t, 20, 1> iacPidMultTable[IAC_PID_MULT_SIZE][IAC_PID_MULT_SIZE];
	// Load
	// offset 4148
	uint8_t iacPidMultLoadBins[IAC_PID_MULT_SIZE];
	// RPM
	// offset 4156
	scaled_channel<uint8_t, 1, 10> iacPidMultRpmBins[IAC_PID_MULT_SIZE];
	// On Single Coil or Wasted Spark setups you have to lower dwell at high RPM
	// RPM
	// offset 4164
	uint16_t sparkDwellRpmBins[DWELL_CURVE_SIZE];
	// ms
	// offset 4180
	scaled_channel<uint16_t, 100, 1> sparkDwellValues[DWELL_CURVE_SIZE];
	// CLT-based target RPM for automatic idle controller
	// C
	// offset 4196
	scaled_channel<int8_t, 1, 2> cltIdleRpmBins[CLT_CURVE_SIZE];
	// See idleRpmPid
	// RPM
	// offset 4212
	scaled_channel<uint8_t, 1, 20> cltIdleRpm[CLT_CURVE_SIZE];
	// CLT-based timing correction
	// C
	// offset 4228
	float cltTimingBins[CLT_TIMING_CURVE_SIZE];
	// degree
	// offset 4260
	float cltTimingExtra[CLT_TIMING_CURVE_SIZE];
	// x
	// offset 4292
	float scriptCurve1Bins[SCRIPT_CURVE_16];
	// y
	// offset 4356
	float scriptCurve1[SCRIPT_CURVE_16];
	// x
	// offset 4420
	float scriptCurve2Bins[SCRIPT_CURVE_16];
	// y
	// offset 4484
	float scriptCurve2[SCRIPT_CURVE_16];
	// x
	// offset 4548
	float scriptCurve3Bins[SCRIPT_CURVE_8];
	// y
	// offset 4580
	float scriptCurve3[SCRIPT_CURVE_8];
	// x
	// offset 4612
	float scriptCurve4Bins[SCRIPT_CURVE_8];
	// y
	// offset 4644
	float scriptCurve4[SCRIPT_CURVE_8];
	// x
	// offset 4676
	float scriptCurve5Bins[SCRIPT_CURVE_8];
	// y
	// offset 4708
	float scriptCurve5[SCRIPT_CURVE_8];
	// x
	// offset 4740
	float scriptCurve6Bins[SCRIPT_CURVE_8];
	// y
	// offset 4772
	float scriptCurve6[SCRIPT_CURVE_8];
	// kPa
	// offset 4804
	float baroCorrPressureBins[BARO_CORR_SIZE];
	// RPM
	// offset 4820
	float baroCorrRpmBins[BARO_CORR_SIZE];
	// ratio
	// offset 4836
	float baroCorrTable[BARO_CORR_SIZE][BARO_CORR_SIZE];
	// Cranking fuel correction coefficient based on TPS
	// Ratio
	// offset 4900
	float crankingTpsCoef[CRANKING_CURVE_SIZE];
	// %
	// offset 4932
	float crankingTpsBins[CRANKING_CURVE_SIZE];
	// Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	// RPM
	// offset 4964
	uint16_t crankingAdvanceBins[CRANKING_ADVANCE_CURVE_SIZE];
	// Optional timing advance table for Cranking (see useSeparateAdvanceForCranking)
	// deg
	// offset 4972
	scaled_channel<int16_t, 100, 1> crankingAdvance[CRANKING_ADVANCE_CURVE_SIZE];
	// RPM-based idle position for coasting
	// RPM
	// offset 4980
	scaled_channel<uint8_t, 1, 100> iacCoastingRpmBins[CLT_CURVE_SIZE];
	// RPM-based idle position for coasting
	// %
	// offset 4996
	scaled_channel<uint8_t, 2, 1> iacCoasting[CLT_CURVE_SIZE];
	// C
	// offset 5012
	float afterstartCoolantBins[AFTERSTART_HOLD_CURVE_SIZE];
	// Seconds
	// offset 5044
	float afterstartHoldTime[AFTERSTART_HOLD_CURVE_SIZE];
	// %
	// offset 5076
	float afterstartEnrich[AFTERSTART_ENRICH_CURVE_SIZE];
	// Seconds
	// offset 5108
	float afterstartDecayTime[AFTERSTART_DECAY_CURVE_SIZE];
	// offset 5140
	scaled_channel<uint8_t, 2, 1> boostTableOpenLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	// RPM
	// offset 5204
	scaled_channel<uint8_t, 1, 100> boostRpmBins[BOOST_RPM_COUNT];
	// offset 5212
	scaled_channel<uint8_t, 1, 2> boostTableClosedLoop[BOOST_RPM_COUNT][BOOST_LOAD_COUNT];
	// %
	// offset 5276
	uint8_t boostTpsBins[BOOST_LOAD_COUNT];
	// %
	// offset 5284
	uint8_t pedalToTpsTable[PEDAL_TO_TPS_SIZE][PEDAL_TO_TPS_SIZE];
	// %
	// offset 5348
	uint8_t pedalToTpsPedalBins[PEDAL_TO_TPS_SIZE];
	// RPM
	// offset 5356
	scaled_channel<uint8_t, 1, 100> pedalToTpsRpmBins[PEDAL_TO_TPS_SIZE];
	// CLT-based cranking position multiplier for simple manual idle controller
	// C
	// offset 5364
	float cltCrankingCorrBins[CLT_CRANKING_CURVE_SIZE];
	// CLT-based cranking position multiplier for simple manual idle controller
	// %
	// offset 5396
	float cltCrankingCorr[CLT_CRANKING_CURVE_SIZE];
	// Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	// RPM
	// offset 5428
	scaled_channel<uint8_t, 1, 50> idleAdvanceBins[IDLE_ADVANCE_CURVE_SIZE];
	// Optional timing advance table for Idle (see useSeparateAdvanceForIdle)
	// deg
	// offset 5436
	float idleAdvance[IDLE_ADVANCE_CURVE_SIZE];
	// RPM
	// offset 5468
	scaled_channel<uint8_t, 1, 10> idleVeRpmBins[IDLE_VE_SIZE];
	// load
	// offset 5472
	uint8_t idleVeLoadBins[IDLE_VE_SIZE];
	// %
	// offset 5476
	scaled_channel<uint16_t, 10, 1> idleVeTable[IDLE_VE_SIZE][IDLE_VE_SIZE];
	// offset 5508
	lua_script_t luaScript;
	// C
	// offset 13508
	float cltFuelCorrBins[CLT_CURVE_SIZE];
	// ratio
	// offset 13572
	float cltFuelCorr[CLT_CURVE_SIZE];
	// C
	// offset 13636
	float iatFuelCorrBins[IAT_CURVE_SIZE];
	// ratio
	// offset 13700
	float iatFuelCorr[IAT_CURVE_SIZE];
	// ratio
	// offset 13764
	float crankingFuelCoef[CRANKING_CURVE_SIZE];
	// C
	// offset 13796
	float crankingFuelBins[CRANKING_CURVE_SIZE];
	// ratio
	// offset 13828
	float crankingCycleCoef[CRANKING_CURVE_SIZE];
	// counter
	// offset 13860
	float crankingCycleBins[CRANKING_CURVE_SIZE];
	// CLT-based idle position multiplier for simple manual idle controller
	// C
	// offset 13892
	float cltIdleCorrBins[CLT_CURVE_SIZE];
	// CLT-based idle position multiplier for simple manual idle controller
	// ratio
	// offset 13956
	float cltIdleCorr[CLT_CURVE_SIZE];
	// Also known as MAF transfer function.
	// kg/hour value.
	// By the way 2.081989116 kg/h = 1 ft3/m
	// kg/hour
	// offset 14020
	float mafDecoding[MAF_DECODING_COUNT];
	// V
	// offset 15044
	float mafDecodingBins[MAF_DECODING_COUNT];
	// deg
	// offset 16068
	scaled_channel<int8_t, 10, 1> ignitionIatCorrTable[8][8];
	// C
	// offset 16132
	int8_t ignitionIatCorrTempBins[8];
	// Load
	// offset 16140
	scaled_channel<uint8_t, 1, 5> ignitionIatCorrLoadBins[8];
	// deg
	// offset 16148
	int16_t injectionPhase[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	// Load
	// offset 16660
	uint16_t injPhaseLoadBins[FUEL_LOAD_COUNT];
	// RPM
	// offset 16692
	uint16_t injPhaseRpmBins[FUEL_RPM_COUNT];
	// onoff
	// offset 16724
	uint8_t tcuSolenoidTable[TCU_SOLENOID_COUNT][TCU_GEAR_COUNT];
	// kPa
	// offset 16784
	scaled_channel<uint16_t, 100, 1> mapEstimateTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	// % TPS
	// offset 17296
	scaled_channel<uint16_t, 100, 1> mapEstimateTpsBins[FUEL_LOAD_COUNT];
	// RPM
	// offset 17328
	uint16_t mapEstimateRpmBins[FUEL_RPM_COUNT];
	// value
	// offset 17360
	int8_t vvtTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	// L
	// offset 17424
	uint16_t vvtTable1LoadBins[SCRIPT_TABLE_8];
	// RPM
	// offset 17440
	uint16_t vvtTable1RpmBins[SCRIPT_TABLE_8];
	// value
	// offset 17456
	int8_t vvtTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	// L
	// offset 17520
	uint16_t vvtTable2LoadBins[SCRIPT_TABLE_8];
	// RPM
	// offset 17536
	uint16_t vvtTable2RpmBins[SCRIPT_TABLE_8];
	// deg
	// offset 17552
	scaled_channel<int16_t, 10, 1> ignitionTable[IGN_RPM_COUNT][IGN_LOAD_COUNT];
	// Load
	// offset 18064
	uint16_t ignitionLoadBins[IGN_LOAD_COUNT];
	// RPM
	// offset 18096
	uint16_t ignitionRpmBins[IGN_RPM_COUNT];
	// %
	// offset 18128
	scaled_channel<uint16_t, 10, 1> veTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	// kPa
	// offset 18640
	uint16_t veLoadBins[FUEL_LOAD_COUNT];
	// RPM
	// offset 18672
	uint16_t veRpmBins[FUEL_RPM_COUNT];
	// lambda
	// offset 18704
	scaled_channel<uint8_t, 147, 1> lambdaTable[FUEL_RPM_COUNT][FUEL_LOAD_COUNT];
	// offset 18960
	uint16_t lambdaLoadBins[FUEL_LOAD_COUNT];
	// RPM
	// offset 18992
	uint16_t lambdaRpmBins[FUEL_RPM_COUNT];
	// value
	// offset 19024
	float tpsTpsAccelTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE];
	// from
	// offset 19280
	float tpsTpsAccelFromRpmBins[TPS_TPS_ACCEL_TABLE];
	// to
	// offset 19312
	float tpsTpsAccelToRpmBins[TPS_TPS_ACCEL_TABLE];
	// value
	// offset 19344
	float scriptTable1[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	// L
	// offset 19600
	int16_t scriptTable1LoadBins[SCRIPT_TABLE_8];
	// RPM
	// offset 19616
	int16_t scriptTable1RpmBins[SCRIPT_TABLE_8];
	// value
	// offset 19632
	uint8_t scriptTable2[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	// L
	// offset 19696
	int16_t scriptTable2LoadBins[SCRIPT_TABLE_8];
	// RPM
	// offset 19712
	int16_t scriptTable2RpmBins[SCRIPT_TABLE_8];
	// value
	// offset 19728
	uint8_t scriptTable3[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	// L
	// offset 19792
	int16_t scriptTable3LoadBins[SCRIPT_TABLE_8];
	// RPM
	// offset 19808
	int16_t scriptTable3RpmBins[SCRIPT_TABLE_8];
	// value
	// offset 19824
	uint8_t scriptTable4[SCRIPT_TABLE_8][SCRIPT_TABLE_8];
	// L
	// offset 19888
	int16_t scriptTable4LoadBins[SCRIPT_TABLE_8];
	// RPM
	// offset 19904
	int16_t scriptTable4RpmBins[SCRIPT_TABLE_8];
	// offset 19920
	uint16_t ignTrimLoadBins[TRIM_SIZE];
	// rpm
	// offset 19928
	uint16_t ignTrimRpmBins[TRIM_SIZE];
	// offset 19936
	cyl_trim_s ignTrims[12];
	// offset 20128
	uint16_t fuelTrimLoadBins[TRIM_SIZE];
	// rpm
	// offset 20136
	uint16_t fuelTrimRpmBins[TRIM_SIZE];
	// offset 20144
	cyl_trim_s fuelTrims[12];
	// ratio
	// offset 20336
	scaled_channel<uint16_t, 100, 1> crankingFuelCoefE100[CRANKING_CURVE_SIZE];
	// Airmass
	// offset 20352
	scaled_channel<uint8_t, 50, 1> tcu_pcAirmassBins[TCU_MAGIC_SIZE];
	// %
	// offset 20360
	uint8_t tcu_pcValsR[TCU_MAGIC_SIZE];
	// %
	// offset 20368
	uint8_t tcu_pcValsN[TCU_MAGIC_SIZE];
	// %
	// offset 20376
	uint8_t tcu_pcVals1[TCU_MAGIC_SIZE];
	// %
	// offset 20384
	uint8_t tcu_pcVals2[TCU_MAGIC_SIZE];
	// %
	// offset 20392
	uint8_t tcu_pcVals3[TCU_MAGIC_SIZE];
	// %
	// offset 20400
	uint8_t tcu_pcVals4[TCU_MAGIC_SIZE];
	// %
	// offset 20408
	uint8_t tcu_pcVals12[TCU_MAGIC_SIZE];
	// %
	// offset 20416
	uint8_t tcu_pcVals23[TCU_MAGIC_SIZE];
	// %
	// offset 20424
	uint8_t tcu_pcVals34[TCU_MAGIC_SIZE];
	// %
	// offset 20432
	uint8_t tcu_pcVals21[TCU_MAGIC_SIZE];
	// %
	// offset 20440
	uint8_t tcu_pcVals32[TCU_MAGIC_SIZE];
	// %
	// offset 20448
	uint8_t tcu_pcVals43[TCU_MAGIC_SIZE];
	// TPS
	// offset 20456
	uint8_t tcu_tccTpsBins[8];
	// MPH
	// offset 20464
	uint8_t tcu_tccLockSpeed[8];
	// MPH
	// offset 20472
	uint8_t tcu_tccUnlockSpeed[8];
	// KPH
	// offset 20480
	uint8_t tcu_32SpeedBins[8];
	// %
	// offset 20488
	uint8_t tcu_32Vals[8];
	// %
	// offset 20496
	scaled_channel<int8_t, 10, 1> throttle2TrimTable[6][6];
	// %
	// offset 20532
	uint8_t throttle2TrimTpsBins[6];
	// RPM
	// offset 20538
	scaled_channel<uint8_t, 1, 100> throttle2TrimRpmBins[6];
	// deg
	// offset 20544
	scaled_channel<uint8_t, 4, 1> maxKnockRetardTable[6][6];
	// %
	// offset 20580
	uint8_t maxKnockRetardLoadBins[6];
	// RPM
	// offset 20586
	scaled_channel<uint8_t, 1, 100> maxKnockRetardRpmBins[6];
	// deg
	// offset 20592
	scaled_channel<int16_t, 10, 1> ALSTimingRetardTable[4][4];
	// TPS
	// offset 20624
	uint16_t alsIgnRetardLoadBins[4];
	// RPM
	// offset 20632
	uint16_t alsIgnRetardrpmBins[4];
	// percent
	// offset 20640
	scaled_channel<int16_t, 10, 1> ALSFuelAdjustment[4][4];
	// TPS
	// offset 20672
	uint16_t alsFuelAdjustmentLoadBins[4];
	// RPM
	// offset 20680
	uint16_t alsFuelAdjustmentrpmBins[4];
	// offset 20688
	blend_table_s ignBlends[IGN_BLEND_COUNT];
	// offset 21440
	blend_table_s veBlends[VE_BLEND_COUNT];
	// %
	// offset 22192
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaBins[12];
	// In units of g/s normalized to choked flow conditions
	// g/s
	// offset 22216
	scaled_channel<uint16_t, 10, 1> throttleEstimateEffectiveAreaValues[12];
	// offset 22240
	blend_table_s boostOpenLoopBlends[BOOST_BLEND_COUNT];
	// offset 22616
	blend_table_s boostClosedLoopBlends[BOOST_BLEND_COUNT];
	// lambda
	// offset 22992
	scaled_channel<uint8_t, 100, 1> lambdaMaxDeviationTable[4][4];
	// offset 23008
	uint16_t lambdaMaxDeviationLoadBins[4];
	// RPM
	// offset 23016
	uint16_t lambdaMaxDeviationRpmBins[4];
	// %
	// offset 23024
	uint8_t injectorStagingTable[6][6];
	// offset 23060
	uint16_t injectorStagingLoadBins[6];
	// RPM
	// offset 23072
	uint16_t injectorStagingRpmBins[6];
};
static_assert(sizeof(persistent_config_s) == 23084);

