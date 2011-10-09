#!/usr/bin/python

# A hangman implementation so Matt and I can talk about computer programming...
# Note that this is optimized for being easy to explain, not nessesarily
# being efficient to execute.

import random
import sys

BLANK_LINE = ' ' * 80
SCREEN = [BLANK_LINE, BLANK_LINE, BLANK_LINE, BLANK_LINE, BLANK_LINE,
          BLANK_LINE, BLANK_LINE, BLANK_LINE, BLANK_LINE, BLANK_LINE]
GUESSES = []


def PickWord():
  words = []
  f = open('/usr/share/dict/words', 'r')
  for word in f.readlines():
    word = word.rstrip()
    if len(word) < 6:
      words.append(word)

  random.shuffle(words)
  return words[0].lower()


def PrintScreen():
  global SCREEN
  global GUESSES

  for line in SCREEN:
    print line
  print 'Guesses: %s' % ''.join(GUESSES)


def WriteToScreen(x, y, s):
  global SCREEN

  line = SCREEN[y]
  line = '%s%s%s' %(line[:x], s, line[x + len(s):])
  SCREEN[y] = line


def DrawHangman(incorrect):
  #  ========
  #  |      |
  #  |     (O)
  #  |      |
  #  |     -|-
  #  |      |
  #  |     / \
  #  |
  #  |\
  #  ==========

  if incorrect == 1:
    WriteToScreen(2, 9, '=' * 5)
  if incorrect == 2:
    WriteToScreen(7, 9, '=' * 6)
  if incorrect == 3:
    WriteToScreen(2, 5, '|')
    WriteToScreen(2, 6, '|')
    WriteToScreen(2, 7, '|')
    WriteToScreen(2, 8, '|\\')
  if incorrect == 4:
    WriteToScreen(2, 1, '|')
    WriteToScreen(2, 2, '|')
    WriteToScreen(2, 3, '|')
    WriteToScreen(2, 4, '|')
  if incorrect == 5:
    WriteToScreen(2, 0, '=' * 4)
  if incorrect == 6:
    WriteToScreen(5, 0, '=' * 4)
  if incorrect == 7:
    WriteToScreen(9, 1, '|')
  if incorrect == 8:
    WriteToScreen(8, 2, '( )')
  if incorrect == 9:
    WriteToScreen(9, 2, '0')
  if incorrect == 10:
    WriteToScreen(9, 3, '|')
  if incorrect == 11:
    WriteToScreen(9, 4, '|')
  if incorrect == 12:
    WriteToScreen(9, 5, '|')
  if incorrect == 13:
    WriteToScreen(8, 6, '/')
  if incorrect == 14:
    WriteToScreen(10, 6, '\\')
  if incorrect == 15:
    WriteToScreen(8, 4, '-')
  if incorrect == 16:
    WriteToScreen(10, 4, '-')


# The main program
word = PickWord()
WriteToScreen(0, 0, word)
WriteToScreen(60, 1, '_' * len(word))

correct = 0
wrong = 0

while True:
  PrintScreen()
  guess = raw_input('Guess? ')[0].lower()
  if guess not in GUESSES:
    GUESSES.append(guess)

    if guess not in word:
      wrong += 1
      DrawHangman(wrong)

    i = 0
    for character in word:
      if character == guess:
        correct += 1
        WriteToScreen(60 + i, 1, guess)

      i += 1

  if correct == len(word):
    PrintScreen()
    print 'YOU WIN!!!'
    sys.exit(0)

  if wrong == 16:
    WriteToScreen(60, 1, word)
    PrintScreen()
    print 'TOO LATE!!!'
    sys.exit(0)
