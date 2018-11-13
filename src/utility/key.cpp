#include "key.h"

#include "Arduino.h"

Key::Key()
{
  // Use default constructor with null character.
  Key::Key(0);
}
Key::Key(char Character)
{
  Key::mCharacter = Character;
  Key::DebouncePeriod = 25;
  Key::mLastTransition = 0;
  Key::mPreviousState = false;
  Key::mIsPressed = false;
}
bool Key::Update(bool CurrentState)
{
  // Get the time of the update.
  unsigned long CurrentTime = millis();
  // Check for a transition.
  if(CurrentState == Key::mPreviousState)
  {
    // Continued state.  Check if in steady state.
    if(CurrentTime - Key::mLastTransition >= Key::DebouncePeriod)
    {
      // In a steady state.
      // Check if transitioning between steady states.
      if(CurrentState != Key::mIsPressed)
      {
        // Transition occured.
        // Update steady state flag.
        Key::mIsPressed = CurrentState;
        // Signal that a transition has occured.
        return true;
      }
    }
  }
  else
  {
    // State transition.
    // Update previous state.
    Key::mPreviousState = CurrentState;
    // Reset transition timer.
    Key::mLastTransition = CurrentTime;
  }
  // If this point has been reached, no transition between steady states has occured.
  return false;
}
char Key::Character()
{
  return Key::mCharacter;
}
bool Key::IsPressed()
{
  return Key::mIsPressed;
}
