#include <sydevs/systems/real_time_buffer.h>

namespace sydevs {
namespace systems {

    
real_time_buffer::real_time_buffer(float64 t_adv_rate, int64 t_syn_rate)
    : t_adv_rate_(t_adv_rate)
    , t_syn_rate_(t_syn_rate)
    , syn_sim_t_()
    , syn_clk_t_()
    , current_sim_t_()
    , current_clk_t_()
    , planned_sim_dt_(0_s)
    , planned_clk_dt_(0.0)
    , syn_count_(0)
{
    recompute_planned_clock_duration();
}


void real_time_buffer::update_time_advancement_rate(float64 t_adv_rate)
{
    t_adv_rate_ = t_adv_rate;
    syn_count_ = 0;
    recompute_planned_clock_duration();
}


void real_time_buffer::update_time_synchronization_rate(float64 t_syn_rate)
{
    t_syn_rate_ = t_syn_rate;
    syn_count_ = 0;
    recompute_planned_clock_duration();
}


void real_time_buffer::update_synchronization_time(const time_point& sim_t, const clock_time& clk_t)
{
    syn_sim_t_ = sim_t;
    syn_clk_t_ = clk_t;
    syn_count_ = 0;
    recompute_planned_clock_duration();
}


void real_time_buffer::update_current_time(const time_point& sim_t, const clock_time& clk_t, duration planned_sim_dt)
{
    current_sim_t_ = sim_t;
    current_clk_t_ = clk_t;
    planned_sim_dt_ = planned_sim_dt.unfixed();
    recompute_planned_clock_duration();
}


void real_time_buffer::recompute_planned_clock_duration()
{
    planned_clk_dt_ = std::chrono::milliseconds(int64((planned_sim_dt_/t_adv_rate_)/1_ms)).count();
    if (t_syn_rate_ > 0) {
        auto syn_planned_sim_dt = current_sim_t_.gap(syn_sim_t_) + planned_sim_dt_;
        auto syn_planned_clk_t = syn_clk_t_ + std::chrono::milliseconds(int64((syn_planned_sim_dt/t_adv_rate_)/1_ms));
        float64 syn_planned_clk_dt = std::chrono::duration_cast<std::chrono::milliseconds>(syn_planned_clk_t - current_clk_t_).count();
        if (std::abs(syn_planned_clk_dt - planned_clk_dt_) >= 2.0) {
            ++syn_count_;
            float64 limited_clk_dt = planned_clk_dt_;
            if (syn_planned_clk_dt > planned_clk_dt_) {
                limited_clk_dt = sqrt(planned_clk_dt_*syn_planned_clk_dt);
            }
            else if (syn_planned_clk_dt < planned_clk_dt_) {
                limited_clk_dt = planned_clk_dt_*sqrt(planned_clk_dt_/(2.0*planned_clk_dt_ - syn_planned_clk_dt));
            }
            float64 t_syn_strength = 2.0/(1.0 + exp(-t_syn_rate_*syn_count_)) - 1.0;
            planned_clk_dt_ = (1.0 - t_syn_strength)*planned_clk_dt_ + t_syn_strength*limited_clk_dt;
        }
        else {
            syn_count_ = 0;
        }
    }
}


}  // namespace
}  // namespace
