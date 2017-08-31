#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid_steer;                          //  accum_cte    max_cte   (lowest scores win)
  pid_steer.Init(0.11f, 0.000000f, 3.0f); //
  pid_steer.Init(0.11f, 0.000001f, 3.0f); //
  pid_steer.Init(0.11f, 0.000001f, 2.0f); //
  pid_steer.Init(0.11f, 0.000000f, 2.0f); //
  pid_steer.Init(0.13f, 0.000001f, 2.0f); //
  pid_steer.Init(0.15f, 0.000001f, 2.0f); //
  pid_steer.Init(0.17f, 0.000001f, 2.0f); //
  pid_steer.Init(0.20f, 0.000001f, 2.0f); //
  pid_steer.Init(0.25f, 0.000001f, 2.0f); //
  pid_steer.Init(0.25f, 0.000001f, 2.5f); //
  pid_steer.Init(0.25f, 0.000001f, 1.5f); //    CRASH
  pid_steer.Init(0.25f, 0.000001f, 2.25f);//
  pid_steer.Init(0.25f, 0.000001f, 1.9f); //
  pid_steer.Init(0.25f, 0.000001f, 2.1f); //
  pid_steer.Init(0.30f, 0.000001f, 2.0f); //    CRASH
  pid_steer.Init(0.275f, 0.000001f, 2.0f);//    CRASH
  pid_steer.Init(0.24f, 0.000001f, 2.0f); //
  pid_steer.Init(0.26f, 0.000001f, 2.0f); //
  pid_steer.Init(0.25f, 0.000001f, 2.0f); //
  pid_steer.Init(0.25f, 0.000001f, 2.0f); //
  pid_steer.Init(0.25f, 0.001000f, 2.0f); //    CRASH
  pid_steer.Init(0.24f, 0.000001f, 2.0f); //    733.672   2.2559
  pid_steer.Init(0.11f, 0.000001f, 2.0f); //    1070.26   2.8449
  pid_steer.Init(0.11f, 0.000010f, 2.0f); //    1023.3    2.5364
  pid_steer.Init(0.11f, 0.000100f, 2.0f); //    855.619   2.957
  pid_steer.Init(0.11f, 0.001000f, 2.0f); //    761.409   2.4297
  pid_steer.Init(0.11f, 0.010000f, 2.0f); //    1209.05   2.8223
  pid_steer.Init(0.11f, 0.005000f, 2.0f); //    657.603   2.0868
  pid_steer.Init(0.11f, 0.007500f, 2.0f); //    770.556   2.115
  pid_steer.Init(0.11f, 0.006000f, 2.0f); //    658.555   2.1434
  pid_steer.Init(0.11f, 0.004000f, 2.0f); //    667.891   1.6225
  pid_steer.Init(0.11f, 0.004500f, 2.0f); //    635.753   1.8915
  pid_steer.Init(0.25f, 0.004500f, 2.0f); //    CRASH
  pid_steer.Init(0.135f,0.004500f, 2.0f); //    608.899   1.6933
  pid_steer.Init(0.16f, 0.004500f, 2.0f); //    607.791   1.7177
  pid_steer.Init(0.15f, 0.001000f, 2.0f); //    633.252   2.4016
  pid_steer.Init(0.15f, 0.000100f, 2.0f); //    711.334   2.4
  pid_steer.Init(0.15f, 0.004500f, 2.0f); //    589.445   1.7385
  pid_steer.Init(0.15f, 0.004500f, 2.1f); //    614.7     1.7751
  pid_steer.Init(0.15f, 0.004500f, 2.25f);//    603.385   2.039
  pid_steer.Init(0.15f, 0.004500f, 2.5f); //    585.164   1.9066
  pid_steer.Init(0.15f, 0.004500f, 2.6f); //    563.991   1.8624
  pid_steer.Init(0.15f, 0.004500f, 3.0f); //    557.061   2.1939
  pid_steer.Init(0.15f, 0.004500f, 2.75f);//    619.778   2.4285
  pid_steer.Init(0.15f, 0.004500f, 2.6f); //    582.937   2.3804
  pid_steer.Init(0.15f, 0.004500f, 2.6f); //    600.621   2.2021
  pid_steer.Init(0.15f, 0.004500f, 2.5f); //    556.127   2.204    THE WINNER!!

  PID pid_throttle;
  pid_throttle.Init(0.9f, 0.000001f, 0.5f);

  int measurement_cnt = 0;
  double accum_cte = 0.0f;
  double max_cte = 0.0f;

  h.onMessage([&pid_steer, &pid_throttle, &measurement_cnt, &accum_cte, &max_cte](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double steer_cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
//          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;

#if 0 // scoring pid coefficients
          // Print score for one lap
          int one_lap = 1700;
          if (fabs(steer_cte) > max_cte)
              max_cte = fabs(steer_cte); // used to measure quality of PID controller over a single lap.
          accum_cte += fabs(steer_cte);  // used to measure quality of PID controller over a single lap.
          measurement_cnt += 1;         // used to measure quality of PID controller over a single lap.
          if (measurement_cnt >= one_lap) {
              std::cout << "Lap end Total CTE: " << accum_cte << " Max CTE: " << max_cte << std::endl;
              return;
          }
#endif

          /*
          * Calculate steering value here. Steering value range is [-1, 1].
          */
          pid_steer.UpdateError(steer_cte);
          steer_value = pid_steer.TotalError();
          if (steer_value > 1.0f)
              steer_value = 1.0f;
          if (steer_value < -1.0f)
              steer_value = -1.0f;

          // Using a 2nd PID controller to control the speed.
          double speed_tgt = 55.0f;   // Made it around that track at 65 once, but not reliably
          double throttle_cte = speed - speed_tgt;
          double throttle_value;
          pid_throttle.UpdateError(throttle_cte);
          throttle_value = pid_throttle.TotalError();
          if (throttle_value > 1.0f)
              throttle_value = 1.0f;
          if (throttle_value < 0.0f)
              throttle_value = 0.0f;

#if 0  // DEBUG
          std::cout << "Steer CTE:    " << steer_cte    << " Steering Value: " << steer_value << std::endl;
          std::cout << "Throttle CTE: " << throttle_cte << " Throttle Value: " << throttle_value << " Speed: " << speed << std::endl;
#endif

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle_value;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
//          std::cout << msg << std::endl << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
