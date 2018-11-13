#ifndef _VOXEL_UTIL_HASH_SDF_H_
#define _VOXEL_UTIL_HASH_SDF_H_

// should be set by application
#ifndef groupthreads
#define groupthreads 1
#endif

#ifndef MINF
#define MINF asfloat(0xff800000)
#endif

#ifndef PINF
#define PINF asfloat(0x7f800000)
#endif

//status flags for hash entries
static const int LOCK_ENTRY = -1;
static const int FREE_ENTRY = -2;
static const int NO_OFFSET  = 0;

//static const uint	 g_WeightSample = 3;	    //weight per sample (per integration step)
//static const uint	 g_WeightMax = 255;		    //maximum weight per voxel
//static const float g_Truncation = 0.05f;	    //5cm world space
//static const float g_TruncScale = 0.02f;	    //how to adapt the truncation: per distance meter, increase truncation by 1 cm

//#define HANDLE_COLLISIONS 1

struct HashEntry
{
	int3 pos;    //hash position (lower left corner of SDFBlock))
	int  offset; //offset for collisions
	int  ptr;	 //pointer into heap to SDFBlock
};

struct Voxel
{
	float sdf;
	uint3 color;
	uint  weight;
	uint  label[12];
    uint  labelFrequency[12];
};

//! computes the (local) virtual voxel pos of an index; idx in [0;511]
int3 delinearizeVoxelIndex(uint idx)
{
	uint x = idx % SDF_BLOCK_SIZE;
	uint y = (idx % (SDF_BLOCK_SIZE * SDF_BLOCK_SIZE)) / SDF_BLOCK_SIZE;
	uint z = idx / (SDF_BLOCK_SIZE * SDF_BLOCK_SIZE);
	return uint3(x,y,z);
}

//! computes the linearized index of a local virtual voxel pos; pos in [0;7]^3
uint linearizeVoxelPos(int3 virtualVoxelPos)
{
	return  virtualVoxelPos.z * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE +
			virtualVoxelPos.y * SDF_BLOCK_SIZE +
			virtualVoxelPos.x;
}

HashEntry getHashEntry(Buffer<int> hash, in uint id)
{
	HashEntry entry;
	int i0 = hash[3*id+0];
	entry.pos.x = i0 & 0x0000ffff;
	if (entry.pos.x & (0x1 << 15))	entry.pos.x |= 0xffff0000;
	entry.pos.y = i0 >> 16;
	if (entry.pos.y & (0x1 << 15))	entry.pos.y |= 0xffff0000;

	int i1 = hash[3*id+1];
	entry.pos.z = i1 & 0x0000ffff;
	if (entry.pos.z & (0x1 << 15))	entry.pos.z |= 0xffff0000;

	entry.offset = i1 >> 16;
	if (entry.offset & (0x1 << 15)) entry.offset |= 0xffff0000;
	
	entry.ptr = hash[3*id+2];
	return entry;
}

HashEntry getHashEntry(RWBuffer<int> hash, in uint id)
{
	HashEntry entry;
	int i0 = hash[3*id+0];
	entry.pos.x = i0 & 0x0000ffff;
	if (entry.pos.x & (0x1 << 15))	entry.pos.x |= 0xffff0000;
	entry.pos.y = i0 >> 16;
	if (entry.pos.y & (0x1 << 15))	entry.pos.y |= 0xffff0000;

	int i1 = hash[3*id+1];
	entry.pos.z = i1 & 0x0000ffff;
	if (entry.pos.z & (0x1 << 15))	entry.pos.z |= 0xffff0000;

	entry.offset = i1 >> 16;
	if (entry.offset & (0x1 << 15)) entry.offset |= 0xffff0000;

	entry.ptr = hash[3*id+2];
	return entry;
}

void setHashEntry(RWBuffer<int> hash, in uint id, const in HashEntry entry)
{
	hash[3*id+0] = (entry.pos.y << 16) | (entry.pos.x & 0x0000ffff);
	hash[3*id+1] = (entry.offset << 16) | (entry.pos.z & 0x0000ffff);
	hash[3*id+2] = entry.ptr;
}

void deleteHashEntry(RWBuffer<int> hash, in uint id) 
{
	hash[3*id+0] = 0;
	hash[3*id+1] = 0;
	hash[3*id+2] = FREE_ENTRY;
}

Voxel getVoxel(RWBuffer<float> sdfBlocksSDF, RWBuffer<int> sdfBlocksRGBW, RWBuffer<int> sdfBlocksLabel_1, RWBuffer<int> sdfBlocksLabel_2, RWBuffer<int> sdfBlocksLabel_3, RWBuffer<int> sdfBlocksLabelFrequency, in uint id)
{
	Voxel voxel;
	voxel.sdf = sdfBlocksSDF[id];

    int last;
    last = sdfBlocksRGBW[id];
	voxel.weight = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.x = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.y = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.z = last & 0x000000ff;
	
	//Label have 32 bits.
    int last2;
	last2 = sdfBlocksLabel_1[id];
	voxel.label[0] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[1] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[2] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[3] = last2 & 0x000000ff;

	last2 = sdfBlocksLabel_2[id];
	voxel.label[4] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[5] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[6] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[7] = last2 & 0x000000ff;

	last2 = sdfBlocksLabel_3[id];
	voxel.label[8] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[9] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[10] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[11] = last2 & 0x000000ff;

    int last3;
    last3 = sdfBlocksLabelFrequency[6*id+0];
    voxel.labelFrequency[0] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[1] = last3 & 0x0000ffff;

    last3 = sdfBlocksLabelFrequency[6*id+1];
    voxel.labelFrequency[2] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[3] = last3 & 0x0000ffff;

    last3 = sdfBlocksLabelFrequency[6*id+2];
    voxel.labelFrequency[4] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[5] = last3 & 0x0000ffff;

    last3 = sdfBlocksLabelFrequency[6*id+3];
    voxel.labelFrequency[6] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[7] = last3 & 0x0000ffff;

    last3 = sdfBlocksLabelFrequency[6*id+4];
    voxel.labelFrequency[8] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[9] = last3 & 0x0000ffff;

    last3 = sdfBlocksLabelFrequency[6*id+5];
    voxel.labelFrequency[10] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[11] = last3 & 0x0000ffff;

	return voxel;
}

Voxel getVoxel(Buffer<float> sdfBlocksSDF, Buffer<int> sdfBlocksRGBW, Buffer<int> sdfBlocksLabel_1, Buffer<int> sdfBlocksLabel_2, Buffer<int> sdfBlocksLabel_3, Buffer<int> sdfBlocksLabelFrequency, in uint id)
{
	Voxel voxel;
	voxel.sdf = sdfBlocksSDF[id];

    int last;
	last = sdfBlocksRGBW[id];
	voxel.weight = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.x = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.y = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.z = last & 0x000000ff;
	
	//Label have 32 bits.
    int last2;
	last2 = sdfBlocksLabel_1[id];
	voxel.label[0] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[1] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[2] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[3] = last2 & 0x000000ff;

	last2 = sdfBlocksLabel_2[id];
	voxel.label[4] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[5] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[6] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[7] = last2 & 0x000000ff;

	last2 = sdfBlocksLabel_3[id];
    voxel.label[8] = last2 & 0x000000ff;
    last2 >>= 0x8;
	voxel.label[9] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[10] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[11] = last2 & 0x000000ff;

    int last3;
    last3 = sdfBlocksLabelFrequency[6*id+0];
    voxel.labelFrequency[0] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[1] = last3 & 0x0000ffff;

    last3 = sdfBlocksLabelFrequency[6*id+1];
    voxel.labelFrequency[2] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[3] = last3 & 0x0000ffff;

    last3 = sdfBlocksLabelFrequency[6*id+2];
    voxel.labelFrequency[4] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[5] = last3 & 0x0000ffff;

    last3 = sdfBlocksLabelFrequency[6*id+3];
    voxel.labelFrequency[6] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[7] = last3 & 0x0000ffff;

    last3 = sdfBlocksLabelFrequency[6*id+4];
    voxel.labelFrequency[8] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[9] = last3 & 0x0000ffff;

    last3 = sdfBlocksLabelFrequency[6*id+5];
    voxel.labelFrequency[10] = last3 & 0x0000ffff;
    last3 >>= 0x10;
    voxel.labelFrequency[11] = last3 & 0x0000ffff;

	return voxel;
}

Voxel getVoxel_onlySDF_RGBW(Buffer<float> sdfBlocksSDF, Buffer<int> sdfBlocksRGBW, in uint id)
{
	Voxel voxel;
	voxel.sdf = sdfBlocksSDF[id];

	int last = sdfBlocksRGBW[id];
	voxel.weight = last & 0x000000ff;
	last >>= 0x8;                                                                                                                                                                                                                                                                                                        
	voxel.color.x = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.y = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.z = last & 0x000000ff;

    for (uint i = 0; i < 12; i++)
    {
        voxel.label[i] = 0;
        voxel.labelFrequency[i] = 0;
    }

    return voxel;
}

void setVoxel(RWBuffer<float> sdfBlocksSDF, RWBuffer<int> sdfBlocksRGBW, RWBuffer<int> sdfBlocksLabel_1, RWBuffer<int> sdfBlocksLabel_2, RWBuffer<int> sdfBlocksLabel_3, RWBuffer<int> sdfBlocksLabelFrequency, uint id, Voxel voxel)
{
	sdfBlocksSDF[id] = voxel.sdf;

    int last = 0;
	last |= voxel.color.z & 0x000000ff;
	last <<= 8;
	last |= voxel.color.y & 0x000000ff;
	last <<= 8;
	last |= voxel.color.x & 0x000000ff;
	last <<= 8;
	last |= voxel.weight  & 0x000000ff;
	sdfBlocksRGBW[id] = last;

    last = 0;
	last |= voxel.label[3]  & 0x000000ff;
	last <<= 8;
	last |= voxel.label[2]  & 0x000000ff;
	last <<= 8;
	last |= voxel.label[1]  & 0x000000ff;
	last <<= 8;
	last |= voxel.label[0]  & 0x000000ff;
	sdfBlocksLabel_1[id] = last;

    last = 0;
	last |= voxel.label[7]  & 0x000000ff;
	last <<= 8;
	last |= voxel.label[6]  & 0x000000ff;
	last <<= 8;
	last |= voxel.label[5]  & 0x000000ff;
	last <<= 8;
	last |= voxel.label[4]  & 0x000000ff;
	sdfBlocksLabel_2[id] = last;

    last = 0;
	last |= voxel.label[11] & 0x000000ff;
	last <<= 8;
	last |= voxel.label[10] & 0x000000ff;
	last <<= 8;
	last |= voxel.label[9]  & 0x000000ff;
	last <<= 8;
	last |= voxel.label[8]  & 0x000000ff;
	sdfBlocksLabel_3[id] = last;

    last = 0;
    last |= voxel.labelFrequency[1] & 0x0000ffff;
    last <<= 16;
    last |= voxel.labelFrequency[0] & 0x0000ffff;
    sdfBlocksLabelFrequency[6*id+0] = last;

    last = 0;
    last |= voxel.labelFrequency[3] & 0x0000ffff;
    last <<= 16;
    last |= voxel.labelFrequency[2] & 0x0000ffff;
    sdfBlocksLabelFrequency[6*id+1] = last;

    last = 0;
    last |= voxel.labelFrequency[5] & 0x0000ffff;
    last <<= 16;
    last |= voxel.labelFrequency[4] & 0x0000ffff;
    sdfBlocksLabelFrequency[6*id+2] = last;

    last = 0;
    last |= voxel.labelFrequency[7] & 0x0000ffff;
    last <<= 16;
    last |= voxel.labelFrequency[6] & 0x0000ffff;
    sdfBlocksLabelFrequency[6*id+3] = last;

    last = 0;
    last |= voxel.labelFrequency[9] & 0x0000ffff;
    last <<= 16;
    last |= voxel.labelFrequency[8] & 0x0000ffff;
    sdfBlocksLabelFrequency[6*id+4] = last;

    last = 0;
    last |= voxel.labelFrequency[11] & 0x0000ffff;
    last <<= 16;
    last |= voxel.labelFrequency[10] & 0x0000ffff;
    sdfBlocksLabelFrequency[6*id+5] = last;
}

void starveVoxel(RWBuffer<int> sdfBlocksRGBW, in uint id)
{
	int last = sdfBlocksRGBW[id];
	int weight = last & 0x000000ff;

	weight = max(0, weight-1);
	last &= 0xffffff00;
	last |= weight;

	sdfBlocksRGBW[id] = last;
}

void deleteVoxel(RWBuffer<float> sdfBlocksSDF, RWBuffer<int> sdfBlocksRGBW, RWBuffer<int> sdfBlocksLabel_1, RWBuffer<int> sdfBlocksLabel_2, RWBuffer<int> sdfBlocksLabel_3, RWBuffer<int> sdfBlocksLabelFrequency, in uint id)
{
	sdfBlocksSDF[id] = 0;
	sdfBlocksRGBW[id] = 0;
	sdfBlocksLabel_1[id] = 0;
	sdfBlocksLabel_2[id] = 0;
	sdfBlocksLabel_3[id] = 0;

    for (uint i = 0; i < 6; i++)
    {
        sdfBlocksLabelFrequency[6*id+i] = 0;
    }
}

float3 worldToVirtualVoxelPosFloat(in float3 pos)
{
	return pos*g_VirtualVoxelResolutionScalar;
}

int3 worldToVirtualVoxelPos(in float3 pos)
{
	const float3 p = pos*g_VirtualVoxelResolutionScalar;
	return (int3)(p+sign(p)*0.5f);
}

int3 virtualVoxelPosToSDFBlock(int3 virtualVoxelPos)
{
	if (virtualVoxelPos.x < 0) virtualVoxelPos.x -= SDF_BLOCK_SIZE-1;
	if (virtualVoxelPos.y < 0) virtualVoxelPos.y -= SDF_BLOCK_SIZE-1;
	if (virtualVoxelPos.z < 0) virtualVoxelPos.z -= SDF_BLOCK_SIZE-1;

	return virtualVoxelPos/SDF_BLOCK_SIZE;
}

// Computes virtual voxel position of corner sample position
int3 SDFBlockToVirtualVoxelPos(int3 sdfBlock)
{
	return sdfBlock*SDF_BLOCK_SIZE;
}

float3 virtualVoxelPosToWorld(in int3 pos)
{   
	return float3(pos)*g_VirtualVoxelSize;
}

float3 SDFBlockToWorld(int3 sdfBlock)
{
	return virtualVoxelPosToWorld(SDFBlockToVirtualVoxelPos(sdfBlock));
}

int3 worldToSDFBlock(float3 worldPos)
{
	const int3 virtualVoxelPos = worldToVirtualVoxelPos(worldPos);
	return virtualVoxelPosToSDFBlock(virtualVoxelPos);
}

int virtualVoxelPosToLocalSDFBlockIndex(int3 virtualVoxelPos)
{
	int3 localVoxelPos = virtualVoxelPos%SDF_BLOCK_SIZE;

	if (localVoxelPos.x < 0) localVoxelPos.x += SDF_BLOCK_SIZE;
	if (localVoxelPos.y < 0) localVoxelPos.y += SDF_BLOCK_SIZE;
	if (localVoxelPos.z < 0) localVoxelPos.z += SDF_BLOCK_SIZE;

	return linearizeVoxelPos(localVoxelPos);
}

int worldToLocalSDFBlockIndex(float3 world)
{
	int3 virtualVoxelPos = worldToVirtualVoxelPos(world);
	return virtualVoxelPosToLocalSDFBlockIndex(virtualVoxelPos);
}

//! see teschner et al. (but with correct prime values)
uint computeHashPos(in int3 virtualVoxelPos)
{ 
	const int p0 = 73856093;
	const int p1 = 19349669;
	const int p2 = 83492791;

	int res = ((virtualVoxelPos.x * p0) ^ (virtualVoxelPos.y * p1) ^ (virtualVoxelPos.z * p2)) % g_HashNumBuckets;
	if (res < 0) res += g_HashNumBuckets;
	return (uint)res;
}

//! see teschner et al. (but with correct prime values)
uint computeHashPosOther(in int3 virtualVoxelPos, in uint hashNumBuckets)
{ 
	const int p0 = 73856093;
	const int p1 = 19349669;
	const int p2 = 83492791;

	int res = ((virtualVoxelPos.x * p0) ^ (virtualVoxelPos.y * p1) ^ (virtualVoxelPos.z * p2)) % hashNumBuckets;
	if (res < 0) res += hashNumBuckets;
	return (uint)res;
}

//merges two voxels (v0 is the input voxel, v1 the currently stored voxel)
Voxel combineVoxel(in Voxel v0, in Voxel v1)
{
    uint labels[12] = { v0.label[0], v0.label[1], v0.label[2], v0.label[3],
                        v0.label[4], v0.label[5], v0.label[6], v0.label[7],
                        v0.label[8], v0.label[9], v0.label[10], v0.label[11] };
    
    uint maxLabel = 0;
    for (uint i = 0; i < 12; i++)
    {
        if (labels[i] > maxLabel)
        {
            maxLabel = labels[i];
        }
    }
    for (uint j = 0; j < 12; j++)
    {
        if (labels[j] == maxLabel)
        {
            v0.labelFrequency[j] = 1;
        }
    }
    
    Voxel v;

	//v.color = (10*v0.weight * v0.color + v1.weight * v1.color)/(10*v0.weight + v1.weight); //give the currently observed color more weight
	//v.color = (v0.weight * v0.color + v1.weight * v1.color)/(v0.weight + v1.weight);
	
    //exponential running average
    //v.color = 0.5f * (v0.color + v1.color);

	//exponential running average
    //v.label[0] = 0.5f * (v0.label[0] + v1.label[0]);
    //v.label[1] = 0.5f * (v0.label[1] + v1.label[1]);
    //v.label[2] = 0.5f * (v0.label[2] + v1.label[2]);
    //v.label[3] = 0.5f * (v0.label[3] + v1.label[3]);
    //v.label[4] = 0.5f * (v0.label[4] + v1.label[4]);

    //v.label[5] = 0.5f * (v0.label[5] + v1.label[5]);
    //v.label[6] = 0.5f * (v0.label[6] + v1.label[6]);
    //v.label[7] = 0.5f * (v0.label[7] + v1.label[7]);
    //v.label[8] = 0.5f * (v0.label[8] + v1.label[8]);
    //v.label[9] = 0.5f * (v0.label[9] + v1.label[9]);

    //v.label[10] = 0.5f * (v0.label[10] + v1.label[10]);
    //v.label[11] = 0.5f * (v0.label[11] + v1.label[11]);

    v.labelFrequency[0] = v0.labelFrequency[0] + v1.labelFrequency[0];
    v.labelFrequency[1] = v0.labelFrequency[1] + v1.labelFrequency[1];
    v.labelFrequency[2] = v0.labelFrequency[2] + v1.labelFrequency[2];
    v.labelFrequency[3] = v0.labelFrequency[3] + v1.labelFrequency[3];
    v.labelFrequency[4] = v0.labelFrequency[4] + v1.labelFrequency[4];

    v.labelFrequency[5] = v0.labelFrequency[5] + v1.labelFrequency[5];
    v.labelFrequency[6] = v0.labelFrequency[6] + v1.labelFrequency[6];
    v.labelFrequency[7] = v0.labelFrequency[7] + v1.labelFrequency[7];
    v.labelFrequency[8] = v0.labelFrequency[8] + v1.labelFrequency[8];
    v.labelFrequency[9] = v0.labelFrequency[9] + v1.labelFrequency[9];

    v.labelFrequency[10] = v0.labelFrequency[10] + v1.labelFrequency[10];
    v.labelFrequency[11] = v0.labelFrequency[11] + v1.labelFrequency[11];

    uint labelNum = v.labelFrequency[0] + v.labelFrequency[1] + v.labelFrequency[2] + v.labelFrequency[3] + 
                    v.labelFrequency[4] + v.labelFrequency[5] + v.labelFrequency[6] + v.labelFrequency[7] + 
                    v.labelFrequency[8] + v.labelFrequency[9] + v.labelFrequency[10] + v.labelFrequency[11];
    
    //if (labelNum > 0)
    //{
    //    v.color = uint3((v0.color + (float)(labelNum - 1) * v1.color) / (float)labelNum);
        
    //    v.label[0] = uint((v0.label[0] + (float)(labelNum - 1) * v1.label[0]) / (float)labelNum);
    //    v.label[1] = uint((v0.label[1] + (float)(labelNum - 1) * v1.label[1]) / (float)labelNum);
    //    v.label[2] = uint((v0.label[2] + (float)(labelNum - 1) * v1.label[2]) / (float)labelNum);
    //    v.label[3] = uint((v0.label[3] + (float)(labelNum - 1) * v1.label[3]) / (float)labelNum);
    //    v.label[4] = uint((v0.label[4] + (float)(labelNum - 1) * v1.label[4]) / (float)labelNum);

    //    v.label[5] = uint((v0.label[5] + (float)(labelNum - 1) * v1.label[5]) / (float)labelNum);
    //    v.label[6] = uint((v0.label[6] + (float)(labelNum - 1) * v1.label[6]) / (float)labelNum);
    //    v.label[7] = uint((v0.label[7] + (float)(labelNum - 1) * v1.label[7]) / (float)labelNum);
    //    v.label[8] = uint((v0.label[8] + (float)(labelNum - 1) * v1.label[8]) / (float)labelNum);
    //    v.label[9] = uint((v0.label[9] + (float)(labelNum - 1) * v1.label[9]) / (float)labelNum);

    //    v.label[10] = uint((v0.label[10] + (float)(labelNum - 1) * v1.label[10]) / (float)labelNum);
    //    v.label[11] = uint((v0.label[11] + (float)(labelNum - 1) * v1.label[11]) / (float)labelNum);
    //}

    // test
    if (labelNum > 0)
    {
        v.color = round((v0.color + (float)(labelNum - 1) * v1.color) / (float)labelNum);
        
        v.label[0] = round((v0.label[0] + (float)(labelNum - 1) * v1.label[0]) / (float)labelNum);
        v.label[1] = round((v0.label[1] + (float)(labelNum - 1) * v1.label[1]) / (float)labelNum);
        v.label[2] = round((v0.label[2] + (float)(labelNum - 1) * v1.label[2]) / (float)labelNum);
        v.label[3] = round((v0.label[3] + (float)(labelNum - 1) * v1.label[3]) / (float)labelNum);
        v.label[4] = round((v0.label[4] + (float)(labelNum - 1) * v1.label[4]) / (float)labelNum);

        v.label[5] = round((v0.label[5] + (float)(labelNum - 1) * v1.label[5]) / (float)labelNum);
        v.label[6] = round((v0.label[6] + (float)(labelNum - 1) * v1.label[6]) / (float)labelNum);
        v.label[7] = round((v0.label[7] + (float)(labelNum - 1) * v1.label[7]) / (float)labelNum);
        v.label[8] = round((v0.label[8] + (float)(labelNum - 1) * v1.label[8]) / (float)labelNum);
        v.label[9] = round((v0.label[9] + (float)(labelNum - 1) * v1.label[9]) / (float)labelNum);

        v.label[10] = round((v0.label[10] + (float)(labelNum - 1) * v1.label[10]) / (float)labelNum);
        v.label[11] = round((v0.label[11] + (float)(labelNum - 1) * v1.label[11]) / (float)labelNum);
    }
    // test

    v.sdf = (v0.sdf * v0.weight + v1.sdf * v1.weight) / (v0.weight + v1.weight);
	v.weight = min(g_WeightMax, v0.weight + v1.weight);
	return v;
}

//! returns the hash entry for a given sdf block id; if there was no hash entry the returned entry will have a ptr with FREE_ENTRY set
HashEntry getHashEntryForSDFBlockPos(Buffer<int> hash, int3 sdfBlock) 
{
	uint h = computeHashPos(sdfBlock);	//hash bucket
	uint hp = h * g_HashBucketSize;		//hash position

	HashEntry entry;
	entry.pos = sdfBlock;
	entry.offset = 0;
	entry.ptr = FREE_ENTRY;

	[allow_uav_condition]
	for (uint j = 0; j < g_HashBucketSize; j++)
    {
		uint i = j + hp;
		HashEntry curr = getHashEntry(hash, i);
		if (curr.pos.x == entry.pos.x && curr.pos.y == entry.pos.y && curr.pos.z == entry.pos.z && curr.ptr != FREE_ENTRY)
        {
			return curr;
		}
	}

	#ifdef HANDLE_COLLISIONS
		const uint idxLastEntryInBucket = (h+1)*g_HashBucketSize - 1;
		int i = idxLastEntryInBucket;	//start with the last entry of the current bucket
		
		//traverse list until end: memorize idx at list end and memorize offset from last element of bucket to list end
		HashEntry curr;
		unsigned int maxIter = 0;
        
		[allow_uav_condition]
		while (true && maxIter < g_MaxLoopIterCount)
        {
			curr = getHashEntry(hash, i);
			
			if (curr.pos.x == entry.pos.x && curr.pos.y == entry.pos.y && curr.pos.z == entry.pos.z && curr.ptr != FREE_ENTRY) 
            {
				return curr;
			}

			if (curr.offset == 0) 
            {	
                //we have found the end of the list
				break;
			}

			i = idxLastEntryInBucket + curr.offset;		//go to next element in the list
			i %= (g_HashBucketSize * g_HashNumBuckets);	//check for overflow

			maxIter++;
		}
	#endif
	return entry;
}

//! returns the hash entry for a given worldPos; if there was no hash entry the returned entry will have a ptr with FREE_ENTRY set
HashEntry getHashEntry(Buffer<int> hash, float3 worldPos) 
{
	//position of sdf block
    int3 blockID = worldToSDFBlock(worldPos); //int3 blockID = worldToSDFVirtualVoxelPos(worldPos)/SDF_BLOCK_SIZE;
	return getHashEntryForSDFBlockPos(hash, blockID);
}

//! returns the truncation of the SDF for a given distance value
float getTruncation(float z) 
{
	return g_Truncation + g_TruncScale * z;
}

bool isInCameraFrustum(in float3 pos) 
{
	float3 pCamera = mul(float4(pos, 1.0f), g_RigidTransformInverse).xyz;
	float3 pProj = cameraToKinectProj(pCamera);
	
    //TODO THIS IS A HACK FIX IT :)
    //pProj *= 0.75f;
	return !(pProj.x < -1.0f || pProj.x > 1.0f || pProj.y < -1.0f || pProj.y > 1.0f || pProj.z < 0.0f || pProj.z > 1.0f);  
}

bool isInCameraFrustumApprox(in float3 pos) 
{
	float3 pCamera = mul(float4(pos, 1.0f), g_RigidTransformInverse).xyz;
	float3 pProj = cameraToKinectProj(pCamera);
	
    //TODO THIS IS A HACK FIX IT :)
    pProj *= 0.95; //pProj *= 1.5f;
	return !(pProj.x < -1.0f || pProj.x > 1.0f || pProj.y < -1.0f || pProj.y > 1.0f || pProj.z < 0.0f || pProj.z > 1.0f);  
}

bool isSDFBlockInCameraFrustumApprox(in int3 sdfBlock) 
{
	float3 posWorld = virtualVoxelPosToWorld(SDFBlockToVirtualVoxelPos(sdfBlock)) + g_VirtualVoxelSize*0.5f*(SDF_BLOCK_SIZE-1.0f);
	return isInCameraFrustumApprox(posWorld);
}

//! deletes a hash entry position for a given sdfBlock index (returns true uppon successful deletion; otherwise returns false)
bool deleteHashEntryElement(RWBuffer<int> hash, AppendStructuredBuffer<uint> heapAppend, RWBuffer<uint> bucketMutex, int3 sdfBlock)
{
	uint h = computeHashPos(sdfBlock);	//hash bucket
	uint hp = h * g_HashBucketSize;		//hash position

	[allow_uav_condition]
	for (uint j = 0; j < g_HashBucketSize; j++) 
    {
		uint i = j + hp;
		HashEntry curr = getHashEntry(hash, i);

		if (curr.pos.x == sdfBlock.x && curr.pos.y == sdfBlock.y && curr.pos.z == sdfBlock.z && curr.ptr != FREE_ENTRY)
        {
			#ifndef HANDLE_COLLISIONS
			    const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
			    heapAppend.Append(curr.ptr / linBlockSize);
			    deleteHashEntry(hash, i);
			    return true;
			#endif

			#ifdef HANDLE_COLLISIONS
			    if (curr.offset != 0) 
                {	
                    //if there was a pointer set it to the next list element
				    int prevValue = 0;
				    InterlockedExchange(bucketMutex[h], LOCK_ENTRY, prevValue);	//lock the hash bucket
				    if (prevValue == LOCK_ENTRY)	return false;
				    if (prevValue != LOCK_ENTRY) 
                    {
					    const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
					    heapAppend.Append(curr.ptr / linBlockSize);
					    int nextIdx = (i + curr.offset) % (g_HashBucketSize * g_HashNumBuckets);
					    setHashEntry(hash, i, getHashEntry(hash, nextIdx));
					    deleteHashEntry(hash, nextIdx);
					    return true;
				    }
			    } 
                else 
                {
				    const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
				    heapAppend.Append(curr.ptr / linBlockSize);
				    deleteHashEntry(hash, i);
				    return true;
			    }
			#endif
		}
	}
    	
	#ifdef HANDLE_COLLISIONS
		const uint idxLastEntryInBucket = (h+1)*g_HashBucketSize - 1;
		int i = idxLastEntryInBucket;
		int offset = 0;
		HashEntry curr;
		curr = getHashEntry(hash, i);

		int prevIdx = i;
		i = idxLastEntryInBucket + curr.offset;		//go to next element in the list
		i %= (g_HashBucketSize * g_HashNumBuckets);	//check for overflow

		unsigned int maxIter = 0;

		[allow_uav_condition]
		while (true && maxIter < g_MaxLoopIterCount) 
        {
			//found that dude that we need/want to delete
            curr = getHashEntry(hash, i);
			if (curr.pos.x == sdfBlock.x && curr.pos.y == sdfBlock.y && curr.pos.z == sdfBlock.z && curr.ptr != FREE_ENTRY) 
            {
				int prevValue = 0;
				InterlockedExchange(bucketMutex[h], LOCK_ENTRY, prevValue);	//lock the hash bucket
				if (prevValue == LOCK_ENTRY)	return false;
				if (prevValue != LOCK_ENTRY) 
                {
					const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
					heapAppend.Append(curr.ptr / linBlockSize);
					deleteHashEntry(hash, i);
					HashEntry prev = getHashEntry(hash, prevIdx);				
					prev.offset = curr.offset;
					setHashEntry(hash, prevIdx, prev);
					return true;
				}
			}

			if (curr.offset == 0) 
            {	
                //we have found the end of the list
				return false;	//should actually never happen because we need to find that guy before
			}
			prevIdx = i;
			i = idxLastEntryInBucket + curr.offset;		//go to next element in the list
			i %= (g_HashBucketSize * g_HashNumBuckets);	//check for overflow

			maxIter++;
		}
	#endif
	return false;
}

#endif