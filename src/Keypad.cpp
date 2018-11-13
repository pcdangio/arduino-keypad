#include "keypad.h"

Keypad::Keypad(byte NRows, byte NCols, char Characters[], byte RowPins[], byte ColPins[])
{
  // Store keypad size.
  Keypad::mNRows = NRows;
  Keypad::mNCols = NCols;
  // Copy the row and column pin arrays, and configure the pins for input/output.
  Keypad::mRowPins = new byte[NRows];
  for(int r = 0; r < NRows; r++)
  {
    Keypad::mRowPins[r] = RowPins[r];
    // Rows are drivers.
    pinMode(Keypad::mRowPins[r], OUTPUT);
    // Inputs use a pullup, so drive non-observed pins high.
    digitalWrite(Keypad::mRowPins[r], HIGH);
  }
  Keypad::mColPins = new byte[NCols];
  for(int c = 0; c < NCols; c++)
  {
    Keypad::mColPins[c] = ColPins[c];
    // Columns are inputs.
    pinMode(Keypad::mColPins[c], INPUT_PULLUP);
  }
  // Create the 2D array of keys.
  Keypad::mKeys = new Key*[NRows];
  for(int r = 0; r < NRows; r++)
  {
    Keypad::mKeys[r] = new Key[NCols];
    for(int c = 0; c < NCols; c++)
    {
      Keypad::mKeys[r][c] = Key(Characters[r*NCols+c]);
    }
  }
  // Set buffer to NULL.
  Keypad::mBuffer = NULL;
  Keypad::mBufferSize = 0;
  Keypad::mBufferWritePosition = 0;
  // Set callbacks to NULL.
  Keypad::mKeyDownCallback = NULL;
  Keypad::mKeyUpCallback = NULL;
}
Keypad::~Keypad()
{
  // Clean up resources.
  delete [] Keypad::mRowPins;
  delete [] Keypad::mColPins;
  for(int r = 0; r < Keypad::mNRows; r++)
  {
    delete [] Keypad::mKeys[r];
  }
  delete [] Keypad::mKeys;
  Keypad::DisableBuffer();
}
void Keypad::Spin()
{
  // Poll the keypad.
  // Use rows as drivers.  Columns use input_pullup so drive low.
  for(int r = 0; r < Keypad::mNRows; r++)
  {
    // Pull the row low.
    digitalWrite(Keypad::mRowPins[r], LOW);
    for(int c = 0; c < Keypad::mNCols; c++)
    {
      // Check the column and update the associated key.
      // If the key update returns true, a state transition has occured.
      if(Keypad::mKeys[r][c].Update(!digitalRead(Keypad::mColPins[c])))
      {
        // Transition hsa occured.  Check if it is a key up or key down transition.
        if(Keypad::mKeys[r][c].IsPressed())
        {
          // Transition is a KeyDown.
          // Add to buffer if enabled.
          Keypad::BufferCharacter(Keypad::mKeys[r][c].Character());
          // Raise callback if one has been attached.
          if(Keypad::mKeyDownCallback)
          {
            (*Keypad::mKeyDownCallback)(Keypad::mKeys[r][c].Character());
          }
        }
        else
        {
          // Transition is a KeyUp.  Raise callback if one has been attached.
          if(Keypad::mKeyUpCallback)
          {
            (*Keypad::mKeyUpCallback)(Keypad::mKeys[r][c].Character());
          }
        }
      }
    }
    // Reset the row to high.
    digitalWrite(Keypad::mRowPins[r], HIGH);
  }
}
void Keypad::EnableBuffer(byte NCharacters)
{
  // Clear out the prior buffer.
  Keypad::DisableBuffer();
  // Create new buffer.
  Keypad::mBuffer = new char[NCharacters];
  Keypad::mBufferSize = NCharacters;
  Keypad::mBufferWritePosition = 0;
  // Initialize the buffer with null characters.
  for(int i = 0; i < NCharacters; i++)
  {
    Keypad::mBuffer[i] = 0;
  }
}
void Keypad::DisableBuffer()
{
  // Clear out the prior buffer.
  if(Keypad::mBuffer)
  {
    delete [] Keypad::mBuffer;
  }
  Keypad::mBuffer = NULL;
  Keypad::mBufferSize = 0;
  Keypad::mBufferWritePosition = 0;
}
const char* Keypad::GetBuffer()
{
  return Keypad::mBuffer;
}
void Keypad::ClearBuffer()
{
  // Write null characters to the entire buffer.
  for(byte i = 0; i < Keypad::mBufferSize; i++)
  {
    Keypad::mBuffer[i] = 0;
  }
  // Reset the write position.
  Keypad::mBufferWritePosition = 0;
}
byte Keypad::BufferSize()
{
  return Keypad::mBufferSize;
}
void Keypad::BufferCharacter(char Character)
{
  if(Keypad::mBuffer)
  {
    // Add the character to the end of the buffer.
    // First check if the buffer is full.
    if(Keypad::mBufferWritePosition < Keypad::mBufferSize)
    {
      // Add the character at the current write position.
      Keypad::mBuffer[Keypad::mBufferWritePosition] = Character;
      // Increment the write position.
      Keypad::mBufferWritePosition++;
    }
    else
    {
      // Buffer is overflowed.
      // Shift all characters backward.
      memcpy(&Keypad::mBuffer[0], &Keypad::mBuffer[1], Keypad::mBufferSize - 1);
      // Set the last character.
      Keypad::mBuffer[Keypad::mBufferSize - 1] = Character;
      // No need to increment the write position.
    }
  }
}

void Keypad::AttachKeyDown(void(*Callback)(char))
{
  Keypad::mKeyDownCallback = Callback;
}
void Keypad::DetachKeyDown()
{
  Keypad::mKeyDownCallback = NULL;
}
void Keypad::AttachKeyUp(void(*Callback)(char))
{
  Keypad::mKeyUpCallback = Callback;
}
void Keypad::DetachKeyUp()
{
  Keypad::mKeyUpCallback = NULL;
}
