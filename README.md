# arduino-iambic-keyer

#### Basic hardware and software demo of an Iambic (paddle) morse code keyer. While holding a paddle in one direction it will play a series of dits, and the other direction a series of dahs, and correct spacings for letters and words.

<img src="https://github.com/xigco/arduino-iambic-keyer/raw/master/assets/prototype.jpg">

This is not a very good keyer device, but it's a fun and easy place to start. It needs very few components and some basic hackery. There is one sketch to run with no libraries.

## Hardware
<img src="https://github.com/xigco/arduino-iambic-keyer/raw/master/assets/breadboard.png">

I used an Arduino Micro (based on the Leonardo board) but any board or chip should do the job. The I/O is connected as follows;
<table>
<tr><th>A0</th><td>IN</td><td>10k potentiometer</td></tr>
<tr><th>D2</th><td>IN</td><td>Paddle left</td></tr>
<tr><th>D3</th><td>IN</td><td>Paddle right</td></tr>
<tr><th>D8</th><td>OUT</td><td>Speaker</td></tr>
<tr><th>D13</th><td>OUT</td><td>LED</td></tr>
</table>

## Making a lever
<img src="https://github.com/xigco/arduino-iambic-keyer/raw/master/assets/prototype-keyer.jpg">

There are some great lever designs available on the web - have a search. Paperclips, bulldog clips, saw blades or machined metal parts make good test levers.

Here I made one with 2 microswitches sandwiched between stripboard. A single bulldog clip lever is hinged at the back, so it swings to close microswitches on either side of it. Sadly the lever needs to move quite far making it difficult to accurately describe morse! My next design won't use microswitches.

## Software
Download the master ZIP file and copy the "IambicKeyer" folder into your Arduino sketches folder. It should appear in the File menu when you open the Arduino IDE. Upload and run!
