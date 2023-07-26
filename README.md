Implementation of Hyper Dimensional Computing (HDC) using FPGA
========

Contributors
--------
* [Professor Sitao Huang](https://engineering.uci.edu/users/sitao-huang)
* [Joseph Prince](https://github.com/josephdprince/)
* [Hong Dong](https://github.com/hongdong1220)
* Kyle Van
* Brad Lacy

Summary
--------

The goal of this project is to accelerate the calculations of HDC (Hyper-dimensional Computing) using an FPGA. We want to enable embedded systems with power consumption restraints to run ML/AI applications.

To do this, we wrote a classification algorithm, in C code, that heavily uses HDC. After that, we synthesize the code on [Vitis HLS](https://www.xilinx.com/products/design-tools/vitis/vitis-hls.html) so that the code can be run on the [PYNQ-Z2 board](https://www.xilinx.com/support/university/xup-boards/XUPPYNQ-Z2.html#overview) using an onboard FPGA. 

![PYNQ-Z2 board](https://www.xilinx.com/content/xilinx/en/support/university/xup-boards/XUPPYNQ-Z2/_jcr_content/root/parsys/xilinxcolumns_f62b/childParsys-0/xilinximage.img.jpg/1650540100168.jpg)

Then, we write the code to run the algorithm on a Jupyter Notebook on the Pynq Z2 board. The components of this algorithm that involve HDC are run on the FPGA. All these steps have been completed and will be discussed more in the paper. 
 
This project used data from the [MNIST dataset](https://en.wikipedia.org/wiki/MNIST_database) to test the speed and accuracy of the algorithm, and we compared the time it took to test with and without the FPGA to see if this project actually sped up the calculation.

![](https://upload.wikimedia.org/wikipedia/commons/f/f7/MnistExamplesModified.png)
 