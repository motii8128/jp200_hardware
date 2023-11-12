#include "jp200_hardware/jp200_hardware.hpp"

#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"


namespace jp200_hardware
{
    CallbackReturn JP200Hardware::on_init(const hardware_interface::HardwareInfo & info)
    {
        RCLCPP_DEBUG(rclcpp::get_logger("JP200_driver"), "configure");
        if(hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS)
        {
            return CallbackReturn::ERROR;
        }

        msgs.resize(info_.joints.size(), JP200Cmd());
        ids.resize(info_.joints.size(), 0);
        for(uint i = 0; i < info_.joints.size(); i++)
        {
            ids[i] = std::stoi(info_.joints[i].parameters.at("id"));
        }
    }
}