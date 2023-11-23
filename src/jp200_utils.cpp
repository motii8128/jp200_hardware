#include "jp200_hardware/jp200_utils.hpp"

namespace jp200_hardware
{
    void jp200Utils::setTargetAngle(JP200Cmd cmd, std::string *packet)
    {
        if(cmd.angle.enable)
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
    }
    void jp200Utils::setTargetAngle(JP200Cmd cmd, std::string *packet)
    {
        if(cmd.angle.enable)
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
    }
}