# Semantic Object Reconstruction
It is the official implementation of paper **''[Semantic Object Reconstruction via Casual Handheld Scanning](https://dl.acm.org/doi/10.1145/3272127.3275024)''**.

## Installation:
The code was developed by Microsoft Visual Studio 2015 on Windows 10.

**Requirements**:
- DirectX SDK June 2010
- Kinect SDK (prev. to 2.0)
- NVIDIA CUDA 8.0 (for the CUDA implementation)
- PCL-1.8.0

**Optional**:
- Kinect SDK (2.0 and above)
- Prime sense SDK

## Input: 
Our method takes RGB images, depth images, and part label images as input, and produces a reconstructed semantic object as output. In our paper, we conducted experiments using the [Redwood dataset](http://redwood-data.org/3dscan/). This dataset contains only RGB-D scans; the labels can be obtained using any pre-trained neural network. All the images are organized as follows:  

```
|--parent folder
      |--depth (*.png)
      |--rgb (*.jpg)
      |--label (*.png)
```

## Some semantic reconstruction results:

<div align="left">
<img src="DepthSensingLabel/SR_Chair.png" height="187" width="187" >
<img src="DepthSensingLabel/SR_Table.png" height="187" width="187" >
<img src="DepthSensingLabel/SR_Motorcycle.png" height="187" width="219" >
 </div>

## Contact:
If you have any question, please feel free to contact me (cheng.wen.bts@gmail.com).

## Citation
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

## License
The code is released under MIT License (see LICENSE file for details).
