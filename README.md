#erle-rangefinder

##Compile instructions

#### Terminal
To visualize the lectures from the sensor in the terminal

```bash
g++ main.cpp LRM30_serial.cpp -o main -I/usr/local/include -L/usr/local/lib -lserial
```

#### Qt

```bash
qtcreator BoschLMR30.pro
```

Click on the hammer to compile ;)

##Dependencies
* [serial](https://github.com/wjwwood/serial)
