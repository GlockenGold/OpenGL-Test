# OpenGL-Test
## Step 1)
Run the following commands in terminal.
```bash
git clone --recursive git@github.com:GlockenGold/OpenGL-Test.git
cd OpenGL-Test/
rm -rf build/
cmake -S . -B build
cd build/
make
```


## Step 2)
Try running the generated binary.

```bash
./test
```

## Step 3)
```c++
if(output == "Failed to create GLFW window"){
  cry();
}
else{
  cry(); // WHYYYYY DOES IT WORK ON YOUR SYSTEM AND NOT MINE?!??!?!?
}
```
