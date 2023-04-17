#ifndef LUM_H
#define LUM_H

#include "config.h"

// Define a structure to hold the data for the luminosity mailbox
struct mailbox_lum 
{
  int state;                                                // Whether the mailbox is empty or full
  int lumPercent;                                           // The percentage of luminosity (0-100)
};

// Define a structure to hold the data for the luminosity sensor
struct Lum 
{
  int timer;                                                // The timer index for the luminosity sensor
  unsigned long period;                                     // The period for reading the luminosity sensor (in microseconds)
};

// This function waits for a specified period of time
unsigned int waitFor(int timer, unsigned long period)
{
  static unsigned long waitForTimer[MAX_WAIT_FOR_TIMER];    // Timer array
  unsigned long newTime = micros() / period;                // Calculate current time
  int delta = newTime - waitForTimer[timer];                // Calculate time since last call
  if ( delta < 0 ) delta = 1 + newTime;                     // Handle overflow
  if ( delta ) waitForTimer[timer] = newTime;               // Update timer array
  return delta;
}

// Function to sets up the Lum structure with a specified period
void setup_Lum(struct Lum * lum, unsigned long period, mailbox_lum * luminosity)
{
  lum->timer = 0;                                           // set the timer number to 0
  lum->period = period;                                     // set the period of the lum sensor
}

// Function to read the value of the photoresistor and convert it to a percentage
int getLuminosity() 
{
  int lumValue = analogRead(LUM_PIN);                       // Read the analog value from the photoresistor
  int lumPercent = map(lumValue, 4095, 800, 0, 100);        // Convert the value to a percentage
  lumPercent = lumPercent > 99 ? 99 : lumPercent;           // Limit the maximum percentage to 99
  return lumPercent;                                        // Return the calculated percentage
}

// Function to update the luminosity mailboxe
void loop_Lum(struct Lum * lum, mailbox_lum * luminosity) 
{
  if (!(waitFor(lum->timer,lum->period))) return;           // Wait for the specified period
  int lumPercent = getLuminosity();                         // Get the current luminosity percentage
  if (luminosity->state == EMPTY)                           // Check if the luminosity mailbox is empty
  {
    luminosity->lumPercent = lumPercent;                    // Update the current luminosity percentage
    luminosity->state = FULL; // Set the luminosity mailbox state to full
  }
}

#endif