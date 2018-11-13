Texture2D<float>				g_InputDepth                 : register( t0 );
Texture2D<float3>				g_InputColor	             : register( t1 );
Buffer<int>						g_PrefixSum		             : register( t2 );
Buffer<int>						g_HashSRV		             : register( t3 );
Buffer<int>						g_HashCompactifiedSRV        : register( t4 );
Buffer<float>					g_SDFBlocksSDFSRV            : register( t5 );
Buffer<int>						g_DecisionArraySRV           : register( t6 );
Buffer<int>						g_SDFBlocksRGBWSRV           : register( t7 );
Buffer<uint>					g_bitMask		             : register( t8 );
Buffer<int>						g_SDFBlocksLabelSRV_1        : register( t9 );
Buffer<int>						g_SDFBlocksLabelSRV_2        : register( t10 );
Buffer<int>						g_SDFBlocksLabelSRV_3        : register( t11 );
Buffer<int>                     g_SDFBlocksLabelFrequencySRV : register( t12 );
Texture2D<float4>				g_InputLabel_1               : register( t13 );
Texture2D<float4>				g_InputLabel_2               : register( t14 );
Texture2D<float4>				g_InputLabel_3               : register( t15 );

RWBuffer<int>					g_Hash				         : register( u0 );
RWBuffer<float>					g_SDFBlocksSDFUAV	         : register( u1 );
ConsumeStructuredBuffer<uint>	g_HeapConsume		         : register( u2 );
AppendStructuredBuffer<uint>	g_HeapAppend		         : register( u3 );
RWStructuredBuffer<uint>        g_HeapStatic                 : register( u4 );
RWBuffer<uint>					g_HashBucketMutex	         : register( u5 );
RWBuffer<int>					g_DecisionArrayUAV	         : register( u6 );
RWBuffer<int>					g_HashCompactified	         : register( u7 );

RWBuffer<int>					g_HashOther		             : register( u1 );
RWBuffer<int>					g_SDFBlocksRGBWUAV	         : register( u7 );
RWBuffer<int>					g_SDFBlocksLabelUAV_1	     : register( u2 );
RWBuffer<int>					g_SDFBlocksLabelUAV_2	     : register( u3 );
RWBuffer<int>					g_SDFBlocksLabelUAV_3	     : register( u5 );
RWBuffer<int>                   g_SDFBlocksLabelFrequencyUAV : register( u6 );

#include "GlobalAppStateShaderBuffer.h.hlsl"
#include "SDFShaderBuffer.h.hlsl"
#include "KinectCameraUtil.h.hlsl"
#include "VoxelUtilHashSDF.h.hlsl"

int log2Integer(int x)
{
    int r = 0;
    while (x >>= 1)
        r++;
    return r;
}

uint linearizeChunkPos(int3 chunkPos)
{
	uint3 p = chunkPos-m_minGridPos;

	return  p.z * m_gridDimensions.x * m_gridDimensions.y +
			p.y * m_gridDimensions.x +
			p.x;
}
   
int3 worldToChunks(float3 posWorld)
{
	float3 p = posWorld/m_voxelExtends;
	float3 s = sign(p);
	return int3(p+s*0.5f);
}

bool isSDFBlockStreamedOut(int3 sdfBlock)
{
	float3 posWorld = virtualVoxelPosToWorld(SDFBlockToVirtualVoxelPos(sdfBlock)); // sdfBlock is assigned to chunk by the bottom right sample pos

	uint index = linearizeChunkPos(worldToChunks(posWorld));
	return ((g_bitMask[index/nBitsInT] & (0x1 << (index%nBitsInT))) != 0x0);
}

void allocBlock(const in int3 pos)
{
	uint h = computeHashPos(pos);		// hash bucket
	uint hp = h * g_HashBucketSize;		// hash position

	int firstEmpty = -1;
	[allow_uav_condition]
	for (uint j = 0; j < g_HashBucketSize; j++)
    {
		uint i = j + hp;
		HashEntry curr = getHashEntry(g_Hash, i);
		
		// in that case the SDF-block is already alloacted and corresponds to the current position -> exit thread
		if (curr.pos.x == pos.x && curr.pos.y == pos.y && curr.pos.z == pos.z && curr.ptr != FREE_ENTRY)
        {
			return;
		}

		// store the first FREE_ENTRY hash entry
		if (firstEmpty == -1 && curr.ptr == FREE_ENTRY)
        {
			firstEmpty = i;
		}
	}
	  
	#ifdef HANDLE_COLLISIONS
		// updated variables as after the loop
		const uint idxLastEntryInBucket = (h+1)*g_HashBucketSize - 1;	// get last index of bucket
		uint i = idxLastEntryInBucket;									// start with the last entry of the current bucket
		
		HashEntry curr;	curr.offset = 0;

		uint maxIter = 0;

		[allow_uav_condition]
		while (true && maxIter < g_MaxLoopIterCount) 
        {
			curr = getHashEntry(g_Hash, i);
			if (curr.pos.x == pos.x && curr.pos.y == pos.y && curr.pos.z == pos.z && curr.ptr != FREE_ENTRY) 
            {
				return;
			}
            
			if (curr.offset == 0) 
            {
				break;
			}
            
			i = idxLastEntryInBucket + curr.offset;		// go to next element in the list
			i %= (g_HashBucketSize * g_HashNumBuckets);	// check for overflow

			maxIter++;
		}
	#endif

	if (firstEmpty != -1)
    {	
        // if there is an empty entry and we havn't allocated the current entry before
		int prevValue = 0;
		InterlockedExchange(g_HashBucketMutex[h], LOCK_ENTRY, prevValue); // lock the hash bucket
		if (prevValue != LOCK_ENTRY)
        {	
            // only proceed if the bucket has been locked
			HashEntry entry;
			entry.pos = pos;
			entry.offset = NO_OFFSET;
			entry.ptr = g_HeapConsume.Consume() * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;	// memory alloc
			setHashEntry(g_Hash, firstEmpty, entry);
		}
		return;
	}

	#ifdef HANDLE_COLLISIONS
		int offset = 0; maxIter = 0;

		[allow_uav_condition]
		while (true && maxIter < g_MaxLoopIterCount) 
        {
			offset++;
			i = (idxLastEntryInBucket + offset) % (g_HashBucketSize * g_HashNumBuckets); //go to next hash element
			if ((offset % g_HashBucketSize) == 0) continue;	//cannot insert into a last bucket element (would conflict with other linked lists)
			curr = getHashEntry(g_Hash, i);
            
			if (curr.ptr == FREE_ENTRY) 
            {	
                //this is the first free entry
				int prevValue = 0;
				InterlockedExchange(g_HashBucketMutex[h], LOCK_ENTRY, prevValue); //lock the original hash bucket
				
                if (prevValue != LOCK_ENTRY) 
                {
					HashEntry lastEntryInBucket = getHashEntry(g_Hash, idxLastEntryInBucket);
					h = i / g_HashBucketSize;
					InterlockedExchange(g_HashBucketMutex[h], LOCK_ENTRY, prevValue); //lock the hash bucket where we have found a free entry
					
                    if (prevValue != LOCK_ENTRY) 
                    {	
                        //only proceed if the bucket has been locked
						HashEntry entry;
						entry.pos = pos;
						entry.offset = lastEntryInBucket.offset;		
						entry.ptr = g_HeapConsume.Consume() * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;	//memory alloc
						setHashEntry(g_Hash, i, entry);

						lastEntryInBucket.offset = offset;
						setHashEntry(g_Hash, idxLastEntryInBucket, lastEntryInBucket);
					}
				} 
				return;	//bucket was already locked
			}

			maxIter++;
		} 
	#endif
}

[numthreads(groupthreads, groupthreads, 1)]
void allocCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex)
{
    if (DTid.x >= g_ImageWidth || DTid.y >= g_ImageHeight) return;
	
	float d = g_InputDepth[DTid.xy];
	if (d == MINF) return;

	if (d >= g_maxIntegrationDistance) return;

	float t = getTruncation(d);
	float minDepth = min(g_maxIntegrationDistance, d-t);
	float maxDepth = min(g_maxIntegrationDistance, d+t);
	if (minDepth >= maxDepth) return;

	float3 rayMin = kinectDepthToSkeleton(DTid.x, DTid.y, minDepth);
	rayMin = mul(float4(rayMin, 1.0f), g_RigidTransform).xyz;
	float3 rayMax = kinectDepthToSkeleton(DTid.x, DTid.y, maxDepth);
	rayMax = mul(float4(rayMax, 1.0f), g_RigidTransform).xyz;
	
	float3 rayDir = normalize(rayMax - rayMin);
	
	int3 idCurrentVoxel = worldToSDFBlock(rayMin);
	int3 idEnd = worldToSDFBlock(rayMax);

	float3 step = sign(rayDir);
	float3 boundaryPos = SDFBlockToWorld(idCurrentVoxel+(int3)clamp(step, 0.0, 1.0f))-0.5f*g_VirtualVoxelSize;
	float3 tMax = (boundaryPos-rayMin)/rayDir;
	float3 tDelta = (step*SDF_BLOCK_SIZE*g_VirtualVoxelSize)/rayDir;

	int3 idBound = idEnd+step;

	[unroll]
	for (int c = 0; c < 3; c++) 
    {
		if (rayDir[c] == 0.0f) { tMax[c] = PINF; tDelta[c] = PINF; }
		if (boundaryPos[c] - rayMin[c] == 0.0f) { tMax[c] = PINF; tDelta[c] = PINF; }
	}

	unsigned int iter = 0; // iter < g_MaxLoopIterCount

    [loop]
    while (true && iter < g_MaxLoopIterCount) 
    {
		if (isSDFBlockInCameraFrustumApprox(idCurrentVoxel) && !isSDFBlockStreamedOut(idCurrentVoxel)) 
        {
			allocBlock(idCurrentVoxel);
		}

		// Traverse voxel grid
		if (tMax.x < tMax.y && tMax.x < tMax.z)
		{
			idCurrentVoxel.x += step.x;
			if (idCurrentVoxel.x == idBound.x) return;
			tMax.x += tDelta.x;
		}
		else if (tMax.z < tMax.y)
		{
			idCurrentVoxel.z += step.z;
			if (idCurrentVoxel.z == idBound.z) return;
			tMax.z += tDelta.z;
		}
		else
		{
			idCurrentVoxel.y += step.y;
			if (idCurrentVoxel.y == idBound.y) return;
			tMax.y += tDelta.y;
		}
		
		iter++;
	}
} 

#define NUM_GROUPS_X 1024

[numthreads(SDF_BLOCK_SIZE*SDF_BLOCK_SIZE*SDF_BLOCK_SIZE, 1, 1)]
void integrateCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex, uint3 GID : SV_GroupID)
{
	uint groupID = GID.x + GID.y*NUM_GROUPS_X;
	if (groupID < g_NumOccupiedSDFBlocks)
    {
		HashEntry entry = getHashEntry(g_HashCompactifiedSRV, groupID);
	
        uint i = GTid.x;
        
        int3 pi_base = SDFBlockToVirtualVoxelPos(entry.pos);
        int3 pi = pi_base + delinearizeVoxelIndex(i);

		float3 pf = virtualVoxelPosToWorld(pi); pf = mul(float4(pf, 1.0f), g_RigidTransformInverse).xyz;
		uint2 screenPos = uint2(cameraToKinectScreenInt(pf)); 
	
		if (screenPos.x < g_ImageWidth && screenPos.y < g_ImageHeight) 
        {	
            // on screen	
			float depth = g_InputDepth[screenPos];
			if (depth != MINF) 
            {	
                // valid depth value 
				if (depth < g_maxIntegrationDistance)
				{
					float depthZeroOne = cameraToKinectProjZ(depth);
							
					float sdf = depth - pf.z;
					float truncation = getTruncation(depth);
					
                    if (sdf > -truncation) // && depthZeroOne >= 0.0f && depthZeroOne <= 1.0f) // check if in truncation range should already be made in depth map computation
					{
						if (sdf >= 0.0f)
						{
							sdf = min(truncation, sdf);
						} 
                        else 
                        {
							sdf = max(-truncation, sdf);
						}

						float weightUpdate = max(g_WeightSample * 1.5f * (1.0f-depthZeroOne), 1.0f);
						
                        // contruct current voxel
                        Voxel curr;
                        curr.sdf = sdf;
						curr.weight = weightUpdate;

                        //float3 c;
                        //c = g_InputColor[screenPos].xyz;
                        //curr.color = (int3)(c*255.0f);

                        //float4 l;
                        //l = g_InputLabel_1[screenPos].xyzw;
                        //curr.label[0]  = (int)(l.x*255.0f);
                        //curr.label[1]  = (int)(l.y*255.0f);
                        //curr.label[2]  = (int)(l.z*255.0f);
                        //curr.label[3]  = (int)(l.w*255.0f);

                        //l = g_InputLabel_2[screenPos].xyzw;
                        //curr.label[4]  = (int)(l.x*255.0f);
                        //curr.label[5]  = (int)(l.y*255.0f);
                        //curr.label[6]  = (int)(l.z*255.0f);
                        //curr.label[7]  = (int)(l.w*255.0f);

                        //l = g_InputLabel_3[screenPos].xyzw;
                        //curr.label[8]  = (int)(l.x*255.0f);
                        //curr.label[9]  = (int)(l.y*255.0f);
                        //curr.label[10] = (int)(l.z*255.0f);
                        //curr.label[11] = (int)(l.w*255.0f);

                        // test
                        float3 c;
                        c = g_InputColor[screenPos].xyz;
                        curr.color = round(c*255.0f);

                        float4 l;
                        l = g_InputLabel_1[screenPos].xyzw;
                        curr.label[0]  = round(l.x*255.0f);
                        curr.label[1]  = round(l.y*255.0f);
                        curr.label[2]  = round(l.z*255.0f);
                        curr.label[3]  = round(l.w*255.0f);

                        l = g_InputLabel_2[screenPos].xyzw;
                        curr.label[4]  = round(l.x*255.0f);
                        curr.label[5]  = round(l.y*255.0f);
                        curr.label[6]  = round(l.z*255.0f);
                        curr.label[7]  = round(l.w*255.0f);

                        l = g_InputLabel_3[screenPos].xyzw;
                        curr.label[8]  = round(l.x*255.0f);
                        curr.label[9]  = round(l.y*255.0f);
                        curr.label[10] = round(l.z*255.0f);
                        curr.label[11] = round(l.w*255.0f);
                        // test

                        curr.labelFrequency[0]  = 0;
                        curr.labelFrequency[1]  = 0;
                        curr.labelFrequency[2]  = 0;
                        curr.labelFrequency[3]  = 0;
                        curr.labelFrequency[4]  = 0;
                        curr.labelFrequency[5]  = 0;
                        curr.labelFrequency[6]  = 0;
                        curr.labelFrequency[7]  = 0;
                        curr.labelFrequency[8]  = 0;
                        curr.labelFrequency[9]  = 0;
                        curr.labelFrequency[10] = 0;
                        curr.labelFrequency[11] = 0;

                        uint idx = entry.ptr + i;
                        Voxel prev = getVoxel(g_SDFBlocksSDFUAV, g_SDFBlocksRGBWUAV, g_SDFBlocksLabelUAV_1, g_SDFBlocksLabelUAV_2, g_SDFBlocksLabelUAV_3, g_SDFBlocksLabelFrequencyUAV, idx);
                        Voxel newVoxel = combineVoxel(curr, prev);
						
                        setVoxel(g_SDFBlocksSDFUAV, g_SDFBlocksRGBWUAV, g_SDFBlocksLabelUAV_1, g_SDFBlocksLabelUAV_2, g_SDFBlocksLabelUAV_3, g_SDFBlocksLabelFrequencyUAV, idx, newVoxel);
                    }
				}
			}
        }
	}
}

/////////////////////
// Reset
/////////////////////

[numthreads( groupthreads * groupthreads * 8, 1, 1)]
void resetHashCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex)
{
	const uint idx = DTid.x;
	if (idx >= g_HashNumBuckets * g_HashBucketSize)	return;

	deleteHashEntry(g_Hash, idx);
}

[numthreads( groupthreads * groupthreads * 8, 1, 1)]
void resetHeapCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex)
{
	const uint idx = DTid.x;
	if (idx >= g_NumSDFBlocks)	return;
	g_HeapStatic[idx] = g_NumSDFBlocks-idx-1;
		
	uint blockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
	uint base_idx = idx * blockSize;
	for (uint i = 0; i < blockSize; i++)
    {
        deleteVoxel(g_SDFBlocksSDFUAV, g_SDFBlocksRGBWUAV, g_SDFBlocksLabelUAV_1, g_SDFBlocksLabelUAV_2, g_SDFBlocksLabelUAV_3, g_SDFBlocksLabelFrequencyUAV, base_idx+i);
    }
}

[numthreads(SDF_BLOCK_SIZE*SDF_BLOCK_SIZE*SDF_BLOCK_SIZE, 1, 1)]
void starveVoxelsCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex, uint3 GID : SV_GroupID)
{
	uint groupID = GID.x + GID.y * NUM_GROUPS_X;
	if (groupID < g_NumOccupiedSDFBlocks) 
    {
		HashEntry entry = getHashEntry(g_HashCompactifiedSRV, groupID);
		uint i = GTid.x;
		uint idx = entry.ptr + i;
		starveVoxel(g_SDFBlocksRGBWUAV, idx);
	}
}

/////////////////////
// Compactify
/////////////////////

[numthreads(groupthreads * groupthreads * 8, 1, 1)]
void decisionArrayFillerCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex) 
{
	const uint idx = DTid.x;
	if (idx < g_HashNumBuckets * g_HashBucketSize)
    {
		g_DecisionArrayUAV[idx] = 0;
		HashEntry entry = getHashEntry(g_HashSRV, idx);
		if (entry.ptr != FREE_ENTRY)
        {
			if (isSDFBlockInCameraFrustumApprox(entry.pos)) 
			{
				g_DecisionArrayUAV[idx] = 1;
			}
		}
	}
}

[numthreads( groupthreads * groupthreads * 8, 1, 1)]
void compactifyHashCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex)
{
	const uint idx = DTid.x;
	if (idx < g_HashNumBuckets * g_HashBucketSize) 
    {
		if (g_DecisionArraySRV[idx] == 1) 
        {
			g_HashCompactified[3*(g_PrefixSum[idx]-1)+0] = g_HashSRV[3*idx+0];
			g_HashCompactified[3*(g_PrefixSum[idx]-1)+1] = g_HashSRV[3*idx+1];
			g_HashCompactified[3*(g_PrefixSum[idx]-1)+2] = g_HashSRV[3*idx+2];
		}
	}
}

/////////////////////
// Garbadge Collect
/////////////////////

[numthreads( groupthreads * groupthreads, 1, 1)]
void garbadgeCollectIdentifyOldCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex)
{
	const uint hashIdx = DTid.x;
	if (hashIdx >= g_NumOccupiedSDFBlocks) return;
	HashEntry entry = getHashEntry(g_HashCompactifiedSRV, hashIdx);

	const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
	float minSDF = PINF; // init with plus infinity
	uint minWeight = 0;
	for (uint i = 0; i < linBlockSize; i++) 
    {
		uint idx = entry.ptr + i;
        Voxel v = getVoxel(g_SDFBlocksSDFSRV, g_SDFBlocksRGBWSRV, g_SDFBlocksLabelSRV_1, g_SDFBlocksLabelSRV_2, g_SDFBlocksLabelSRV_3, g_SDFBlocksLabelFrequencySRV, idx);
		if (v.weight > 0) 
        {
			minSDF = min(minSDF, abs(v.sdf));
			minWeight = max(minWeight, abs(v.weight));
		}
	}

	g_DecisionArrayUAV[hashIdx] = 0;

	float t = getTruncation(g_SensorDepthWorldMax);
	if (minSDF > t || minWeight == 0) 
    {
		g_DecisionArrayUAV[hashIdx] = 1;
	}
}

groupshared float	shared_MinSDF[SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE / 2];
groupshared uint	shared_MinWeight[SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE / 2];

[numthreads( SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE / 2, 1, 1)]
void garbadgeCollectIdentifyCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex, uint3 GID : SV_GroupID)
{
	const uint groupID = GID.x + GID.y * NUM_GROUPS_X;
	const uint hashIdx = groupID;

	if (hashIdx < g_NumOccupiedSDFBlocks) 
    { 
		HashEntry entry = getHashEntry(g_HashCompactifiedSRV, hashIdx);

		uint idx0 = entry.ptr + 2*GTid.x+0;
		uint idx1 = entry.ptr + 2*GTid.x+1;

        Voxel v0 = getVoxel(g_SDFBlocksSDFSRV, g_SDFBlocksRGBWSRV, g_SDFBlocksLabelSRV_1, g_SDFBlocksLabelSRV_2, g_SDFBlocksLabelSRV_3, g_SDFBlocksLabelFrequencySRV, idx0);
        Voxel v1 = getVoxel(g_SDFBlocksSDFSRV, g_SDFBlocksRGBWSRV, g_SDFBlocksLabelSRV_1, g_SDFBlocksLabelSRV_2, g_SDFBlocksLabelSRV_3, g_SDFBlocksLabelFrequencySRV, idx1);

		if (v0.weight == 0)	v0.sdf = PINF;
		if (v1.weight == 0) v1.sdf = PINF;

		shared_MinSDF[GTid.x] = min(abs(v0.sdf), abs(v1.sdf));	//init shared memory
		shared_MinWeight[GTid.x] = max(v0.weight, v1.weight);
		
		uint numGroupThreads = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE / 2;
		[loop]
		for (uint stride = 2; stride <= numGroupThreads; stride <<= 1) 
        {
			GroupMemoryBarrierWithGroupSync();
			if ((GTid.x  & (stride-1)) == (stride-1)) 
            {
				shared_MinSDF[GTid.x] = min(shared_MinSDF[GTid.x-stride/2], shared_MinSDF[GTid.x]);
				shared_MinWeight[GTid.x] = max(shared_MinWeight[GTid.x-stride/2], shared_MinWeight[GTid.x]);
			}
		}

		if (GTid.x == numGroupThreads - 1) 
        {
            float minSDF = shared_MinSDF[GTid.x];
			uint minWeight = shared_MinWeight[GTid.x];
            
			g_DecisionArrayUAV[hashIdx] = 0;
			float t = getTruncation(g_maxIntegrationDistance);
			if (minSDF >= t || minWeight == 0) 
            {
				g_DecisionArrayUAV[hashIdx] = 1;
			}
		}
	}
}

[numthreads( groupthreads * groupthreads, 1, 1)] 
void garbadgeCollectFreeCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex) 
{
	const uint hashIdx = DTid.x;

	if (hashIdx < g_NumOccupiedSDFBlocks) 
    {
		if (g_DecisionArraySRV[hashIdx] == 1) 
        {	
            // decision to delete the hash entry
			HashEntry entry = getHashEntry(g_HashCompactifiedSRV, hashIdx);
		
			if (deleteHashEntryElement(g_Hash, g_HeapAppend, g_HashBucketMutex, entry.pos)) 
            {	
                // delete hash entry from hash (and performs heap append)
				const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;

				[allow_uav_condition]
				for (uint i = 0; i < linBlockSize; i++) 
                {	
					uint idx = entry.ptr + i;
                    deleteVoxel(g_SDFBlocksSDFUAV, g_SDFBlocksRGBWUAV, g_SDFBlocksLabelUAV_1, g_SDFBlocksLabelUAV_2, g_SDFBlocksLabelUAV_3, g_SDFBlocksLabelFrequencyUAV, idx);
                }
			}
		}
	}
}
 
/////////////////////
// Local - Global
/////////////////////
	
bool integrateHashEntryToOther(RWBuffer<int> hashOther, const in HashEntry entry) 
{
	uint h = computeHashPosOther(entry.pos, g_Other_HashNumBuckets);		//hash bucket
	uint hp = h * g_Other_HashBucketSize;									//hash position

	[allow_uav_condition]
	for (uint j = 0; j < g_Other_HashBucketSize; j++) 
    {
		uint i = j + hp;		
		HashEntry curr = getHashEntry(hashOther, i);

		int prevWeight = 0;
		InterlockedCompareExchange(hashOther[3*i+2], FREE_ENTRY, LOCK_ENTRY, prevWeight);
		if (prevWeight == FREE_ENTRY) 
        {
			setHashEntry(hashOther, i, entry);
			return true;
		}
	}
	return false;
}
 
[numthreads( groupthreads * groupthreads * 8, 1, 1)]
void removeAndIntegrateCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex) 
{
	const uint idx = DTid.x;
	if (idx < g_HashNumBuckets * g_HashBucketSize)
    {
		HashEntry entry = getHashEntry(g_Hash, idx);

		if (entry.ptr != FREE_ENTRY) 
        {
	#ifdef MOVE_OUT_FRUSTUM
			if (!isSDFBlockInCameraFrustumApprox(entry.pos)) 
            { 
	#endif
	#ifdef MOVE_IN_FRUSTUM
			if (isSDFBlockInCameraFrustumApprox(entry.pos)) 
            {
	#endif
				if (integrateHashEntryToOther(g_HashOther, entry)) 
                {
					deleteHashEntry(g_Hash, idx);	//only delete if there was space to integrate it into other
				}
	
	#if (defined MOVE_OUT_FRUSTUM) || (defined MOVE_IN_FRUSTUM)
			}
	#endif
		}
	}
}