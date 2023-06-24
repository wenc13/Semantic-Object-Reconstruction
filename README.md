# Semantic Object Reconstruction
This is the official implementation of paper: **Semantic Object Reconstruction via Casual Handheld Scanning**

### Contact:
cheng.wen.bts@gmail.com

### Installation:
The code was developed by VS2015 on Windows 10.

**Requirements**:
- DirectX SDK June 2010
- Kinect SDK (prev. to 2.0)
- NVIDIA CUDA 8.0 (for the CUDA implementation)
- PCL-1.8.0

**Optional**:
- Kinect SDK (2.0 and above)
- Prime sense SDK

### Input: 
```
|--parent
      |--depth
      |--rgb
      |--label
```
images are organized as follows:
depthimage.png
colorimage.jpg
labelpart1.png
labelpart2.png
...
labelpartn.png

### Some semantic reconstruction results:

<div align="left">
<img src="DepthSensingLabel/SR_Chair.png" height="187" width="187" >
<img src="DepthSensingLabel/SR_Table.png" height="187" width="187" >
<img src="DepthSensingLabel/SR_Motorcycle.png" height="187" width="219" >
 </div>

### Citation
If you find our work useful in your research, please consider citing:
```
@article{hu2018semantic,
  title={Semantic object reconstruction via casual handheld scanning},
  author={Hu, Ruizhen and Wen, Cheng and Van Kaick, Oliver and Chen, Luanmin and Lin, Di and Cohen-Or, Daniel and Huang, Hui},
  journal={ACM Transactions on Graphics (TOG)},
  volume={37},
  number={6},
  pages={1--12},
  year={2018},
  publisher={ACM New York, NY, USA}
}
```
