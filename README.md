# Course practice
# lbp feature classifier

  * required : 
    + opencv 3
    + boost

  * install :

  ```console
    git submodule update --init
    mkdir build && cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
  ```

  * train & run
    * data
        * pos
            * ooo.jpg
            * ooo.jpg
        * neg
            * xxx.jpg
            * xxx.jpg

  ```console
    ./build/test/train data # trained model model.xml
    ./build/src/detect model.xml testImg/001.jpg
  ```
