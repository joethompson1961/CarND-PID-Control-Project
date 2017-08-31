# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## PID Behavior

A PID controller has 3 elements of control: proportional, differential and integral.

These elements are biased using cooefficients which allows the affects of each element to be increased or decreased to achieve an optimal final outcome, i.e. a controller that is stable over the range of inputs.

Fine tuning a PID controller is largely an empirical problem. By making repeated adjustments to the coefficients and observing the resulting behavior one can nudge the controller toward a stable, desirable behavior.

Before fine tuning the PID controller for this problem, i.e. controlling steering steering based on cross track error with car moving at high speed, I evaluated each of the control coefficients (Kp, Ki, Kd) in an ordered way, first focussing on the proportional part of the controller, Kp, while setting the other coefficients to zero. I ran the car at various speeds.  Here's a description of the resulting behaviors:

1. **10 MPH**, Kp = 0.1, Ki = 0.0,  Kd = 0.0:
  - Car navigates the entire track but steering swings very wide around tight curves.  Some osscilations present but not too severe.  
2. **20 MPH**, Kp = 0.1, Ki = 0.0,  Kd = 0.0:
  - The first hard turn throws the steering into heavy oscillations, car barely manages to stay on the track but makes a successful loop.
3. **30 MPH**, Kp = 0.1, Ki = 0.0,  Kd = 0.0:
  - Steering osscillations begin immediately and slowly increase heading into first curve where the car loses control and leaves the track.  

Holding and Kp and speed at those values I then began incrementing the differential portion of the controller, Kd, to dampen the proportional response.   

4. 30 MPH, Kp = 0.1, Ki = 0.0,  **Kd = 0.1**:
  - Steering osscillations begin immediately but were slightly improved, making it around the first curve before crashing. 
5. 30 MPH, Kp = 0.1, Ki = 0.0,  **Kd = 0.5**:
  - This significantly damped the steering oscillations, significantly reducing the over and under shoot of the proportional part of the controller.  The car completes a loop though not without some close calls. 

Holding Kp, Kd and speed at those values I then introduce the integral element, Ki, to the controller, adjusting it in steps.

6. 30 MPH, Kp = 0.1, **Ki = 0.1**,  Kd = 0.5:
  - The steering immediately begins oscillating wildly.  
7. 30 MPH, Kp = 0.1, **Ki = 0.01**,  Kd = 0.5:
  - The steering immediately begins oscillating badly, but improved slightly.  
8. 30 MPH, Kp = 0.1, **Ki = 0.001**,  Kd = 0.5:
  - The steering oscillations are greatly reduced but still there is an acculuation of slowly increasing oscillation that finally causes the car to crash.  
9. 30 MPH, Kp = 0.1, **Ki = 0.0001**,  Kd = 0.5:
  - The vehicle makes a complete loop, sufficient to pass the grader, but steering feels sluglish and more fine tuning necessary to reach higher speeds.  

## PID Tuning Method

In the beginning I manually experimented with various coeffient values to find a set that looked pretty good.  I then increased the target speed and found the results to be less desirable.

To better fine tune the controller at higher speeds I found it important to have an objective method for evaulating the results. I implemented a scoring method that calculated 2 values over a fixed number of measurements steps (approximately 1 lap around the course):
- accumulated cross-track error - the sum of all the absolute values of cross track error for an entire lap.
- max cross-track error - the maximum absolute value of cross track error for an entire lap.
The goal is to find the set of coefficients that yield the lowest overall score for both values.

Using this scoring method I increased the speed to 45 MPH and manually tried a variety of different coefficient values in a way similar to twiddle, recordin the scores after each lap.  After a decent score was achieved I was able to increase the speed even further and still navigate the course successfully.  It was able to run reliably for many laps at 55MPH. 65MPH was the highest speed it could handle though it eventually crashed.
