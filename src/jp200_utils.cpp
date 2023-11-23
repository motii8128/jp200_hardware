#include "jp200_hardware/jp200_utils.hpp"

namespace jp200_hardware
{
    void jp200Utils::setTargetAngle(JP200Cmd cmd, std::string *packet)
    {
            auto target_angle = std::to_string(cmd.angle.value * 100.0);
            *packet += "TA=" + target_angle;
            if(cmd.angle.trajectory != 0.0)
            {
                auto target_tajectory = std::to_string(cmd.angle.trajectory);
                *packet += ";" + target_tajectory;
            }
            if(cmd.angle.transition_time != 0.0)
            {
                auto target_transition_time = std::to_string(cmd.angle.transition_time);
                *packet += ";" + target_transition_time;
            }
    }
    void jp200Utils::setTargetVelocity(JP200Cmd cmd, std::string *packet)
    {
            auto target_velocity = std::to_string(cmd.velocity.value * 1000.0);
            *packet += "TV=" + target_velocity;
            if(cmd.velocity.trajectory != 0.0)
            {
                auto target_tajectory = std::to_string(cmd.velocity.trajectory);
                *packet += ";" + target_tajectory;
            }
            if(cmd.velocity.transition_time != 0.0)
            {
                auto target_transition_time = std::to_string(cmd.velocity.transition_time);
                *packet += ";" + target_transition_time;
            }
    }
    void jp200Utils::setTargetCurrent(JP200Cmd cmd, std::string *packet)
    {
            auto target_current = std::to_string(cmd.current.value);
            *packet += "TC=" + target_current;
            if(cmd.current.trajectory != 0.0)
            {
                auto target_tajectory = std::to_string(cmd.current.trajectory);
                *packet += ";" + target_tajectory;
            }
            if(cmd.current.transition_time != 0.0)
            {
                auto target_transition_time = std::to_string(cmd.current.transition_time);
                *packet += ";" + target_transition_time;
            }
    }
    void jp200Utils::setPWM(JP200Cmd cmd, std::string *packet)
    {
            auto target_pwm = std::to_string(cmd.pwm_rate);
            *packet += "TP=" + target_pwm;
    }
    void jp200Utils::setGetStateEnable(JP200Cmd cmd, std::string *packet)
    {
        if(cmd.angle.get_state)*packet += "CA";
        if(cmd.velocity.get_state)*packet += "CV";
        if(cmd.current.get_state)*packet += "CC";
        if(cmd.get_pwm)*packet += "CP";
        if(cmd.get_mpu_temp)*packet += "CT0";
        if(cmd.get_amp_temp)*packet += "CT1";
        if(cmd.get_motor_temp)*packet += "CT2";
        if(cmd.get_voltage)*packet += "CB";
        if(cmd.get_status)*packet += "ST";
    }
    
}