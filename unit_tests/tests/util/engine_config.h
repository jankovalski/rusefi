//
// Created by kifir on 11/18/24.
//

#pragma once

#include "batt_lag_corr_curve.h"

class EngineConfig {
public:
    EngineConfig clone() const;

    // Launch Control
    std::optional<switch_input_pin_e> getLaunchActivatePin() const { return m_launchActivatePin; }
    std::optional<bool> getLaunchActivateInverted() const { return m_launchActivateInverted; }

    // Shift Torque Reduction (Flat Shift)
    std::optional<bool> getTorqueReductionEnabled() const { return m_isTorqueReductionEnabled; }
    std::optional<torqueReductionActivationMode_e> getTorqueReductionActivationMode() const {
        return m_torqueReductionActivationMode;
    }
    std::optional<switch_input_pin_e> getTorqueReductionTriggerPin() const { return m_torqueReductionTriggerPin; }
    std::optional<bool> getTorqueReductionTriggerPinInverted() const { return m_torqueReductionTriggerPinInverted; }
    std::optional<bool> getLimitTorqueReductionTime() const { return m_limitTorqueReductionTime; }
    std::optional<float> getTorqueReductionTime() const { return m_torqueReductionTime; }
    std::optional<float> getTorqueReductionArmingRpm() const { return m_torqueReductionArmingRpm; }
    std::optional<float> getTorqueReductionArmingApp() const { return m_torqueReductionArmingApp; }
    std::optional<int8_t> getTorqueReductionIgnitionCut() const { return m_torqueReductionIgnitionCut; }
    std::optional<float> getTorqueReductionIgnitionRetard() const { return m_torqueReductionIgnitionRetard; }

    // Injector
    std::optional<float> getInjectorFlow() const { return m_injectorFlow; }
    std::optional<BattLagCorrCurve> getInjectorBattLagCorr() const { return m_injectorBattLagCorrCurve; }
    std::optional<bool> getInjectorFlowAsMassFlow() const { return m_injectorFlowAsMassFlow; }

    // Secondary injector
    std::optional<float> getInjectorSecondaryFlow() const { return m_injectorSecondaryFlow; }
    std::optional<BattLagCorrCurve> getInjectorSecondaryBattLagCorr() const { return m_injectorSecondaryBattLagCorrCurve; }

    // Staged injection
    std::optional<bool> getStagedInjectionEnabled() const { return m_isStagedInjectionEnabled; }

    // We do not core about performance in tests, but we want to use builder-like style, so setters return new instance
    // of configuration:

    // Launch Control
    EngineConfig setLaunchActivatePin(std::optional<switch_input_pin_e> value);
    EngineConfig setLaunchActivateInverted(std::optional<bool> value);

    // Shift Torque Reduction (Flat Shift)
    EngineConfig setTorqueReductionEnabled(std::optional<bool> value);
    EngineConfig setTorqueReductionActivationMode(std::optional<torqueReductionActivationMode_e> value);
    EngineConfig setTorqueReductionTriggerPin(std::optional<switch_input_pin_e> value);
    EngineConfig setTorqueReductionTriggerPinInverted(std::optional<bool> value);
    EngineConfig setLimitTorqueReductionTime(std::optional<bool> value);
    EngineConfig setTorqueReductionTime(std::optional<float> value);
    EngineConfig setTorqueReductionArmingRpm(std::optional<float> value);
    EngineConfig setTorqueReductionArmingApp(std::optional<float> value);
    EngineConfig setTorqueReductionIgnitionCut(std::optional<int8_t> value);
    EngineConfig setTorqueReductionIgnitionRetard(std::optional<float> value);

    // Injector
    EngineConfig setInjectorFlowAsMassFlow(std::optional<bool> injectorFlowAsMassFlow);
    EngineConfig setInjectorFlow(std::optional<float> flow);
    EngineConfig setInjectorBattLagCorr(std::optional<BattLagCorrCurve> battLagCorr);

    // Secondary injector
    EngineConfig setInjectorSecondaryFlow(std::optional<float> flow);
    EngineConfig setInjectorSecondaryBattLagCorr(std::optional<BattLagCorrCurve> battLagCorr);

    // Staged injection
    EngineConfig setStagedInjectionEnabled(std::optional<bool> value);
private:
    // Launch Control
    std::optional<switch_input_pin_e> m_launchActivatePin;
    std::optional<bool> m_launchActivateInverted;

    // Shift Torque Reduction (Flat Shift)
    std::optional<bool> m_isTorqueReductionEnabled;
    std::optional<torqueReductionActivationMode_e> m_torqueReductionActivationMode;
    std::optional<switch_input_pin_e> m_torqueReductionTriggerPin;
    std::optional<bool> m_torqueReductionTriggerPinInverted;
    std::optional<bool> m_limitTorqueReductionTime;
    std::optional<float> m_torqueReductionTime;
    std::optional<float> m_torqueReductionArmingRpm;
    std::optional<float> m_torqueReductionArmingApp;
    std::optional<int8_t> m_torqueReductionIgnitionCut;
    std::optional<float> m_torqueReductionIgnitionRetard;

    // Injector
    std::optional<float> m_injectorFlow;
    std::optional<BattLagCorrCurve> m_injectorBattLagCorrCurve;
    std::optional<bool> m_injectorFlowAsMassFlow;;

    // Secondary injector
    std::optional<float> m_injectorSecondaryFlow;
    std::optional<BattLagCorrCurve> m_injectorSecondaryBattLagCorrCurve;

    // Staged injection
    std::optional<bool> m_isStagedInjectionEnabled;
};