// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_state.txt Tue Mar 21 13:46:06 UTC 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of trigger_state_s
struct trigger_state_s {
	/**
	 * Crank sync counter
	 * Usually matches crank revolutions
	 * offset 0
	 */
	uint32_t crankSynchronizationCounter = (uint32_t)0;
	/**
	 * offset 4
	 */
	float vvtSyncGapRatio = (float)0;
	/**
	us
	 * offset 8
	 */
	uint32_t vvtToothDurations0 = (uint32_t)0;
	/**
	 * offset 12
	 */
	float vvtCurrentPosition = (float)0;
	/**
	 * @@GAUGE_NAME_TRG_GAP@@
	 * offset 16
	 */
	float triggerSyncGapRatio = (float)0;
	/**
	 * offset 20
	 */
	uint8_t triggerStateIndex = (uint8_t)0;
	/**
	 * offset 21
	 */
	uint8_t vvtCounter = (uint8_t)0;
	/**
	 * offset 22
	 */
	uint8_t camResyncCounter = (uint8_t)0;
	/**
	 * offset 23
	 */
	uint8_t vvtStateIndex = (uint8_t)0;
};
static_assert(sizeof(trigger_state_s) == 24);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_state.txt Tue Mar 21 13:46:06 UTC 2023
