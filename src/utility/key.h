/// \file key.h
/// \brief Defines the Key class.
#ifndef KEY_H
#define KEY_H

/// \brief Represents a single key within the keypad.
class Key
{
public:
    // CONSTRUCTORS
    /// \brief Default constructor required only for Key array initialization.
    Key();
    /// \brief Creates a new key instance.
    /// \param Character The character assigned to the key.
    Key(char Character);

    // METHODS
    /// \brief Updates the steady state of the key by debouncing immediate state reads.
    /// \param CurrentState The immediately read state of they key.  TRUE if pressed, FALSE if not pressed.
    /// \return Returns TRUE if the key has transitioned to a different steady state, otherwise FALSE if it remains in the same steady state.
    /// \note This method uses steady-state-time to debounce the key.
    /// \note Use the return from this function to serve as the "event" raised by a transition of the key's steady state.
    bool Update(bool CurrentState);

    // PROPERTIES
    /// \brief The amount of time (in ms) that a key must remain in steady state before being accepted as "transitioned" into that state.
    unsigned long DebouncePeriod;
    /// \brief Gets the character assigned to the key.
    /// \return The character assigned to the key.
    char Character();
    /// \brief Gets the debounced state of the key.
    /// \return Returns TRUE if the key is pressed, otherwise FALSE if the key is not pressed.
    bool IsPressed();

private:
    // ATTRIBUTES
    /// \brief Stores the character assigned to the key.
    char mCharacter;
    /// \brief Stores the last time an immediate (not debounced) transition was made.  Used for debouncing.
    unsigned long mLastTransition;
    /// \brief Stores the last immediate (not debounced) state the key was measured to be in.  Used for debouncing.
    bool mPreviousState;
    /// \brief Stores the debounced state of the key.
    bool mIsPressed;
};

#endif // KEY_H
