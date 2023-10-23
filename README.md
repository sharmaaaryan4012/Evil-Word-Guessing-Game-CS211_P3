# Evil Word Guessing Game

The Evil Word Guessing Game is a nefarious twist on classic word games like Hangman. Instead of playing by the rules, the computer (chooser) actively tries to deceive the human player (guesser) by not committing to a single word, but rather dynamically updating its word choices based on the player's guesses. This project will enhance your skills in dynamic memory management, C-string operations, command-line arguments, and general programming practices, all while creating a highly entertaining piece of software.

### Game Rules

1. **Chooser**: Chooses a secret word and writes out dashes equal to the word's length.
2. **Guesser**: Begins guessing letters. Correct guesses reveal the letter in the word, while incorrect guesses count against the player.
3. **End Game**: The game ends when either all letters are revealed or the guesser runs out of incorrect guesses.

### Evil Strategy

The Evil Word Guessing Game deviates from the standard rules by not committing to a single word. Instead, it maintains a list of possible words and narrows down the options based on the player's guesses, aiming to prolong the game and confuse the player. The chooser dynamically updates the word choices to maintain ambiguity and increase the difficulty for the guesser.

### How to Play

Compile the program with:

```sh
gcc main.c -o game.out
```

Run the game with different options:

- Default settings with interactive guesses:
  ```sh
  ./game.out
  ```

- Custom settings (e.g., word size of 9, 15 guesses, all special print modes ON):
  ```sh
  ./game.out -n 9 -g 15 -v
  ```

- Default settings with a predefined set of guesses:
  ```sh
  ./game.out < randomLets.txt
  ```

- Custom settings with predefined guesses:
  ```sh
  ./game.out -s -g 12 -l -n 21 < alphabetIn.txt
  ```

### Command-Line Arguments

- `-n [number]`: Set word size (default is 5).
- `-g [number]`: Set the number of guesses (default is 26).
- `-s`: Enable stats mode.
- `-w`: Enable word list mode.
- `-l`: Enable letter list mode.
- `-p`: Enable pattern list mode.
- `-v`: Enable verbose mode (all other modes turned ON).
- `< [filename]`: Use predefined set of guesses from a file.

### Modes

- **Stats Mode**: Prints statistics about the dictionary and the number of possible words remaining during gameplay.
- **Word List Mode**: Prints the full list of possible words before every guess.
- **Letter List Mode**: Prints a list of previously guessed letters before each new guess.
- **Pattern List Mode**: Prints a list of all unique patterns in the word list, together with the frequency of each pattern and the number of changes in the pattern due to the latest guess.
- **Verbose Mode**: Enables all other modes.

### File Reading

The original word list is built from `dictionary.txt`, containing over 120,000 words.

### Interactive User Input

Guesses are made interactively during gameplay. Enter lower-case letters as guesses, or '#' to end the game.

### Example Gameplay

```sh
Number of guesses remaining: 21
Previously guessed letters: a i l n r s t
Guess a letter (# to end game): o
...
```

### Termination

If an invalid command-line argument is provided, the program will display "Invalid command-line argument." and terminate.

Enjoy deceiving your friends with the Evil Word Guessing Game, and may the odds be ever in your (evil) favor!
