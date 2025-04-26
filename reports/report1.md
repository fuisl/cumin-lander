# First attempt report

This is the

## Materials

- Brass rod has been cut down to brass wire and then use a drill to straighten it to a rod, so we save about 50k.
- The brass rod straighten by this method isn't actually straight, but it is good enough for our purpose.
- We dont need a brass plate for the legs part, we make brass ring with a tri connection.
- We use Xiao Seeed studio nrf52840 as recommended, which is a very small, but not so cheap. We figured out that there are other variants of the nrf52840 that are cheaper, but we are not sure if they are compatible with the code.
- For the sensors, we use standard BME280, costs about 100k. Amazing performance, high accuracy, low power consumption, and small size.
- We used the drill which has no speed control. The rod usually breaks.

## Coding

- When they go low, we go low-er. This is the most ridiculous thing I have ever seen. Low means high and high means low. I don't know how to explain it, but it is a fact. Ohio momment fr
- The code didn't work from the beginning when we tested the whole components on the breadboard. But we has mistakenly thought that the code was broken. Turns out the connection between the Xiao Seed and the breadboard was the problem.
- typing cat is a cool feature
- We haven't customized the code yet (since the last sculpture was fucked)

> _more explanation later_

## The sculpture

1. The frame was ease to made, but I didn't account for the joint, and was too careless about precision so the frame turned out to be kinda bent. Since the wire is 0.8mm OD the, it get heated very quick to the point a solder joint far from the working one get melted and it was frustrating af.
   > try to distant joints as much as possible.
2. The making of the leg was decent, nhưng nó bị méo, phần khó là phần gắn chân vào sub frame, vẫn chưa tìm được quy trình tốt cho việc gắn chân.

> when soldering FLUX IS CRUCIAL, FLUX IS LIFE.

2. Joint cleaning is fine but it's optional

3. 2 ways led is way cooler than 1 face led, the length should be the same to the screen which is 128mm, the resistor part is cool but we need to minimize the resistor leg length to avoid bending.

> Resin does make a really good job in secure the soldering joint hard stuck in place

4. Now come the battery, it is the most bull shit things i have every touch in my life, 5v is all it took to burn my skin, the surface is really hard to heat up for solder.

   > we do bad GND tbh.

5. The button might died due to heat we don't really know yet

## MISTAKE

1. no flux at the start, we didn't realized how important flux is to soldering.
2. no drop test.
3. short circuit is costly expensive

## The flow

1. first thing first, the frame has to be perfectly straght. then come the legs.
2. when assembling the lcd, check out for the gnd session first handed. 2 gnd pins must be perfectly allign for a L shaped rod.
3. then come the sensor.
4. after everything work out, make the sub-frame for the legs and buzzer, assemble the legs and the buzzer onto the sub-frame
   > đừng tiếc công nếu dây turn out short.
   > check if the components is work after every step.
5. Led should be the final step

# FLUX IS LOVE!!! FLUX IS LIFE!!!
