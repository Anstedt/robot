/* NOTES
   32*200 = 6,400 pulses per revolution, 1 revolution is 360 degrees
   32*200 PPR / 360 DPR = 17.7 pulses / degree
   
   We need to tune the PIO rate based on the required maximum PPS we select.

   800 pulses per second is 0.00125 second delay
   25  pulses per second is 0.04000 second delay
   If delay unit 1 is 0.00125 to get 0.04 we need 32 unit delay

   I would prefer being closer to:
   1 degree or less per second minimum speed
   1/4 revolution or 800 PPS maximum

   fast = delay of 0.00125 seconds = 800 PPS or 1/4 revolutions per second 
   slow = delay of 0.056 seconds = 17.7 pulses per second = 1 degree per second

   That means for minimum speed I need a delay of 17.7 pps or 0.056
   seconds per pulse since we are running at 250 Hz that is a minimum
   of 250 pps. We could get around that by having delays that take
   longer that 0.04 seconds
   
   We can do that by either of 2 methods: 

   1. Just use the requested delay and when finished delaying based on
   that data even if greater than the 0.04 time, pull data from the
   input buffer until it is empty and then use the last value we
   pulled. In the larger than 0.04 case the buffer would have extra
   data and we are assuming the sender can buffer some data
   automatically.

   2. Pull the data and run for the max delay of 0.04 seconds even if
   it is greater than 0.04 seconds and then check for new data and
   pull it. If the new data is NOT smaller than our original amount
   just keep counting down the old data until we are done or we hit
   0.04 again. The assumption is that since we want a max delay of
   0.056 this will happen at most twice. If the new data is smaller
   then the old data, pulse, and start using the new data. If we do
   not pulse we can get into a situation where we do not pulse. Also
   if the delay is getting smaller an extra pulse moaves us at most a
   little faster than we requested.

Scheme 1:
// In this case we either continue using old data or use the new data
// We also need to pull till the buffer is empty since long
   delays will cause the buffer to overflow
old_delay = 0; means no speed
loop: if delay = pull // pull till buffer is empty, if we get any new data
 old_delay = delay // remember the new data
else
 delay = old_delay  // reuse the old data
count down delay
pulse when delay hits 0
goto loop

Scheme 2:
new_delay = pull
if new_delay < delay
  delay = new_delay
  pulse
if delay > 0.04
  delay -= 0.04
  delay_0.04
else
  count down delay
  pulse when delay hits 0
goto loop

 */
/* Maximum speed I want is 1/4 rev per second, 32*200/4=800 pulses per second

   Minimum speed I want is 32*200/250=25.6 pulses per second which is
   the smallest delay that makes sense because we are running at 250Hz

   800 pulses per second is 0.00125 second delay
   25  pulses per second is 0.04000 second delay
   If delay unit 1 is 0.00125 to get 0.04 we need 32 unit delay

   A compromise:
   1  = 0.0016 = 625 PPS
   25 = 0.0400 = 250 PPS
*/

/* The output shift registers for the state machines on the RP2040 can
   only hold 32 bits at a time, so if we use the 90-degree example
   above, we will need to send the pulses 32bits at a time, so for 200
   steps, we’ll need to send 32 + 32 + 32 + 32 + 32 + 32 + 8 within
   different and not necessarily sequential loops in the main program,
   we do that by checking if the Tx fifo is empty before sending the
   next 32 bits, this does not block the main loop as we only need to
   run a few lines of code to check and send the next bits. */

// speed = 0 to 64000;

delay = 0xffff; // Inverse of speed, range is 0 to 0xffff, sent in raw binary 

// pseudo code
// old cpp code, remember distance not speed base

    // Run the motor while we have more steps
    if(m_motor_steps_to_go > 0)
    {
      m_motor_steps_to_go--;
      // Pulse high time is always the same
      gpioWrite(m_motor_pulse_gpio, 1);

      // Delay Time High: Pulse the motor high then use the actual pulse time to
      // determine the pulse low time.
      m_pulse_low_us = GetPulseLowTime(gpioDelay(m_pulse_high_us));

      // Delay Time Low: Now do the low pulse
      gpioWrite(m_motor_pulse_gpio, 0);
      gpioDelay(m_pulse_low_us);
      cout << " steps_to_go=" << m_motor_steps_to_go << " pulse_low_us=" << m_pulse_low_us << " pulse_high_us="  << m_pulse_high_us << std::endl;
    }
    else
    {
      // If we have no data, sleep some waiting on new data, using 2ms which is
      // half gyro rate.
      gpioDelay(500);
    }

// Since we receive data at 250Hz and we would like minimum rate to be
// 0 and maximum total rate should be 32x200 for 1 revolution for
// second.
//
// What that means is for very slow speeds we should get delays of
// 32x200/250 because we will reread the delay often and if it should
// still be slow. For this case the delay would need to be 26 or more.
//
// 



new_delay = read(); read delay
if (new_delay => 0)
  {
    delay = new_delay;
  }
 else
   {
     if (--delay 0)
       pulse()
   }

if (delay > 0xffff / 2)
  {
    delay_loop_32768();
  }
 else if (delay > 0xffff / 4)
   {
     delay_loop_16768();
   }
 else if (delay > 0xffff / 8)
   {
     delay_loop_8768();
   }
 else if (delay > 0xffff / 16)
   {
     delay_loop_4096();
   }
 else if (delay > 0xffff / 32)
   {
     delay_loop_2048();
   }
 else if (delay > 0xffff / 64)
   {
     delay_loop_1028();
   }
 else if (delay > 0xffff / 128)
   {
     delay_loop_512();
   }
 else if (delay > 0xffff / 256)
   {
     delay_loop_256();
   }




 else if (delay > 0xffff / 4)

 else if (delay > 0xffff / 8)
 else if (delay > 0xffff / 8)
 else if (delay > 0xffff / 8)
 else if (delay > 0xffff / 8)
                  
                      
                
