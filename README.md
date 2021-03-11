# Steganography Generator: Text → Bitmap


### Overview
This program takes text input from the user, converts it into a series of six-digit hex color codes, converts each hex value into a 24-bit sRGB color, and outputs a bitmap image.


### Notes / Usage
* Make sure input is plain text
* Conversions include space and newline characters
* Longer text produces larger images (more characters = more pixels = bigger dimensions)
* Excessively large inputs, such as entire books or long documents, can be processed (within reason), though it takes a few extra seconds to generate larger bitmaps
* The output image gets saved to the cmake-build-debug directory

----

### Example
**Input:**
[ _Here Comes the Sun_ by The Beatles (Lyrics)](https://genius.com/The-beatles-here-comes-the-sun-lyrics)


**Output** (enlarged and uploaded as PNG)**:**

<img width="275" alt="herecomesthesun" src="https://user-images.githubusercontent.com/35256452/110777227-db8e2600-829b-11eb-9e86-0356c0591d6e.png">
