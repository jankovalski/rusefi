package com.rusefi.ldmp;

import com.rusefi.config.generated.FuelComputer;
import com.rusefi.config.generated.IgnitionState;
import com.rusefi.config.generated.TsOutputs;
import com.rusefi.enums.live_data_e;
import org.junit.jupiter.api.Test;

import static org.junit.Assert.assertEquals;

public class StateDictionaryTest {
    @Test
    public void testStateDictionaryIsComplete() {
        // we assert that this does not fail
        StateDictionary.INSTANCE.getFields(live_data_e.LDS_ac_control);
    }

    @Test
    public void testOffset() {
        int outputsSize = StateDictionary.getSize(TsOutputs.VALUES);
        assertEquals(outputsSize
                + StateDictionary.getSize(FuelComputer.VALUES)
                        + StateDictionary.getSize(IgnitionState.VALUES)
                , StateDictionary.INSTANCE.getOffset(live_data_e.LDS_knock_controller));
    }
}
