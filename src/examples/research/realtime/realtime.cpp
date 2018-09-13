#include <examples/research/realtime/realtime.h>
#include <iostream>

namespace sydevs_examples {

using namespace sydevs;
using namespace sydevs::systems;


realtime::realtime()
    : sim_ptr_()
    , clock_t0_()
    , t_(0_s)
    , high_g_(false)
    , fast_as_possible_(false)
    , t_syn_rate_(0)
{
}


void realtime::mainloop()
{
    try {
        sim_ptr_ = std::make_unique<real_time_simulation<bouncing_ball_interactive_system>>(duration::inf(), 0, std::cout);
        sim_ptr_->update_time_advancement_rate(1);
        sim_ptr_->update_time_synchronization_rate(t_syn_rate_);
        while (true) {
            observation_phase();
            interaction_phase();
        }
    }
    catch (const system_node::error& e) {
        std::cout << "SYSTEM NODE ERROR: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "OTHER ERROR: " << e.what() << std::endl;
    }
}


void realtime::observation_phase()
{
    print_header();
    auto pause_t = t_ + 5_s;
    auto done = false;
    auto prev_prev_x = distance();
    auto prev_x = distance();
    while (!done) {
        int64 event_count = sim_ptr_->process_frame_if_time_reached();
        if (event_count > 0) {
            if (sim_ptr_->frame_index() == 0) {
                clock_t0_ = sim_ptr_->frame_clock_time();
                sim_ptr_->update_synchronization_time(sim_ptr_->frame_time(), clock_t0_);
            }
            t_ = sim_ptr_->frame_time().gap(time_point()).rescaled(milli);
            distance x = sim_ptr_->observation();
            print_frame(x);
            if (t_ >= pause_t) {
                done = ((x > prev_x) && (prev_x <= prev_prev_x));
            }
            prev_prev_x = prev_x;
            prev_x = x;
        }
    }
    print_footer();
}


void realtime::interaction_phase()
{
    print_menu();
    char input_ch;
    std::cin >> input_ch;
    if (input_ch == 'g') {
        high_g_ = !high_g_;
        if (high_g_) {
            sim_ptr_->injection() = 19620_mm/_s/_s;
        }
        else {
            sim_ptr_->injection() = 9810_mm/_s/_s;
        }
    }
    else if (input_ch == 'f') {
        fast_as_possible_ = !fast_as_possible_;
        if (fast_as_possible_) {
            sim_ptr_->update_time_advancement_rate(std::numeric_limits<float64>::infinity());
        }
        else {
            sim_ptr_->update_time_advancement_rate(1);
        }
    }
    else if (isdigit(input_ch)) {
        int64 input_int = input_ch - '0';
        if (input_int == 0) {
            t_syn_rate_ = 0;
        }
        else {
            t_syn_rate_ = pow(10, input_int - 5);
        }
        sim_ptr_->update_time_synchronization_rate(t_syn_rate_);
    }
}


void realtime::print_header()
{
    std::cout << std::endl;
    std::cout << std::string(48, ' ') << "     simulated    wallclock" << std::endl;
    std::cout << std::string(48, '-') << "---------------------------" << std::endl;
}


void realtime::print_footer()
{
    std::cout << std::string(48, '-') << "---------------------------" << std::endl;
    std::cout << std::endl;
}


void realtime::print_frame(distance x)
{
    auto clock_t = sim_ptr_->frame_clock_time();
    int64 pos = int64(x/250_mm + 0.5);
    int64 flight_chars = std::min(pos, int64(48));
    int64 ball_chars = (flight_chars < 48 ? 1 : 0);
    int64 animation_padding = 48 - flight_chars - ball_chars;
    auto animation_str = std::string(flight_chars, '-') + 
                            std::string(ball_chars, '0') +
                            std::string(animation_padding, ' ') + "|";
    auto t_str = (string_builder() << t_).str();
    int64 t_padding = std::max(int64(0), int64(12 - t_str.length()));
    t_str = std::string(t_padding, ' ') + t_str;
    auto ct = duration(std::chrono::duration_cast<std::chrono::milliseconds>(clock_t - clock_t0_).count(), milli);
    auto ct_str = (string_builder() << ct).str();
    int64 ct_padding = std::max(int64(0), int64(12 - ct_str.length()));
    ct_str = std::string(ct_padding, ' ') + ct_str;
    std::cout << animation_str << " " << t_str << " " << ct_str << std::endl;
}


void realtime::print_menu()
{
    std::cout << "Commands:" << std::endl;
    std::cout << "  c   | continue" << std::endl;
    std::cout << "  g   | toggle high gravity (currently " << (high_g_ ? "high" : "low") << ")" << std::endl;
    std::cout << "  f   | toggle fast as possible (currently " << (fast_as_possible_ ? "on" : "off") << ")" << std::endl;
    std::cout << "  0-9 | synchronization rate (currently " << t_syn_rate_ << ")" << std::endl;
    std::cout << "Enter a command sequence: ";
}


}  // namespace
