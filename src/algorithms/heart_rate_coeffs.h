#pragma once

/* 2nd-order Butterworth bandpass 0.5–4 Hz @ 25 Hz, from scipy.signal.butter(2, [0.5,4], btype='band', fs=25, output='sos'). */
namespace whm {
namespace hr_coeffs {

static constexpr unsigned kSosSections = 2U;
static constexpr double kSos[kSosSections][6] = {
    {0.1173510367246091, 0.2347020734492183, 0.1173510367246091, 1.0, -0.8071314388538938,
     0.3479734330783196},
    {1.0, -2.0, 1.0, 1.0, -1.8291984415883695, 0.846721328641201},
};

} /* namespace hr_coeffs */
} /* namespace whm */
