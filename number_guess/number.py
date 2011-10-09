#!/usr/bin/python

# A simple number guessing game to implement with Matt

import random
import sys

number = random.randint(1, 10)

guess = -1
while True:
  try:
    guess = int(raw_input('I am thinking of a number between 1 and 10. Guess? '))
  except:
    print 'It has to be a number, muggins!'

  if guess > number:
    print 'Lower'
  if guess == number:
    print 'Right!!!'
    sys.exit(0)
  if guess < number:
    print 'Higher'
