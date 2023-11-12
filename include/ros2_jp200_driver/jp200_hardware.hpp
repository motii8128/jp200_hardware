#include "rclcpp/rclcpp.hpp"
#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/system_interface.hpp"
#include "rclcpp_lifecycle/state.hpp"

#include <vector>

using hardware_interface::CallbackReturn;
using hardware_interface::return_type;

namespace jp200_driver
{
    class Jp200Driver : public hardware_interface::SystemInterface
    {
        public:
            RCLCPP_SHARED_PTR_DEFINITIONS(Jp200Driver);
            unsigned int main_loop_update_rate_, desired_hardware_update_rate_ = 100; 
            CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;
            CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;
            return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;
            return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;
    };

    struct Cmd
    {
        double cmd = 0.0;
        double trajectory = 0.0;
        double transition_time = 0.0;
    };
    struct State
    {
        bool angle = false;
        bool velocity = false;
        bool current = false;
        bool pwm = false;
        bool mpu_temp = false;
        bool amp_temp = false;
        bool motor_temp = false;
        bool voltage = false;
        bool status = false;
    };
    struct PIDConfig
    {
        int p_gain;
        int i_gain;
        int d_gain;
        int f_gain;
    };

    struct JP200Message
    {
        uint8_t id = 1;
        uint8_t control_mode;
        Cmd angle_cmd;
        Cmd velocity_cmd;
        Cmd current_cmd;
        double pwm_cmd = 0.0;
        State state;
        
    };
    
}