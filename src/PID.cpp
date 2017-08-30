#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double kp, double ki, double kd) {
    Kp = kp;
    Ki = ki;
    Kd = kd;
    p_error = 0.0f;
    i_error = 0.0f;
    d_error = 0.0f;
}

void PID::UpdateError(double cte) {
    d_error = cte - p_error;
    i_error += cte;
    p_error = cte;
}

double PID::TotalError() {
    return(-(Kp*p_error + Kd*d_error + Ki*i_error));
}
