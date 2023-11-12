#include "ros2_jp200_driver/jp200_hardware.hpp"

#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"


namespace jp200_driver
{
    CallbackReturn Jp200Driver::on_init(const hardware_interface::HardwareInfo & info)
    {
        RCLCPP_DEBUG(rclcpp::get_logger("JP200_driver"), "configure");
        if(hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS)
        {
            return CallbackReturn::ERROR;
        }

        msgs.resize(info_.joints.size(), JP200Cmd());
        
    }
}