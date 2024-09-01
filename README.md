# Pixy2 camera and STS3215/ST3215 servo automatic dice roller/tester (STM32H503RB)
An STM32 HAL demo of an automatic dice roller with a computer vision to automatically log consecutive results.

![STS3215/ST3215 in action](/Assets/Images/pixy2_dice_roller_in_action.jpg)

![STS3215/ST3215 OLED](/Assets/Images/pixy2_dice_roller_oled.jpg)

The ST3215 servo from Waveshare seems to be a rebranded STS3215 servo produced by Feetech - you can use manuals and examples from both companies to write your own library. I cannot share here a complete implementation for STM32 because writing the missing part is the lab exercise :innocent: The demo code is written as a blocking one. One of the tasks is to rewrite the library into a non-blocking one (_IT, _DMA, _ReceiveToIdle). Other class activities may include error handling and object-like structures for more elegant handling of multi-servo scenarios. This is exactly the same story as for SC15/SCS15 already told [here](https://github.com/ufnalski/scs15_servo_half_duplex_l432kc). Note that not all registers are copycatted. For example, in the SCS15 servo the motor mode is activated by setting both angle limits to zero, whereas the STS2315 servo has a dedicated register to enable switching between modes.

And now let's use the servo in some useless machine [^1]. Or maybe let's prototype something potentially more useful - some testing equipment. My contraption was inspired by [Fair Dice Tester](https://gerry-chen.com/projects/fairdicetester/). However, I had to cut corners because I had alloted only two evenings for building it and making it run. Therefore, writing my own computer vision system was not an option.

There are almost countless evaluation kits helping us dive into the word of robotic vision systems. Maybe you already have one in your home laboratory? If yes, try to use it in a similar contraption. If not, try to use ESP32-CAM (can easily be bought under $10). My vision system was not bought deliberately for this project. A couple of years ago the [Pixy2](https://pixycam.com/pixy2/) vision system from [Charmed Labs](https://charmedlabs.com/default/about/) caught my attention for its straightforward integration with the LEGO EV3 platform[^2] and Arduino projects[^3]. I still use a timeless line follower from time to time to lure high schoolers into robotics and then to welcome them at our Alma Mater as the full-blown university students :sunglasses: Back then the choice of such pretrained camera systems for tinkerers was rather limited. Today, i.e. when I'm writing this README.md, it is a whole new ball game. You have to do your own research and pick one that best suits you current (and future) needs. Coincidentally, my Pixy2 turned out to be a perfect choice for the dice with color-coded faces.

[^1]: [Useless machines/boxes on YT](https://www.youtube.com/results?search_query=useless+machine)
[^2]: [https://www.mathworks.com/hardware-support/lego-mindstorms-ev3-simulink.html](https://www.mathworks.com/help/simulink/supportpkg/legomindstormsev3_ref/pixy2visionsensor.html)
[^3]: [Hooking up Pixy to a Microcontroller (like an Arduino)](https://docs.pixycam.com/wiki/doku.php?id=wiki:v1:hooking_up_pixy_to_a_microcontroller_-28like_an_arduino-29)

# Missing files?
Don't worry :slightly_smiling_face: Just hit Alt-K to generate /Drivers/CMCIS/ and /Drivers/STM32H5xx_HAL_Driver/ based on the .ioc file. After a couple of seconds your project will be ready for building.

# Libraries
* OLED: [stm32-ssd1306](https://github.com/afiskon/stm32-ssd1306) (MIT license)

# Software tools
* A serial monitor such as [YAT](https://sourceforge.net/projects/y-a-terminal/), [RealTerm](https://sourceforge.net/projects/realterm/) or [Tabby](https://tabby.sh/).

![Pixy2 dice roller serial debug](/Assets/Images/pixy2_dice_roller_debug.JPG)

# Exemplary hardware
* [Kostka kolorowa do gry K6 16 mm Kostki Oko](https://allegro.pl/oferta/kostka-kolorowa-do-gry-k6-16-mm-kostki-oko-11627761577) / [Color-Dice 20mm, White, 6 Colors](https://bartlshop.de/en/color-dice-20mm-white-6-p3677/)
* [Baseus Mini Clip Lamp](https://e-baseus.com/product-pol-3049-Baseus-Mini-Clip-Lamp-Lampka-bezprzewodowa-LED-na-klips.html) - a diffused white light helps me get consistent readings. Note also tires/tyres[^4] deployed to limit the field of view and a neutral to the dice faces black hot-melt adhesive [^5].

Pixy2 in action:

![Die blue face](/Assets/Images/blue_face.jpg)
![Die green face](/Assets/Images/green_face.jpg)
![Die orange face](/Assets/Images/orange_face.jpg)
![Die red face](/Assets/Images/red_face.jpg)
![Die violet face](/Assets/Images/violet_face.jpg)
![Die yellow face](/Assets/Images/yellow_face.jpg)

[^4]: [fun fact] [Largest annual volume toy tyre manufacturer ever](https://www.guinnessworldrecords.com/world-records/100909-largest-tyre-manufacture-per-annum)
[^5]: The initial orange hot glue stolen from my daughter's workshop turned out to be totally impractical. Guess why :slightly_smiling_face:

# Docs, libraries, tutorials and examples
## STS3215 servo drive
* [ST3215 Servo](https://www.waveshare.com/wiki/ST3215_Servo) (Waveshare)
* [Arduino library for Feetech's STS smart servo](https://github.com/matthieuvigne/STS_servos) (matthieuvigne on GitHub)
* [Feetech STS3215 manuals EN](https://github.com/matthieuvigne/STS_servos/tree/main/extras) (matthieuvigne on GitHub)

## Robot vision for tinkerers and beyond - just a couple of examples out of almost countless out there
* [Edge Impulse](https://edgeimpulse.com/)
* [Simple ESP32-CAM Object Detection](https://www.youtube.com/watch?v=HDRvZ_BYd08) (DroneBot Workshop)
* [ESP32-CAM: Machine Vision Tips, Camera Guides and Projects](https://www.arducam.com/esp32-machine-vision-learning-guide/) (ArduCam)
* [SenseCraft](https://sensecraft.seeed.cc/) (Seeed Studio)
* [ESP32 CAM Object Detection & Identification with OpenCV](https://how2electronics.com/esp32-cam-based-object-detection-identification-with-opencv/) (How to Electronics)
* [Machine Vision with HuskyLens](https://www.youtube.com/watch?v=E140gPLPz4A) (DroneBot Workshop)
* [Pixy2 Camera - Image Recognition for Arduino & Raspberry Pi](https://www.youtube.com/watch?v=391dXDjqzXA) (DroneBot Workshop)
* [Pixy2 and STM32](https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:porting_guide) (PixyCam)
* [OpenMV](https://openmv.io/)
* [JeVois Smart Machine Vision](https://www.jevoisinc.com/)
* [OpenMV vs. Huskylens: which one to buy with LEGO MINDSTORMS or SPIKE](https://www.youtube.com/watch?v=vYGV4TNVVfw) (Antons Mindstorms)
* [NVIDIA Jetson Nano](https://www.waveshare.com/product/ai/boards-kits/jetson-nano.htm) (Waveshare)
* [Get Your AI in Gear with a JetBot AI Robot Kit](https://www.nvidia.com/en-us/autonomous-machines/embedded-systems/jetbot-ai-robot-kit/) (NVIDIA)

## The Chi-squared test
* [Understanding The Chi-Squared Test](https://matteocourthoud.github.io/post/chisquared/) (Matteo Courthoud)
* [Understanding the Chi-Square Test: An Introduction to Its Concept and Applications](https://medium.com/@anishnama20/understanding-the-chi-square-test-an-introduction-to-its-concept-and-applications-9c9009ddb38) (Medium)
* [Chi-Square Test](https://www.geeksforgeeks.org/chi-square-test/) (GeeksforGeeks)
* [Chi-square statistic for hypothesis testing | AP Statistics | Khan Academy](https://www.youtube.com/watch?v=jABsbNBPXIk)
* [GNU Scientific Library](https://www.gnu.org/software/gsl/doc/html/)

## Randomness, prime numbers and cryptosystems
* [What is Random?](https://www.youtube.com/watch?v=9rIy0xY99a0) (Vsauce)
* [What is NOT Random?](https://www.youtube.com/watch?v=sMb00lz-IfE) (Veritasium)
* [Riemann hypothesis](https://en.wikipedia.org/wiki/Riemann_hypothesis) (Wikipedia)
* [RSA (cryptosystem)](https://en.wikipedia.org/wiki/RSA_(cryptosystem)) (Wikipedia)
* [Could Riemann hypothesis solve certainly RSA?](https://crypto.stackexchange.com/questions/31024/could-riemann-hypothesis-solve-certainly-rsa) (StackExchange)


# What next?
Write a function that implements Pearson's chi-squared test[^6][^7]. Let your robot roll the dice at least for 30 times[^8]. Is you dice fair? No? Even if the dice is biased you can still generate a truly random bitstream. Why? It is still a chaotic system. How? Check the Von Neumann extractor[^9][^10]. Verify the resulting stream using one of the statistical test suites[^11].

[^6]: [Pearson's chi-squared test](https://en.wikipedia.org/wiki/Pearson%27s_chi-squared_test) (Wikipedia)
[^7]: [Dice Testing with the Running Chi-Square DistributionDice Testing with the Running Chi-Square Distribution](https://digitalcommons.wku.edu/cgi/viewcontent.cgi?article=1006&context=seas_faculty_pubs) (Western Kentucky University)
[^8]: [How can I test whether a die is fair?](https://rpg.stackexchange.com/questions/70802/how-can-i-test-whether-a-die-is-fair) (StackExchange)
[^9]: [Von Neumann extractor](https://prefetch.eu/know/concept/von-neumann-extractor/)
[^10]: [How to best obtain bit sequences from throwing normal dice?](https://crypto.stackexchange.com/questions/6175/how-to-best-obtain-bit-sequences-from-throwing-normal-dice) (StackExchange)
[^11]: [How to test random numbers?](https://stackoverflow.com/questions/778718/how-to-test-random-numbers) (StackExchange)

# Call for action
Create your own [home laboratory/workshop/garage](http://ufnalski.edu.pl/control_engineering_for_hobbyists/2024_dzien_otwarty_we/Dzien_Otwarty_WE_2024_Control_Engineering_for_Hobbyists.pdf)! Get inspired by [ControllersTech](https://www.youtube.com/@ControllersTech), [DroneBot Workshop](https://www.youtube.com/@Dronebotworkshop), [Andreas Spiess](https://www.youtube.com/@AndreasSpiess), [GreatScott!](https://www.youtube.com/@greatscottlab), [ElectroBOOM](https://www.youtube.com/@ElectroBOOM), [Phil's Lab](https://www.youtube.com/@PhilsLab), [atomic14](https://www.youtube.com/@atomic14), [That Project](https://www.youtube.com/@ThatProject), [Paul McWhorter](https://www.youtube.com/@paulmcwhorter), [Max Imagination](https://www.youtube.com/@MaxImagination), and many other professional hobbyists sharing their awesome projects and tutorials! Shout-out/kudos to all of them!

> [!WARNING]
> Automation and control engineering - do try this at home :exclamation:

190+ challenges to start from: [Control Engineering for Hobbyists at the Warsaw University of Technology](http://ufnalski.edu.pl/control_engineering_for_hobbyists/Control_Engineering_for_Hobbyists_list_of_challenges.pdf).

Stay tuned!
