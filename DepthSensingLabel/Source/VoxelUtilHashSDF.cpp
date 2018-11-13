#include "stdafx.h"

#include "VoxelUtilHashSDF.h"

// voxel
Voxel VoxelUtilHelper::getVoxel(const SDFBlock& sdfBlocks, unsigned int id)
{
	Voxel voxel;

	voxel.sdf = ((float*)&(sdfBlocks.data[0]))[11*id+0];

	int last;
	last = sdfBlocks.data[11*id+1];
	voxel.weight  = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.x = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.y = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.z = last & 0x000000ff;

	int last2;
	last2 = sdfBlocks.data[11*id+2];
	voxel.label[0]  = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[1]  = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[2]  = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[3]  = last2 & 0x000000ff;

	last2 = sdfBlocks.data[11*id+3];
	voxel.label[4]  = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[5]  = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[6]  = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[7]  = last2 & 0x000000ff;

	last2 = sdfBlocks.data[11*id+4];
	voxel.label[8]  = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[9]  = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[10] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[11] = last2 & 0x000000ff;

	int last3;
	for (unsigned int i = 0; i < 6; i++)
	{
		last3 = sdfBlocks.data[11*id+5+i];
		voxel.labelFrequency[2*i+0] = last3 & 0x0000ffff;
		last3 >>= 0x10;
		voxel.labelFrequency[2*i+1] = last3 & 0x0000ffff;
	}

	return voxel;
}

void VoxelUtilHelper::setVoxel(SDFBlock& sdfBlocks, unsigned int id, const Voxel& voxel)
{
	unsigned int* f = (unsigned int*)&voxel.sdf; sdfBlocks.data[11*id+0] = *f;
	
	int last = 0;
	last |= voxel.color.z & 0x000000ff;
	last <<= 8;
	last |= voxel.color.y & 0x000000ff;
	last <<= 8;
	last |= voxel.color.x & 0x000000ff;
	last <<= 8;
	last |= voxel.weight  & 0x000000ff;
	sdfBlocks.data[11*id+1] = last;

	last = 0;
	last |= (unsigned int)voxel.label[3]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)voxel.label[2]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)voxel.label[1]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)voxel.label[0]  & 0x000000ff;
	sdfBlocks.data[11*id+2] = last;

	last = 0;
	last |= (unsigned int)voxel.label[7]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)voxel.label[6]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)voxel.label[5]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)voxel.label[4]  & 0x000000ff;
	sdfBlocks.data[11*id+3] = last;

	last = 0;
	last |= (unsigned int)voxel.label[11] & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)voxel.label[10] & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)voxel.label[9]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)voxel.label[8]  & 0x000000ff;
	sdfBlocks.data[11*id+4] = last;

	for (unsigned int i = 0; i < 6; i++)
	{
		last = 0;
		last |= (unsigned int)voxel.labelFrequency[2*i+1] & 0x0000ffff;
		last <<= 16;
		last |= (unsigned int)voxel.labelFrequency[2*i+0] & 0x0000ffff;
		sdfBlocks.data[11*id+5+i] = last;
	}
}

void VoxelUtilHelper::setVoxelLabel(SDFBlock& sdfBlocks, unsigned int id, unsigned int *label)
{
	int last = 0;
	last |= (unsigned int)label[3]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)label[2]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)label[1]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)label[0]  & 0x000000ff;
	sdfBlocks.data[11*id+2] = last;
	
	last = 0;
	last |= (unsigned int)label[7]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)label[6]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)label[5]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)label[4]  & 0x000000ff;
	sdfBlocks.data[11*id+3] = last;

	last = 0;
	last |= (unsigned int)label[11] & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)label[10] & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)label[9]  & 0x000000ff;
	last <<= 8;
	last |= (unsigned int)label[8]  & 0x000000ff;
	sdfBlocks.data[11*id+4] = last;

	unsigned int preLabelFrequency[12] = { 0 };
	unsigned int curLabelFrequency[12] = { 0 };

	last = 0;
	for (unsigned int i = 0; i < 6; i++)
	{
		last = sdfBlocks.data[11*id+5+i];
		preLabelFrequency[2*i+0] = last & 0x0000ffff;
		last >>= 0x10;
		preLabelFrequency[2*i+1] = last & 0x0000ffff;
	}
	
	unsigned int maxIndex = 0; unsigned int maxLabel = 0;
	for (int i = 0; i < GlobalAppState::getInstance().s_labelNum; i++)
	{
		if (label[i] > maxLabel)
		{
			maxIndex = i;
			maxLabel = label[i];
		}
	}

	for (unsigned int i = 0; i < 12; i++)
	{
		curLabelFrequency[maxIndex] += preLabelFrequency[i];
	}

	for (unsigned int i = 0; i < 6; i++)
	{
		last = 0;
		last |= (unsigned int)curLabelFrequency[2*i+1] & 0x0000ffff;
		last <<= 16;
		last |= (unsigned int)curLabelFrequency[2*i+0] & 0x0000ffff;
		sdfBlocks.data[11*id+5+i] = last;
	}
}

// block-voxel
vec3i VoxelUtilHelper::SDFBlockToVirtualVoxelPos(vec3i sdfBlock)
{
	return sdfBlock*SDF_BLOCK_SIZE;
}

vec3i VoxelUtilHelper::virtualVoxelPosToSDFBlock(vec3i virtualVoxelPos)
{
	if (virtualVoxelPos.x < 0) virtualVoxelPos.x -= SDF_BLOCK_SIZE-1;
	if (virtualVoxelPos.y < 0) virtualVoxelPos.y -= SDF_BLOCK_SIZE-1;
	if (virtualVoxelPos.z < 0) virtualVoxelPos.z -= SDF_BLOCK_SIZE-1;

	return virtualVoxelPos/SDF_BLOCK_SIZE;
}

// world-block
vec3i VoxelUtilHelper::worldToSDFBlock(vec3f worldPos)
{
	const vec3i virtualVoxelPos = worldToVirtualVoxelPos(worldPos);
	return virtualVoxelPosToSDFBlock(virtualVoxelPos);
}

vec3f VoxelUtilHelper::SDFBlockToWorld(vec3i sdfBlock)
{
	return virtualVoxelPosToWorld(SDFBlockToVirtualVoxelPos(sdfBlock));
}

vec3f VoxelUtilHelper::computeSDFBlockCenter(vec3i sdfBlock)
{
	vec3f posWorld = SDFBlockToWorld(sdfBlock);
	const float centerOffset(0.5f*(SDF_BLOCK_SIZE - 1)*GlobalAppState::getInstance().s_virtualVoxelSize);

	posWorld.x += centerOffset;
	posWorld.y += centerOffset;
	posWorld.z += centerOffset;

	return posWorld;
}

int VoxelUtilHelper::worldToLocalSDFBlockIndex(vec3f posWorld)
{
	vec3i virtualVoxelPos = worldToVirtualVoxelPos(posWorld);
	return virtualVoxelPosToLocalSDFBlockIndex(virtualVoxelPos);
}

// world-voxel
vec3i VoxelUtilHelper::worldToVirtualVoxelPos(vec3f pos)
{
	const vec3f p = pos/GlobalAppState::getInstance().s_virtualVoxelSize;
	vec3f s;
	s.x = (float)math::sign(p.x);
	s.y = (float)math::sign(p.y);
	s.z = (float)math::sign(p.z);
	return (vec3i)(p+s*0.5f);
}

vec3f VoxelUtilHelper::virtualVoxelPosToWorld(vec3i pos)
{
	return vec3f(pos)*GlobalAppState::getInstance().s_virtualVoxelSize;
}

int VoxelUtilHelper::virtualVoxelPosToLocalSDFBlockIndex(vec3i virtualVoxelPos)
{
	vec3i localVoxelPos;

	localVoxelPos.x = virtualVoxelPos.x%SDF_BLOCK_SIZE;
	localVoxelPos.y = virtualVoxelPos.y%SDF_BLOCK_SIZE;
	localVoxelPos.z = virtualVoxelPos.z%SDF_BLOCK_SIZE;

	if (localVoxelPos.x < 0) localVoxelPos.x += SDF_BLOCK_SIZE;
	if (localVoxelPos.y < 0) localVoxelPos.y += SDF_BLOCK_SIZE;
	if (localVoxelPos.z < 0) localVoxelPos.z += SDF_BLOCK_SIZE;

	return linearizeVoxelPos(localVoxelPos);
}

vec3i VoxelUtilHelper::delinearizeVoxelIndex(unsigned int idx)
{
	unsigned int x = idx % SDF_BLOCK_SIZE;
	unsigned int y = (idx % (SDF_BLOCK_SIZE * SDF_BLOCK_SIZE)) / SDF_BLOCK_SIZE;
	unsigned int z = idx / (SDF_BLOCK_SIZE * SDF_BLOCK_SIZE);
	return vec3ui(x, y, z);
}

unsigned int VoxelUtilHelper::linearizeVoxelPos(vec3i virtualVoxelPos)
{
	return virtualVoxelPos.z * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE +
		   virtualVoxelPos.y * SDF_BLOCK_SIZE +
		   virtualVoxelPos.x;
}

// world-chunk
vec3i VoxelUtilHelper::worldToChunk(vec3f posWorld)
{
	vec3f p;
	p.x = posWorld.x / GlobalAppState::getInstance().s_voxelExtends.x;
	p.y = posWorld.y / GlobalAppState::getInstance().s_voxelExtends.y;
	p.z = posWorld.z / GlobalAppState::getInstance().s_voxelExtends.z;

	vec3f s;
	s.x = (float)math::sign(p.x);
	s.y = (float)math::sign(p.y);
	s.z = (float)math::sign(p.z);

	return vec3i(p + s*0.5f);
}

vec3f VoxelUtilHelper::chunkToWorld(vec3i posChunk)
{
	vec3f res;
	res.x = posChunk.x*GlobalAppState::getInstance().s_voxelExtends.x;
	res.y = posChunk.y*GlobalAppState::getInstance().s_voxelExtends.y;
	res.z = posChunk.z*GlobalAppState::getInstance().s_voxelExtends.z;

	return res;
}

vec3i VoxelUtilHelper::delinearizeChunkIndex(unsigned int idx)
{
	unsigned int x = idx % GlobalAppState::getInstance().s_gridDimensions.x;
	unsigned int y = (idx % (GlobalAppState::getInstance().s_gridDimensions.x * GlobalAppState::getInstance().s_gridDimensions.y)) / GlobalAppState::getInstance().s_gridDimensions.x;
	unsigned int z = idx / (GlobalAppState::getInstance().s_gridDimensions.x * GlobalAppState::getInstance().s_gridDimensions.y);

	return GlobalAppState::getInstance().s_minGridPos + vec3i(x, y, z);
}

unsigned int VoxelUtilHelper::linearizeChunkPos(vec3i chunkPos)
{
	vec3ui p = chunkPos - GlobalAppState::getInstance().s_minGridPos;

	return  p.z * GlobalAppState::getInstance().s_gridDimensions.x * GlobalAppState::getInstance().s_gridDimensions.y +
			p.y * GlobalAppState::getInstance().s_gridDimensions.x +
			p.x;
}