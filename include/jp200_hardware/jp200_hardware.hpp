#ifndef ROS2_JP200_DRIVER__JP200_HARDWARE_HPP_
#define ROS2_JP200_DRIVER__JP200_HARDWARE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/state.hpp"

#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/system_interface.hpp"

#include "jp200_hardware/port_handler.hpp"

#include <vector>
#include <string>

using hardware_interface::CallbackReturn;
using hardware_interface::return_type;

namespace jp200_hardware
{
    struct Cmd
    {
        double cmd;
        double trajectory;
        double transition_time;
    };
    struct State
    {
        bool angle{false};
        bool velocity{false};
        bool current{false};
        bool pwm{false};
        bool mpu_temp{false};
        bool amp_temp{false};
        bool motor_temp{false};
        bool voltage{false};
        bool status{false};
    };
    struct Gains
    {
        int p_gain;
        int i_gain;
        int d_gain;
        int f_gain;
    };

    struct JP200Cmd
    {
        uint8_t control_mode;
        Cmd angle_cmd;
        Cmd velocity_cmd;
        Cmd current_cmd;
        double pwm_cmd;
        State state;
        Gains position_gain;
        Gains velocity_gain;
        Gains current_gain; 
    };
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
            std::shared_ptr<jp200_hardware::PortHandler> port_handler;
            std::vector<uint8_t> ids;
            std::vector<JP200Cmd> msgs;

        
            template <typename T>
            T get_hard_param(const std::string param_name) const
            {
                T value;
                get_hard_param(param_name, value);
                return value;
            }
            void get_hard_param(const std::string & param_name, std::string & value) const
            {
                try
                {
                    value = info_.hardware_parameters.at(param_name);
                }
                catch(std::out_of_range & e)
                {
                    RCLCPP_ERROR_STREAM(
                        rclcpp::get_logger("jp200_hardware"),
                        "hardware parameter :" << param_name << "does not exist.");
                }
                
            }
            void get_hard_param(const std::string & param_name, int & value) const
            {
                std::string param_str;
                get_hard_param(param_name, param_str);
                value = std::stoi(param_str);
            }
            void get_hard_param(const std::string & param_name, bool & value) const
            {
                value = false;
                std::string param_str;
                get_hard_param(param_name, param_str);
                if(param_str == "true" || param_str == "True")
                {
                    value = true;
                }
            }
    };

    
} // namespace jp200_driver

#endif