# led-control
Test project for Ivideon

## led-server ##

##### HOW TO BUILD #####

0. Clone repo:
```
    git clone https://github.com/borune-k12/led-control.git
```

1. Cd into led-server folder
```
    cd led-control/led-server
``` 
2. Create build dir and cd into it: 
```   
        mkdir build && cd build
```    
3. Configure project:
```   
        cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/path/to/Qt5Config.cmake/or/qt5-config.cmake/ ..
```        
4. Build project:
```
        cmake --build . --target all
```    
##### BUILDING TESTS #####

0. Cd into test folder:
```
        cd led-control/led-server/tests
```
1. Clone googletest repo:
```
        git clone https://github.com/google/googletest.git
```
2. Configure project with -DBUILD_TESTS=True
```
        cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=True -DCMAKE_PREFIX_PATH=/path/to/Qt5Config.cmake/or/qt5-config.cmake/ ..    
````
3. Build test target:
```
        cmake --build . --target led-server-test
```

##### USAGE #####
./led-server [path-to-pipe], 
        by default led-server create and use "/tmp/led.pipe"

## led-client ##

##### USAGE #####

Add executable flag:
```
        chmod a+x led-client.sh
```
and
```
 ./led_client.sh -c command [-a argument] [-p file] [-t timeout]

 Parameters:

 -c, --command	command to execute
		Allowed values:
		 set-led-state, set-led-rate, set-led-color, get-led-state, get-led-rate, get-led-color

 -a, --argument	value to set in case of 'set' command
		Allowed values:
		 on or off                     	for set-led-state command
		 an integer in range [0; 5]    	for set-led-rate command
		 red, green or blue            	for set-led-color command

 -p, --pipe	named pipe for communicating with server

 -t, --timeout	timeout for reading from the pipe

 -h, --help	display this help and exit

 -v, --version	output version information and exit
 ```
