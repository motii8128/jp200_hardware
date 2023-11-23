#ifndef ROS2_JP200_DRIVER__JP200_HARDWARE_HPP_
#define ROS2_JP200_DRIVER__JP200_HARDWARE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/state.hpp"

#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/system_interface.hpp"

#include "jp200_hardware/port_handler.hpp"
#include "jp200_hardware/packet_handler.hpp"
#include "jp200_hardware/jp200_utils.hpp"

#include <vector>
#include <string>

using hardware_interface::CallbackReturn;
using hardware_interface::return_type;

namespace jp200_hardware
{
    
    class JP200Hardware : public hardware_interface::SystemInterface
    {
        public:
            RCLCPP_SHARED_PTR_DEFINITIONS(JP200Hardware);
            unsigned int main_loop_update_rate_, desired_hardware_update_rate_ = 100; 
            CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;

            CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;
            CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;

            std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
            std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

            return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;
            return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;


        private:
            std::string port_name;
            int boudrate;
            std::shared_ptr<jp200_hardware::HandlerBase> port_handler;
            std::shared_ptr<jp200_hardware::PacketHandler> packet_handler;
            std::vector<jp200Utils::JP200Cmd> cmds;

            jp200Utils::JP200Cmd getJP200Param()
                {
                    jp200Utils::JP200Cmd cmd = {};
                    cmd.id = getHardwareParam<int>("id");
                    // 0~254
                    cmd.enable_control_mode = getHardwareParam<bool>("enable_control_mode");
                    cmd.control_mode = getHardwareParam<int>("control_mode");
                    /*
                    0:shutdown free mode
                    1:angle mode
                    2:velocity mode
                    3:angle+velocity mode
                    4:current mode 
                    5:angle+current mode 
                    6:velocity+current mode
                    7:angle+velocity+current mode
                    8:pwm mode
                    14:Stop(pwm) mode
                    15:shutdown break mode
                    */

                    cmd.angle.enable = getHardwareParam<bool>("target_angle");
                    cmd.velocity.enable = getHardwareParam<bool>("target_velocuty");
                    cmd.current.enable = getHardwareParam<bool>("target_current");
                    cmd.pwm_enable = getHardwareParam<bool>("target_pwm");

                    cmd.angle.get_state = getHardwareParam<bool>("get_angle");
                    cmd.velocity.get_state = getHardwareParam<bool>("get_velocity");
                    cmd.current.get_state = getHardwareParam<bool>("get_current");
                    cmd.get_pwm = getHardwareParam<bool>("get_pwm");

                    cmd.get_mpu_temp = getHardwareParam<bool>("get_mpu_temp");
                    cmd.get_amp_temp = getHardwareParam<bool>("get_amp_temp");
                    cmd.get_motor_temp = getHardwareParam<bool>("get_motor_temp");
                    cmd.get_status = getHardwareParam<bool>("get_status");

                    cmd.position_gain.enable = getHardwareParam<bool>("set_position_gain");
                    cmd.velocity_gain.enable = getHardwareParam<bool>("set_position_gain");
                    cmd.current_gain.enable = getHardwareParam<bool>("set_current_gain");
                    
                    return cmd;
                }
        
            template <typename T>
            T getHardwareParam(const std::string param_name) const
            {
                T value;
                getHardParam(param_name, value);
                return value;
            }
            void getHardwareParam(const std::string & param_name, std::string & value) const
            {
                try
                {
                    value = info_.hardware_parameters.at(param_name);
                }
                catch(std::out_of_range & e)
                {
                    RCLCPP_ERROR_STREAM(
                        rclcpp::get_logger("jp200_hardware"),
                        "Set" << param_name << "default.");
                    value = "0";
                }
                
            }
            void getHardwareParam(const std::string & param_name, int & value) const
            {
                std::string param_str;
                getHardwareParam(param_name, param_str);
                value = std::stoi(param_str);
            }
            void getHardwareParam(const std::string & param_name, bool & value) const
            {
                value = false;
                std::string param_str;
                getHardwareParam(param_name, param_str);
                if(param_str == "true" || param_str == "True")
                {
                    value = true;
                }
            }
    };

    
} // namespace jp200_driver

#endif