#include "jp200_hardware/jp200_hardware.hpp"
#include <memory>
#include <vector>
#include <string>

#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"


namespace jp200_hardware
{
    CallbackReturn JP200Hardware::on_init(const hardware_interface::HardwareInfo & info)
    {
        RCLCPP_DEBUG(rclcpp::get_logger("JP200_hardware_interface"), "configure");
        if(hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS)
        {
            return CallbackReturn::ERROR;
        }

        RCLCPP_INFO_STREAM(rclcpp::get_logger("jp200_hardware_interface"), "initialize hardware" << info.name);

        port_name = getHardwareParam<std::string>("port_name");
        boudrate = getHardwareParam<int>("baudrate");

        RCLCPP_INFO(rclcpp::get_logger("jp200_hardware_interface"), "Get Hardware Parameter");
        for(int i = 0; i < ids.size(); i++)
        {
            cmds[i] = getJP200Param();
        }

        RCLCPP_INFO(rclcpp::get_logger("jp200_hardware_interface"), "initialize port handler");
        port_handler = std::shared_ptr<jp200_hardware::HandlerBase>(HandlerBase::getHandler(port_name));
        port_handler->setBaudrate(boudrate);

        RCLCPP_INFO(rclcpp::get_logger("jp200_hardware_interface"), "initialize packet handler");
        packet_handler = std::shared_ptr<jp200_hardware::PacketHandler>(PacketHandler::getInstance());

        if(port_handler->openPort())
        {
            RCLCPP_INFO(rclcpp::get_logger("jp200_hardware_interface"), "Succeed to open serial port");
        }
        else
        {
            RCLCPP_ERROR(rclcpp::get_logger("jp200_hardware_interface"), "Failed to open serial port");
            return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
        }


    }
    
}
