#include "jp200_hardware/jp200_utils.hpp"

namespace jp200_hardware
{
    void jp200Utils::setTargetAngle(JP200Cmd cmd, std::string *packet)
    {
        if(cmd.angle.enable)
        {
            auto value_str = std::to_string(cmd.angle.value);
            *packet += "TA=" + value_str;
        }
    }
}