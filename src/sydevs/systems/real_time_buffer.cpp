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
        if (abs(syn_planned_clk_dt - planned_clk_dt_) >= 2.0) {
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

    /*
    planned_clock_t_ = clock_time();
    int64 depth = std::min(ta_depth_, max_depth_);
    if (depth > 0) {
        float64 accel_clock_dt_sum = 0.0;
        float64 decel_clock_dt_sum = 0.0;
        int64 accel_clock_dt_count = 0;
        int64 decel_clock_dt_count = 0;
        const auto& retained_t0 = time_points_[0];
        const auto& retained_clock_t0 = clock_times_[0];
        float64 clock_dt0 = 0.0;
        for (int64 i = 0; i < depth; ++i) {
            const auto& retained_ti = time_points_[i];
            const auto& retained_clock_ti = clock_times_[i];
            auto dti = retained_t0.gap(retained_ti) + planned_dt_.unfixed();
            auto clock_ti = retained_clock_ti + std::chrono::microseconds(int64((dti/ta_rate_)/1_us));
            float64 clock_dti = 0.0;
            if (clock_ti > retained_clock_t0) {
                clock_dti = float64(std::chrono::duration_cast<std::chrono::microseconds>(clock_ti - retained_clock_t0).count());
            }
            if (i == 0) {
                clock_dt0 = clock_dti;
            }
            else {
                clock_dti = std::min(clock_dti, 2.0*clock_dt0);
            }
            if (clock_dti <= clock_dt0) {
                accel_clock_dt_sum += clock_dti;
                ++accel_clock_dt_count;
            }
            if (clock_dti >= clock_dt0) {
                decel_clock_dt_sum += clock_dti;
                ++decel_clock_dt_count;
            }
            if (i == depth - 1) {
                if (clock_dti < clock_dt0) {
                    decel_clock_dt_sum = 0.0;
                    decel_clock_dt_count = 0;
                }
                else if (clock_dti > clock_dt0) {
                    accel_clock_dt_sum = 0.0;
                    accel_clock_dt_count = 0;
                }
                else {
                    accel_clock_dt_sum = clock_dti;
                    accel_clock_dt_count = 1;
                    decel_clock_dt_sum = 0.0;
                    decel_clock_dt_count = 0;
                }
            }
        }
        float64 clock_dt = (accel_clock_dt_sum + decel_clock_dt_sum)/(accel_clock_dt_count + decel_clock_dt_count);
        planned_clock_t_ = retained_clock_t0 + std::chrono::microseconds(int64(clock_dt));
    }
    */
}


}  // namespace
}  // namespace
