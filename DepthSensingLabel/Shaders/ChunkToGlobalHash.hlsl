struct SDFBlockDesc
{
	int3 pos;
	int ptr;
};

StructuredBuffer<SDFBlockDesc> g_input : register(t0);

RWBuffer<int> g_Hash : register(u0);
RWStructuredBuffer<uint> g_heapStatic1 : register (u1);

#include "GlobalAppStateShaderBuffer.h.hlsl"
#include "SDFShaderBuffer.h.hlsl"
#include "KinectCameraUtil.h.hlsl"
#include "VoxelUtilHashSDF.h.hlsl"

cbuffer consts : register(b1)
{
	uint nSDFBlockDescs;
	uint heapFreeCountPrev;
	uint heapFreeCountEnd;
	uint aling2;
};

//-------------------------------------------------------
// Pass 1: Allocate memory
//-------------------------------------------------------

#define NUM_GROUPS_X 1024 // has to be in sync with the other application code and the CPU !!!

#ifndef groupthreads
#define groupthreads 1
#endif

bool integrateHashEntry(RWBuffer<int> hash, in HashEntry entry)
{
	uint h = computeHashPos(entry.pos);
	uint hp = h * g_HashBucketSize;

	[allow_uav_condition]
	for (uint j = 0; j < g_HashBucketSize; j++)
    {
		uint i = j + hp;
		HashEntry curr = getHashEntry(hash, i);

		int prevWeight = 0;
		InterlockedCompareExchange(hash[3*i + 2], FREE_ENTRY, LOCK_ENTRY, prevWeight);
		if (prevWeight == FREE_ENTRY)
        {
			setHashEntry(hash, i, entry);
			return true;
		}
	}
	 
	#ifdef HANDLE_COLLISIONS
		// updated variables as after the loop
        uint idxLastEntryInBucket = (h+1)*g_HashBucketSize - 1;	// get last index of bucket
        uint i = idxLastEntryInBucket; // start with the last entry of the current bucket
    
        uint maxIter = 0;
        uint offset = 0;

        HashEntry curr;

        [allow_uav_condition]
        while (true && maxIter < g_MaxLoopIterCount)
        {
	        curr = getHashEntry(hash, i);
	        if (curr.offset == 0) break;

	        i = idxLastEntryInBucket + curr.offset;
	        i %= (g_HashBucketSize * g_HashNumBuckets);

	        maxIter++;
        }

        maxIter = 0;
        
        [allow_uav_condition]
        while (true && maxIter < g_MaxLoopIterCount)
        {
	        offset++;
	        uint i = (idxLastEntryInBucket + offset) % (g_HashBucketSize * g_HashNumBuckets); // go to next hash element
	        if ((offset % g_HashBucketSize) == 0) continue;	// cannot insert into a last bucket element (would conflict with other linked lists)

	        int prevWeight = 0;
	        InterlockedCompareExchange(hash[3*i + 2], FREE_ENTRY, LOCK_ENTRY, prevWeight);
	        
            if (prevWeight == FREE_ENTRY)
	        {
		        HashEntry lastEntryInBucket = getHashEntry(hash, idxLastEntryInBucket);

		        int newOffsetPrev = (offset << 16) | (lastEntryInBucket.pos.z & 0x0000ffff);
		        int oldOffsetPrev = 0;

		        InterlockedExchange(hash[3 * idxLastEntryInBucket + 1], newOffsetPrev, oldOffsetPrev);
		        entry.offset = oldOffsetPrev >> 16;	// remove prev z-pos from old offset

		        setHashEntry(hash, i, entry); // sets the current hashEntry with: curr->next = prev->next
		        return true;
	        }

	        maxIter++;
        }
	#endif

	return false;
}

static const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;

[numthreads(groupthreads, 1, 1)]
void chunkToGlobalHashPass1CS(int3 dTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex, uint3 GID : SV_GroupID)
{
    uint ID = groupthreads*(GID.x + GID.y*NUM_GROUPS_X) + GI;

    if (ID < nSDFBlockDescs)
    {
        uint ptr = g_heapStatic1[heapFreeCountPrev-nSDFBlockDescs+ID] * linBlockSize;

        HashEntry entry;
        entry.pos = g_input[nSDFBlockDescs-ID-1].pos;
        entry.offset = 0;
        entry.ptr = ptr;

        bool ok = integrateHashEntry(g_Hash, entry);
    }
}

//-------------------------------------------------------
// Pass 2: Copy input to SDFBlocks
//-------------------------------------------------------

Buffer<int> g_inputBlock : register(t0);

RWBuffer<float> g_outputSDFBlocksSDF          : register(u0);
RWStructuredBuffer<uint> g_heapStatic2        : register(u1); // Why UAV -> SRV sufficient
RWBuffer<int> g_outputSDFBlocksRGBW           : register(u2);
RWBuffer<int> g_outputSDFBlocksLabel_1        : register(u3);
RWBuffer<int> g_outputSDFBlocksLabel_2        : register(u4);
RWBuffer<int> g_outputSDFBlocksLabel_3        : register(u5);
RWBuffer<int> g_outputSDFBlocksLabelFrequency : register(u6);

[numthreads(SDF_BLOCK_SIZE*SDF_BLOCK_SIZE*SDF_BLOCK_SIZE, 1, 1)]
void chunkToGlobalHashPass2CS(int3 dTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex, uint3 GID : SV_GroupID)
{
	uint idxBlock = GID.x + GID.y*NUM_GROUPS_X;
	
	if (idxBlock < nSDFBlockDescs)
	{
		uint ptr = g_heapStatic2[heapFreeCountPrev-idxBlock-1]*linBlockSize;

		g_outputSDFBlocksSDF[ptr+GI]     = asfloat(g_inputBlock[11*(idxBlock*linBlockSize+GI)+0]);
		g_outputSDFBlocksRGBW[ptr+GI]    = g_inputBlock[11*(idxBlock*linBlockSize+GI)+1];
        g_outputSDFBlocksLabel_1[ptr+GI] = g_inputBlock[11*(idxBlock*linBlockSize+GI)+2];
        g_outputSDFBlocksLabel_2[ptr+GI] = g_inputBlock[11*(idxBlock*linBlockSize+GI)+3];
        g_outputSDFBlocksLabel_3[ptr+GI] = g_inputBlock[11*(idxBlock*linBlockSize+GI)+4];
        for (uint i = 0; i < 6; i++)
        {
            g_outputSDFBlocksLabelFrequency[6*(ptr+GI)+i] = g_inputBlock[11*(idxBlock*linBlockSize+GI)+5+i];
        }
    }
}