#!/bin/bash

# This is a demo for pandascript. Here we recreate the sample document that
# panda creates, but using our little scripting language. Pandascript reads
# from stdin, and creates a PDF, so we need to create a file to cat into
# pandascript. If anyone can think of a better way of doing this, then please
# let me know at mikal@stillhq.com...

echo "Welcome to the PandaScript 0.3 sample application...\n"

echo "output \"output.pdf\"" > commands.$$

lineDepth=0
currpageWidth=592
currpageHeight=841

while [ $lineDepth -ne 8 ]
do
  echo "newpage" >> commands.$$

  # Put in the background images
  echo "imagebox 0 0 $(( $currpageHeight / 2 )) $currpageWidth \"input.tif\" TIFFIMG" >> commands.$$
  echo "imagebox $(( $currpageHeight / 2 )) 0 $currpageHeight $currpageWidth \"input2.tif\" TIFFIMG" >> commands.$$

  # Put some text onto it
  echo "setfontmode $lineDepth" >> commands.$$
  echo "setcharacterspacing $lineDepth.0" >> commands.$$
  echo "setwordspacing $lineDepth.0" >> commands.$$
  echo "sethorizontalscaling 0.87" >> commands.$$
  echo "setleading 16.0" >> commands.$$

  # I am not drawing a multiline string here because I am not sure how to 
  # represent this in the PDF at the moment, also pandascript does not have
  # access to the superscript and subscript stuff at the moment.
  echo "textbox $(( $lineDepth * 20 + 10 )) $(( $lineDepth + 10 )) 100 30 \"Hello World! My name is Panda!\nAnd I am a PDF generator\nI handle multiple line text ok .once you have set a leading.\"" >> commands.$$

  echo "setfont \"Symbol\"" >> commands.$$
  echo "textbox $(( $lineDepth * 20 + 50 )) $(( 10 + $lineDepth )) 10 30 \"Symbol\"" >> commands.$$

  echo "setfont \"Helvetica-Bold\"" >> commands.$$
  echo "textbox $(( $lineDepth * 20 + 70 )) $(( 30 + $lineDepth )) 100 30 \"A line in Helvetica-Bold\"" >> commands.$$

  # Insert some images
  echo "imagebox 100 100 150 150 \"gnu-head.jpg\" JPEGIMG" >> commands.$$
  echo "textbox 90 110 200 200 \"INFRONTINFRONTINFRONT\"" >> commands.$$
  echo "textbox 190 210 300 300 \"BEHINDBEHINDBEHIND\"" >> commands.$$
  echo "imagebox 200 200 317 317 \"gnu_box.jpg\" JPEGIMG" >> commands.$$
  echo "imagebox 317 317 434 434 \"gnu_box.jpg\" JPEGIMG" >> commands.$$
  echo "imagebox 434 434 551 551 \"gnu_box.jpg\" JPEGIMG" >> commands.$$

  # Do an imageboxrot or two to test the code included by Ceasar Miquel
  echo "imageboxrot 600 0 717 117 15.0 \"gnu_box.jpg\" JPEGIMG" >> commands.$$
  echo "imageboxrot 600 200 717 317 30.0 \"gnu_box.jpg\" JPEGIMG" >> commands.$$
  echo "imageboxrot 600 400 717 517 42.0 \"gnu_box.jpg\" JPEGIMG" >> commands.$$
  echo "imageboxrot 700 0 817 117 90.0 \"gnu_box.jpg\" JPEGIMG" >> commands.$$
  echo "imageboxrot 700 200 817 317 132.0 \"gnu_box.jpg\" JPEGIMG" >> commands.$$

  echo "Created textbox and inserted first image"
  echo "textbox 300 10 400 50 \"A second textbox on the page\"" >> commands.$$

  echo "Created second textbox"
  echo "setfont \"Times-Roman\"" >> commands.$$
  echo "textbox 600 10 700 300 \"The background image on this page is Copyright 2000 Andrew Cagney\"" >> commands.$$
  echo "textbox 620 10 720 300 \"and is distributed under the terms of the GPL...\"" >> commands.$$

  lineDepth=$(( $lineDepth + 1 ))
done

echo "close" >> commands.$$

# Now we can run the script
cat commands.$$ | ./pandascript
