/// \file keypad.h
/// \brief Defines the Keypad class.
#ifndef KEYPAD_H
#define KEYPAD_H

#include "Arduino.h"

/// \brief The Keypad class enables simple use of a typical matrix keypad with Arduino.
class Keypad
{
public:
    // CONSTRUCTORS
    /// \brief Instantiates a new keypad.
    /// \param NRows The number of rows in the keypad.
    /// \param NCols The number of columns in the keypad.
    /// \param Characters An array defining the characters of the keypad in row-major order.
    /// \param RowPins An array defining the Arduino pins connected to each row of the matrix.
    /// \param ColPins An array defining the Arduino pins connected to each column of the matrix.
    Keypad(byte NRows, byte NCols, char Characters[], byte RowPins[], byte ColPins[]);
    /// \brief Destroys the keypad instance and cleans up memory.
    ~Keypad();

    // METHODS
    /// \brief Polls the keypad and performs assigned duties.
    void Spin();
    // Buffer Management
    /// \brief Enables the keypad's internal FIFO character buffer.
    /// \param NCharacters Specifies the size of the character buffer.
    /// \note The keypad's FIFO buffer will discard old data in favor of new data once the buffer becomes filled.
    /// \note Empty positions in the buffer are filled with null characters.
    void EnableBuffer(byte NCharacters);
    /// \brief Disables the keypad's internal FIFO character buffer.
    void DisableBuffer();
    /// \brief Enables read access to the keypad's FIFO character buffer.
    /// \return A constant reference to teh keypad's FIFO character buffer.
    const char* GetBuffer();
    /// \brief Clears the contents of the keypad's FIFO character buffer.
    void ClearBuffer();
    /// \brief Gets the size of the keypad's internal FIFO buffer.
    /// \return The size of the buffer as a number of characters.
    byte BufferSize();
    // Callback Management
    /// \brief Attaches a key down callback to the keypad.
    /// \param Callback The callback function that will be executed each time a key is pressed.
    /// \details This callback will be executed once as soon as a key is pressed.  The function will be passed the character that was pressed as a parameter.
    void AttachKeyDown(void(*Callback)(char));
    /// \brief Detaches the assigned key down callback from the keypad.
    void DetachKeyDown();
    /// \brief Attaches a key up callback to the keypad.
    /// \param Callback The callback function that will be executed each time a key is released.
    /// \details This callback will be executed once as soon as a key is released.  The function will be passed the character that was released as a parameter.
    void AttachKeyUp(void(*Callback)(char));
    /// \brief Detaches the assigned key up callback from the keypad.
    void DetachKeyUp();

private:
    // ATTRIBUTES
    /// \brief Stores the number of rows in the keypad.
    byte mNRows;
    /// \brief Stores the number of columns in the keypad.
    byte mNCols;
    /// \brief Stores an array of the Arduino pins connected to the rows of the keypad.
    byte* mRowPins;
    /// \brief Stores an array of the Arduino pins connected to the columns of the keypad.
    byte* mColPins;
    /// \brief Stores a 2D array [rows x cols] of the keys in the keypad.
    Key** mKeys;
    /// \brief The keypad's internal FIFO buffer.
    /// \note Set as NULL when no buffer is used.
    char* mBuffer;
    /// \brief The size of the keypad's internal FIFO buffer, in number of characters.
    byte mBufferSize;
    /// \brief Stores the current position in which a new character should be written to in the keypad's internal FIFO buffer.
    int mBufferWritePosition;
    // Callbacks
    /// \brief Stores a reference to the attached key down callback function.
    /// \note Set as NULL when no callback is attached.
    void(*mKeyDownCallback)(char);
    /// \brief Stores a reference to the attached key down callback function.
    /// \note Set as NULL when no callback is attached.
    void(*mKeyUpCallback)(char);

    // METHODS
    /// \brief Adds a new character to the end of the keypad's internal FIFO buffer.
    /// \details This method will remove old characters from the buffer as new characters are read in (FIFO implementation).
    void BufferCharacter(char Character);
};

#endif // KEYPAD_H
